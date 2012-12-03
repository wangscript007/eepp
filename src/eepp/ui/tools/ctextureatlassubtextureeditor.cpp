#include <eepp/ui/tools/ctextureatlassubtextureeditor.hpp>
#include <eepp/ui/tools/ctextureatlaseditor.hpp>
#include <eepp/graphics/cprimitives.hpp>
#include <eepp/ui/cuimanager.hpp>

namespace EE { namespace UI { namespace Tools {

cTextureAtlasSubTextureEditor::cTextureAtlasSubTextureEditor( const cUIComplexControl::CreateParams& Params, cTextureAtlasEditor * Editor ) :
	cUIComplexControl( Params ),
	mGfx( NULL ),
	mEditor( Editor )
{
	if ( NULL == cUIThemeManager::instance()->DefaultTheme() ) {
		return;
	}

	mTheme = cUIThemeManager::instance()->DefaultTheme();

	mGfx = mTheme->CreateGfx( NULL, this );

	cUIDragable::CreateParams DragParams;
	DragParams.Parent( this );
	DragParams.SizeSet( 500000, 500000 );
	mDrag = eeNew( cUIDragable, ( DragParams ) );
	mDrag->Enabled( true );
	mDrag->Visible( true );
	mDrag->DragEnable( true );

	GetCenter();
}

cTextureAtlasSubTextureEditor::~cTextureAtlasSubTextureEditor() {
}

void cTextureAtlasSubTextureEditor::Draw() {
	cPrimitives P;
	P.SetColor( eeColorA( 255, 0, 0, mAlpha ) );
	P.DrawLine( mScreenPos.x, mScreenPos.y + mUICenter.y, mScreenPos.x + mSize.Width(), mScreenPos.y + mUICenter.y );
	P.DrawLine( mScreenPos.x + mUICenter.x, mScreenPos.y, mScreenPos.x + mUICenter.x, mScreenPos.y + mSize.Height() );

	cUIComplexControl::Draw();
}

void cTextureAtlasSubTextureEditor::Update() {
	eeVector2i Pos = mDrag->Pos();

	cUIComplexControl::Update();

	if ( NULL != mGfx->SubTexture() && mDrag->DragEnable() && mDrag->Dragging() && Pos != mDrag->Pos() ) {
		eeVector2i Diff = -( Pos - mDrag->Pos() );

		mGfx->SubTexture()->OffsetX( mGfx->SubTexture()->OffsetX() + Diff.x );
		mGfx->SubTexture()->OffsetY( mGfx->SubTexture()->OffsetY() + Diff.y );

		mEditor->SpinOffX()->Value( mGfx->SubTexture()->OffsetX() );
		mEditor->SpinOffY()->Value( mGfx->SubTexture()->OffsetY() );
	}

	mGfx->Pos( mUICenter );
}

void cTextureAtlasSubTextureEditor::OnSizeChange() {
	GetCenter();
}

cSubTexture * cTextureAtlasSubTextureEditor::SubTexture() const {
	return mGfx->SubTexture();
}

void cTextureAtlasSubTextureEditor::SubTexture( cSubTexture * subTexture ) {
	mGfx->SubTexture( subTexture );
}

cUIGfx * cTextureAtlasSubTextureEditor::Gfx() const {
	return mGfx;
}

void cTextureAtlasSubTextureEditor::GetCenter() {
	mUICenter = eeVector2i( mSize.Width() / 2, mSize.Height() / 2 );
}

}}}