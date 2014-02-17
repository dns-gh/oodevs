// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "Use.h"
#include "References_ABC.h"
#include "Transformations.h"
#include <stdexcept>

using namespace svg;

// -----------------------------------------------------------------------------
// Name: Use constructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
Use::Use( xml::xistream& input, References_ABC& references, const PropertyFactory& factory )
    : Node( input, references, factory )
    , translation_( 0 )
{
    // $$$$ AGE 2006-08-22: width and height 
    // $$$$ AGE 2006-08-22: gerer les svg et les symbols
    float x = 0, y = 0;
    input >> xml::optional() >> xml::attribute( "x", x )
          >> xml::optional() >> xml::attribute( "y", y );
    translation_ = Transformations::Translation( x, y );

    // $$$$ AGE 2006-08-22: faire un truc pour parser les liens
    std::string target;
    input >> xml::attribute( "xlink:href", target ); // $$$$ AGE 2006-08-22: 
    if( target.empty() )
        throw std::runtime_error( __FUNCTION__ );
    if( target[0] == '#' )
        target_ = target.substr( 1 );
    else
        target_ = target;
    // $$$$ AGE 2006-08-22: n'importe quoi
}

// -----------------------------------------------------------------------------
// Name: Use constructor
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
Use::Use( const Use& rhs )
    : Node( rhs )
    , target_( rhs.target_ )
    , translation_( new Transformations( *rhs.translation_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Use destructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
Use::~Use()
{
    delete translation_;
}

// -----------------------------------------------------------------------------
// Name: Use::DrawInternal()
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
void Use::DrawInternal( RenderingContext_ABC& context, References_ABC& references ) const
{
    translation_->Apply();
    references.Draw( target_, context );
    translation_->Undo();
}

// -----------------------------------------------------------------------------
// Name: Use::CompileInternal
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
Node& Use::CompileInternal( RenderingContext_ABC& , References_ABC& ) const
{
    return *new Use( *this );
}

