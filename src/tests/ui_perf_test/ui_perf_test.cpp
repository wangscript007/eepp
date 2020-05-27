#include <eepp/ee.hpp>

// This file is used to test some UI related stuffs.
// It's not a benchmark or a real test suite.
// It's just used to test whatever I need to test at any given moment.

EE::Window::Window* win = NULL;

void mainLoop() {
	win->getInput()->update();

	if ( win->getInput()->isKeyUp( KEY_ESCAPE ) ) {
		win->close();
	}

	// Update the UI scene.
	SceneManager::instance()->update();

	// Check if the UI has been invalidated ( needs redraw ).
	if ( SceneManager::instance()->getUISceneNode()->invalidated() ) {
		win->clear();

		// Redraw the UI scene.
		SceneManager::instance()->draw();

		win->display();
	} else {
		Sys::sleep( Milliseconds( 8 ) );
	}
}

EE_MAIN_FUNC int main( int argc, char* argv[] ) {
	win = Engine::instance()->createWindow( WindowSettings( 800, 600, "eepp - UI Perf Test" ),
											ContextSettings( true ) );

	if ( win->isOpen() ) {
		FileSystem::changeWorkingDirectory( Sys::getProcessPath() );
		PixelDensity::setPixelDensity(
			Engine::instance()->getDisplayManager()->getDisplayIndex( 0 )->getPixelDensity() );
		FontTrueType* font =
			FontTrueType::New( "NotoSans-Regular", "assets/fonts/NotoSans-Regular.ttf" );
		UISceneNode* uiSceneNode = UISceneNode::New();
		SceneManager::instance()->add( uiSceneNode );
		uiSceneNode->getUIThemeManager()->setDefaultFont( font );
		/*StyleSheetParser styleSheetParser;
		styleSheetParser.loadFromFile( "assets/ui/breeze.css" );
		uiSceneNode->setStyleSheet( styleSheetParser.getStyleSheet() );*/
		std::string pd;
		if ( PixelDensity::getPixelDensity() >= 1.5f )
			pd = "1.5x";
		else if ( PixelDensity::getPixelDensity() >= 2.f )
			pd = "2x";
		UITheme* theme =
			UITheme::load( "uitheme" + pd, "uitheme" + pd, "assets/ui/uitheme" + pd + ".eta", font,
						   "assets/ui/uitheme.css" );
		uiSceneNode->setStyleSheet( theme->getStyleSheet() );
		uiSceneNode->getUIThemeManager()
			->setDefaultEffectsEnabled( true )
			->setDefaultTheme( theme )
			->setDefaultFont( font )
			->add( theme );

		auto* vlay = UILinearLayout::NewVertical();
		vlay->setLayoutSizePolicy( SizePolicy::MatchParent, SizePolicy::MatchParent );

		/* ListBox test */ /*
		std::vector<String> strings;
		for ( size_t i = 0; i < 10000; i++ )
			strings.emplace_back( String::format(
				"This is a very long string number %ld. Cover the full width of the listbox.",
				i ) );
		 auto* lbox = UIListBox::New();
		 std::cout << "Time New: " << clock.getElapsed().asMilliseconds() << " ms" << std::endl;
		 lbox->setParent( vlay );
		 std::cout << "Time setParent: " << clock.getElapsed().asMilliseconds() << " ms"
				   << std::endl;
		 lbox->setLayoutMargin( Rect( 4, 4, 4, 4 ) );
		 std::cout << "Time setLayoutMargin: " << clock.getElapsed().asMilliseconds() << " ms"
				   << std::endl;
		 lbox->setLayoutSizePolicy( SizePolicy::MatchParent, SizePolicy::MatchParent );
		 std::cout << "Time setLayoutSizePolicy: " << clock.getElapsed().asMilliseconds() << " ms"
				   << std::endl;
		 for ( size_t i = 0; i < 10; i++ )
			 lbox->addListBoxItem( String::format(
				 "This is a very long string number %ld. Cover the full width of the listbox.",
				 i ) );
		 std::cout << "Time addListBoxItem: " << clock.getElapsed().asMilliseconds() << " ms"
				   << std::endl;
		 lbox->addListBoxItems( strings );
		 std::cout << "Time addListBoxItems: " << clock.getElapsed().asMilliseconds() << " ms"
				   << std::endl;*/

		/* Create Widget test */
		Clock total;
		/*for ( size_t i = 0; i < 10000; i++ ) {
			auto* widget = UINode::New();
		}
		std::cout << "Time UINode total: " << total.getElapsedTime().asMilliseconds() << " ms"
				  << std::endl;

		uiSceneNode->getRoot()->childsCloseAll();
		SceneManager::instance()->update();*/

		total.restart();
		for ( size_t i = 0; i < 100000; i++ ) {
			auto* widget = UIWidget::New();
			widget->setParent( vlay );
			widget->setLayoutSizePolicy( SizePolicy::MatchParent, SizePolicy::Fixed );
			widget->setSize( Sizef( 0, 4 ) );
			Colorf col;
			col.hsv.h = Math::randf( 0, 360 );
			col.hsv.s = 1;
			col.hsv.v = 1;
			col.hsv.a = 1;
			widget->setBackgroundColor( Color::fromHsv( col ) );
		}
		std::cout << "Time UIWidget total: " << total.getElapsedTime().asMilliseconds() << " ms"
				  << std::endl;

		/*UIWindow* wind = UIWindow::New();
		wind->setSize( 500, 500 );
		wind->setWinFlags( UI_WIN_DEFAULT_FLAGS | UI_WIN_RESIZEABLE | UI_WIN_MAXIMIZE_BUTTON );

		UILinearLayout* layWin = UILinearLayout::NewVertical();
		layWin->setLayoutSizePolicy( SizePolicy::MatchParent, SizePolicy::MatchParent );
		layWin->setParent( wind );

		UILinearLayout* layPar = UILinearLayout::NewHorizontal();
		layPar->setParent( layWin );
		layPar->setLayoutMargin( Rect( 10, 10, 10, 10 ) );
		layPar->setLayoutSizePolicy( SizePolicy::MatchParent, SizePolicy::WrapContent );
		layPar->setLayoutGravity( UI_VALIGN_CENTER | UI_HALIGN_CENTER );
		layPar->setBackgroundColor( 0x999999FF );

		UILinearLayout* lay = UILinearLayout::NewVertical();
		lay->setLayoutGravity( UI_HALIGN_CENTER | UI_VALIGN_CENTER );
		lay->setLayoutSizePolicy( SizePolicy::MatchParent, SizePolicy::WrapContent );
		lay->setBackgroundColor( 0x333333FF );
		lay->setLayoutWeight( 0.7f );

		UITextView::New()
			->setText( "Text on test 1" )
			->setLayoutMargin( Rect( 10, 10, 10, 10 ) )
			->setLayoutSizePolicy( SizePolicy::WrapContent, SizePolicy::WrapContent )
			->setParent( lay );
		UITextView::New()
			->setText( "Text on test 2" )
			->setLayoutMargin( Rect( 10, 10, 10, 10 ) )
			->setLayoutSizePolicy( SizePolicy::MatchParent, SizePolicy::WrapContent )
			->setParent( lay );
		UICheckBox::New()
			->setText( "Checkbox" )
			->setLayoutMargin( Rect( 10, 10, 10, 10 ) )
			->setLayoutSizePolicy( SizePolicy::MatchParent, SizePolicy::WrapContent )
			->setParent( lay );
		UITextView::New()
			->setText( "Text on test 3" )
			->setLayoutMargin( Rect( 10, 10, 10, 10 ) )
			->setLayoutSizePolicy( SizePolicy::MatchParent, SizePolicy::WrapContent )
			->setParent( lay );
		UITextView::New()
			->setText( "Text on test 4" )
			->setLayoutMargin( Rect( 10, 10, 10, 10 ) )
			->setLayoutSizePolicy( SizePolicy::MatchParent, SizePolicy::WrapContent )
			->setParent( lay );
		UITextInput::New()
			->setLayoutMargin( Rect( 10, 10, 10, 10 ) )
			->setLayoutSizePolicy( SizePolicy::MatchParent, SizePolicy::WrapContent )
			->setParent( lay );

		UILinearLayout* lay2 = UILinearLayout::NewVertical();
		lay2->setId( "hardlay" );
		lay2->setLayoutGravity( UI_HALIGN_CENTER | UI_VALIGN_CENTER );
		lay2->setLayoutSizePolicy( SizePolicy::Fixed, SizePolicy::WrapContent );
		lay2->setBackgroundColor( Color::Black );
		lay2->setLayoutWeight( 0.3f );

		UIPushButton::New()
			->setText( "PushButton" )
			->setLayoutMargin( Rect( 10, 10, 10, 10 ) )
			->setLayoutSizePolicy( SizePolicy::MatchParent, SizePolicy::WrapContent )
			->setLayoutGravity( UI_VALIGN_CENTER )
			->setParent( lay2 );
		UIListBox* lbox = UIListBox::New();
		lbox->setLayoutMargin( Rect( 10, 10, 10, 10 ) )
			->setLayoutSizePolicy( SizePolicy::MatchParent, SizePolicy::Fixed )
			->setSize( 0, 105 )
			->setParent( lay2 );
		lbox->addListBoxItems( {"This", "is", "a", "ListBox"} );
		lay2->setParent( layPar );
		lay->setParent( layPar );

		UIDropDownList* drop = UIDropDownList::New();
		drop->setLayoutMargin( Rect( 10, 10, 10, 10 ) )
			->setLayoutSizePolicy( SizePolicy::MatchParent, SizePolicy::WrapContent )
			->setParent( layWin );
		drop->getListBox()->addListBoxItems( {"Car", "Bus", "Plane", "Submarine"} );
		drop->getListBox()->setSelected( 0 );
		wind->show();*/

		win->runMainLoop( &mainLoop );
	}

	Engine::destroySingleton();
	MemoryManager::showResults();

	return EXIT_SUCCESS;
}
