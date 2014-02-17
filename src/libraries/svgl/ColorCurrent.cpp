// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "ColorCurrent.h"
#include "RenderingContext_ABC.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: ColorCurrent constructor
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
ColorCurrent::ColorCurrent()
    : context_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ColorCurrent destructor
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
ColorCurrent::~ColorCurrent()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ColorCurrent::Setup
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
bool ColorCurrent::Setup( References_ABC& references , float opacity ) const
{
    if( ! context_ )
        throw std::runtime_error( __FUNCTION__ );
    return context_->GetProperty< Paint_ABC >( RenderingContext_ABC::color ).Setup( references, opacity );
}

// -----------------------------------------------------------------------------
// Name: ColorCurrent::ReachTop
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
void ColorCurrent::ReachTop( const RenderingContext_ABC& context )
{
    context_ = &context;
}

// -----------------------------------------------------------------------------
// Name: ColorCurrent::LeaveTop
// Created: AGE 2006-09-06
// -----------------------------------------------------------------------------
void ColorCurrent::LeaveTop( const RenderingContext_ABC& )
{
    context_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ColorCurrent::Clone
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
ColorCurrent& ColorCurrent::Clone() const
{
    return *new ColorCurrent();
}
