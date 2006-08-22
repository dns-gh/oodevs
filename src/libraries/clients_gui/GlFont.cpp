// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GlFont.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: GlFont::GlFont
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
GlFont::GlFont( const std::string& name, bool outlines )
    : baseList_( glGenLists( 256 ) )
{
    HDC hDC = qt_display_dc();
    HFONT font = CreateFont(  -12,
                                0,
                                0,
                                0,
                                FW_THIN,
                                FALSE,
                                FALSE,
                                FALSE,
                                DEFAULT_CHARSET,
                                OUT_TT_PRECIS,
                                CLIP_DEFAULT_PRECIS,
                                ANTIALIASED_QUALITY,
                                FF_DONTCARE|DEFAULT_PITCH,
                                name.c_str() );
    if( font == NULL )
        throw std::runtime_error( "Unable to create font " + name );

    if( SelectObject( hDC, font ) == NULL )
        throw std::runtime_error( "Unable to select font " + name );

    BOOL result = wglUseFontOutlines( hDC,
                                      0,
                                      255,
                                      baseList_,
                                      0.005f,
                                      0.0f,
                                      outlines ? WGL_FONT_LINES : WGL_FONT_POLYGONS,
                                      gmfPoly_ );

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
    glDeleteLists( baseList_, 256 );
}

// -----------------------------------------------------------------------------
// Name: GlFont::Print
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
void GlFont::Print( const std::string& message )
{
    if( message.empty() )
        return;
    glPushAttrib( GL_LIST_BIT );
    glListBase( baseList_ );
    glCallLists( message.length(), GL_UNSIGNED_BYTE, message.c_str() );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: GlFont::GetTextSize
// Created: SBO 2006-03-21
// -----------------------------------------------------------------------------
geometry::Vector2f GlFont::GetTextSize( const std::string& message ) const
{
    float x = 0, y = 0;
    for( std::string::const_iterator it = message.begin(); it != message.end(); ++it )
    {
        x += gmfPoly_[ *it ].gmfCellIncX;
        y = std::max( y, gmfPoly_[ *it ].gmfBlackBoxY );
    }
    return geometry::Vector2f( x, y );
}
