// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
// LTO
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RolePion_Illumination.h"
#include "CheckPoints/MIL_CheckPointInArchive.h"
#include "CheckPoints/MIL_CheckPointOutArchive.h"
#include "Entities/MIL_Entity_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include <boost/serialization/set.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_RolePion_Illumination )

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Illumination constructor
// Created: MGD 2010-02-15
// -----------------------------------------------------------------------------
PHY_RolePion_Illumination::PHY_RolePion_Illumination()
    : bIlluminatedDefinitely_( false )
    , bHit_( false )
    , target_( 0 )
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
    if( !bIlluminatedDefinitely_ && illuminators_.size() == 0 )
        return false;
    return true;
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

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Illumination::load
// Created: LDC 2010-02-19
// -----------------------------------------------------------------------------
void PHY_RolePion_Illumination::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    ar >>boost::serialization::base_object<PHY_RoleInterface_Illumination>( *this );
    ar >> bIlluminatedDefinitely_;
    ar >> bHit_;
    MIL_Entity_ABC* target = 0;
    ar >> target;
    target_ = target;
    std::set< MIL_Entity_ABC* > illuminators;
    ar >> illuminators;
    for( std::set< MIL_Entity_ABC* >::const_iterator it = illuminators.begin(); it != illuminators.end(); ++it )
        illuminators_.insert( *it );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Illumination::save
// Created: LDC 2010-02-19
// -----------------------------------------------------------------------------
void PHY_RolePion_Illumination::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << boost::serialization::base_object<PHY_RoleInterface_Illumination>( *this );
    ar << bIlluminatedDefinitely_;
    ar << bHit_;
    ar << target_;
    ar << illuminators_;
}
