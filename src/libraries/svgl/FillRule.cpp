// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "FillRule.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: FillRule constructor
// Created: AGE 2006-08-17
// -----------------------------------------------------------------------------
FillRule::FillRule( const std::string& name )
    : type_( GLU_TESS_WINDING_NONZERO )
{
    if( name == "evenodd" )
        type_ = GLU_TESS_WINDING_ODD;
}

// -----------------------------------------------------------------------------
// Name: FillRule destructor
// Created: AGE 2006-08-17
// -----------------------------------------------------------------------------
FillRule::~FillRule()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FillRule::GetType
// Created: AGE 2006-08-17
// -----------------------------------------------------------------------------
unsigned int FillRule::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: FillRule::Clone
// Created: AGE 2006-08-17
// -----------------------------------------------------------------------------
FillRule& FillRule::Clone() const
{
    return *new FillRule( *this );
}
