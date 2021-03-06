#ifndef EE_UIUIWIDGET_HPP
#define EE_UIUIWIDGET_HPP

#include <eepp/ui/css/propertydefinition.hpp>
#include <eepp/ui/css/stylesheetproperty.hpp>
#include <eepp/ui/css/stylesheetselector.hpp>
#include <eepp/ui/uinode.hpp>

namespace pugi {
class xml_node;
}

namespace EE { namespace UI { namespace CSS {
class PropertyDefinition;
}}} // namespace EE::UI::CSS

using namespace EE::UI::CSS;

namespace EE { namespace UI {

class UITooltip;
class UIStyle;

class EE_API UIWidget : public UINode {
  public:
	static UIWidget* New();

	static UIWidget* NewWithTag( const std::string& tag );

	UIWidget();

	virtual ~UIWidget();

	virtual Uint32 getType() const;

	virtual bool isType( const Uint32& type ) const;

	virtual Node* setSize( const Sizef& size );

	virtual UINode* setFlags( const Uint32& flags );

	virtual UINode* unsetFlags( const Uint32& flags );

	virtual UIWidget* setAnchors( const Uint32& flags );

	virtual void setTheme( UITheme* Theme );

	virtual UINode* setThemeSkin( const std::string& skinName );

	virtual UINode* setThemeSkin( UITheme* Theme, const std::string& skinName );

	virtual Node* setSize( const Float& Width, const Float& Height );

	virtual Node* setId( const std::string& id );

	virtual const Sizef& getSize() const;

	UITooltip* getTooltip();

	void tooltipRemove();

	UIWidget* setTooltipText( const String& Text );

	String getTooltipText();

	void updateAnchorsDistances();

	const Rectf& getLayoutMargin() const;

	const Rectf& getLayoutPixelsMargin() const;

	UIWidget* setLayoutMargin( const Rectf& margin );

	UIWidget* setLayoutMarginLeft( const Float& marginLeft );

	UIWidget* setLayoutMarginRight( const Float& marginRight );

	UIWidget* setLayoutMarginTop( const Float& marginTop );

	UIWidget* setLayoutMarginBottom( const Float& marginBottom );

	Float getLayoutWeight() const;

	UIWidget* setLayoutWeight( const Float& weight );

	Uint32 getLayoutGravity() const;

	UIWidget* setLayoutGravity( const Uint32& layoutGravity );

	const SizePolicy& getLayoutWidthPolicy() const;

	UIWidget* setLayoutWidthPolicy( const SizePolicy& widthPolicy );

	const SizePolicy& getLayoutHeightPolicy() const;

	UIWidget* setLayoutHeightPolicy( const SizePolicy& heightPolicy );

	UIWidget* setLayoutSizePolicy( const SizePolicy& widthPolicy, const SizePolicy& heightPolicy );

	UIWidget* setLayoutPositionPolicy( const PositionPolicy& layoutPositionPolicy, UIWidget* of );

	UIWidget* getLayoutPositionPolicyWidget() const;

	PositionPolicy getLayoutPositionPolicy() const;

	virtual void loadFromXmlNode( const pugi::xml_node& node );

	void notifyLayoutAttrChange();

	void notifyLayoutAttrChangeParent();

	void setStyleSheetInlineProperty( const std::string& name, const std::string& value,
									  const Uint32& specificity = UINT32_MAX -
																  1 /*SpecificityInline*/ );

	virtual bool applyProperty( const StyleSheetProperty& attribute );

	const Rectf& getPadding() const;

	const Rectf& getPixelsPadding() const;

	UIWidget* setPadding( const Rectf& padding );

	UIWidget* setPaddingLeft( const Float& paddingLeft );

	UIWidget* setPaddingRight( const Float& paddingRight );

	UIWidget* setPaddingTop( const Float& paddingTop );

	UIWidget* setPaddingBottom( const Float& paddingBottom );

	const std::string& getStyleSheetTag() const;

	const std::string& getStyleSheetId() const;

	const std::vector<std::string>& getStyleSheetClasses() const;

	UIWidget* getStyleSheetParentElement() const;

	UIWidget* getStyleSheetPreviousSiblingElement() const;

	UIWidget* getStyleSheetNextSiblingElement() const;

	const std::vector<std::string>& getStyleSheetPseudoClasses() const;

	void addClass( const std::string& cls );

	void addClasses( const std::vector<std::string>& classes );

	void removeClass( const std::string& cls );

	void removeClasses( const std::vector<std::string>& classes );

	bool hasClass( const std::string& cls ) const;

	void setElementTag( const std::string& tag );

	const std::string& getElementTag() const;

	virtual void pushState( const Uint32& State, bool emitEvent = true );

	virtual void popState( const Uint32& State, bool emitEvent = true );

	UIStyle* getUIStyle() const;

	void reloadStyle( const bool& reloadChilds = true, const bool& disableAnimations = false,
					  const bool& reportStateChange = true );

	void beginAttributesTransaction();

	void endAttributesTransaction();

	const Uint32& getStyleState() const;

	const Uint32& getStylePreviousState() const;

	std::vector<UIWidget*> findAllByClass( const std::string& className );

	std::vector<UIWidget*> findAllByTag( const std::string& tag );

	UIWidget* findByClass( const std::string& className );

	template <typename T> T* findByClass( const std::string& className ) {
		return reinterpret_cast<T*>( findByClass( className ) );
	}

	UIWidget* findByTag( const std::string& tag );

	template <typename T> T* findByTag( const std::string& tag ) {
		return reinterpret_cast<T*>( findByTag( tag ) );
	}

	UIWidget* querySelector( const CSS::StyleSheetSelector& selector );

	UIWidget* querySelector( const std::string& selector );

	template <typename T> T* querySelector( const std::string& selector ) {
		return reinterpret_cast<T*>( querySelector( selector ) );
	}

	std::vector<UIWidget*> querySelectorAll( const CSS::StyleSheetSelector& selector );

	std::vector<UIWidget*> querySelectorAll( const std::string& selector );

	std::string getPropertyString( const std::string& property );

	virtual std::string getPropertyString( const PropertyDefinition* propertyDef,
										   const Uint32& propertyIndex = 0 );

	bool isSceneNodeLoading() const;

	const std::string& getMinWidthEq() const;

	void setMinSizeEq( const std::string& minWidthEq, const std::string& minHeightEq );

	void setMinWidthEq( const std::string& minWidthEq );

	const std::string& getMinHeightEq() const;

	void setMinHeightEq( const std::string& minHeightEq );

	const std::string& getMaxWidthEq() const;

	void setMaxSizeEq( const std::string& maxWidthEq, const std::string& maxHeightEq );

	void setMaxWidthEq( const std::string& maxWidthEq );

	const std::string& getMaxHeightEq() const;

	void setMaxHeightEq( const std::string& maxHeightEq );

	void reportStyleStateChangeRecursive( bool disableAnimations = false );

	void createTooltip();

	Sizef getCurrentMinSize();

	Sizef getCurrentMaxSize();

	bool isTabStop() const;

	UIWidget* getNextTabWidget() const;

  protected:
	friend class UIManager;
	friend class UISceneNode;
	friend class UIEventDispatcher;

	std::string mTag;
	UITheme* mTheme;
	UIStyle* mStyle;
	UITooltip* mTooltip;
	Rect mDistToBorder;
	Rectf mLayoutMargin;
	Rectf mLayoutMarginPx;
	Rectf mPadding;
	Rectf mPaddingPx;
	Float mLayoutWeight;
	Uint32 mLayoutGravity;
	SizePolicy mWidthPolicy;
	SizePolicy mHeightPolicy;
	PositionPolicy mLayoutPositionPolicy;
	UIWidget* mLayoutPositionPolicyWidget;
	int mAttributesTransactionCount;
	std::string mSkinName;
	std::vector<std::string> mClasses;
	std::vector<std::string> mPseudoClasses;
	std::string mMinWidthEq;
	std::string mMinHeightEq;
	std::string mMaxWidthEq;
	std::string mMaxHeightEq;

	explicit UIWidget( const std::string& tag );

	void updatePseudoClasses();

	virtual void onChildCountChange( Node* child, const bool& removed );

	virtual Uint32 onMouseMove( const Vector2i& Pos, const Uint32& Flags );

	virtual Uint32 onMouseOver( const Vector2i& Pos, const Uint32& Flags );

	virtual Uint32 onMouseLeave( const Vector2i& Pos, const Uint32& Flags );

	virtual void onParentSizeChange( const Vector2f& SizeChange );

	virtual void onPositionChange();

	virtual void onVisibilityChange();

	virtual void onSizeChange();

	virtual void onAutoSize();

	virtual void onWidgetCreated();

	virtual void onPaddingChange();

	virtual void onMarginChange();

	virtual void onThemeLoaded();

	virtual void onParentChange();

	virtual void onClassChange();

	virtual void onTagChange();

	virtual void onTabPress();

	void updateAnchors( const Vector2f& SizeChange );

	void alignAgainstLayout();

	void reportStyleStateChange( bool disableAnimations = false );

	std::string getLayoutWidthPolicyString() const;

	std::string getLayoutHeightPolicyString() const;

	std::string getLayoutGravityString() const;

	std::string getGravityString() const;

	std::string getFlagsString() const;

	bool checkPropertyDefinition( const StyleSheetProperty& property );

	Vector2f getTooltipPosition();

	void createStyle();

	void enableCSSAnimations();

	void disableCSSAnimations();

	void reloadFontFamily();

	UIWidget* getNextWidget() const;
};

}} // namespace EE::UI

#endif
