// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "Definitions.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: Definitions constructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
Definitions::Definitions( xml::xistream& input, const Factory_ABC& factory, References_ABC& references, const PropertyFactory& propertyFactory )
    : Container( input, factory, references, propertyFactory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Definitions constructor
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
Definitions::Definitions( const Definitions& rhs )
    : Container( rhs )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Definitions destructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
Definitions::~Definitions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Definitions::DrawInternal()
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
void Definitions::DrawInternal( RenderingContext_ABC&, References_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Definitions::CompileInternal
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
Node& Definitions::CompileInternal( RenderingContext_ABC& context, References_ABC& references ) const
{
    return CompileChildren( *new Definitions( *this ), context, references );
}

