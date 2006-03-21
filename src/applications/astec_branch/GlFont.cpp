// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "GlFont.h"

// -----------------------------------------------------------------------------
// Name: GlFont::GlFont
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
GlFont::GlFont( const std::string& name )
    : nBasePoly_( glGenLists( 256 ) )
{
    HDC hDC = qt_display_dc();
    HFONT font = CreateFont(  -12,                              // Height Of Font
                                0,                              // Width Of Font
                                0,                              // Angle Of Escapement
                                0,                              // Orientation Angle
                                FW_THIN,                        // Font Weight
                                FALSE,                          // Italic
                                FALSE,                          // Underline
                                FALSE,                          // Strikeout
                                DEFAULT_CHARSET,                // Character Set Identifier
                                OUT_TT_PRECIS,                  // Output Precision
                                CLIP_DEFAULT_PRECIS,            // Clipping Precision
                                ANTIALIASED_QUALITY,            // Output Quality
                                FF_DONTCARE|DEFAULT_PITCH,      // Family And Pitch
                                name.c_str() );
    if( font == NULL )
        throw std::runtime_error( "Unable to create font " + name );

    if( SelectObject( hDC, font ) == NULL ) // Selects The Font We Created
        throw std::runtime_error( "Unable to select font " + name );

    BOOL result = wglUseFontOutlines( hDC,      // Select The Current DC
                                      0,                              // Starting Character
                                      255,                            // Number Of Display Lists To Build
                                      nBasePoly_,                     // Starting Display Lists
                                      0.005,                          // Deviation From The True Outlines
                                      0.0f,                           // Font Thickness In The Z Direction
                                      WGL_FONT_POLYGONS,              // Use Polygons, Not Lines
                                      gmfPoly_ );                     // Address Of Buffer To Recieve Data
    
    if( result == FALSE )
        throw std::runtime_error( "wglUseFontOutlines failed" );

    DeleteObject( font );
}
    
// -----------------------------------------------------------------------------
// Name: GlFont::~GlFont
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
GlFont::~GlFont()
{
    glDeleteLists( nBasePoly_, 256 );
}

// -----------------------------------------------------------------------------
// Name: GlFont::Print
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
void GlFont::Print( const geometry::Point2f& where, const std::string& message, float size /*= 1.0f*/ )
{
    if( message.empty() )
        return;

    glPushMatrix();
    glTranslatef( where.X(), where.Y(), 0.0f );
    glScalef( size, size, 1.f );
    glPushAttrib( GL_LIST_BIT );
    glListBase( nBasePoly_ );
    glCallLists( message.length(), GL_UNSIGNED_BYTE, message.c_str() );
    glPopAttrib();
    glPopMatrix();
}

// -----------------------------------------------------------------------------
// Name: GlFont::GetTextSize
// Created: SBO 2006-03-21
// -----------------------------------------------------------------------------
geometry::Point2f GlFont::GetTextSize( const std::string& message ) const
{
    geometry::Point2f size( 0. ,0. );
    for( std::string::const_iterator it = message.begin(); it != message.end(); ++it )
        size.Set( size.X() + gmfPoly_[ *it ].gmfCellIncX, std::max( size.Y(), gmfPoly_[ *it ].gmfBlackBoxY ) );
    return size;
}