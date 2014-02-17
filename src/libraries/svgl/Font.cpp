// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "Font.h"
#include "TextAnchor.h"
#include "FontSize.h"
#include "RenderingContext_ABC.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: Font constructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
Font::Font( const std::string& name, int weight )
    : baseList_( glGenLists( 256 ) )
{
    HDC hDC = wglGetCurrentDC();
    HFONT font = CreateFont(  -12,
                                0,
                                0,
                                0,
                                weight,
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
                                      WGL_FONT_POLYGONS,
                                      gmfPoly_ );

    if( result == FALSE )
    {
        std::stringstream errorMessage;
        errorMessage << "wglUseFontOutlines failed for font " << name << " with error code: " << GetLastError() << std::endl;
        DeleteObject( font );
        throw std::runtime_error( errorMessage.str() );
    }

    DeleteObject( font );
}

// -----------------------------------------------------------------------------
// Name: Font destructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
Font::~Font()
{
    glDeleteLists( baseList_, 256 );
}

namespace
{
    const float symetryMatrix[4][4] = 
        { { 1, 0, 0, 0 },
        { 0, -1, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 } };
}

// -----------------------------------------------------------------------------
// Name: Font::Render
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
void Font::Render( const std::string& message, RenderingContext_ABC& context )
{
    if( message.empty() )
        return;
    glPushMatrix();
        glMultMatrixf( (const float*)symetryMatrix );
        context.GetProperty< FontSize >( RenderingContext_ABC::fontSize ).ApplyScale();
        AlignText( message, context );
        glPushAttrib( GL_LIST_BIT );
            glListBase( baseList_ );
            glCallLists( static_cast< GLsizei >( message.length() ), GL_UNSIGNED_BYTE, message.c_str() );
        glPopAttrib();
    glPopMatrix();
}

// -----------------------------------------------------------------------------
// Name: Font::GetTextLength
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
float Font::GetTextLength( const std::string& message ) const
{
    float x = 0;
    for( std::string::const_iterator it = message.begin(); it != message.end(); ++it )
        x += gmfPoly_[ *it ].gmfCellIncX;
    return x;
}

// -----------------------------------------------------------------------------
// Name: Font::AlignText
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
void Font::AlignText( const std::string& text, RenderingContext_ABC& context )
{
    const float factor = context.GetProperty< TextAnchor >( RenderingContext_ABC::textAnchor ).GetFactor();
    if( factor )
        glTranslatef( - factor * GetTextLength( text ), 0, 0 );
}
