#ifndef EE_WINDOWCCURSORMANAGER_HPP
#define EE_WINDOWCCURSORMANAGER_HPP

#include <eepp/window/base.hpp>

#include <eepp/graphics/cimage.hpp>
#include <eepp/graphics/ctexture.hpp>
using namespace EE::Graphics;

#include <eepp/window/cwindow.hpp>
#include <eepp/window/cursorhelper.hpp>
#include <eepp/window/ccursor.hpp>
using namespace EE::Window::Cursor;

#include <set>

namespace EE { namespace Window {

class EE_API cCursorManager {
	public:
		cCursorManager( Window::cWindow * window );

		virtual ~cCursorManager();

		virtual cCursor *		Create( cTexture * tex, const eeVector2i& hotspot, const std::string& name ) = 0;

		virtual cCursor *		Create( cImage * img, const eeVector2i& hotspot, const std::string& name ) = 0;

		virtual cCursor *		Create( const std::string& path, const eeVector2i& hotspot, const std::string& name ) = 0;

		virtual cCursor *		Add( cCursor * cursor );

		virtual void			Remove( cCursor * cursor, bool Delete = false ) = 0;

		virtual void			Remove( const std::string& name, bool Delete = false );

		virtual void			Remove( const Uint32& id, bool Delete = false );

		virtual cCursor *		Get( const std::string& name );

		virtual cCursor *		Get( const Uint32& id );

		virtual void			Set( const std::string& name );

		virtual void			Set( const Uint32& id );

		virtual void			Set( cCursor * cursor ) = 0;

		virtual void			Set( EE_SYSTEM_CURSOR syscurid ) = 0;

		virtual void			Show() = 0;

		virtual void			Hide() = 0;

		virtual void			Reload() = 0;

		virtual void			Visible( bool visible ) = 0;

		virtual bool			Visible();

		cCursor *				Current() const;

		EE_SYSTEM_CURSOR		CurrentSysCursor() const;

		bool					CurrentIsSysCursor() const;
	protected:
		typedef	std::set<cCursor*> CursorsList;
		cWindow *				mWindow;
		cCursor *				mCurrent;
		EE_SYSTEM_CURSOR		mSysCursor;
		CursorsList				mCursors;
		bool					mCurSysCursor;
		bool					mVisible;
};

}}

#endif
