// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DirectFirePionEventListener.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Protection/PHY_RoleInterface_ActiveProtection.h"
#include "Entities/Agents/Actions/Firing/PHY_FireResults_Pion.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Objects/StructuralCapacity.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include "simulation_terrain/TER_PopulationManager.h"
#include "simulation_terrain/TER_World.h"
#include <core/Model.h>

using namespace sword;

//-----------------------------------------------------------------------------
// Name: DirectFirePionEventListener constructor
// Created: MCO 2012-04-26
//-----------------------------------------------------------------------------
DirectFirePionEventListener::DirectFirePionEventListener( core::Facade& facade )
    : EventListenerBase( facade, "direct fire pion" )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: DirectFirePionEventListener destructor
// Created: MCO 2012-04-26
//-----------------------------------------------------------------------------
DirectFirePionEventListener::~DirectFirePionEventListener()
{
    for( auto it = results_.begin(); it != results_.end(); ++it )
        it->second->DecRef();
}

// -----------------------------------------------------------------------------
// Name: DirectFirePionEventListener::Notify
// Created: MCO 2012-04-26
// -----------------------------------------------------------------------------
void DirectFirePionEventListener::Notify( const core::Model& event )
{
    try
    {
        if( event[ "running" ] )
            Update( event );
        else
            Remove( event );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Exception in " << __FUNCTION__ << " : " << tools::GetExceptionMsg( e ) );
    }
    catch( ... )
    {
        MT_LOG_ERROR_MSG( "Unknown exception in " << __FUNCTION__ );
    }
}

namespace
{
    void UrbanObjectApplyDirectFire( const MIL_Agent_ABC& target, const PHY_DotationCategory& category, bool hit )
    {
        const MIL_UrbanObject_ABC* urbanObject = target.GetRole< PHY_RoleInterface_UrbanLocation >().GetCurrentUrbanBlock();
        if( urbanObject )
        {
            urbanObject->ApplyDirectFire();
            if( !hit )
            {
                StructuralCapacity* capacity = const_cast< StructuralCapacity* >( urbanObject->Retrieve< StructuralCapacity >() );
                if( capacity )
                    capacity->ApplyDirectFire( *urbanObject, category );
            }
        }
    }
    void HandleCollateralDamage( PHY_FireResults_Pion* results, const MIL_AgentPion& pion, const MIL_Agent_ABC& target ) // $$$$ MCO 2012-09-05: move to a separate listener
    {
        // handle direct-indirect fire on populations
        const PHY_RoleInterface_Location& firerLocation = pion.GetRole< PHY_RoleInterface_Location >();
        const PHY_RoleInterface_Location& targetLocation = target.GetRole< PHY_RoleInterface_Location >();
        const MT_Vector2D firerPosition ( firerLocation.GetPosition().rX_, firerLocation.GetPosition().rY_ );
        const MT_Vector2D targetPosition( targetLocation.GetPosition().rX_, targetLocation.GetPosition().rY_ );
        TER_PopulationConcentration_ABC::T_PopulationConcentrationVector concentrations;
        TER_World::GetWorld().GetPopulationManager().GetConcentrationManager()
                             .GetListIntersectingLine( firerPosition, targetPosition, concentrations );
        for( TER_PopulationConcentration_ABC::CIT_PopulationConcentrationVector itConcentration = concentrations.begin();
            itConcentration != concentrations.end(); ++itConcentration )
        {
            MIL_PopulationConcentration* pElement = static_cast< MIL_PopulationConcentration* >( *itConcentration );
            pElement->ApplyFire( 1, *results, true );
        }
        TER_PopulationFlow_ABC::T_PopulationFlowVector flows;
        TER_World::GetWorld().GetPopulationManager().GetFlowManager()
                             .GetListIntersectingLine( firerPosition, targetPosition, flows );
        for( TER_PopulationFlow_ABC::CIT_PopulationFlowVector itFlow = flows.begin(); itFlow != flows.end(); ++itFlow )
        {
            MIL_PopulationFlow* pElement = static_cast< MIL_PopulationFlow* >( *itFlow );
            pElement->ApplyFire( 1, *results, true );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DirectFirePionEventListener::Update
// Created: MCO 2012-05-04
// -----------------------------------------------------------------------------
void DirectFirePionEventListener::Update( const core::Model& event )
{
    MIL_AgentPion& pion = event[ "entity/data" ].GetUserData< MIL_AgentPion >();
    MIL_AgentPion& target = event[ "enemy/data" ].GetUserData< MIL_AgentPion >();
    const int dotation = event[ "dotation" ];
    const PHY_DotationCategory* category = PHY_DotationType::FindDotationCategory( static_cast< unsigned int >( dotation ) );
    if( ! category )
    {
        MT_LOG_ERROR_MSG( "Unknown dotation category in DirectFirePionEventListener : " << dotation );
        return;
    }
    if( event[ "hit" ] )
    {
        PHY_ComposantePion& component = event[ "component/data" ].GetUserData< PHY_ComposantePion >();
        PHY_FireResults_Pion*& results = results_[ event[ "entity/identifier" ] ]; // $$$$ MCO 2012-04-26: use command id instead ? can a unit have several direct fire running in parallel ?
        if( ! results )
        {
            results = new PHY_FireResults_Pion( pion, target );
            results->IncRef();
        }
        target.GetRole< PHY_RoleInterface_Composantes >().ApplyDirectFire( component, *category, *results );
        HandleCollateralDamage( results, pion, target ); // $$$$ MCO 2012-09-10: move to a separate listener
    }
    if( event[ "use-ph" ] )
    {
        target.GetRole< PHY_RoleInterface_ActiveProtection >().UseAmmunition( *category );
        UrbanObjectApplyDirectFire( target, *category, event[ "hit" ] ); // $$$$ MCO 2012-09-10: move to a separate listener
    }
}

// -----------------------------------------------------------------------------
// Name: DirectFirePionEventListener::Remove
// Created: MCO 2012-05-04
// -----------------------------------------------------------------------------
void DirectFirePionEventListener::Remove( const core::Model& event )
{
    auto it = results_.find( event[ "entity/identifier" ] );
    if( it != results_.end() )
    {
        it->second->DecRef();
        results_.erase( it );
    }
}