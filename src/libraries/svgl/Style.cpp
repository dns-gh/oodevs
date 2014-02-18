// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "Style.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: Style constructor
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
Style::Style( const std::string& css, const PropertyFactory& factory )
    : properties_( css, factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Style destructor
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
Style::~Style()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Style::Push
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
void Style::Push( RenderingContext_ABC& context, References_ABC& references ) const
{
    properties_.Push( context, references );
}

// -----------------------------------------------------------------------------
// Name: Style::Pop
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
void Style::Pop( RenderingContext_ABC& context, References_ABC& references ) const
{
    properties_.Pop( context, references );
}
