// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-06-30 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_GLFont.cpp $
// $Author: Ape $
// $Modtime: 13/07/04 17:10 $
// $Revision: 1 $
// $Workfile: MT_GLFont.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MT_GLFont.h"

class MT_GLFontManager
{
public:
     MT_GLFontManager() {}
    ~MT_GLFontManager() { MT_DELETEOWNED( fonts_ ); }

public:
    typedef std::vector< MT_GLFont* >   T_FontVector;
    typedef T_FontVector::iterator      IT_FontVector;

    T_FontVector fonts_;
};


MT_GLFontManager gFontManager_;


// -----------------------------------------------------------------------------
// Name: MT_GLFont constructor
// Created: APE 2004-06-30
// -----------------------------------------------------------------------------
MT_GLFont::MT_GLFont( const std::string& strFontName, float rPrecision, uint nCharSet, bool bUseOutline, int nTypeSize )
    : strFontName_ ( strFontName )
    , nBaseLine_   ( 0 )
    , nBasePoly_   ( 0 )
    , hDC_         ( 0 )
    , nCharset_    ( nCharSet )
    , bUseOutline_ ( bUseOutline )
    , nTypeSize_   ( nTypeSize )
    , rPrecision_  ( rPrecision )
    , bValid_      ( true )
{
bUseOutline_ = true;

    HFONT   font;                                       // Windows Font ID
    nBasePoly_ = glGenLists(256);                       // Storage For 256 Characters
    if( bUseOutline_ )
        nBaseLine_ = glGenLists(256);                   // Storage For 256 Characters

    if( nBasePoly_ == 0 || ( bUseOutline && nBaseLine_ == 0 ) )
    {
        bValid_ = false;
        return;
    }

    hDC_ = qt_display_dc();
    font = CreateFont(  -12,                            // Height Of Font
                        0,                              // Width Of Font
                        0,                              // Angle Of Escapement
                        0,                              // Orientation Angle
                        nTypeSize_,                     // Font Weight
                        FALSE,                          // Italic
                        FALSE,                          // Underline
                        FALSE,                          // Strikeout
                        nCharset_,                      // Character Set Identifier
                        OUT_TT_PRECIS,                  // Output Precision
                        CLIP_DEFAULT_PRECIS,            // Clipping Precision
                        ANTIALIASED_QUALITY,            // Output Quality
                        FF_DONTCARE|DEFAULT_PITCH,      // Family And Pitch
                        strFontName_.c_str() );

    SelectObject( hDC_, font);                          // Selects The Font We Created


    bValid_ = bValid_ && wglUseFontOutlines( hDC_,      // Select The Current DC
                        0,                              // Starting Character
                        255,                            // Number Of Display Lists To Build
                        nBasePoly_,                     // Starting Display Lists
                        rPrecision_,                    // Deviation From The True Outlines
                        0.0f,                           // Font Thickness In The Z Direction
                        WGL_FONT_POLYGONS,              // Use Polygons, Not Lines
                        gmfPoly_);                      // Address Of Buffer To Recieve Data

    if( bUseOutline_ )
    {
        bValid_ = bValid_ && wglUseFontOutlines( hDC_,  // Select The Current DC
                            0,                          // Starting Character
                            255,                        // Number Of Display Lists To Build
                            nBaseLine_,                 // Starting Display Lists
                            rPrecision_,                // Deviation From The True Outlines
                            0.0f,                       // Font Thickness In The Z Direction
                            WGL_FONT_LINES,             // Use Polygons, Not Lines
                            gmfLine_);                  // Address Of Buffer To Recieve Data
    }
}


// -----------------------------------------------------------------------------
// Name: MT_GLFont destructor
// Created: APE 2004-06-30
// -----------------------------------------------------------------------------
MT_GLFont::~MT_GLFont()
{
    glDeleteLists( nBasePoly_, 256);

    if( bUseOutline_ )
        glDeleteLists( nBaseLine_, 256);
}



// -----------------------------------------------------------------------------
// Name: MT_GLFont::Print
// Created: APE 2004-06-30
// -----------------------------------------------------------------------------
void MT_GLFont::Print( MT_Vector2D vPos, const std::string& strText, float rFontSize, int nOutlineSize, E_Alignement nAlign )
{
    if( strText == "" )
        return;

    glPushMatrix();                                     // Store The Projection Matrix
    glTranslatef( vPos.rX_, vPos.rY_, 0.0f );           // Center Our Text On The Screen
    glScalef( rFontSize, rFontSize, 1.f );

    // Position the text
    MT_Vector2D size = GetTextSize( strText );
    if( nAlign & eHCenter )
        glTranslatef( - ( size.rX_ / 2.f ), 0.0f, 0.0f );
    else if( nAlign & eHLeft )
        glTranslatef( - size.rX_ , 0.0f, 0.0f );

    if( nAlign & eVCenter )
        glTranslatef( 0.0f, - ( size.rY_ / 2.f ), 0.0f );
    else if( nAlign & eVBottom )
        glTranslatef( 0.0f, - size.rY_, 0.0f );

    glPushAttrib(GL_LIST_BIT);                              // Pushes The Display List Bits
    if( nOutlineSize > 0 && bUseOutline_ )
    {
        glLineWidth( nOutlineSize );
        glListBase( nBaseLine_ );                                           // Sets The Base Character to 0
    }
    else
        glListBase( nBasePoly_ );                                           // Sets The Base Character to 0

    glCallLists( strText.length(), GL_UNSIGNED_BYTE, strText.c_str() ); // Draws The Display List Text


    glPopAttrib();                                          // Pops The Display List Bits
    glPopMatrix();                                          // Restore The Old Projection Matrix
}


// -----------------------------------------------------------------------------
// Name: MT_GLFont::GetTextSize
// Created: APE 2004-06-30
// -----------------------------------------------------------------------------
MT_Vector2D MT_GLFont::GetTextSize( const std::string& sText ) const
{
    MT_Vector2D vSize( 0. ,0. );
    const char* pText = sText.c_str();
    for( uint i = 0 ; i < strlen( pText ) ; ++i )
    {
        unsigned char c = pText[i];

        vSize.rX_ += gmfPoly_[ c ].gmfCellIncX;

        MT_Float rSizeY = gmfPoly_[ c ].gmfBlackBoxY;
        if( vSize.rY_ < rSizeY )
            vSize.rY_ = rSizeY;
    }

    return vSize;
}


// -----------------------------------------------------------------------------
// Name: MT_GLFont::Font
// Created: APE 2004-07-13
// -----------------------------------------------------------------------------
MT_GLFont* MT_GLFont::Font( const std::string& strFontName, float rPrecision, uint nCharSet, bool bUseOutline, int nTypeSize )
{
    // Check if the font exists.
    for( MT_GLFontManager::IT_FontVector it = gFontManager_.fonts_.begin(); it != gFontManager_.fonts_.end(); ++it )
    {
        if(    (*it)->strFontName_ == strFontName
            && (*it)->rPrecision_  == rPrecision
            && (*it)->nCharset_    == nCharSet
            && (*it)->nTypeSize_   == nTypeSize
            && (*it)->bUseOutline_ >= bUseOutline )
            return *it;
    }

    // If not, try to create it.
    MT_GLFont* pNewFont = new MT_GLFont( strFontName, rPrecision, nCharSet, bUseOutline, nTypeSize );
    if( ! pNewFont->IsValid() )
    {
        delete pNewFont;
        return 0;
    }

    gFontManager_.fonts_.push_back( pNewFont );
    return pNewFont;
}


// -----------------------------------------------------------------------------
// Name: MT_GLFont::Print
// Created: APE 2004-07-13
// -----------------------------------------------------------------------------
void MT_GLFont::Print( const std::string& strFontName, MT_Vector2D vPos, const std::string& strText, float rFontSize, int nOutlineSize, E_Alignement nAlign )
{
    MT_GLFont* pFont = 0;
    for( MT_GLFontManager::IT_FontVector it = gFontManager_.fonts_.begin(); it != gFontManager_.fonts_.end(); ++it )
    {
        if((*it)->strFontName_ == strFontName )
        {
            pFont = *it;
            break;
        }
    }
    
    if( pFont == 0 )
        pFont = MT_GLFont::Font( strFontName );

    if( pFont != 0 )
        pFont->Print( vPos, strText, rFontSize, nOutlineSize, nAlign );
}


// -----------------------------------------------------------------------------
// Name: MT_GLFont::DeleteFonts
// Created: APE 2004-07-13
// -----------------------------------------------------------------------------
void MT_GLFont::DeleteFonts()
{
    MT_DELETEOWNED( gFontManager_.fonts_ );
    gFontManager_.fonts_.clear();
}
