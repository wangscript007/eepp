# CSS Specification

## WIP

This document is a work in progress. You'll find that many features are currently not documented.

## Introduction

eepp CSS custom implementation is heavily based on the official [CSS standard](https://www.w3.org/TR/CSS/#css).
This document will try to explain the shared features and the current differences with the CSS
specification. Since CSS is widely used and documented, every feature that it's shared with the CSS
standard will be directly linked to the Mozilla CSS documentation. If you are totally new to CSS
please go the [Mozilla CSS portal](https://developer.mozilla.org/en-US/docs/Web/CSS).

## Relevant differences with CSS standard

* Layout properties are not supported (display, float, etc), since eepp layout system differs from
the HTML+CSS specification (design decision).

* [inheritance](https://developer.mozilla.org/en-US/docs/Web/CSS/Inheritance) is not supported,
except for [CSS custom properties](https://developer.mozilla.org/en-US/docs/Web/CSS/--*)
(design decision).

* [Attribute selector](https://developer.mozilla.org/en-US/docs/Web/CSS/Attribute_selectors)
is not supported (probably will not be supported in the near future, since collides with some
implementation decisions).

* CSS relative paths start from the process current working directory instead the relative path from
the CSS file.

* eepp CSS supports alternative path methods besides paths (resource locator to previously loaded
resources by the process).

* eepp CSS supports [Device-independent
pixel](https://en.wikipedia.org/wiki/Device-independent_pixel) unit `dp`.

* CSS files should be always UTF-8 encoded.

## CSS Selectors

CSS selectors define the elements to which a set of CSS rules apply.

---

### Basic selectors

#### Universal Selector

[Universal selector](https://developer.mozilla.org/en-US/docs/Web/CSS/Universal_selectors)
selects all elements. Optionally, it may be restricted to a specific namespace or to all namespaces.

Syntax: `* ns|* *|*`

Example: `*` will match all the elements of the document.

---

#### Type Selector

[Type selector](https://developer.mozilla.org/en-US/docs/Web/CSS/Type_selectors) selects all
elements that have the given node name.

Syntax: `elementname`

Example: `widget` will match any `<widget>` element.

---

#### Class Selector

[Class selector](https://developer.mozilla.org/en-US/docs/Web/CSS/Class_selectors) selects all
elements that have the given class attribute.

Syntax: `.classname`

Example: `.index` will match any element that has a class of "index".

---

#### ID Selector

[ID selector](https://developer.mozilla.org/en-US/docs/Web/CSS/ID_selectors) selects an element
based on the value of its id attribute. There should be only one element with a given ID in a
document.

Syntax: `#idname`

Example: `#toc` will match the element that has the ID "toc".

---

### Grouping selectors

#### Selector List

The [selector list](https://developer.mozilla.org/en-US/docs/Web/CSS/Selector_list) `,` is a
grouping method, it selects all the matching nodes.

Syntax: `A, B`

Example: `widget, image` will match both `<span>` and `<image>` elements.

---

### Combinators

#### Descendant combinator

The [descendant combinator](https://developer.mozilla.org/en-US/docs/Web/CSS/Descendant_combinator)
" " (space) combinator selects nodes that are descendants of the first element.

Syntax: `A B`

Example: `LinearLayout image` will match all `<image>` elements that are inside a `<LinearLayout>`
element.

#### Child combinator

The [child combinator](https://developer.mozilla.org/en-US/docs/Web/CSS/Child_combinator) `>`
selects nodes that are direct children of the first element.

Syntax: `A > B`

Example: `LinearLayout > listbox` will match all `<listbox>` elements that are nested directly
inside a `<LinearLayout>` element.

#### General sibling combinator

The [general sibling combinator](https://developer.mozilla.org/en-US/docs/Web/CSS/General_sibling_combinator)
`~` selects siblings. This means that the second element follows the first (though not necessarily
immediately), and both share the same parent.

Syntax: `A ~ B`

Example: `TextView ~ image` will match all `<image>` elements that follow a `<TextView>`,
immediately or not.

#### Adjacent sibling combinator

The [adjacent sibling combinator](https://developer.mozilla.org/en-US/docs/Web/CSS/Adjacent_sibling_combinator)
`+` selects adjacent siblings. This means that the second element directly follows the first, and
both share the same parent.

Syntax: `A + B`

Example: `TextView + image` will match all `<image>` elements that directly follow an `<TextView>`.

---

## CSS Properties

eepp CSS properties can be categorized in two big groups: CSS standard properties and eepp CSS
properties.

**Definitions**:

* Property names are *case insensitive*.
* Some properties have aliases.
* All property names that contain dashes (`-`), have aliases without those dashes.
  * For example: `animation-delay` also is `animationDelay`.
  * Other aliases will be clarified in the properties documentation.
* All properties can be used as attributes of the XML node.
  * XML attributes are treated as inline CSS styling.
  * For example:
    * Something like:
    * `LinearLayout {  layout-width: match_parent; layout_height: match_parent; orientation: vertical; }`
    * is similar to (but not equivalent, since specificity of inline CSS is higher):
    * `<LinearLayout layout_width="match_parent" layout_height="match_parent" orientation="vertical"></LinearLayout>`
* CSS standard properties directly linked to the Mozilla docs are fully supported, otherwise a clarification will appear.
* _Applicable to_ will link to the EE::UI::UIWidget that can use that property, and betweeen parenthesis is the CSS element name.

---

### animation-delay

Read [animation-delay](https://developer.mozilla.org/en-US/docs/Web/CSS/animation-delay) docs.

---

### animation-direction

Read [animation-direction](https://developer.mozilla.org/en-US/docs/Web/CSS/animation-direction) documenation.

---

### animation-duration

Read [animation-duration](https://developer.mozilla.org/en-US/docs/Web/CSS/animation-duration) documenation.

---

### animation-fill-mode

Read [animation-fill-mode](https://developer.mozilla.org/en-US/docs/Web/CSS/animation-fill-mode) documentation.

---

### animation-iteration-count

Read [animation-iteration-count](https://developer.mozilla.org/en-US/docs/Web/CSS/animation-iteration-count) documentation.

---

### animation-name

Read [animation-name](https://developer.mozilla.org/en-US/docs/Web/CSS/animation-name) documentation.

### animation-play-state

Read [animation-play-state](https://developer.mozilla.org/en-US/docs/Web/CSS/animation-play-state) documentation.

---

### animation-timing-function

Read [animation-timing-function](https://developer.mozilla.org/en-US/docs/Web/CSS/animation-timing-function) documentation.

Timing function names are custom, and not the same as the standard, but `cubic-bezier` will be added
soon. Current timing functions supported: linear, quadratic-in, quadratic-out, quadratic-in-out,
sine-in, sine-out, sine-in-out, exponential-in, exponential-out, exponential-in-out, quartic-in,
quartic-out, quartic-in-out, circular-in, circular-out, circular-in-out, cubic-in, cubic-out,
cubic-in-out, back-in, back-out, back-in-out, bounce-in, bounce-out, bounce-in-out, elastic-in,
elastic-out, elastic-in-out, none.

---

### allow-editing

Enable or disable editing on input elements.

* Applicable to: EE::UI::UITextEdit (TextEdit), EE::UI::UITextInput (TextInput)
* Value Type: boolean
* Default value: `true`

---

### allow-float

On a input that only accept numbers, enables floating point numbers.

* Applicable to: EE::UI::UITextInput (TextInput), EE::UI::UISpinBox (SpinBox::input)
* Value Type: boolean
* Default value: `false`

---

### animation-speed

Sets the loader speed animation.

* Applicable to: EE::UI::UILoader (Loader)
* Value type: float
* Default value: `0.5`

---

### arc-start-angle

The angle value where a loader progress starts rendering.

* Applicable to: EE::UI::UILoader (Loader)
* Value type: float
* Default value: `0`

---

### background-color

Read [background-color](https://developer.mozilla.org/en-US/docs/Web/CSS/background-color) documentation.

---

### background-expand

Sets if the element skin background should be expanded to the element dimensions.

* Applicable to: EE::UI::UISlider (Slider), EE::UI::UIScrollBar (ScrollBar)
* Value Type: boolean
* Default value: `false`

---

### background-image

Read [background-image](https://developer.mozilla.org/en-US/docs/Web/CSS/background-image) documentation.

---

### background-position-x

Read [background-position-x](https://developer.mozilla.org/en-US/docs/Web/CSS/background-position-x) documentation.

---

### background-position-y

Read [background-position-y](https://developer.mozilla.org/en-US/docs/Web/CSS/background-position-y) documentation.

---

### backtround-repeat

Read [background-repeat](https://developer.mozilla.org/en-US/docs/Web/CSS/background-repeat) documentation.
`space` and `round` values are not supported yet.

* Default value: `no-repeat` (this is different from the CSS standard)

---

### background-size

Read [background-size](https://developer.mozilla.org/en-US/docs/Web/CSS/background-size) documentation.

---

### blend-mode

Sets the blend mode to the widget.

* Applicable to: Any element.
* Value Type: string-list
* Value List:
  * `add`
  * `alpha`
  * `multiply`
  * `none`
* Default value: `alpha`

### border-bottom-color

Read [border-bottom-color](https://developer.mozilla.org/en-US/docs/Web/CSS/border-bottom-color) documentation.

---

### border-bottom-left-radius

Read [border-bottom-left-radius](https://developer.mozilla.org/en-US/docs/Web/CSS/border-bottom-left-radius) documentation.

---

### border-bottom-right-radius

Read [border-bottom-right-radius](https://developer.mozilla.org/en-US/docs/Web/CSS/border-bottom-right-radius) documentation.

---

### border-bottom-width

Read [border-bottom-width](https://developer.mozilla.org/en-US/docs/Web/CSS/border-bottom-width) documentation.

---

### border-left-color

Read [border-left-color](https://developer.mozilla.org/en-US/docs/Web/CSS/border-left-color) documentation.

---

### border-left-width

Read [border-left-width](https://developer.mozilla.org/en-US/docs/Web/CSS/border-left-width) documentation.

---

### border-right-color

Read [border-right-color](https://developer.mozilla.org/en-US/docs/Web/CSS/border-right-color) documentation.

---

### border-right-width

Read [border-right-width](https://developer.mozilla.org/en-US/docs/Web/CSS/border-right-width) documentation.

---

### border-top-color

Read [border-top-color](https://developer.mozilla.org/en-US/docs/Web/CSS/border-top-color) documentation.

---

### border-top-left-radius

Read [border-top-left-radius](https://developer.mozilla.org/en-US/docs/Web/CSS/border-top-left-radius) documentation.

---

### border-top-right-radius

Read [border-top-right-radius](https://developer.mozilla.org/en-US/docs/Web/CSS/border-top-right-radius) documentation.

---

### border-top-width

Read [border-top-width](https://developer.mozilla.org/en-US/docs/Web/CSS/border-top-width) documentation.

---

### border-type

Defines where the border box is drawn.

* Applicable to: Any element.
* Value Type: string-list
* Value List:
  * `inside`: The border will be drawn inside the content box.
  * `outside`: The border will be drawn outside the content box.
  * `outline`: The border will be drawn in the middle point between the content box and the border width.

---

### change-page-percent

Sets the percentage of scroll of a page that triggers a page change in a ViewPager.

* Applicable to: EE::UI::UIViewPager (ViewPager).
* Value Type: float (normalized between `0` and `1`)
* Default value: `0.33`

---

### click-step

For any element that has a stepped value change, this sets the step variation for a click to the
element.

* Applicable to: EE::UI::UIScrollBar (ScrollBar), EE::UI::UISlider (Slider), EE::UI::UISpinBox (SpinBox).
* Value Type: float
* Default value:
  * ScrollBar: `0.1`
  * Slider: `0.1`
  * SpinBox: `1`

---

### clip

Enables/disables clipping to the element box.

* Applicable to: Any element.
* Value Type: boolean
* Default Value: Varies for each widget/element.

---

### color

Read [color](https://developer.mozilla.org/en-US/docs/Web/CSS/color) documentation.

---

### column-mode

Sets the column mode of a grid layout.
The column mode defines how the size of the column of a grid layout is measured.

* Applicable to: EE::UI::UIGridLayout (GridLayout)
* Value Type: string-list
* Value List:
  * `size`: The column width of each grid child will be defined with a fixed width based on the value of the `column-width` length.
  * `weight`: The column width of each grid child will be calculated as a normalized percentage of the total grid width using the `column-weight` value.
* Default value: `weight`

---

### column-margin

Sets the horizontal separation between each element in the grid layout.

* Applicable to: EE::UI::UIGridLayout (GridLayout)
* Value Type: length
* Default value: `0dp`

---

### column-weight

Sets the percentage width of the child elements of a grid layout.
`column-mode` must be `weight` in order to work.

* Applicable to: EE::UI::UIGridLayout (GridLayout)
* Value Type: float (normalized between `0` and `1`)
* Default value: `0.25`

---

### column-width

Sets the width of the child elements of a grid layout.

* Applicable to: EE::UI::UIGridLayout (GridLayout)
* Value Type: length
* Default value: `0`

---

### cursor

Read [cursor](https://developer.mozilla.org/en-US/docs/Web/CSS/cursor) documentation.

Custom cursors not yet supported (but supported by the engine, only not implemented in the CSS side).

---

### display-percent

Enables/disables displaying the percentage of progress in the progress bar.

* Applicable to: EE::UI::UIProgressBar (ProgressBar)
* Value Type: boolean
* Default value: `false`

---

### drag-resistance

Sets the amount of drag that is needed to start dragging an element.

* Applicable to: EE::UI::UIViewPager (ViewPager)
* Value Type: length
* Default value: `8dp`

---

### enabled

Sets if the element is enabled

* Applicable to: Any element.
* Value Type: boolean
* Default value: `true`

---

### font-family

Read [font-family](https://developer.mozilla.org/en-US/docs/Web/CSS/font-family) documentation.

---

### font-size

Read [font-size](https://developer.mozilla.org/en-US/docs/Web/CSS/font-size) documentation.

---

### font-style

Read [font-style](https://developer.mozilla.org/en-US/docs/Web/CSS/font-style) documentation.

---

### fill-color

Sets the fill color of a element.

* Applicable to: EE::UI::UILoader (Loader)
* Value Type: color
* Default value: `white`

---

### flags

Allows to set some very specific flags to the element.
Multiple flags can be set, flags are separated by `|`.

* Applicable to: Any element.
* Value Type: string-list
* Value List:
  * `autosize`: This flag allows the widget/element to resize itself to a minimum size that allows the widget to be fully visible.
  * `clip`: Enables clipping of the element box.
  * `multiselect`: Enables multiple selection on elements that support selection. EE::UI::UIListBox (ListBox) for the moment.
  * `autopadding`: Enables the element to calculate the padding based on the skin size.
  * `reportsizechangetochilds`: When enabled the element will emmit a event (`OnParentSizeChange`) to its childs reporting the size change of the parent.
* Default value: _No value_

---

### foreground-color

Same as [background-color](#background-color) but for the foreground.

---

### foreground-image

Same as [background-image](#background-image) but for the foreground.

---

### foreground-position-x

Same as [background-position-x](#background-position-x) but for the foreground.

---

### foreground-position-y

Same as [background-position-y](#background-position-y) but for the foreground.

---

### foreground-repeat

Same as [background-repeat](#background-repeat) but for the foreground.

---

### foreground-size

Same as [background-size](#background-size) but for the foreground.

---

### free-editing

Allows the cursor to move freely in an input.

* Applicable to: EE::UI::UITextInput (TextInput)
* Value Type: boolean
* Default value: `true`

---

### gravity

Sets the horizontal and vertical align of the inner elements/contents of a widget (for example the
text on a TextView). Vertical and horizontal can be set by using `|` as a separator of each rule.

* Applicable to: Any element. This applies at the EE::UI::UIWidget (Widget) level, the base element
  of all widgets. But every widget has it's own behaviour, based on it's own content. Some may not
  do anything.
* Value Type: string-list.
* Value List:
  * `left`
  * `right`
  * `center_horizontal`
  * `top`
  * `bottom`
  * `center_vertical`
  * `center`: shorthand of `center_horizontal|center_vertical`
* Default value: Varies on each widget implementation.
* Syntax:

```CSS
gravity: center;
gravity: left;
gravity: left|top;
gravity: center_horizontal|bottom;
```

---

### half-slider

Defines how the slider widget should set its movement limits against the slider.
With `half-slider` enabled the button will start and end from its center (and not its border).

* Applicable to: EE::UI::UISlider (Slider)
* Value Type: boolean
* Default value: `false`

---

### hint

Sets the hint text of an input. This is the equivalen of `placeholder` in HTML.
Usually this property will be set not in the CSS file but in the XML or by code, but it's
available as an option.

* Applicable to: EE::UI::UITextInput (TextInput)
* Value Type: string
* Default value: _No value_

---

### hint-color

Sets the hint font color.

* Applicable to: EE::UI::UITextInput (TextInput)
* Value Type: color
* Default value: `white`

---

### hint-font-family

Sets the hint font family.

* Applicable to: EE::UI::UITextInput (TextInput)
* Value Type: string
* Default value: _No value_

---

### hint-font-size

Sets the hint font size.

* Applicable to: EE::UI::UITextInput (TextInput)
* Value Type: length
* Default value: `12px`

---

### hint-font-style

Sets the hint font style.

* Applicable to: EE::UI::UITextInput (TextInput)
* Value Type: string-list
* Value List:
  * `underline`
  * `bold`
  * `italic`
  * `strikethrough`
  * `shadow`: Drops a shadow to the text.
* Default value: _No value_

---

### hint-shadow-color

Sets the hint font shadow color.

* Applicable to: EE::UI::UITextInput (TextInput)
* Value Type: color
* Default value: `#323232E6`

---

### hint-stroke-color

Sets the hint font stroke (the outline) color.

* Applicable to: EE::UI::UITextInput (TextInput)
* Value Type: color
* Default value: `black`

---

### hint-stroke-width

Sets the hint font stroke (the outline) width.

* Applicable to: EE::UI::UITextInput (TextInput)
* Value Type: length
* Default value: `0dp`

---

### hscroll-mode

Sets the horizontal scroll mode to any scrollable element.
The mode defines the visibility of the horizontal scroll.

* Applicable to: EE::UI::DropDownList (DropDownList), EE::UI::UIListBox (ListBox),
  EE::UI::UIScrollView (ScrollView), EE::UI::UITable (Table), EE::UI::UITextEdit (TextEdit).
* Value Type: string-list
* Value List:
  * `auto`: The horizontal scrollbar will be visible only when needed.
  * `on`: The horizontal scrollbar will be always visible.
  * `off`: The horizontal scrollbar will be always hidden.
* Default value: `auto`

---

### icon

Sets an icon to an element that support icons.

* Applicable to: EE::UI::UIImage (Image), EE::UI::UIPushButton (PushButton), EE::UI::UITab (Tab)
* Value Type: resource-path (`url()`)
* Default value: _No value_

---

### icon-auto-margin

When enabled automatically calculates the optimal horizontal position of an icon in a push button.

* Applicable to: EE::UI::UIPushButton (PushButton)
* Value Type: boolean
* Default value: `true`

---

### icon-horizontal-margin

Manually sets the horizontal position of an icon in a push button.

* Applicable to: EE::UI::UIPushButton (PushButton)
* Value Type: length
* Default value: `4dp`

---

### indeterminate

Enables/disables indeterminate mode for a loader. Indeterminate mode is used when the progress of
the load can't be determined.

* Applicable to: EE::UI::UILoader (Loader)
* Value Type: boolean
* Default value: `true`

---

### layout-gravity

The layout gravity defines how the element gravitates againts its parent (when possible). Gravity
specifies how a component should be placed in its group of cells. Must be one or more (separated by
`|`) of the _value list_ values. This behaves as the [android:layout_gravity](https://developer.android.com/reference/android/widget/LinearLayout.LayoutParams#attr_android:layout_gravity)
XML attribute (for those who are familiar with).

* Applicable to: Any element child of a layout element
* Value Type: string-list
* Value List:
  * `left`: Push element to the left of its container.
  * `right`: Push element to the right of its container.
  * `center_horizontal`: Push element to the horizontal center of its container.
  * `top`: Push element to the top of its container.
  * `bottom`: Push element to the bottom of its container.
  * `center_vertical`: Place element in the vertical center of its container.
  * `center`: shorthand of `center_horizontal|center_vertical`
* Default value: `left|top`
* Aliases: `layout_gravity`

---

### layout-height

Specifies the basic height of the element. This is a required attribute for any element inside of a
containing layout element that doesn't use any fixed size for its childs (ej: EE::UI::UILinearLayout,
EE::UI::UIRelativeLayout). Its value may be a length/dimension (such as `12dp`) for a constant height
or one of the special constants.

* Applicable to: Any element child of a EE::UI::UILinearLayout (LinearLayout) or
  EE::UI::UIRelativeLayout (RelativeLayout)
* Value Type: length or string-list
* Value List: (for the constant values)
  * `match_parent`: The element must be as big as its parent.
  * `wrap_content`: The element should be only big enough to enclose its content (plus padding).
* Default value: `wrap_content`

---

### layout-to-bottom-of

Positions the top edge of this element below the given anchor element ID.
Accommodates top margin of this element and bottom margin of anchor view.

* Applicatable to: Any child of a EE::UI::UIRelativeLayout (RelativeLayout)
* Value Type: string (anchor element ID)
* Default value: _No value_

---

### layout-to-left-of

Positions the right edge of this element to the left of the given anchor element ID.
Accommodates right margin of this element and left margin of anchor element.

* Applicatable to: Any child of a EE::UI::UIRelativeLayout (RelativeLayout)
* Value Type: string (anchor element ID)
* Default value: _No value_

---

### layout-to-right-of

Positions the left edge of this element to the right of the given anchor element ID. Accommodates
left margin of this element and right margin of anchor element.

* Applicatable to: Any child of a EE::UI::UIRelativeLayout (RelativeLayout)
* Value Type: string (anchor element ID)
* Default value: _No value_

---

### layout-to-top-of

Positions the bottom edge of this element above the given anchor element ID.
Accommodates bottom margin of this element and top margin of anchor view.

* Applicatable to: Any child of a EE::UI::UIRelativeLayout (RelativeLayout)
* Value Type: string (anchor element ID)
* Default value: _No value_

---

### layout-weight

Indicates how much of the extra space in the EE::UI::UILinearLayout (LinearLayout) is allocated to
the element. Specify 0 if the element should not be stretched. Otherwise the extra pixels will be
pro-rated among all views whose weight is greater than 0. The weight sum of all elements must be 1
to fill the whole parent length. The weight is applied to the orientation that the LinearLayout uses.
This property behaves the same as the [android:layout_weight](https://developer.android.com/reference/android/widget/LinearLayout.LayoutParams#attr_android:layout_weight)
XML attribute in Android (for those who are familiar with it).
LinearLayout supports assigning a weight to individual children. This attribute assigns an
"importance" value to a element, and allows it to expand to fill any remaining space in the parent
view. Element/widget default weight is zero.
In order to work properly you must set the [layout-width](#layout-width) or the
[layout-height](#layout-height) to `0` to the element that uses layout weight. If the parent is a
horizontal LinearLayout the [layout-width](#layout-width) must be set to `0`, and if the parent if a
vertical LinearLayout the [layout-height](#layout-height) must be set to `0`, in order to use the
layout weight.

* Applicable to: Any element child of a EE::UI::UILinearLayout (LinearLayout)
* Value Type: float (normalized from `0` to `1`)
* Default value: `0`

---

### layout-width

Specifies the basic width of the element. This is a required attribute for any element inside of a
containing layout element that doesn't use any fixed size for its childs (ej: EE::UI::UILinearLayout,
EE::UI::UIRelativeLayout). Its value may be a length/dimension (such as `12dp`) for a constant width
or one of the special constants.

* Applicable to: Any element child of a EE::UI::UILinearLayout (LinearLayout) or
  EE::UI::UIRelativeLayout (RelativeLayout)
* Value Type: length or string-list
* Value List: (for the constant values)
  * `match_parent`: The element must be as big as its parent.
  * `wrap_content`: The element should be only big enough to enclose its content (plus padding).
* Default value: `wrap_content`

---

### line-below-tabs

---

### line-below-tabs-color

---

### line-below-tabs-y-offset

---

### margin-bottom

Read [margin-bottom](https://developer.mozilla.org/en-US/docs/Web/CSS/margin-bottom) documentation.

---

### margin-left

Read [margin-left](https://developer.mozilla.org/en-US/docs/Web/CSS/margin-left) documentation.

---

### margin-right

Read [margin-right](https://developer.mozilla.org/en-US/docs/Web/CSS/margin-right) documentation.

---

### margin-top

Read [margin-top](https://developer.mozilla.org/en-US/docs/Web/CSS/margin-top) documentation.

---

### max-height

Read [max-height](https://developer.mozilla.org/en-US/docs/Web/CSS/max-height) documentation.

---

### max-width

Read [max-width](https://developer.mozilla.org/en-US/docs/Web/CSS/max-width) documentation.

---

### max-edge-resistance

---

### max-length

---

### max-progress

---

### max-tab-width

---

### max-text-length

---

### max-value

---

### max-visible-items

---

### min-height

Read [min-height](https://developer.mozilla.org/en-US/docs/Web/CSS/min-height) documentation.

---

### min-icon-size

---

### min-icon-space

---

### min-margin-right

---

### min-tab-width

---

### min-value

---

### min-width

Read [min-width](https://developer.mozilla.org/en-US/docs/Web/CSS/min-width) documentation.

---

### movement-speed

---

### name

---

### only-numbers

---

### opacity

Read [opacity](https://developer.mozilla.org/en-US/docs/Web/CSS/opacity) documentation.

---

### orientation

---

### outline-thickness

---

### owns

---

### padding-bottom

Read [padding-bottom](https://developer.mozilla.org/en-US/docs/Web/CSS/padding-bottom) documentation.

---

### padding-left

Read [padding-left](https://developer.mozilla.org/en-US/docs/Web/CSS/padding-left) documentation.

---

### padding-right

Read [padding-right](https://developer.mozilla.org/en-US/docs/Web/CSS/padding-right) documentation.

---

### padding-top

Read [padding-top](https://developer.mozilla.org/en-US/docs/Web/CSS/padding-top) documentation.

---

### page-locked

---

### page-step

---

### popup-to-main-control

---

### progress

---

### radius

---

### reverse-draw

---

### rotation

---

### rotation-origin-point-x

---

### rotation-origin-point-y

---

### row-height

---

### row-mode

---

### row-margin

---

### row-weight

---

### scale

---

### scale-origin-point-x

---

### scale-origin-point-y

---

### scale-type

---

### scrollbar-mode

---

### scrollbar-type

---

### selected

---

### selected-color

---

### selected-index

---

### selectex-text

---

### selection-back-color

---

### shadow-color

---

### skin

---

### skin-color

---

### span

---

### special-border-tabs

---

### src

---

### tab-closable

---

### tab-separation

---

### text

---

### text-align

Read [text-align](https://developer.mozilla.org/en-US/docs/Web/CSS/text-align) documentation.

---

### text-selection

---

### text-stroke-color

---

### text-stroke-width

---

### theme

---

### timing-function

---

### tint

---

### tooltip

---

### total-steps

---

### touch-drag

---

### touch-drag-deceleration

---

### transition-delay

Read [transition-delay](https://developer.mozilla.org/en-US/docs/Web/CSS/transition-delay) documentation.

---

### transition-duration

Read [transition-duration](https://developer.mozilla.org/en-US/docs/Web/CSS/transition-duration) documentation.

---

### transition-property

Read [transition-property](https://developer.mozilla.org/en-US/docs/Web/CSS/transition-property) documentation.

---

### transition-timing-function

Read [transition-timing-function](https://developer.mozilla.org/en-US/docs/Web/CSS/transition-timing-function) documentation.

Timing function names are custom, and not the same as the standard, but `cubic-bezier` will be added
soon. Current timing functions supported: linear, quadratic-in, quadratic-out, quadratic-in-out,
sine-in, sine-out, sine-in-out, exponential-in, exponential-out, exponential-in-out, quartic-in,
quartic-out, quartic-in-out, circular-in, circular-out, circular-in-out, cubic-in, cubic-out,
cubic-in-out, back-in, back-out, back-in-out, bounce-in, bounce-out, bounce-in-out, elastic-in,
elastic-out, elastic-in-out, none.

### value

---

### vertical-expand

---

### visible

---

### vscroll-mode

---

### window-border-auto-size

Sets if the window border size should be automatically calculated based on the skin size.

* Applicable to: EE::UI::UIWindow (Window)
* Value Type: boolean
* Default value: `true`

---

### window-border-size

Manually sets the size of the window border.

* Applicable to: EE::UI::UIWindow (Window)
* Value Type: vector2-length
* Default value: _No value_

---

### window-buttons-offset

Sets the window titlebar buttons position offset.

* Applicable to: EE::UI::UIWindow (Window)
* Value Type: vector2-length
* Default Value: `0dp 0dp`

---

### window-buttons-separation

Sets the separation between the window titlebar buttons.

* Applicable to: EE::UI::UIWindow (Window)
* Value Type: length
* Default value: `4dp`

---

### window-corner-distance

Sets the minimum distance where the hovering cursor detects an edge of the window to enable edge
resizing.

* Applicable to: EE::UI::UIWindow (Window)
* Value Type: length
* Default value: `24dp`

---

### window-flags

Sets the window flags. Window flags activate a set of different boolean options of the window.
Multiple values are separated with a `|` (as a logical or).

* Applicable to: EE::UI::UIWindow (Window)
* Value Type: string-list
* Value List:
  * `default`: same as `close|buttonactions|resizeable|shareopacity`
  * `close`: Adds a close button to the titlebar.
  * `maximize`: Adds a maximize button to the titlebar.
  * `minimize`: Adds a minimize button to the titlebar.
  * `dragable`: Makes the window background dragable.
  * `shadow`: The window will project a shadow.
  * `modal`: Makes the window modal (blocks interacting any element ouside the window).
  * `undecorated`: Creates an undecorated window (no borders and titlebar).
  * `resizeable`: Makes the window resizeable.
  * `shareopacity`: Childs will inherit the window opacity (otherwise if you change the opacity of the window will only affect its borders, background and titlebar).
  * `buttonactions`: Implements the window minimize, maximize and close buttons default actions. This means that close button click will close, minimize will hide (but not close) and maximize will maximize.
  * `framebuffer`: Creates the window on its own frame buffer.
  * `colorbuffer`: Enables the color buffer to the frame buffer (if active).
* Default value: `default`

---

### window-min-size

Sets the window minimum size.

* Applicable to: EE::UI::Window (Window)
* Value Type: vector2-length
* Default value: _No value_

---

### window-opacity

Sets the base opacity of a window, this is the maximum opacity that the window will be able to have.
And the opacity property applied to the window will be multiplied by this value.

* Applicable to: EE::UI::UIWindow (window)
* Value type: float
* Default value: `1`

---

### window-titlebar-auto-size

Sets if the window titlebar size should be automatically calculated based on the skin size.

* Applicable to: EE::UI::UIWindow (Window)
* Value Type: boolean
* Default value: `true`

---

### window-titlebar-size

Manually sets the size of the window titlebar.

* Applicable to: EE::UI::UIWindow (Window)
* Value Type: vector2-length
* Default value: _No value_.

---

### word-wrap

Enables/disables word-wrap in the text view element.

* Applicable to: EE::UI::UITextVIew (TextView), EE::UI::UITextInput (TextInput),
  EE::UI::UICheckBox (CheckBox), EE::UI::UIRadioButton (RadioButton),
  EE::UI::UIListBoxItem (ListBox::item), EE::UI::UITextInputPassword (TextInputPassword),
  EE::UI::UIDropDownList (DropDownList).
* Value type: boolean
* Default value: `false`

---

## CSS Shorthands

[Shorthand properties](https://developer.mozilla.org/en-US/docs/Web/CSS/Shorthand_properties) are
CSS properties that let you set the values of multiple other CSS properties simultaneously.
Using a shorthand property, you can write more concise (and often more readable) style sheets,
saving time and energy.

---

### animation

Read [animation](https://developer.mozilla.org/en-US/docs/Web/CSS/animation) documentation.

---

### background

Read [background](https://developer.mozilla.org/en-US/docs/Web/CSS/background) documentation.
Only eepp CSS properties supported are valid in the shorthand.

---

### background-position

Read [background-position](https://developer.mozilla.org/en-US/docs/Web/CSS/background-position) documentation.

---

### border-color

Read [border-color](https://developer.mozilla.org/en-US/docs/Web/CSS/border-color) documentation.

---

### border-radius

Read [border-radius](https://developer.mozilla.org/en-US/docs/Web/CSS/border-radius) documentation.

---

### border-width

Read [border-width](https://developer.mozilla.org/en-US/docs/Web/CSS/border-width) documentation.

---

### box-margin

Shorthand for [column-margin](#column-margin) and [row-margin](#row-margin) (in that order).

The syntax consists in two lengths separated by a space.

Syntax:

```CSS
box-margin: 10dp 10dp;
```

---

### foreground

Same as the [background](#background) but for the foreground of the node.
The foreground is rendered on the top of the element.

---

### foreground-position

Same as the [background-position](#background-position) but for the foreground of the node.

---

### foreground-radius

Same as the [border-radius](#border-radius) but for the foreground of the node.

---

### margin

Read [margin](https://developer.mozilla.org/en-US/docs/Web/CSS/margin) documentation.

---

### padding

Read [padding](https://developer.mozilla.org/en-US/docs/Web/CSS/padding) documentation.

---

### rotation-origin-point

The origin point of the rotation expressed as two numeric lengths or percentages separated by a
space.

Default value: `center center`.

Syntax:

```CSS
rotation-origin-point: 50dp 10dp;
rotation-origin-point: center top;
rotation-origin-point: 100% 0%;
```

---

### scale-origin-point

The origin point of the node scale expressed as two numeric lengths or percentages separated by a
space.

Default value: `center center`.

Syntax:

```CSS
scale-origin-point: 50dp 10dp;
scale-origin-point: center top;
scale-origin-point: 100% 0%;
```

---

### transition

Read [transition](https://developer.mozilla.org/en-US/docs/Web/CSS/transition) documentation.

---

## At-rules

[At-rules](https://developer.mozilla.org/en-US/docs/Web/CSS/At-rule) are CSS statements that
instructs CSS how to behave. They begin with an at sign, '@' (U+0040 COMMERCIAL AT), followed by an
identifier and includes everything up to the next semicolon, ';' (U+003B SEMICOLON), or the next CSS
block, whichever comes first.

The following at-rules are currently supported:

* [@import](https://developer.mozilla.org/en-US/docs/Web/CSS/@import) —
  Tells the CSS engine to include an external style sheet.

* [@media](https://developer.mozilla.org/en-US/docs/Web/CSS/@media) — A conditional group rule that
  will apply its content if the device meets the criteria of the condition defined using a media
  query. Media features currently supported:
  [width](https://developer.mozilla.org/en-US/docs/Web/CSS/@media/width),
  [height](https://developer.mozilla.org/en-US/docs/Web/CSS/@media/height),
  [device-width](https://developer.mozilla.org/en-US/docs/Web/CSS/@media/device-width),
  [device-height](https://developer.mozilla.org/en-US/docs/Web/CSS/@media/device-height),
  [orientation](https://developer.mozilla.org/en-US/docs/Web/CSS/@media/orientation),
  [aspect-ratio](https://developer.mozilla.org/en-US/docs/Web/CSS/@media/aspect-ratio),
  [device-aspect-ratio](https://developer.mozilla.org/en-US/docs/Web/CSS/@media/device-aspect-ratio),
  [color](https://developer.mozilla.org/en-US/docs/Web/CSS/@media/color),
  [color-index](https://developer.mozilla.org/en-US/docs/Web/CSS/@media/color-index),
  [monochrome](https://developer.mozilla.org/en-US/docs/Web/CSS/@media/monochrome),
  [resolution](https://developer.mozilla.org/en-US/docs/Web/CSS/@media/resolution).
  Also, eepp only uses `screen` media type.

* [@font-face](https://developer.mozilla.org/en-US/docs/Web/CSS/@font-face) —
  Describes the aspect of an external font to be downloaded.
  Only [font-family](https://developer.mozilla.org/en-US/docs/Web/CSS/@font-face/font-family) and
  [src](https://developer.mozilla.org/en-US/docs/Web/CSS/@font-face/src) properties are supported at
  the moment.

* [@keyframes](https://developer.mozilla.org/en-US/docs/Web/CSS/@keyframes) — Describes the aspect
  of intermediate steps in a CSS animation sequence.

## Pseudo-class selectors

A [CSS pseudo-class](https://developer.mozilla.org/en-US/docs/Web/CSS/Pseudo-classes) is a keyword
added to a selector that specifies a special state of the selected element(s). For example, :hover
can be used to change a button's color when the user's pointer hovers over it.

Most common pseudo-class selectors are supported. The list is:

* [:checked](https://developer.mozilla.org/en-US/docs/Web/CSS/:checked)

* [:disabled](https://developer.mozilla.org/en-US/docs/Web/CSS/:disabled)

* [:empty](https://developer.mozilla.org/en-US/docs/Web/CSS/:empty)

* [:enabled](https://developer.mozilla.org/en-US/docs/Web/CSS/:enabled)

* [:first-child](https://developer.mozilla.org/en-US/docs/Web/CSS/:first-child)

* [:first-of-type](https://developer.mozilla.org/en-US/docs/Web/CSS/:first-of-type)

* [:hover](https://developer.mozilla.org/en-US/docs/Web/CSS/:hover)

* [:last-child](https://developer.mozilla.org/en-US/docs/Web/CSS/:last-child)

* [:last-of-type](https://developer.mozilla.org/en-US/docs/Web/CSS/:last-of-type)

* [:not()](https://developer.mozilla.org/en-US/docs/Web/CSS/:not)

* [:nth-child()](https://developer.mozilla.org/en-US/docs/Web/CSS/:nth-child)

* [:nth-last-child()](https://developer.mozilla.org/en-US/docs/Web/CSS/:nth-last-child)

* [:nth-last-of-type()](https://developer.mozilla.org/en-US/docs/Web/CSS/:nth-last-of-type)

* [:nth-of-type()](https://developer.mozilla.org/en-US/docs/Web/CSS/:nth-of-type)

* [:only-child](https://developer.mozilla.org/en-US/docs/Web/CSS/:only-child)

* [:root](https://developer.mozilla.org/en-US/docs/Web/CSS/:root)