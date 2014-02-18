// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "SvgElement.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: SvgElement constructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
SvgElement::SvgElement( xml::xistream& input, const Factory_ABC& factory, References_ABC& references, const PropertyFactory& propertyFactory )
    : Container( input, factory, references, propertyFactory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SvgElement constructor
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
SvgElement::SvgElement( const SvgElement& rhs )
    : Container( rhs )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SvgElement destructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
SvgElement::~SvgElement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SvgElement::DrawInternal
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
void SvgElement::DrawInternal( RenderingContext_ABC& context, References_ABC& references ) const
{
    DrawChildren( context, references );
}

// -----------------------------------------------------------------------------
// Name: SvgElement::CompileInternal
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
Node& SvgElement::CompileInternal( RenderingContext_ABC& context, References_ABC& references ) const
{
    return CompileChildren( *new SvgElement( *this ), context, references );
}
