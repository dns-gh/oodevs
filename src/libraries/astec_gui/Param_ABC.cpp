// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Param_ABC.cpp $
// $Author: Nld $
// $Modtime: 26/07/04 18:06 $
// $Revision: 4 $
// $Workfile: Param_ABC.cpp $
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "Param_ABC.h"
#include "astec_gaming/ActionController.h"

// -----------------------------------------------------------------------------
// Name: Param_ABC constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Param_ABC::Param_ABC()
    : bIsOptional_( false )
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
void Param_ABC::SetOptional( bool optional )
{
    bIsOptional_ = optional;
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void Param_ABC::Draw( const geometry::Point2f&, const geometry::Rectangle2f& extent, const GlTools_ABC& ) const
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
    return bIsOptional_;
}
