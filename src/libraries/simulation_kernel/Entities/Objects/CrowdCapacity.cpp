// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "CrowdCapacity.h"
#include "MIL_Object_ABC.h"
#include "Entities/Populations/MIL_PopulationType.h"
#include "Entities/Populations/MIL_PopulationAttitude.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Tools/MIL_Tools.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( CrowdCapacity )

// -----------------------------------------------------------------------------
// Name: CrowdCapacity constructor
// Created: SLG 2011-01-27
// -----------------------------------------------------------------------------
CrowdCapacity::CrowdCapacity()
    : type_( 0 )
    , densityFactor_( 1.f )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrowdCapacity constructor
// Created: SLG 2011-01-27
// -----------------------------------------------------------------------------
CrowdCapacity::CrowdCapacity( const MIL_PopulationType& type, double densityFactor )
    : type_( &type )
    , densityFactor_( densityFactor )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrowdCapacity constructor
// Created: SLG 2011-01-27
// -----------------------------------------------------------------------------
CrowdCapacity::CrowdCapacity( const CrowdCapacity& from )
    : type_   ( from.type_ )
    , densityFactor_( from.densityFactor_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrowdCapacity destructor
// Created: SLG 2011-01-27
// -----------------------------------------------------------------------------
CrowdCapacity::~CrowdCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrowdCapacity::load
// Created: SLG 2011-01-27
// -----------------------------------------------------------------------------
void CrowdCapacity::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    std::string popuName;
    ar >> boost::serialization::base_object< ObjectCapacity_ABC >( *this )
       >> popuName
       >> densityFactor_;
    type_ = MIL_PopulationType::Find( popuName );
    if( !type_ )
        throw std::runtime_error( "Unknown dotation category - " + popuName + " - " );
}

// -----------------------------------------------------------------------------
// Name: CrowdCapacity::save
// Created: SLG 2011-01-27
// -----------------------------------------------------------------------------
void CrowdCapacity::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << boost::serialization::base_object< ObjectCapacity_ABC >( *this )
       << type_->GetName()
       << densityFactor_;
}

// -----------------------------------------------------------------------------
// Name: CrowdCapacity::Register
// Created: SLG 2011-01-27
// -----------------------------------------------------------------------------
void CrowdCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: CrowdCapacity::Instanciate
// Created: SLG 2011-01-27
// -----------------------------------------------------------------------------
void CrowdCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    object.AddCapacity( new CrowdCapacity( *this ) );
}

// -----------------------------------------------------------------------------
// Name: CrowdCapacity::ApplySpeedPolicy
// Created: SLG 2011-01-27
// -----------------------------------------------------------------------------
double CrowdCapacity::ApplySpeedPolicy( const MIL_Entity_ABC& entity ) const
{
    double rMaxSpeed = std::numeric_limits< double >::max();
    if ( type_ )
    {
        std::set< const PHY_Volume* > volumes_;
        const PHY_RolePion_Composantes* role = entity.RetrieveRole< PHY_RolePion_Composantes >();
        if( role )
        {
            role->GetVisibleVolumes( volumes_ );
            for( std::set< const PHY_Volume* >::const_iterator itVolume = volumes_.begin(); itVolume != volumes_.end(); ++itVolume )
                rMaxSpeed = std::min( rMaxSpeed, type_->GetPionMaxSpeed( *MIL_PopulationAttitude::Find( 0 ), densityFactor_ * type_->GetDefaultFlowDensity(), **itVolume ) );
        }
    }
    return rMaxSpeed;
}

// -----------------------------------------------------------------------------
// Name: CrowdCapacity::GetDensity
// Created: SLG 2011-02-03
// -----------------------------------------------------------------------------
double CrowdCapacity::GetDensity() const
{
    return densityFactor_ * type_->GetDefaultFlowDensity();
}

// -----------------------------------------------------------------------------
// Name: CrowdCapacity::SetDensityFactor
// Created: SLG 2011-02-11
// -----------------------------------------------------------------------------
void CrowdCapacity::SetDensityFactor( double densityFactor )
{
    densityFactor_ = densityFactor;
}

