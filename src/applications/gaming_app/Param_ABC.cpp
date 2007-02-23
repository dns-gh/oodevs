// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "Param_ABC.h"
#include "clients_kernel/ActionController.h"
#include "OptionalParamFunctor_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Param_ABC constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Param_ABC::Param_ABC()
    : pOptional_( 0 )
    , controller_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Param_ABC destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Param_ABC::~Param_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::RemoveFromController
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void Param_ABC::RemoveFromController()
{
    if( controller_ )
        controller_->Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::RegisterIn
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void Param_ABC::RegisterIn( ActionController& controller )
{
    controller.Register( *this );
    controller_ = &controller;
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::SetOptional
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void Param_ABC::SetOptional( OptionalParamFunctor_ABC* optional )
{
    pOptional_ = optional;
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void Param_ABC::Draw( const geometry::Point2f&, const Viewport_ABC& extent, const GlTools_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::CheckValidity
// Created: APE 2004-04-22
// -----------------------------------------------------------------------------
bool Param_ABC::CheckValidity()
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::IsOptional
// Created: SBO 2005-11-08
// -----------------------------------------------------------------------------
bool Param_ABC::IsOptional() const
{
    return pOptional_;
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::SetOptionalPresent
// Created: SBO 2006-11-20
// -----------------------------------------------------------------------------
void Param_ABC::SetOptionalPresent()
{
    if( pOptional_ )
        pOptional_->SetOptionalPresent();
}
