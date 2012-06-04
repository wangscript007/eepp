#include "cwindowsdl.hpp"

#ifdef EE_BACKEND_SDL2

#include "cclipboardsdl.hpp"
#include "cinputsdl.hpp"
#include "ccursormanagersdl.hpp"

#include "../../../graphics/cglobalbatchrenderer.hpp"
#include "../../../graphics/cshaderprogrammanager.hpp"
#include "../../../graphics/cvertexbuffermanager.hpp"
#include "../../../graphics/cframebuffermanager.hpp"
#include "../../../graphics/ctexturefactory.hpp"
#include "../../platform/platformimpl.hpp"

namespace EE { namespace Window { namespace Backend { namespace SDL2 {

cWindowSDL::cWindowSDL( WindowSettings Settings, ContextSettings Context ) :
	cWindow( Settings, Context, eeNew( cClipboardSDL, ( this ) ), eeNew( cInputSDL, ( this ) ), eeNew( cCursorManagerSDL, ( this ) ) ),
	mSDLWindow( NULL ),
	mGLContext( NULL )
{
	Create( Settings, Context );
}

cWindowSDL::~cWindowSDL() {
}

bool cWindowSDL::Create( WindowSettings Settings, ContextSettings Context ) {
	if ( mWindow.Created )
		return false;

	mWindow.WindowConfig	= Settings;
	mWindow.ContextConfig	= Context;

	if ( SDL_Init( SDL_INIT_VIDEO ) != 0 ) {
		cLog::instance()->Write( "Unable to initialize SDL: " + std::string( SDL_GetError() ) );

		LogFailureInit( "cWindowSDL", GetVersion() );

		return false;
	}

	SDL_DisplayMode dpm;
	SDL_GetDesktopDisplayMode( 0, &dpm );

	mWindow.DesktopResolution = eeSize( dpm.w, dpm.h );

	#if EE_PLATFORM == EE_PLATFORM_ANDROID
		mWindow.WindowConfig.Style = WindowStyle::Fullscreen | WindowStyle::UseDesktopResolution;
	#endif

	if ( mWindow.WindowConfig.Style & WindowStyle::UseDesktopResolution ) {
		mWindow.WindowConfig.Width	= mWindow.DesktopResolution.Width();
		mWindow.WindowConfig.Height	= mWindow.DesktopResolution.Height();
	}

	mWindow.Flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

	if ( mWindow.WindowConfig.Style & WindowStyle::Resize ) {
		mWindow.Flags |= SDL_WINDOW_RESIZABLE;
	}

	if ( mWindow.WindowConfig.Style & WindowStyle::NoBorder ) {
		mWindow.Flags |= SDL_WINDOW_BORDERLESS;
	}

	SetGLConfig();

	Uint32 mTmpFlags = mWindow.Flags;

	if ( mWindow.WindowConfig.Style & WindowStyle::Fullscreen ) {
		mTmpFlags |= SDL_WINDOW_FULLSCREEN;
	}

	mSDLWindow = SDL_CreateWindow( mWindow.WindowConfig.Caption.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWindow.WindowConfig.Width, mWindow.WindowConfig.Height, mTmpFlags );

	mWindow.WindowSize = eeSize( mWindow.WindowConfig.Width, mWindow.WindowConfig.Height );

	if ( NULL == mSDLWindow ) {
		cLog::instance()->Write( "Unable to create window: " + std::string( SDL_GetError() ) );

		LogFailureInit( "cWindowSDL", GetVersion() );

		return false;
	}

	#if EE_PLAFORM == EE_PLATFORM_ANDROID || EE_PLATFORM == EE_PLATFORM_IOS
		#ifdef EE_GLES1
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		#endif

		#ifdef EE_GLES2
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
		#endif
	#endif

	mGLContext = SDL_GL_CreateContext( mSDLWindow );

	if ( NULL == mGLContext ) {
		cLog::instance()->Write( "Unable to create context: " + std::string( SDL_GetError() ) );

		LogFailureInit( "cWindowSDL", GetVersion() );

		return false;
	}

	SDL_GL_SetSwapInterval( ( mWindow.ContextConfig.VSync ? 1 : 0 ) );								// VSync

	if ( NULL == cGL::ExistsSingleton() ) {
		cGL::CreateSingleton( mWindow.ContextConfig.Version );
		cGL::instance()->Init();
	}

	SDL_GL_MakeCurrent( mSDLWindow, mGLContext );

	CreatePlatform();

	GetMainContext();

	Caption( mWindow.WindowConfig.Caption );

	CreateView();

	Setup2D();

	mWindow.Created = true;

	if ( "" != mWindow.WindowConfig.Icon ) {
		Icon( mWindow.WindowConfig.Icon );
	}

	/// Init the clipboard after the window creation
	reinterpret_cast<cClipboardSDL*> ( mClipboard )->Init();

	/// Init the input after the window creation
	reinterpret_cast<cInputSDL*> ( mInput )->Init();

	mCursorManager->Set( Cursor::SYS_CURSOR_DEFAULT );

	LogSuccessfulInit( GetVersion() );

	return true;
}

std::string cWindowSDL::GetVersion() {
	SDL_version ver;

	SDL_GetVersion( &ver );

	return StrFormated( "SDL %d.%d.%d", ver.major, ver.minor, ver.patch );
}

void cWindowSDL::CreatePlatform() {
	eeSAFE_DELETE( mPlatform );
#if EE_PLATFORM == EE_PLATFORM_WIN || EE_PLATFORM == EE_PLATFORM_MACOSX || defined( EE_X11_PLATFORM )
	SDL_VERSION( &mWMinfo.version );
	SDL_GetWindowWMInfo ( mSDLWindow, &mWMinfo );
#endif

#if defined( EE_X11_PLATFORM )
	mPlatform = eeNew( Platform::cX11Impl, ( this, mWMinfo.info.x11.display, mWMinfo.info.x11.window, mWMinfo.info.x11.window, NULL, NULL ) );
#elif EE_PLATFORM == EE_PLATFORM_WIN
	mPlatform = eeNew( Platform::cWinImpl, ( this, GetWindowHandler() ) );
#elif EE_PLATFORM == EE_PLATFORM_MACOSX
	mPlatform = eeNew( Platform::cOSXImpl, ( this ) );
#else
	cWindow::CreatePlatform();
#endif
}

void cWindowSDL::SetGLConfig() {
	if ( mWindow.ContextConfig.DepthBufferSize ) SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE	, mWindow.ContextConfig.DepthBufferSize );				// Depth
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, ( mWindow.ContextConfig.DoubleBuffering ? 1 : 0 ) );	// Double Buffering
	if ( mWindow.ContextConfig.StencilBufferSize ) SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, mWindow.ContextConfig.StencilBufferSize );

	if ( mWindow.WindowConfig.BitsPerPixel == 16 ) {
		SDL_GL_SetAttribute( SDL_GL_RED_SIZE	, 4 );
		SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE	, 4 );
		SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE	, 4 );
		SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE	, 4 );
	} else {
		SDL_GL_SetAttribute( SDL_GL_RED_SIZE	, 8 );
		SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE	, 8 );
		SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE	, 8 );
		SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE	, 8 );
	}
}

void cWindowSDL::ToggleFullscreen() {
	bool WasMaximized = mWindow.Maximized;

	if ( Windowed() ) {
		Size( mWindow.WindowConfig.Width, mWindow.WindowConfig.Height, !Windowed() );
	} else {
		Size( mWindow.WindowSize.Width(), mWindow.WindowSize.Height(), !Windowed() );
	}

	if ( WasMaximized ) {
		Maximize();
	}

	GetCursorManager()->Reload();
}

void cWindowSDL::Caption( const std::string& Caption ) {
	mWindow.WindowConfig.Caption = Caption;

	SDL_SetWindowTitle( mSDLWindow, Caption.c_str() );
}

bool cWindowSDL::Active() {
	Uint32 flags = 0;

	flags = SDL_GetWindowFlags( mSDLWindow );

	return 0 != ( ( flags & SDL_WINDOW_INPUT_FOCUS ) && ( flags & SDL_WINDOW_MOUSE_FOCUS ) );
}

bool cWindowSDL::Visible() {
	Uint32 flags = 0;

	flags = SDL_GetWindowFlags( mSDLWindow );

	return 0 != ( ( flags & SDL_WINDOW_SHOWN ) && !( flags & SDL_WINDOW_MINIMIZED ) );
}

void cWindowSDL::Size( Uint32 Width, Uint32 Height, bool Windowed ) {
	if ( ( !Width || !Height ) ) {
		Width	= mWindow.DesktopResolution.Width();
		Height	= mWindow.DesktopResolution.Height();
	}

	if ( this->Windowed() == Windowed && Width == mWindow.WindowConfig.Width && Height == mWindow.WindowConfig.Height )
		return;

	#ifdef EE_SUPPORT_EXCEPTIONS
	try {
	#endif
		cLog::instance()->Writef( "Switching from %s to %s. Width: %d Height %d.", this->Windowed() ? "windowed" : "fullscreen", Windowed ? "windowed" : "fullscreen", Width, Height );

		#if EE_PLATFORM == EE_PLATFORM_WIN || EE_PLATFORM == EE_PLATFORM_MACOSX
		#if EE_PLATFORM == EE_PLATFORM_WIN
		bool Reload = this->Windowed() != Windowed;
		#else
		bool Reload = true;
		#endif

		if ( Reload )
			Graphics::cTextureFactory::instance()->GrabTextures();
		#endif

		Uint32 oldWidth		= mWindow.WindowConfig.Width;
		Uint32 oldHeight	= mWindow.WindowConfig.Height;

		mWindow.WindowConfig.Width    = Width;
		mWindow.WindowConfig.Height   = Height;

		if ( Windowed ) {
			mWindow.WindowSize = eeSize( Width, Height );
		} else {
			mWindow.WindowSize = eeSize( oldWidth, oldHeight );
		}

		if ( this->Windowed() && !Windowed ) {
			mWinPos = Position();
		} else {
			SDL_SetWindowFullscreen( mSDLWindow, Windowed ? SDL_FALSE : SDL_TRUE );
		}

		SDL_SetWindowSize( mSDLWindow, Width, Height );

		if ( this->Windowed() && !Windowed ) {
			mWinPos = Position();

			SetGLConfig();

			SDL_SetWindowFullscreen( mSDLWindow, Windowed ? SDL_FALSE : SDL_TRUE );
		}

		#if EE_PLATFORM == EE_PLATFORM_WIN || EE_PLATFORM == EE_PLATFORM_MACOSX
		if ( Reload ) {
			cGL::instance()->Init();

			Graphics::cTextureFactory::instance()->UngrabTextures();		// Reload all textures
			Graphics::cShaderProgramManager::instance()->Reload();			// Reload all shaders
			Graphics::Private::cFrameBufferManager::instance()->Reload(); 	// Reload all frame buffers
			Graphics::Private::cVertexBufferManager::instance()->Reload(); 	// Reload all vertex buffers
			GetMainContext();												// Recover the context
			CreatePlatform();
		}
		#endif

		if ( !this->Windowed() && Windowed ) {
			Position( mWinPos.x, mWinPos.y );
		}

		SetFlagValue( &mWindow.WindowConfig.Style, WindowStyle::Fullscreen, !Windowed );

		mDefaultView.SetView( 0, 0, Width, Height );

		Setup2D();

		SDL_PumpEvents();

		SDL_FlushEvent( SDL_WINDOWEVENT );

		mCursorManager->Reload();

		SendVideoResizeCb();
	#ifdef EE_SUPPORT_EXCEPTIONS
	} catch (...) {
		cLog::instance()->Write( "Unable to change resolution: " + std::string( SDL_GetError() ) );
		cLog::instance()->Save();
		mWindow.Created = false;
	}
	#endif
}

void cWindowSDL::SwapBuffers() {	
	SDL_GL_SwapWindow( mSDLWindow );
}

std::vector< std::pair<unsigned int, unsigned int> > cWindowSDL::GetPossibleResolutions() const {
	std::vector< std::pair<unsigned int, unsigned int> > result;

	for ( Int32 i = 0; i < SDL_GetNumDisplayModes(0); i++ ) {
		SDL_DisplayMode mode;
		SDL_GetDisplayMode( 0, i, &mode );

		result.push_back( std::pair<unsigned int, unsigned int>( mode.w, mode.h ) );
	}

	return result;
}

void cWindowSDL::SetGamma( eeFloat Red, eeFloat Green, eeFloat Blue ) {
	eeclamp( &Red	, (eeFloat)0.1f, (eeFloat)10.0f );
	eeclamp( &Green	, (eeFloat)0.1f, (eeFloat)10.0f );
	eeclamp( &Blue	, (eeFloat)0.1f, (eeFloat)10.0f );

	Uint16 red_ramp[256];
	Uint16 green_ramp[256];
	Uint16 blue_ramp[256];

	SDL_CalculateGammaRamp( Red, red_ramp );

	if ( Green == Red ) {
		SDL_memcpy(green_ramp, red_ramp, sizeof(red_ramp));
	} else {
		SDL_CalculateGammaRamp( Green, green_ramp );
	}

	if ( Blue == Red ) {
		SDL_memcpy( blue_ramp, red_ramp, sizeof(red_ramp) );
	} else if ( Blue == Green ) {
		SDL_memcpy( blue_ramp, green_ramp, sizeof(green_ramp) );
	} else {
		SDL_CalculateGammaRamp( Blue, blue_ramp );
	}

	SDL_SetWindowGammaRamp( mSDLWindow, red_ramp, green_ramp, blue_ramp );
}

eeWindowHandler	cWindowSDL::GetWindowHandler() {
#if EE_PLATFORM == EE_PLATFORM_WIN
	return mWMinfo.info.win.window;
#elif defined( EE_X11_PLATFORM )
	return mWMinfo.info.x11.display;
#elif EE_PLATFORM == EE_PLATFORM_MACOSX
	return mWMinfo.info.cocoa.window;
#else
	return 0;
#endif
}

bool cWindowSDL::Icon( const std::string& Path ) {
	int x, y, c;

	if ( !mWindow.Created ) {
		if ( stbi_info( Path.c_str(), &x, &y, &c ) ) {
			mWindow.WindowConfig.Icon 	= Path;

			return true;
		}

		return false;
	}

	cImage Img( Path );

	if ( NULL != Img.GetPixelsPtr() ) {
		const Uint8 * Ptr = Img.GetPixelsPtr();
		x = Img.Width();
		y = Img.Height();
		c = Img.Channels();

		if ( ( x  % 8 ) == 0 && ( y % 8 ) == 0 ) {
			Uint32 rmask, gmask, bmask, amask;
			#if SDL_BYTEORDER == SDL_BIG_ENDIAN
				rmask = 0xff000000;
				gmask = 0x00ff0000;
				bmask = 0x0000ff00;
				amask = 0x000000ff;
			#else
				rmask = 0x000000ff;
				gmask = 0x0000ff00;
				bmask = 0x00ff0000;
				amask = 0xff000000;
			#endif
			SDL_Surface * TempGlyphSheet = SDL_CreateRGBSurface( SDL_SWSURFACE, x, y, c * 8, rmask, gmask, bmask, amask );

			SDL_LockSurface( TempGlyphSheet );

			Uint32 ssize = TempGlyphSheet->w * TempGlyphSheet->h * c;

			for ( Uint32 i=0; i < ssize; i++ ) {
				( static_cast<Uint8*>( TempGlyphSheet->pixels ) )[i+0] = (Ptr)[i];
			}

			SDL_UnlockSurface( TempGlyphSheet );

			SDL_SetWindowIcon( mSDLWindow, TempGlyphSheet );

			SDL_FreeSurface( TempGlyphSheet );

			return true;
		}
	}

	return false;
}

void cWindowSDL::Minimize() {
	SDL_MinimizeWindow( mSDLWindow );
}

void cWindowSDL::Maximize() {
	SDL_MaximizeWindow( mSDLWindow );
}

void cWindowSDL::Hide() {
	SDL_HideWindow( mSDLWindow );
}

void cWindowSDL::Raise() {
	SDL_RaiseWindow( mSDLWindow );
}

void cWindowSDL::Show() {
	SDL_ShowWindow( mSDLWindow );
}

void cWindowSDL::Position( Int16 Left, Int16 Top ) {
	SDL_SetWindowPosition( mSDLWindow, Left, Top );
}

eeVector2i cWindowSDL::Position() {
	eeVector2i p;

	SDL_GetWindowPosition( mSDLWindow, &p.x, &p.y );

	return p;
}

SDL_Window * cWindowSDL::GetSDLWindow() const {
	return mSDLWindow;
}

}}}}

#endif