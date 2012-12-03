#include <eepp/graphics/cfont.hpp>
#include <eepp/graphics/cfontmanager.hpp>
#include <eepp/graphics/cglobalbatchrenderer.hpp>

namespace EE { namespace Graphics {

cFont::cFont( const Uint32& Type, const std::string& Name ) :
	mType( Type ),
	mCacheData(true),
	mColor(255,255,255,255),
	mShadowColor(0xFF000000),
	mNumLines(1),
	mTexId(0),
	mCachedWidth(0.f),
	mHeight(0),
	mSize(0),
	mLineSkip(0),
	mAscent(0),
	mDescent(0)
{
	this->Name( Name );
	cFontManager::instance()->Add( this );
}

cFont::~cFont() {
	mText.clear();
	mGlyphs.clear();

	if ( !cFontManager::instance()->IsDestroying() ) {
		cFontManager::instance()->Remove( this, false );
	}
}

void cFont::SetText( const String& Text ) {
	if ( mText.size() != Text.size() ) {
		Int32 size = Text.size() * EE_QUAD_VERTEX;

		mRenderCoords.resize( size );
		mColors.resize( size, mColor );
	}

	mText = Text;

	if ( mCacheData )
		CacheWidth();
}

const eeColorA& cFont::Color() const {
	return mColor;
}

void cFont::Color(const eeColorA& Color) {
	if ( mColor != Color ) {
		mColor = Color;

		mColors.assign( mText.size() * EE_QUAD_VERTEX, mColor );
	}
}

const eeColorA& cFont::ShadowColor() const {
	return mShadowColor;
}

void cFont::ShadowColor(const eeColorA& Color) {
	mShadowColor = Color;
}

void cFont::CacheNumLines() {
	Int32 c = 1;

	for ( eeUint i = 0; i < mText.size(); i++ ) {
		if ( mText[i] == '\n' )
			c++;
	}

	mNumLines = c;
}

eeInt cFont::GetNumLines() {
	return mNumLines;
}

eeFloat cFont::GetTextWidth( const String& Text ) {
	SetText( Text );
	return mCachedWidth;
}

eeFloat cFont::GetTextWidth() const {
	return mCachedWidth;
}

Uint32 cFont::GetFontSize() const {
	return mSize;
}

Uint32 cFont::GetFontHeight() const {
	return mHeight;
}

Int32 cFont::GetLineSkip() const {
	return mLineSkip;
}

Int32 cFont::GetFontAscent() const {
	return mAscent;
}

Int32 cFont::GetFontDescent() const {
	return mDescent;
}

String cFont::GetText() {
	return mText;
}

void cFont::CacheData( bool Cache ) {
	mCacheData = Cache;
}

const bool& cFont::CacheData() const {
	return mCacheData;
}

eeFloat cFont::GetTextHeight() {
	return (eeFloat)GetFontHeight() * (eeFloat)GetNumLines();
}

const std::vector<eeFloat>& cFont::GetLinesWidth() const {
	return mLinesWidth;
}

void cFont::Draw( const eeFloat& X, const eeFloat& Y, const Uint32& Flags, const eeFloat& Scale, const eeFloat& Angle, const EE_BLEND_MODE& Effect) {
	SubDraw( mText, X, Y, Flags, Scale, Angle, true, Effect );
}

void cFont::Draw( const String& Text, const eeFloat& X, const eeFloat& Y, const Uint32& Flags, const eeFloat& Scale, const eeFloat& Angle, const EE_BLEND_MODE& Effect ) {
	SubDraw( Text, X, Y, Flags, Scale, Angle, false, Effect );
}

void cFont::Draw( cTextCache& TextCache, const eeFloat& X, const eeFloat& Y, const Uint32& Flags, const eeFloat& Scale, const eeFloat& Angle, const EE_BLEND_MODE& Effect ) {
	if ( !TextCache.Text().size() )
		return;

	cGlobalBatchRenderer::instance()->Draw();
	cTextureFactory::instance()->Bind( mTexId );
	BlendMode::SetMode( Effect );

	if ( Flags & FONT_DRAW_SHADOW ) {
		Uint32 f = Flags;

		f &= ~FONT_DRAW_SHADOW;

		eeColorA Col = TextCache.Color();

		SetText( TextCache.Text() );

		if ( Col.A() != 255 ) {
			eeColorA ShadowColor = TextCache.ShadowColor();

			ShadowColor.Alpha = (Uint8)( (eeFloat)ShadowColor.Alpha * ( (eeFloat)Col.A() / (eeFloat)255 ) );

			Color( ShadowColor );
		} else {
			Color( TextCache.ShadowColor() );
		}

		Draw( X + 1, Y + 1, f, Scale, Angle, Effect );

		Color( Col );
	}

	eeFloat cX = (eeFloat) ( (Int32)X );
	eeFloat cY = (eeFloat) ( (Int32)Y );
	eeFloat nX = 0;
	eeFloat nY = 0;
	Int16 Char = 0;
	eeUint Line = 0;
	eeUint numvert = 0;

	if ( Angle != 0.0f || Scale != 1.0f ) {
		GLi->PushMatrix();

		eeVector2f Center( cX + TextCache.GetTextWidth() * 0.5f, cY + TextCache.GetTextHeight() * 0.5f );
		GLi->Translatef( Center.x , Center.y, 0.f );
		GLi->Rotatef( Angle, 0.0f, 0.0f, 1.0f );
		GLi->Scalef( Scale, Scale, 1.0f );
		GLi->Translatef( -Center.x + X, -Center.y + Y, 0.f );
	}

	std::vector<eeVertexCoords>& RenderCoords = TextCache.VertextCoords();
	std::vector<eeColorA>& Colors = TextCache.Colors();

	if ( !TextCache.CachedCoords() ) {
		switch ( FontHAlignGet( Flags ) ) {
			case FONT_DRAW_CENTER:
				nX = (eeFloat)( (Int32)( ( TextCache.GetTextWidth() - TextCache.LinesWidth()[ Line ] ) * 0.5f ) );
				Line++;
				break;
			case FONT_DRAW_RIGHT:
				nX = TextCache.GetTextWidth() - TextCache.LinesWidth()[ Line ];
				Line++;
				break;
		}

		Int32 tGlyphSize = (Int32)mGlyphs.size();

		for ( eeUint i = 0; i < TextCache.Text().size(); i++ ) {
			Char = static_cast<Int32>( TextCache.Text().at(i) );

			if ( Char < 0 && Char > -128 )
				Char = 256 + Char;

			if ( Char >= 0 && Char < tGlyphSize ) {
				eeTexCoords* C = &mTexCoords[ Char ];

				switch( Char ) {
					case '\v':
					{
						if (mVerticalDraw)
							nY += GetFontHeight();
						else
							nX += mGlyphs[ Char ].Advance;
						break;
					}
					case '\t':
					{
						if (mVerticalDraw)
							nY += GetFontHeight() * 4;
						else
							nX += mGlyphs[ Char ].Advance * 4;
						break;
					}
					case '\n':
					{
						if (mVerticalDraw) {
							nX += (GetFontHeight() * Scale);
							nY = 0;
						} else {
							if ( i + 1 < TextCache.Text().size() ) {
								switch ( FontHAlignGet( Flags ) ) {
									case FONT_DRAW_CENTER:
										nX = (eeFloat)( (Int32)( ( TextCache.GetTextWidth() - TextCache.LinesWidth()[ Line ] ) * 0.5f ) );
										break;
									case FONT_DRAW_RIGHT:
										nX = TextCache.GetTextWidth() - TextCache.LinesWidth()[ Line ];
										break;
									default:
										nX = 0;
								}
							}

							nY += (GetFontHeight() * Scale);
							Line++;
						}

						break;
					}
					default:
					{
						#ifndef EE_GLES
						for ( Uint8 z = 0; z < 8; z+=2 ) {
							RenderCoords[ numvert ].TexCoords[0]	= C->TexCoords[z];
							RenderCoords[ numvert ].TexCoords[1]	= C->TexCoords[ z + 1 ];
							RenderCoords[ numvert ].Vertex[0]		= cX + C->Vertex[z] + nX;
							RenderCoords[ numvert ].Vertex[1]		= cY + C->Vertex[ z + 1 ] + nY;
							numvert++;
						}
						#else
							RenderCoords[ numvert ].TexCoords[0]	= C->TexCoords[2];
							RenderCoords[ numvert ].TexCoords[1]	= C->TexCoords[ 2 + 1 ];
							RenderCoords[ numvert ].Vertex[0]		= cX + C->Vertex[2] + nX;
							RenderCoords[ numvert ].Vertex[1]		= cY + C->Vertex[ 2 + 1 ] + nY;
							numvert++;

							RenderCoords[ numvert ].TexCoords[0]	= C->TexCoords[0];
							RenderCoords[ numvert ].TexCoords[1]	= C->TexCoords[ 0 + 1 ];
							RenderCoords[ numvert ].Vertex[0]		= cX + C->Vertex[0] + nX;
							RenderCoords[ numvert ].Vertex[1]		= cY + C->Vertex[ 0 + 1 ] + nY;
							numvert++;

							RenderCoords[ numvert ].TexCoords[0]	= C->TexCoords[6];
							RenderCoords[ numvert ].TexCoords[1]	= C->TexCoords[ 6 + 1 ];
							RenderCoords[ numvert ].Vertex[0]		= cX + C->Vertex[6] + nX;
							RenderCoords[ numvert ].Vertex[1]		= cY + C->Vertex[ 6 + 1 ] + nY;
							numvert++;

							RenderCoords[ numvert ].TexCoords[0]	= C->TexCoords[2];
							RenderCoords[ numvert ].TexCoords[1]	= C->TexCoords[ 2 + 1 ];
							RenderCoords[ numvert ].Vertex[0]		= cX + C->Vertex[2] + nX;
							RenderCoords[ numvert ].Vertex[1]		= cY + C->Vertex[ 2 + 1 ] + nY;
							numvert++;

							RenderCoords[ numvert ].TexCoords[0]	= C->TexCoords[4];
							RenderCoords[ numvert ].TexCoords[1]	= C->TexCoords[ 4 + 1 ];
							RenderCoords[ numvert ].Vertex[0]		= cX + C->Vertex[4] + nX;
							RenderCoords[ numvert ].Vertex[1]		= cY + C->Vertex[ 4 + 1 ] + nY;
							numvert++;

							RenderCoords[ numvert ].TexCoords[0]	= C->TexCoords[6];
							RenderCoords[ numvert ].TexCoords[1]	= C->TexCoords[ 6 + 1 ];
							RenderCoords[ numvert ].Vertex[0]		= cX + C->Vertex[6] + nX;
							RenderCoords[ numvert ].Vertex[1]		= cY + C->Vertex[ 6 + 1 ] + nY;
							numvert++;
						#endif

						if (mVerticalDraw)
							nY += GetFontHeight();
						else
							nX += mGlyphs[ Char ].Advance;
					}
				}
			}
		}

		TextCache.CachedCoords( true );
		TextCache.CachedVerts( numvert );
	} else {
		numvert = TextCache.CachedVerts();
	}

	Uint32 alloc = numvert * sizeof(eeVertexCoords);

	GLi->ColorPointer	( 4, GL_UNSIGNED_BYTE	, 0						, reinterpret_cast<char*>( &Colors[0] )								, alloc );
	GLi->TexCoordPointer( 2, GL_FP			, sizeof(eeVertexCoords), reinterpret_cast<char*>( &RenderCoords[0] )						, alloc );
	GLi->VertexPointer	( 2, GL_FP			, sizeof(eeVertexCoords), reinterpret_cast<char*>( &RenderCoords[0] ) + sizeof(eeFloat) * 2 , alloc );

	#ifndef EE_GLES
	GLi->DrawArrays( GL_QUADS, 0, numvert );
	#else
	GLi->DrawArrays( GL_TRIANGLES, 0, numvert );
	#endif

	if ( Angle != 0.0f || Scale != 1.0f ) {
		GLi->PopMatrix();
	}
}

void cFont::SubDraw( const String& Text, const eeFloat& X, const eeFloat& Y, const Uint32& Flags, const eeFloat& Scale, const eeFloat& Angle, const bool& Cached, const EE_BLEND_MODE& Effect ) {
	if ( !Text.size() )
		return;

	cGlobalBatchRenderer::instance()->Draw();
	cTextureFactory::instance()->Bind( mTexId );
	BlendMode::SetMode( Effect );

	if ( !Cached && ( Text.size() != mRenderCoords.size() / EE_QUAD_VERTEX || Angle != 0.f || Scale != 1.f || FontHAlignGet( Flags ) == FONT_DRAW_CENTER || FontHAlignGet( Flags ) == FONT_DRAW_RIGHT ) ) {
		SetText( Text );
	}

	if ( Flags & FONT_DRAW_SHADOW ) {
		Uint32 f = Flags;
		f &= ~FONT_DRAW_SHADOW;

		eeColorA Col = mColor;

		if ( Col.A() != 255 ) {
			eeColorA ShadowColor = mShadowColor;

			ShadowColor.Alpha = (Uint8)( (eeFloat)ShadowColor.Alpha * ( (eeFloat)Col.A() / (eeFloat)255 ) );

			Color( ShadowColor );
		} else {
			Color( mShadowColor );
		}

		SubDraw( Text, X + 1, Y + 1, f, Scale, Angle, true, Effect );

		Color( Col );
	}

	eeFloat cX = (eeFloat) ( (Int32)X );
	eeFloat cY = (eeFloat) ( (Int32)Y );
	eeFloat nX = 0;
	eeFloat nY = 0;
	Int16 Char = 0;
	eeUint Line = 0;
	eeUint numvert = 0;

	if ( Angle != 0.0f || Scale != 1.0f ) {
		GLi->PushMatrix();

		eeVector2f Center( cX + GetTextWidth() * 0.5f, cY + GetTextHeight() * 0.5f );
		GLi->Translatef( Center.x , Center.y, 0.f );
		GLi->Rotatef( Angle, 0.0f, 0.0f, 1.0f );
		GLi->Scalef( Scale, Scale, 1.0f );
		GLi->Translatef( -Center.x, -Center.y, 0.f );
	}

	switch ( FontHAlignGet( Flags ) ) {
		case FONT_DRAW_CENTER:
			nX = (eeFloat)( (Int32)( ( mCachedWidth - mLinesWidth[ Line ] ) * 0.5f ) );
			Line++;
			break;
		case FONT_DRAW_RIGHT:
			nX = mCachedWidth - mLinesWidth[ Line ];
			Line++;
			break;
	}

	Int32 tGlyphSize = (Int32)mGlyphs.size();

	for ( eeUint i = 0; i < Text.size(); i++ ) {
		Char = static_cast<Int32>( Text.at(i) );

		if ( Char < 0 && Char > -128 )
			Char = 256 + Char;

		if ( Char >= 0 && Char < tGlyphSize ) {
			eeTexCoords * C = &mTexCoords[ Char ];

			switch( Char ) {
				case '\v':
					if (mVerticalDraw)
						nY += GetFontHeight();
					else
						nX += mGlyphs[ Char ].Advance;
					break;
				case '\t':
					if (mVerticalDraw)
						nY += GetFontHeight() * 4;
					else
						nX += mGlyphs[ Char ].Advance * 4;
					break;
				case '\n':
					if (mVerticalDraw) {
						nX += (GetFontHeight() * Scale);
						nY = 0;
					} else {
						if ( i + 1 < Text.size() ) {
							switch ( FontHAlignGet( Flags ) ) {
								case FONT_DRAW_CENTER:
									nX = (eeFloat)( (Int32)( ( mCachedWidth - mLinesWidth[ Line ] ) * 0.5f ) );
									break;
								case FONT_DRAW_RIGHT:
									nX = mCachedWidth - mLinesWidth[ Line ];
									break;
								default:
									nX = 0;
							}
						}

						nY += (GetFontHeight() * Scale);
						Line++;
					}

					break;
				default:
					#ifndef EE_GLES
					for ( Uint8 z = 0; z < 8; z+=2 ) {
						mRenderCoords[ numvert ].TexCoords[0] 	= C->TexCoords[z];
						mRenderCoords[ numvert ].TexCoords[1] 	= C->TexCoords[ z + 1 ];
						mRenderCoords[ numvert ].Vertex[0] 		= cX + C->Vertex[z] + nX;
						mRenderCoords[ numvert ].Vertex[1]		= cY + C->Vertex[ z + 1 ] + nY;
						numvert++;
					}
					#else
					mRenderCoords[ numvert ].TexCoords[0]	= C->TexCoords[2];
					mRenderCoords[ numvert ].TexCoords[1]	= C->TexCoords[ 2 + 1 ];
					mRenderCoords[ numvert ].Vertex[0]		= cX + C->Vertex[2] + nX;
					mRenderCoords[ numvert ].Vertex[1]		= cY + C->Vertex[ 2 + 1 ] + nY;
					numvert++;

					mRenderCoords[ numvert ].TexCoords[0]	= C->TexCoords[0];
					mRenderCoords[ numvert ].TexCoords[1]	= C->TexCoords[ 0 + 1 ];
					mRenderCoords[ numvert ].Vertex[0]		= cX + C->Vertex[0] + nX;
					mRenderCoords[ numvert ].Vertex[1]		= cY + C->Vertex[ 0 + 1 ] + nY;
					numvert++;

					mRenderCoords[ numvert ].TexCoords[0]	= C->TexCoords[6];
					mRenderCoords[ numvert ].TexCoords[1]	= C->TexCoords[ 6 + 1 ];
					mRenderCoords[ numvert ].Vertex[0]		= cX + C->Vertex[6] + nX;
					mRenderCoords[ numvert ].Vertex[1]		= cY + C->Vertex[ 6 + 1 ] + nY;
					numvert++;

					mRenderCoords[ numvert ].TexCoords[0]	= C->TexCoords[2];
					mRenderCoords[ numvert ].TexCoords[1]	= C->TexCoords[ 2 + 1 ];
					mRenderCoords[ numvert ].Vertex[0]		= cX + C->Vertex[2] + nX;
					mRenderCoords[ numvert ].Vertex[1]		= cY + C->Vertex[ 2 + 1 ] + nY;
					numvert++;

					mRenderCoords[ numvert ].TexCoords[0]	= C->TexCoords[4];
					mRenderCoords[ numvert ].TexCoords[1]	= C->TexCoords[ 4 + 1 ];
					mRenderCoords[ numvert ].Vertex[0]		= cX + C->Vertex[4] + nX;
					mRenderCoords[ numvert ].Vertex[1]		= cY + C->Vertex[ 4 + 1 ] + nY;
					numvert++;

					mRenderCoords[ numvert ].TexCoords[0]	= C->TexCoords[6];
					mRenderCoords[ numvert ].TexCoords[1]	= C->TexCoords[ 6 + 1 ];
					mRenderCoords[ numvert ].Vertex[0]		= cX + C->Vertex[6] + nX;
					mRenderCoords[ numvert ].Vertex[1]		= cY + C->Vertex[ 6 + 1 ] + nY;
					numvert++;
					#endif

					if ( mVerticalDraw )
						nY += GetFontHeight();
					else
						nX += mGlyphs[ Char ].Advance;
			}
		}
	}

	Uint32 alloc = numvert * sizeof(eeVertexCoords);

	GLi->ColorPointer		( 4, GL_UNSIGNED_BYTE	, 0						, reinterpret_cast<char*>( &mColors[0] )								, alloc	);
	GLi->TexCoordPointer	( 2, GL_FP			, sizeof(eeVertexCoords), reinterpret_cast<char*>( &mRenderCoords[0] )							, alloc );
	GLi->VertexPointer		( 2, GL_FP			, sizeof(eeVertexCoords), reinterpret_cast<char*>( &mRenderCoords[0] ) + sizeof(eeFloat) * 2	, alloc );

	#ifndef EE_GLES
	GLi->DrawArrays( GL_QUADS, 0, numvert );
	#else
	GLi->DrawArrays( GL_TRIANGLES, 0, numvert );
	#endif

	if ( Angle != 0.0f || Scale != 1.0f )
		GLi->PopMatrix();
}

void cFont::CacheWidth( const String& Text, std::vector<eeFloat>& LinesWidth, eeFloat& CachedWidth, eeInt& NumLines ) {
	LinesWidth.clear();

	eeFloat Width = 0, MaxWidth = 0;
	Int32 CharID;
	Int32 Lines = 1;

	Int32 tGlyphSize = (Int32)mGlyphs.size();

	for (std::size_t i = 0; i < Text.size(); ++i) {
		CharID = static_cast<Int32>( Text.at(i) );

		if ( CharID >= 0 && CharID < tGlyphSize ) {
			Width += mGlyphs[CharID].Advance;

			if ( CharID == '\t' )
				Width += mGlyphs[CharID].Advance * 3;

			if ( CharID == '\n' ) {
				Lines++;
				eeFloat lWidth = ( CharID == '\t' ) ? mGlyphs[CharID].Advance * 4.f : mGlyphs[CharID].Advance;
				LinesWidth.push_back( Width - lWidth );
				Width = 0;
			}

			if ( Width > MaxWidth )
				MaxWidth = Width;
		}
	}

	if ( Text.size() && Text.at( Text.size() - 1 ) != '\n' ) {
		LinesWidth.push_back( Width );
	}

	CachedWidth = MaxWidth;
	NumLines = Lines;
}

void cFont::CacheWidth() {
	CacheWidth( mText, mLinesWidth, mCachedWidth, mNumLines );
}

void cFont::ShrinkText( std::string& Str, const Uint32& MaxWidth ) {
	if ( !Str.size() )
		return;

	eeFloat		tCurWidth		= 0.f;
	eeFloat 	tWordWidth		= 0.f;
	eeFloat 	tMaxWidth		= (eeFloat) MaxWidth;
	char *		tStringLoop		= &Str[0];
	char *		tLastSpace		= NULL;
	Uint32 		tGlyphSize 		= (Uint32)mGlyphs.size();

	while ( *tStringLoop ) {
		if ( (Uint32)( *tStringLoop ) < tGlyphSize ) {
			eeGlyph * pChar = &mGlyphs[ ( *tStringLoop ) ];
			eeFloat fCharWidth	= (eeFloat)pChar->Advance;

			if ( ( *tStringLoop ) == '\t' )
				fCharWidth += pChar->Advance * 3;

			tWordWidth		+= fCharWidth;

			if ( ' ' == *tStringLoop || '\0' == *( tStringLoop + 1 ) ) {
				if ( tCurWidth + tWordWidth < tMaxWidth ) {
					tCurWidth		+= tWordWidth;
					tLastSpace		= tStringLoop;

					tStringLoop++;
				} else {
					if ( NULL != tLastSpace ) {
						*tLastSpace		= '\n';
						tStringLoop	= tLastSpace + 1;
					} else {
						*tStringLoop	= '\n';
					}

					if ( '\0' == *( tStringLoop + 1 ) )
						tStringLoop++;

					tLastSpace		= NULL;
					tCurWidth		= 0.f;
				}

				tWordWidth = 0.f;
			} else if ( '\n' == *tStringLoop ) {
				tWordWidth 		= 0.f;
				tCurWidth 		= 0.f;
				tLastSpace		= NULL;
				tStringLoop++;
			} else {
				tStringLoop++;
			}
		} else {
			*tStringLoop		= ' ';
		}
	}
}

void cFont::ShrinkText( String& Str, const Uint32& MaxWidth ) {
	if ( !Str.size() )
		return;

	eeFloat		tCurWidth		= 0.f;
	eeFloat 	tWordWidth		= 0.f;
	eeFloat 	tMaxWidth		= (eeFloat) MaxWidth;
	String::StringBaseType *	tStringLoop		= &Str[0];
	String::StringBaseType *	tLastSpace		= NULL;

	while ( *tStringLoop ) {
		if ( (String::StringBaseType)( *tStringLoop ) < mGlyphs.size() ) {
			eeGlyph * pChar = &mGlyphs[ ( *tStringLoop ) ];
			eeFloat fCharWidth	= (eeFloat)pChar->Advance;

			if ( ( *tStringLoop ) == '\t' )
				fCharWidth += pChar->Advance * 3;

			// Add the new char width to the current word width
			tWordWidth		+= fCharWidth;

			if ( ' ' == *tStringLoop || '\0' == *( tStringLoop + 1 ) ) {

				// If current width plus word width is minor to the max width, continue adding
				if ( tCurWidth + tWordWidth < tMaxWidth ) {
					tCurWidth		+= tWordWidth;
					tLastSpace		= tStringLoop;

					tStringLoop++;
				} else {
					// If it was an space before, replace that space for an new line
					// Start counting from the new line first character
					if ( NULL != tLastSpace ) {
						*tLastSpace		= '\n';
						tStringLoop	= tLastSpace + 1;
					} else {	// The word is larger than the current possible width
						*tStringLoop	= '\n';
					}

					if ( '\0' == *( tStringLoop + 1 ) )
						tStringLoop++;

					// Set the last spaces as null, because is a new line
					tLastSpace		= NULL;

					// New line, new current width
					tCurWidth		= 0.f;
				}

				// New word, so we reset the current word width
				tWordWidth = 0.f;
			} else if ( '\n' == *tStringLoop ) {
				tWordWidth 		= 0.f;
				tCurWidth 		= 0.f;
				tLastSpace		= NULL;
				tStringLoop++;
			} else {
				tStringLoop++;
			}
		} else {	// Replace any unknown char as spaces.
			*tStringLoop		= ' ';
		}
	}
}

const Uint32& cFont::GetTexId() const {
	return mTexId;
}

const Uint32& cFont::Type() const {
	return mType;
}

const std::string& cFont::Name() const {
	return mFontName;
}

void cFont::Name( const std::string& name ) {
	mFontName = name;
	mFontHash = String::Hash( mFontName );
}

const Uint32& cFont::Id() {
	return mFontHash;
}

}}