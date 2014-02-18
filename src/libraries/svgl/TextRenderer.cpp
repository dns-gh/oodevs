// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "TextRenderer.h"
#include "RenderingContext_ABC.h"
#include "FontFamily.h"
#include "FontWeight.h"
#include "Font.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: TextRenderer constructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
TextRenderer::TextRenderer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TextRenderer destructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
TextRenderer::~TextRenderer()
{
    for( T_Fonts::const_iterator it = fonts_.begin(); it != fonts_.end(); ++it )
        delete it->second;
}

// -----------------------------------------------------------------------------
// Name: TextRenderer::Render
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
void TextRenderer::Render( const std::string& text, RenderingContext_ABC& context )
{
    const std::string& fontFamily = context.GetProperty< FontFamily >( RenderingContext_ABC::fontFamily ).GetName();
    const int          fontWeight = context.GetProperty< FontWeight >( RenderingContext_ABC::fontWeight ).GetValue();
    Font*& font = fonts_[ FontId( fontFamily, fontWeight ) ];
    if( ! font )
        font = new Font( fontFamily, fontWeight );
    font->Render( text, context );
}

// -----------------------------------------------------------------------------
// Name: TextRenderer::InitializeFont
// Created: AGE 2007-05-24
// -----------------------------------------------------------------------------
void TextRenderer::InitializeFont( const std::string& fontFamily, int fontWeight )
{
    Font*& font = fonts_[ FontId( fontFamily, fontWeight ) ];
    if( ! font )
        font = new Font( fontFamily, fontWeight );
}
