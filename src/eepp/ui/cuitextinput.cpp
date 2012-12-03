#include <eepp/ui/cuitextinput.hpp>
#include <eepp/ui/cuimanager.hpp>
#include <eepp/window/cengine.hpp>

namespace EE { namespace UI {

cUITextInput::cUITextInput( const cUITextInput::CreateParams& Params ) :
	cUITextBox( Params ),
	mCursorPos(0),
	mAllowEditing( true ),
	mShowingWait( true )
{
	mTextBuffer.Start();
	mTextBuffer.Active( false );
	mTextBuffer.SupportFreeEditing( Params.SupportFreeEditing );
	mTextBuffer.MaxLength( Params.MaxLength );
	mTextBuffer.SetReturnCallback( cb::Make0( this, &cUITextInput::PrivOnPressEnter ) );

	ApplyDefaultTheme();
}

cUITextInput::~cUITextInput() {
}

Uint32 cUITextInput::Type() const {
	return UI_TYPE_TEXTINPUT;
}

bool cUITextInput::IsType( const Uint32& type ) const {
	return cUITextInput::Type() == type ? true : cUITextBox::IsType( type );
}

void cUITextInput::Update() {
	cUITextBox::Update();

	if ( mTextBuffer.ChangedSinceLastUpdate() ) {
		eeVector2f offSet = mAlignOffset;

		cUITextBox::Text( mTextBuffer.Buffer() );

		mAlignOffset = offSet;

		ResetWaitCursor();

		AlignFix();

		mCursorPos = mTextBuffer.CurPos();

		mTextBuffer.ChangedSinceLastUpdate( false );

		return;
	}

	if ( mCursorPos != mTextBuffer.CurPos() ) {
		AlignFix();
		mCursorPos = mTextBuffer.CurPos();
		OnCursorPosChange();
	}
}

void cUITextInput::OnCursorPosChange() {
	SendCommonEvent( cUIEvent::EventOnCursorPosChange );
}

void cUITextInput::Draw() {
	cUITextBox::Draw();

	if ( mVisible && mTextBuffer.Active() && mTextBuffer.SupportFreeEditing() ) {
		mWaitCursorTime += cUIManager::instance()->Elapsed();

		if ( mShowingWait ) {
			bool disableSmooth = mShowingWait && GLi->IsLineSmooth();

			if ( disableSmooth )
				GLi->LineSmooth( false );

			cPrimitives P;
			P.SetColor( mFontColor );

			eeFloat CurPosX = mScreenPos.x + mAlignOffset.x + mCurPos.x + 1 + mPadding.Left;
			eeFloat CurPosY = mScreenPos.y + mAlignOffset.y + mCurPos.y		+ mPadding.Top;

			if ( CurPosX > (eeFloat)mScreenPos.x + (eeFloat)mSize.x )
				CurPosX = (eeFloat)mScreenPos.x + (eeFloat)mSize.x;

			P.DrawLine( CurPosX, CurPosY, CurPosX, CurPosY + mTextCache->Font()->GetFontHeight(), 1.f );

			if ( disableSmooth )
				GLi->LineSmooth( true );
		}

		if ( mWaitCursorTime >= 500.f ) {
			mShowingWait = !mShowingWait;
			mWaitCursorTime = 0.f;
		}
	}
}

Uint32 cUITextInput::OnFocus() {
	cUIControlAnim::OnFocus();

	if ( mAllowEditing ) {
		mTextBuffer.Active( true );

		ResetWaitCursor();
	}

	return 1;
}

Uint32 cUITextInput::OnFocusLoss() {
	mTextBuffer.Active( false );

	cUITextBox::OnFocusLoss();

	return 1;
}

Uint32 cUITextInput::OnPressEnter() {
	SendCommonEvent( cUIEvent::EventOnPressEnter );
	return 0;
}

void cUITextInput::PrivOnPressEnter() {
	OnPressEnter();
}

void cUITextInput::PushIgnoredChar( const Uint32& ch ) {
	mTextBuffer.PushIgnoredChar( ch );
}

void cUITextInput::ResetWaitCursor() {
	mShowingWait = true;
	mWaitCursorTime = 0.f;
}

void cUITextInput::AlignFix() {
	if ( FontHAlignGet( Flags() ) == UI_HALIGN_LEFT ) {
		Uint32 NLPos	= 0;
		Uint32 LineNum	= mTextBuffer.GetCurPosLinePos( NLPos );

		mTextCache->Font()->SetText( mTextBuffer.Buffer().substr( NLPos, mTextBuffer.CurPos() - NLPos ) );

		eeFloat tW	= mTextCache->Font()->GetTextWidth();
		eeFloat tX	= mAlignOffset.x + tW;

		mCurPos.x	= tW;
		mCurPos.y	= (eeFloat)LineNum * (eeFloat)mTextCache->Font()->GetFontHeight();

		if ( !mTextBuffer.SupportNewLine() ) {
			if ( tX < 0.f )
				mAlignOffset.x = -( mAlignOffset.x + ( tW - mAlignOffset.x ) );
			else if ( tX > mSize.Width() - mPadding.Left - mPadding.Right )
				mAlignOffset.x = mSize.Width() - mPadding.Left - mPadding.Right - ( mAlignOffset.x + ( tW - mAlignOffset.x ) );
		}
	}
}

void cUITextInput::SetTheme( cUITheme * Theme ) {
	cUIControl::SetThemeControl( Theme, "textinput" );

	AutoPadding();
	AutoSize();
}

void cUITextInput::AutoSize() {
	if ( mFlags & UI_AUTO_SIZE ) {
		Size( mSize.x, GetSkinSize().Height() );
	}
}

void cUITextInput::AutoPadding() {
	if ( mFlags & UI_AUTO_PADDING ) {
		mPadding = MakePadding( true, true, false, false );
	}
}

cInputTextBuffer * cUITextInput::GetInputTextBuffer() {
	return &mTextBuffer;
}

void cUITextInput::AllowEditing( const bool& allow ) {
	mAllowEditing = allow;

	if ( !mAllowEditing && mTextBuffer.Active() )
		mTextBuffer.Active( false );
}

const bool& cUITextInput::AllowEditing() const {
	return mAllowEditing;
}

void cUITextInput::Text( const String& text ) {
	cUITextBox::Text( text );

	mTextBuffer.Buffer( text );

	mTextBuffer.CursorToEnd();
}

const String& cUITextInput::Text() {
	return cUITextBox::Text();
}

void cUITextInput::ShrinkText( const Uint32& MaxWidth ) {
	mTextCache->Text( mTextBuffer.Buffer() );

	cUITextBox::ShrinkText( MaxWidth );

	mTextBuffer.Buffer( mTextCache->Text() );

	AlignFix();
}

}}