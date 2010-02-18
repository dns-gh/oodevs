// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RolePion_Illumination.h"

#include "Entities/MIL_Entity_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_RolePion_Illumination )

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Illumination constructor
// Created: MGD 2010-02-15
// -----------------------------------------------------------------------------
PHY_RolePion_Illumination::PHY_RolePion_Illumination()
    : target_( 0 )
    , bIlluminatedDefinitely_( false )
    , bHit_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Illumination destructor
// Created: MGD 2010-02-15
// -----------------------------------------------------------------------------
PHY_RolePion_Illumination::~PHY_RolePion_Illumination()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Illumination::IsIlluminating
// Created: MGD 2010-02-15
// -----------------------------------------------------------------------------
bool PHY_RolePion_Illumination::IsIlluminating() const
{
    return target_ != 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Illumination::NotifyStartIlluminating
// Created: MGD 2010-02-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Illumination::NotifyStartIlluminate( const MIL_Entity_ABC& entity )
{
    target_ = &entity;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Illumination::NotifyStopIlluminating
// Created: MGD 2010-02-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Illumination::NotifyStopIlluminate()
{
    target_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Illumination::IsIlluminated
// Created: MGD 2010-02-15
// -----------------------------------------------------------------------------
bool PHY_RolePion_Illumination::IsIlluminated() const
{
    return bIlluminatedDefinitely_ || illuminators_.size() > 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Illumination::IsIlluminated
// Created: MGD 2010-02-15
// -----------------------------------------------------------------------------
bool PHY_RolePion_Illumination::IsDefinitevelyIlluminated() const
{
    return bIlluminatedDefinitely_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Illumination::NotifyStartIlluminatedBy
// Created: MGD 2010-02-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Illumination::NotifyStartIlluminatedBy( const MIL_Entity_ABC& entity )
{
    illuminators_.insert( &entity );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Illumination::NotifyStopIlluminatedBy
// Created: MGD 2010-02-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Illumination::NotifyStopIlluminatedBy( const MIL_Entity_ABC& entity )
{
    illuminators_.erase( &entity );
    if( illuminators_.size() == 0 )
        bHit_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Illumination::NotifyDefinitelyIlluminated
// Created: MGD 2010-02-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Illumination::NotifyDefinitelyIlluminated()
{
    bIlluminatedDefinitely_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Illumination::IsUnderIndirectFire
// Created: MGD 2010-02-17
// -----------------------------------------------------------------------------
bool PHY_RolePion_Illumination::IsUnderIndirectFire() const
{
    return bHit_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Illumination::NotifyHitByIndirectFire
// Created: MGD 2010-02-17
// -----------------------------------------------------------------------------
void PHY_RolePion_Illumination::NotifyHitByIndirectFire()
{
    bHit_ = true;
}