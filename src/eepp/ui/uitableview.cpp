#include <eepp/ui/uilinearlayout.hpp>
#include <eepp/ui/uipushbutton.hpp>
#include <eepp/ui/uiscenenode.hpp>
#include <eepp/ui/uiscrollbar.hpp>
#include <eepp/ui/uitableview.hpp>

namespace EE { namespace UI {

UITableView* UITableView::New() {
	return eeNew( UITableView, () );
}

UITableView::UITableView( const std::string& tag ) : UIAbstractTableView( tag ) {
	clipEnable();
}

UITableView::UITableView() : UITableView( "tableview" ) {}

Uint32 UITableView::getType() const {
	return UI_TYPE_TABLEVIEW;
}

bool UITableView::isType( const Uint32& type ) const {
	return UITableView::getType() == type ? true : UIAbstractTableView::isType( type );
}

void UITableView::drawChilds() {
	int realIndex = 0;

	size_t start = mScrollOffset.y / getRowHeight();
	size_t end =
		eemin<size_t>( (size_t)eeceil( ( mScrollOffset.y + mSize.getHeight() ) / getRowHeight() ),
					   getItemCount() );
	Float yOffset;
	for ( size_t i = start; i < end; i++ ) {
		yOffset = getHeaderHeight() + i * getRowHeight();
		ModelIndex index( getModel()->index( i ) );
		if ( yOffset - mScrollOffset.y > mSize.getHeight() )
			break;
		if ( yOffset - mScrollOffset.y + getRowHeight() < 0 )
			continue;
		for ( size_t colIndex = 0; colIndex < getModel()->columnCount(); colIndex++ ) {
			if ( columnData( colIndex ).visible ) {
				updateCell( realIndex, getModel()->index( index.row(), colIndex, index.parent() ),
							0, yOffset );
			}
		}
		updateRow( realIndex, index, yOffset )->nodeDraw();
		realIndex++;
	}

	if ( mHeader && mHeader->isVisible() )
		mHeader->nodeDraw();
	if ( mHScroll->isVisible() )
		mHScroll->nodeDraw();
	if ( mVScroll->isVisible() )
		mVScroll->nodeDraw();
}

Node* UITableView::overFind( const Vector2f& point ) {
	mUISceneNode->setIsLoading( true );

	Node* pOver = NULL;
	if ( mEnabled && mVisible ) {
		updateWorldPolygon();
		if ( mWorldBounds.contains( point ) && mPoly.pointInside( point ) ) {
			writeNodeFlag( NODE_FLAG_MOUSEOVER_ME_OR_CHILD, 1 );
			mSceneNode->addMouseOverNode( this );
			if ( mHScroll->isVisible() && ( pOver = mHScroll->overFind( point ) ) )
				return pOver;
			if ( mVScroll->isVisible() && ( pOver = mVScroll->overFind( point ) ) )
				return pOver;
			if ( mHeader && ( pOver = mHeader->overFind( point ) ) )
				return pOver;
			int realIndex = 0;
			Float yOffset;
			size_t start = mScrollOffset.y / getRowHeight();
			size_t end = eemin<size_t>(
				(size_t)eeceil( ( mScrollOffset.y + mSize.getHeight() ) / getRowHeight() ),
				getItemCount() );
			for ( size_t i = start; i < end; i++ ) {
				yOffset = getHeaderHeight() + i * getRowHeight();
				ModelIndex index( getModel()->index( i ) );
				if ( yOffset - mScrollOffset.y > mSize.getHeight() )
					break;
				if ( yOffset - mScrollOffset.y + getRowHeight() < 0 )
					continue;
				for ( size_t colIndex = 0; colIndex < getModel()->columnCount(); colIndex++ ) {
					if ( columnData( colIndex ).visible ) {
						updateCell( realIndex,
									getModel()->index( index.row(), colIndex, index.parent() ), 0,
									yOffset );
					}
				}
				pOver = updateRow( realIndex, index, yOffset )->overFind( point );
				if ( pOver )
					break;
				realIndex++;
			}
			if ( !pOver )
				pOver = this;
		}
	}

	mUISceneNode->setIsLoading( false );
	return pOver;
}

Float UITableView::getMaxColumnContentWidth( const size_t& colIndex ) {
	Float lWidth = 0;
	getUISceneNode()->setIsLoading( true );
	Float yOffset = getHeaderHeight();
	for ( size_t i = 0; i < getItemCount(); i++ ) {
		ModelIndex index( getModel()->index( i, colIndex ) );
		UIWidget* widget = updateCell( 0, index, 0, yOffset );
		if ( widget->isType( UI_TYPE_PUSHBUTTON ) ) {
			Float w = widget->asType<UIPushButton>()->getContentSize().getWidth();
			if ( w > lWidth )
				lWidth = w;
		}
		yOffset += getRowHeight();
	}
	getUISceneNode()->setIsLoading( false );
	return lWidth;
}

void UITableView::createOrUpdateColumns() {
	if ( !getModel() )
		return;
	UIAbstractTableView::createOrUpdateColumns();
	updateContentSize();
}

void UITableView::updateContentSize() {
	Sizef oldSize( mContentSize );
	mContentSize = UIAbstractTableView::getContentSize();
	if ( oldSize != mContentSize )
		onContentSizeChange();
}

void UITableView::onColumnSizeChange( const size_t& ) {
	updateContentSize();
}

Uint32 UITableView::onKeyDown( const KeyEvent& event ) {
	auto curIndex = getSelection().first();
	int pageSize = eefloor( getVisibleArea().getHeight() / getRowHeight() ) - 1;

	switch ( event.getKeyCode() ) {
		case KEY_PAGEUP: {
			if ( curIndex.row() - pageSize < 0 ) {
				getSelection().set( getModel()->index( 0, 0 ) );
				scrollToTop();
			} else {
				moveSelection( -pageSize );
			}
			return 1;
		}
		case KEY_PAGEDOWN: {
			if ( curIndex.row() + pageSize >= (Int64)getModel()->rowCount() ) {
				getSelection().set( getModel()->index( getItemCount() - 1 ) );
				scrollToBottom();
			} else {
				moveSelection( pageSize );
			}
			return 1;
		}
		case KEY_UP: {
			if ( !getModel() )
				return 0;
			auto& model = *this->getModel();
			ModelIndex foundIndex;
			if ( !getSelection().isEmpty() ) {
				auto oldIndex = getSelection().first();
				if ( oldIndex.row() == 0 ) {
					getSelection().set( getModel()->index( 0, 0 ) );
					scrollToTop();
					return 1;
				}
				foundIndex = model.index( oldIndex.row() - 1, oldIndex.column() );
			} else {
				foundIndex = model.index( 0, 0 );
			}
			if ( model.isValid( foundIndex ) ) {
				Float curY = getHeaderHeight() + getRowHeight() * foundIndex.row();
				getSelection().set( foundIndex );
				if ( curY < mScrollOffset.y + getHeaderHeight() + getRowHeight() ||
					 curY > mScrollOffset.y + getPixelsSize().getHeight() - mPaddingPx.Top -
								mPaddingPx.Bottom - getRowHeight() ) {
					curY -= getHeaderHeight();
					mVScroll->setValue( curY / getScrollableArea().getHeight() );
				}
			}
			return 1;
		}
		case KEY_DOWN: {
			if ( !getModel() )
				return 0;
			auto& model = *this->getModel();
			ModelIndex foundIndex;
			if ( !getSelection().isEmpty() ) {
				auto oldIndex = getSelection().first();
				foundIndex = model.index( oldIndex.row() + 1, oldIndex.column() );
			} else {
				foundIndex = model.index( 0, 0 );
			}
			if ( model.isValid( foundIndex ) ) {
				Float curY = getHeaderHeight() + getRowHeight() * foundIndex.row();
				getSelection().set( foundIndex );
				if ( curY < mScrollOffset.y ||
					 curY > mScrollOffset.y + getPixelsSize().getHeight() - mPaddingPx.Top -
								mPaddingPx.Bottom - getRowHeight() ) {
					curY -=
						eefloor( getVisibleArea().getHeight() / getRowHeight() ) * getRowHeight() -
						getRowHeight();
					mVScroll->setValue( curY / getScrollableArea().getHeight() );
				}
			}
			return 1;
		}
		case KEY_END: {
			scrollToBottom();
			getSelection().set( getModel()->index( getItemCount() - 1 ) );
			return 1;
		}
		case KEY_HOME: {
			scrollToTop();
			getSelection().set( getModel()->index( 0, 0 ) );
			return 1;
		}
		case KEY_RETURN:
		case KEY_KP_ENTER: {
			if ( curIndex.isValid() )
				onOpenModelIndex( curIndex, &event );
			return 1;
		}
		default:
			break;
	}
	return UIAbstractTableView::onKeyDown( event );
}

ModelIndex UITableView::findRowWithText( const std::string& text, const bool& caseSensitive,
										 const bool& exactMatch ) {
	Model* model = getModel();
	if ( !model || model->rowCount() == 0 )
		return {};
	size_t rc = model->rowCount();
	for ( size_t i = 0; i < rc; i++ ) {
		ModelIndex index = model->index(
			i, model->keyColumn() != -1 ? model->keyColumn()
										: ( model->treeColumn() >= 0 ? model->treeColumn() : 0 ) );
		Variant var = model->data( index );
		if ( var.isValid() &&
			 ( exactMatch ? var.toString() == text
						  : String::startsWith( caseSensitive ? var.toString()
															  : String::toLower( var.toString() ),
												text ) ) )
			return model->index( index.row(), 0 );
	}
	return {};
}

}} // namespace EE::UI