#include "cuiskin.hpp"

namespace EE { namespace UI {

const char * UISkinStatesNames[] = {
	"normal",
	"focus",
	"unfocus",
	"menter",
	"mexit",
	"mdown"
};

const char * cUISkin::GetSkinStateName( const Uint32& State ) {
	return UISkinStatesNames[ State ];
}

cUISkin::cUISkin( const std::string& Name ) :
	mName( Name ),
	mNameHash( MakeHash( mName ) ),
	mCurState(0),
	mLastState(0),
	mTheme(NULL)
{
	mColorDefault	= 0xFFFFFFFF;

	for ( Int32 i = 0; i < StateCount; i++ )
		mColor[ i ] = eeColorA( 0xFFFFFFFF );
}

cUISkin::~cUISkin() {
}

void cUISkin::SetColor( const Uint32& State, const eeColorA& Color ) {
	eeASSERT ( State < StateCount );

	Write32BitKey( &mColorDefault, State, 0 );

	mColor[ State ] = Color;
}

const eeColorA& cUISkin::GetColor( const Uint32& State ) const {
	eeASSERT ( State < StateCount );

	return mColor[ State ];
}

void cUISkin::SetState( const Uint32& State ) {
	eeASSERT ( State < StateCount );

	if ( !Read32BitKey( &mColorDefault, State ) )
		mCurState = State;
	else
		StateBack( State );
}

const Uint32& cUISkin::GetState() const {
	return mCurState;
}

const std::string& cUISkin::Name() const {
	return mName;
}

void cUISkin::Name( const std::string& name ) {
	mName = name;
	mNameHash = MakeHash( mName );
}

const Uint32& cUISkin::Id() const {
	return mNameHash;
}

void cUISkin::SetSkins() {
	for ( Int32 i = 0; i < StateCount; i++ )
		SetSkin( i );
}

void cUISkin::StateBack( const Uint32& State ) {
	if ( StateFocus == State && StateMouseEnter == mCurState && StateNormal == mLastState ) {
		return;
	}

	if ( !( mCurState == StateFocus && ( State == StateMouseEnter || State == StateMouseExit || State == StateMouseDown ) ) ) {
		mLastState 	= mCurState;
		mCurState 	= StateNormal;
	}
}

void cUISkin::SetPrevState() {
	if ( StateMouseDown == mCurState ) {
		Uint32 State = mCurState;
		mCurState = mLastState;
		mLastState = State;
	}
}

cUITheme * cUISkin::Theme() const {
	return mTheme;
}

void cUISkin::Theme( cUITheme * theme ) {
	mTheme = theme;
}

}}
