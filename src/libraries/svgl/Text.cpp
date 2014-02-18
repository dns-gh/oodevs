// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "Text.h"
#include "RenderingContext_ABC.h"
#include "Transformations.h"
#include "FontFamily.h"
#include "TextRenderer.h"
#include <sstream>

using namespace svg;

// -----------------------------------------------------------------------------
// Name: Text constructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
Text::Text( xml::xistream& input, References_ABC& references, TextRenderer& renderer, const PropertyFactory& factory )
    : Node( input, references, factory )
    , renderer_( renderer )
    , translation_( 0 )
{
    std::string strX, strY;
    input >> xml::optional() >> xml::attribute( "x", strX )
          >> xml::optional() >> xml::attribute( "y", strY )
          >> xml::optional() >> text_;

    float x = 0, y = 0;
    {
        // $$$$ AGE 2006-10-23: unit.
        std::stringstream str( strX );
        str >> x;
    }
    {
        std::stringstream str( strY );
        str >> y;
    }
    translation_ = Transformations::Translation( x, y );
}

// -----------------------------------------------------------------------------
// Name: Text constructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
Text::Text( const Text& rhs )
    : Node( rhs )
    , renderer_( rhs.renderer_ )
    , translation_( new Transformations( *rhs.translation_ ) )
    , text_( rhs.text_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Text destructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
Text::~Text()
{
    delete translation_;
}

// -----------------------------------------------------------------------------
// Name: Text::CompileInternal
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
Node& Text::CompileInternal( RenderingContext_ABC& , References_ABC& ) const
{
    return *new Text( *this );
}

// -----------------------------------------------------------------------------
// Name: Text::DrawInternal
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
void Text::DrawInternal( RenderingContext_ABC& context, References_ABC& references ) const
{
    translation_->Apply();
    context.SetupFill( references );
    renderer_.Render( text_, context );
    translation_->Undo();
}
