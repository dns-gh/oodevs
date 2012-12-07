// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_PerceptionDisaster.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/DisasterCapacity.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeDisaster.h"
#include "Entities/Agents/MIL_Agent_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionDisaster constructor
// Created: LGY 2012-12-06
// -----------------------------------------------------------------------------
PHY_PerceptionDisaster::PHY_PerceptionDisaster( PHY_RoleInterface_Perceiver& perceiver )
    : PHY_Perception_ABC( perceiver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionDisaster destructor
// Created: LGY 2012-12-06
// -----------------------------------------------------------------------------
PHY_PerceptionDisaster::~PHY_PerceptionDisaster()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionDisaster::FinalizePerception
// Created: LGY 2012-12-06
// -----------------------------------------------------------------------------
void PHY_PerceptionDisaster::FinalizePerception()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionDisaster::Execute
// Created: LGY 2012-12-06
// -----------------------------------------------------------------------------
void PHY_PerceptionDisaster::Execute( const TER_Object_ABC::T_ObjectVector& perceivableObjects )
{
    const PHY_RoleInterface_Location& location =  perceiver_.GetPion().GetRole< PHY_RoleInterface_Location >();
    const MT_Vector2D& position = location.GetPosition();
 
    for( TER_Object_ABC::CIT_ObjectVector itObject = perceivableObjects.begin(); itObject != perceivableObjects.end(); ++itObject )
    {
        MIL_Object_ABC& object = static_cast< MIL_Object_ABC& >( **itObject );
        if( object.Retrieve< DisasterCapacity >() && Compute( object, position ) > PHY_PerceptionLevel::notSeen_ )
            perceiver_.NotifyPerception( object, position, location.GetDirection() );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionDisaster::Compute
// Created: LGY 2012-12-06
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionDisaster::Compute( const MIL_Object_ABC& object, const MT_Vector2D& position ) const
{
    if( !object.IsInside( position ) )
        return PHY_PerceptionLevel::notSeen_;

    const PHY_PerceptionLevel* pBestLevel = &PHY_PerceptionLevel::notSeen_;
    const PHY_RoleInterface_Perceiver::T_DisasterDetectors& detectors = perceiver_.GetDisasterDetectors();
    for( auto it = detectors.begin(); it != detectors.end(); ++it )
    {
        const PHY_PerceptionLevel& currentLevel = (*it)->ComputePerception( position, object );
        if( currentLevel > *pBestLevel )
        {
            pBestLevel = &currentLevel;
            if( pBestLevel->IsBestLevel() )
                return *pBestLevel;
        }
    }
    return *pBestLevel;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionDisaster::Compute
// Created: LGY 2012-12-06
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionDisaster::Compute( const DEC_Knowledge_Object& knowledge ) const
{
    const PHY_RoleInterface_Location& location = perceiver_.GetPion().GetRole< PHY_RoleInterface_Location >();
    const MT_Vector2D& position = location.GetPosition();

    const PHY_PerceptionLevel* pBestLevel = &PHY_PerceptionLevel::notSeen_;
    const PHY_RoleInterface_Perceiver::T_DisasterDetectors& detectors = perceiver_.GetDisasterDetectors();
    for( auto it = detectors.begin(); it != detectors.end(); ++it )
    {
        const PHY_PerceptionLevel& currentLevel = (*it)->ComputePerception( position, knowledge );
        if( currentLevel > *pBestLevel )
        {
            pBestLevel = &currentLevel;
            if( pBestLevel->IsBestLevel() )
                return *pBestLevel;
        }
    }
    return *pBestLevel;
}
