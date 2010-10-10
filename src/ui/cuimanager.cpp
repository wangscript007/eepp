#include "cuimanager.hpp"

namespace EE { namespace UI {

cUIManager::cUIManager() :
	mEE( NULL ),
	mKM( NULL ),
	mInit( false ),
	mControl( NULL ),
	mFocusControl( NULL ),
	mOverControl( NULL ),
	mCbId(-1)
{
	mEE = cEngine::instance();
	mKM = cInput::instance();
}

cUIManager::~cUIManager() {
	Shutdown();
}

void cUIManager::Init() {
	if ( mInit )
		Shutdown();

	mInit			= true;
	mControl		= eeNew( cUIControlAnim, ( cUIControl::CreateParams( NULL, eeVector2i( 0, 0 ), eeSize( cEngine::instance()->GetWidth(), cEngine::instance()->GetHeight() ) ) ) );
	mControl->Visible( true );
	mControl->Enabled( true );

	mFocusControl	= mControl;
	mOverControl	= mControl;

	mCbId = cInput::instance()->PushCallback( cb::Make1( this, &cUIManager::InputCallback ) );
}

void cUIManager::Shutdown() {
	if ( mInit ) {
		if ( -1 != mCbId )
			cInput::instance()->PopCallback( mCbId );

		mOverControl = NULL;
		mFocusControl = NULL;

		eeSAFE_DELETE( mControl );

		mInit = false;
	}

	cUIThemeManager::DestroySingleton();
}

void cUIManager::InputCallback( EE_Event * Event ) {
	switch( Event->type ) {
		case SDL_KEYUP:
			SendKeyUp( Event );
			break;
		case SDL_KEYDOWN:
			SendKeyDown( Event );
			break;
	}
}

void cUIManager::ResizeControl() {
	mControl->Size( mEE->GetWidth(), mEE->GetHeight() );
	SendMsg( mControl, cUIMessage::MsgWindowResize );
}

void cUIManager::SendKeyUp( EE_Event * Event ) {
	cUIEventKey	KeyEvent	= cUIEventKey( mFocusControl, cUIEvent::EventKeyUp, Event->key.keysym.sym, Event->key.keysym.unicode );
	cUIControl * CtrlLoop	= mFocusControl;

	while( NULL != CtrlLoop ) {
		if ( CtrlLoop->Enabled() && CtrlLoop->OnKeyUp( KeyEvent ) )
			break;

		CtrlLoop = CtrlLoop->Parent();
	}
}

void cUIManager::SendKeyDown( EE_Event * Event ) {
	cUIEventKey	KeyEvent	= cUIEventKey( mFocusControl, cUIEvent::EventKeyDown, Event->key.keysym.sym, Event->key.keysym.unicode );
	cUIControl * CtrlLoop	= mFocusControl;

	while( NULL != CtrlLoop ) {
		if ( CtrlLoop->Enabled() && CtrlLoop->OnKeyDown( KeyEvent ) )
			break;

		CtrlLoop = CtrlLoop->Parent();
	}
}

cUIControl * cUIManager::FocusControl() const {
	return mFocusControl;
}

void cUIManager::FocusControl( cUIControl * Ctrl ) {
	mFocusControl = Ctrl;
}

cUIControl * cUIManager::OverControl() const {
	return mOverControl;
}

void cUIManager::OverControl( cUIControl * Ctrl ) {
	mOverControl = Ctrl;
}

void cUIManager::SendMsg( cUIControl * Ctrl, const Uint32& Msg, const Uint32& Flags ) {
	cUIMessage tMsg( Ctrl, Msg, Flags );

	Ctrl->MessagePost( &tMsg );
}

void cUIManager::Update() {
	mElapsed = mEE->Elapsed();

	if ( mKM->ReleaseTrigger() ) {
		if ( NULL != mFocusControl ) {
			mFocusControl->OnMouseUp( mKM->GetMousePos(), mKM->ReleaseTrigger() );

			if ( mKM->ClickTrigger() ) {
				SendMsg( mFocusControl, cUIMessage::MsgClick, mKM->ClickTrigger() );
				mFocusControl->OnMouseClick( mKM->GetMousePos(), mKM->ClickTrigger() );

				if ( mKM->DoubleClickTrigger() ) {
					SendMsg( mFocusControl, cUIMessage::MsgDoubleClick, mKM->DoubleClickTrigger() );
					mFocusControl->OnMouseDoubleClick( mKM->GetMousePos(), mKM->DoubleClickTrigger() );
				}
			}
		}
	}

	cUIControl * pOver = mControl->OverFind( mKM->GetMousePos() );

	if ( pOver != mOverControl ) {
		if ( NULL != mOverControl ) {
			SendMsg( mOverControl, cUIMessage::MsgMouseEnter );
			mOverControl->OnMouseExit( mKM->GetMousePos(), 0 );
		}

		mOverControl = pOver;

		if ( NULL != mOverControl ) {
			SendMsg( mOverControl, cUIMessage::MsgMouseEnter );
			mOverControl->OnMouseEnter( mKM->GetMousePos(), 0 );
		}
	} else {
		if ( NULL != mOverControl )
			mOverControl->OnMouseMove( mKM->GetMousePos(), mKM->PressTrigger() );
	}

	if ( mKM->PressTrigger() ) {
		if ( NULL != mOverControl ) {
			if ( mOverControl != mFocusControl ) {
				mOverControl->OnFocus();
				mFocusControl->OnFocusLoss();

				mFocusControl = mOverControl;
			}

			mOverControl->OnMouseDown( mKM->GetMousePos(), mKM->PressTrigger() );
		}
	}

	mControl->Update();
	mControl->CheckClose();
}

void cUIManager::Draw() {
	mControl->InternalDraw();
}

cUIControlAnim * cUIManager::MainControl() const {
	return mControl;
}

const eeFloat& cUIManager::Elapsed() const {
	return mElapsed;
}

void cUIManager::ClipEnable( const Int32& x, const Int32& y, const Int32& Width, const Int32& Height ) {
	GLdouble tX = (GLdouble)x;
	GLdouble tY = (GLdouble)y;
	GLdouble tW = (GLdouble)Width;
	GLdouble tH = (GLdouble)Height;

	GLdouble clip_left[] = { 1.0, 0.0, 0.0, 0.0 };
	clip_left[3] = -tX;

	GLdouble clip_right[] = { -1.0, 0.0, 0.0, 0.0 };
	clip_right[3] = tX + tW;

	GLdouble clip_top[] = { 0.0, 1.0, 0.0, 0.0 };
	clip_top[3] = -tY;

	GLdouble clip_bottom[] = { 0.0, -1.0, 0.0, 0.0 };
	clip_bottom[3] = tY + tH;

	glEnable(GL_CLIP_PLANE0);
	glEnable(GL_CLIP_PLANE1);
	glEnable(GL_CLIP_PLANE2);
	glEnable(GL_CLIP_PLANE3);

	glClipPlane(GL_CLIP_PLANE0, clip_left);
	glClipPlane(GL_CLIP_PLANE1, clip_right);
	glClipPlane(GL_CLIP_PLANE2, clip_top);
	glClipPlane(GL_CLIP_PLANE3, clip_bottom);

}

void cUIManager::ClipDisable() {
	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_CLIP_PLANE1);
	glDisable(GL_CLIP_PLANE2);
	glDisable(GL_CLIP_PLANE3);
}

void cUIManager::SetTheme( const std::string& Theme ) {
	SetTheme( cUIThemeManager::instance()->GetByName( Theme ) );
}

void cUIManager::SetTheme( cUITheme * Theme ) {
	if ( NULL != mControl ) {
		mControl->SetTheme( Theme );
		mControl->SetThemeToChilds( Theme );
	}
}

}}
