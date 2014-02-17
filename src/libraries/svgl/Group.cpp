// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "Group.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: Group constructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
Group::Group( xml::xistream& input, const Factory_ABC& factory, References_ABC& references, const PropertyFactory& propertyFactory )
    : Container( input, factory, references, propertyFactory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Group constructor
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
Group::Group( const Group& rhs )
    : Container( rhs )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Group destructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
Group::~Group()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Group::DrawInternal()
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
void Group::DrawInternal( RenderingContext_ABC& context, References_ABC& references ) const
{
    DrawChildren( context, references );
}

// -----------------------------------------------------------------------------
// Name: Group::CompileInternal
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
Node& Group::CompileInternal( RenderingContext_ABC& context, References_ABC& references ) const
{
    return CompileChildren( *new Group( *this ), context, references );
}

