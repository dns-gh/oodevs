//*****************************************************************************
//
// $Created: FBD 02-12-13 $
// $Archive: /MVW_v10/Build/SDK/GFX/src/GFX_Text.cpp $
// $Author: Age $
// $Modtime: 10/05/05 14:10 $
// $Revision: 10 $
// $Workfile: GFX_Text.cpp $
//
//*****************************************************************************

#include "GFX_Pch.h"

#include "GFX_Text.h"

#ifndef USE_INLINE
#	include "GFX_Text.inl"
#endif

#include <qgl.h>

#include "GFX_Color.h"

//-----------------------------------------------------------------------------
// Name: GFX_Text constructor
// Created: FBD 02-12-13
//-----------------------------------------------------------------------------
GFX_Text::GFX_Text( const std::string& sFontName, MT_Float rPrecision, uint nCharSet, bool bUseLine, int nTypeSize )
    : sFontName_ ( sFontName )
    , nBaseLine_ ( 0 )
    , nBasePoly_ ( 0 )
    , hDC_       ( 0 )
    , nCharset_  ( nCharSet )
    , bUseLine_  ( bUseLine )
    , nTypeSize_ ( nTypeSize )
    , rPrecision_( rPrecision )
{
}


//-----------------------------------------------------------------------------
// Name: GFX_Text destructor
// Created: FBD 02-12-13
//-----------------------------------------------------------------------------
GFX_Text::~GFX_Text()
{
}


//-----------------------------------------------------------------------------
// Name: GFX_Text::Initialize
// Created: FBD 02-12-13
//-----------------------------------------------------------------------------
void GFX_Text::Initialize(  )
{
    InitFont();
}



//-----------------------------------------------------------------------------
// Name: GFX_Text::Terminate
// Created: FBD 02-12-13
//-----------------------------------------------------------------------------
void GFX_Text::Terminate()
{
    DeleteFont();
}


//-----------------------------------------------------------------------------
// Name: GFX_Text::PrintLine
// Created: FBD 02-12-13
//-----------------------------------------------------------------------------
void GFX_Text::PrintLine( MT_Vector2D vPos, MT_Float rSizeFont, MT_Float rSizeLine, const GFX_Color& color, bool bCenter, const char *fmt, ...)
{
    if ( !bUseLine_ )
        return;
    
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	    vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	MT_Float rZ = 0.0;

    glBindTexture( GL_TEXTURE_2D, 0 );
    glLineWidth( rSizeLine );


	glPushMatrix();											// Store The Projection Matrix
    color.SetGLColor();
    glTranslatef( vPos.rX_, vPos.rY_, rZ );	// Center Our Text On The Screen
	glScalef( rSizeFont, rSizeFont, 1.f );

	if ( bCenter )
	{
        MT_Vector2D size = GetSizeText( std::string( text ) );
		glTranslatef( - ( size.rX_ / 2.f ), 0.0f, 0.0f);	// Center Our Text On The Screen
	}
		

	glPushAttrib(GL_LIST_BIT);								// Pushes The Display List Bits
	glListBase( nBaseLine_ );										// Sets The Base Character to 0
	glCallLists( strlen(text), GL_UNSIGNED_BYTE, text );	// Draws The Display List Text
	glPopAttrib();											// Pops The Display List Bits
	glPopMatrix();											// Restore The Old Projection Matrix
}

//-----------------------------------------------------------------------------
// Name: GFX_Text::glPrint
// Created: FBD 02-12-02
//-----------------------------------------------------------------------------
void GFX_Text::Print( MT_Vector2D vPos, MT_Float rSizeFont, const GFX_Color& color, bool bCenter, const char *fmt, ...)
{
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	    vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	MT_Float rZ = 0.0;

    glBindTexture( GL_TEXTURE_2D, 0 );

	glPushMatrix();											// Store The Projection Matrix
    color.SetGLColor();
    glTranslatef( vPos.rX_, vPos.rY_, rZ );	// Center Our Text On The Screen
	glScalef( rSizeFont, rSizeFont, 1.f );

	if ( bCenter )
	{
        MT_Vector2D size = GetSizeText( std::string( text ) );
		glTranslatef( - ( size.rX_ / 2.f ), 0.0f, 0.0f);	// Center Our Text On The Screen
	}

	glPushAttrib(GL_LIST_BIT);								// Pushes The Display List Bits
	glListBase( nBasePoly_ );								// Sets The Base Character to 0
	glCallLists( strlen(text), GL_UNSIGNED_BYTE, text );	// Draws The Display List Text
	glPopAttrib();											// Pops The Display List Bits
	glPopMatrix();											// Restore The Old Projection Matrix
}

//-----------------------------------------------------------------------------
// Name: GFX_Text::InitFont
// Created: FBD 02-12-02
//-----------------------------------------------------------------------------
void GFX_Text::InitFont()
{
	HFONT	font;										// Windows Font ID
	nBasePoly_ = glGenLists(256);						// Storage For 256 Characters
	nBaseLine_ = glGenLists(256);						// Storage For 256 Characters

    hDC_ = qt_display_dc();
	font = CreateFont(  -12,							// Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						nTypeSize_,						// Font Weight
						FALSE,							// Italic
						FALSE,							// Underline
						FALSE,							// Strikeout
						nCharset_,					    // Character Set Identifier
						OUT_STRING_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						DRAFT_QUALITY,			        // Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						sFontName_.c_str() );	
    if( ! font ) 
        throw std::exception( ( "Unable to create font '" + sFontName_ + "'" ).c_str() );

	SelectObject( hDC_, font);							// Selects The Font We Created


    wglUseFontOutlines(	hDC_,							// Select The Current DC
						0,								// Starting Character
						255,							// Number Of Display Lists To Build
						nBasePoly_,							// Starting Display Lists
						rPrecision_,							// Deviation From The True Outlines
						0.0f,							// Font Thickness In The Z Direction
						WGL_FONT_POLYGONS,				        // Use Polygons, Not Lines
						gmfPoly_);							// Address Of Buffer To Recieve Data

    if ( bUseLine_ )
    {
	    wglUseFontOutlines(	hDC_,							// Select The Current DC
						    0,								// Starting Character
						    255,							// Number Of Display Lists To Build
						    nBaseLine_,						// Starting Display Lists
						    rPrecision_,							// Deviation From The True Outlines
						    0.0f,							// Font Thickness In The Z Direction
						    WGL_FONT_LINES,				        // Use Polygons, Not Lines
						    gmfLine_);							// Address Of Buffer To Recieve Data
    }
}

//-----------------------------------------------------------------------------
// Name: GFX_Text::DeleteFont
// Created: FBD 02-12-02
//-----------------------------------------------------------------------------
GLvoid GFX_Text::DeleteFont()
{
    glDeleteLists( nBasePoly_, 256);

    if ( bUseLine_ )
        glDeleteLists( nBaseLine_, 256);



}


//-----------------------------------------------------------------------------
// Name: GFX_Text::GetSizeChar
// Created: FBD 02-12-13
//-----------------------------------------------------------------------------
MT_Vector2D GFX_Text::GetSizeText( const std::string& sText ) const
{
    MT_Vector2D vSize( 0. ,0. );
    const char* pText = sText.c_str();
    for ( uint i = 0 ; i < strlen( pText ) ; ++i )
    {
        unsigned char c = pText[i];

        vSize.rX_ += gmfPoly_[ c ].gmfCellIncX;

        MT_Float rSizeY = gmfPoly_[ c ].gmfBlackBoxY; 
        if ( vSize.rY_ < rSizeY )
            vSize.rY_ = rSizeY;
    }

    return vSize;
}


