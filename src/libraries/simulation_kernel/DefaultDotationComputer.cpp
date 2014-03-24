// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "simulation_kernel/DefaultDotationComputer.h"
#include "simulation_kernel/Entities/Agents/Units/Dotations/PHY_DotationGroupContainer.h"
#include "simulation_kernel/Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include <string>

using namespace dotation;

// -----------------------------------------------------------------------------
// Name: DefaultDotationComputer::DefaultDotationComputer
// Created: MGD 2009-09-30
// -----------------------------------------------------------------------------
DefaultDotationComputer::DefaultDotationComputer()
    : pDotationContainer_ ( 0 )
    , forbiddenAmmunition_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultDotationComputer::~DefaultDotationComputer
// Created: MGD 2009-09-30
// -----------------------------------------------------------------------------
DefaultDotationComputer::~DefaultDotationComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultDotationComputer::Reset
// Created: MGD 2009-09-30
// -----------------------------------------------------------------------------
void DefaultDotationComputer::Reset()
{
    pDotationContainer_ = 0;
}

// -----------------------------------------------------------------------------
// Name: DefaultDotationComputer::Reset
// Created: MGD 2009-09-30
// -----------------------------------------------------------------------------
void DefaultDotationComputer::SetDotationContainer( PHY_DotationGroupContainer& container )
{
    pDotationContainer_ = &container;
}

// -----------------------------------------------------------------------------
// Name: DefaultDotationComputer::HasDotation
// Created: MGD 2009-09-30
// -----------------------------------------------------------------------------
bool DefaultDotationComputer::HasDotation( const PHY_DotationCategory& category ) const
{
    if( forbiddenAmmunition_ &&
        std::find( forbiddenAmmunition_->begin(), forbiddenAmmunition_->end(), &category ) != forbiddenAmmunition_->end() )
        return false;
    assert( pDotationContainer_ );
    return pDotationContainer_->GetValue( category ) > 0.;
}

// -----------------------------------------------------------------------------
// Name: DefaultDotationComputer::GetDotationValue
// Created: MGD 2009-09-30
// -----------------------------------------------------------------------------
double DefaultDotationComputer::GetDotationValue( const PHY_DotationCategory& category ) const
{
    assert( pDotationContainer_ );
    return pDotationContainer_->GetValue( category );
}

// -----------------------------------------------------------------------------
// Name: DefaultDotationComputer::GetDotationCapacity
// Created: MGD 2009-09-30
// -----------------------------------------------------------------------------
double DefaultDotationComputer::GetDotationCapacity( const PHY_DotationCategory& category ) const
{
    assert( pDotationContainer_ );
    return pDotationContainer_->GetCapacity( category );
}

// -----------------------------------------------------------------------------
// Name: DefaultDotationComputer::SetForbiddenAmmunition
// Created: HBD 2010-04-21
// -----------------------------------------------------------------------------
void DefaultDotationComputer::SetForbiddenAmmunition( const std::vector< const PHY_DotationCategory* >* container )
{
    forbiddenAmmunition_ = container;
}
