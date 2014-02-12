// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ObjectInteraction.h"
#include "MIL_Object_ABC.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "simulation_terrain/TER_World.h"
#include "simulation_terrain/TER_AgentManager.h"
#include "simulation_terrain/TER_PopulationManager.h"
#include <boost/range/algorithm_ext/erase.hpp>
#include <boost/range/algorithm.hpp>
#pragma warning( push, 0 )
#include <boost/function_output_iterator.hpp>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
MIL_ObjectInteraction::MIL_ObjectInteraction()
    : height_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction destructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
MIL_ObjectInteraction::~MIL_ObjectInteraction()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::load
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> agentsInside_
         >> agentsEntering_
         >> agentsExiting_
         >> agentsMovingInside_
         >> agentsDelayedEntering_
         >> populationsInside_
         >> populationsMovingInside_
         >> height_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::save
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << agentsInside_
         << agentsEntering_
         << agentsExiting_
         << agentsMovingInside_
         << agentsDelayedEntering_
         << populationsInside_
         << populationsMovingInside_
         << height_;
}

namespace
{
    template< typename C >
    void RemoveDeletedAgent( C& agents, MIL_Object_ABC* object = 0 )
    {
        for( auto it = agents.begin(); it != agents.end(); )
        {
            if( (*it)->IsMarkedForDestruction() )
            {
                if( object )
                    object->ProcessAgentExiting( **it );
                it = agents.erase( it );
            }
            else
                ++it;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction::Clean
// Created: JSR 2013-01-29
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::Clean( MIL_Object_ABC& object )
{
    RemoveDeletedAgent( agentsInside_, &object );
    RemoveDeletedAgent( agentsEntering_ );
    RemoveDeletedAgent( agentsExiting_ );
    RemoveDeletedAgent( agentsMovingInside_ );
    RemoveDeletedAgent( agentsDelayedEntering_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction::UpdateInteraction
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::UpdateInteraction( MIL_Object_ABC& object, const TER_Localisation& location )
{
    double precision = object.GetPrecision();
    UpdateAgents( object, location, precision );
    UpdatePopulations( location, precision );
}

namespace
{
    struct Comparator
    {
        bool operator()( const MIL_Agent_ABC* lhs, const MIL_Agent_ABC* rhs ) const
        {
            return lhs->GetID() < rhs->GetID();
        }
    };
    typedef std::set< MIL_Agent_ABC*, Comparator > T_SortedAgents;

    T_SortedAgents GetAgentsInside( const TER_Localisation& location, double precision )
    {
        TER_Agent_ABC::T_AgentPtrVector agents;
        TER_World::GetWorld().GetAgentManager().GetListWithinLocalisation( location, agents, precision );
        T_SortedAgents result;
        for( auto it = agents.begin(); it != agents.end(); ++it )
            result.insert( &static_cast< PHY_RoleInterface_Location* >( *it )->GetAgent() );
        return result;
    }
    void NotifyTerrainPutOutsideObject( MIL_Agent_ABC* agent, MIL_Object_ABC& object )
    {
        agent->GetRole< PHY_RoleInterface_Location >().NotifyTerrainPutOutsideObject( object );
    }
    void NotifyTerrainPutInsideObject( MIL_Agent_ABC* agent, MIL_Object_ABC& object )
    {
        agent->GetRole< PHY_RoleInterface_Location >().NotifyTerrainPutInsideObject( object );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction::UpdateAgents
// Created: MCO 2013-02-13
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::UpdateAgents( MIL_Object_ABC& object, const TER_Localisation& location, double precision )
{
    const T_SortedAgents agents( agentsInside_.begin(), agentsInside_.end() );
    const T_SortedAgents inside = GetAgentsInside( location, precision );
    boost::set_difference( agents, inside, boost::make_function_output_iterator( boost::bind( &NotifyTerrainPutOutsideObject, _1, boost::ref( object ) ) ) );
    boost::set_difference( inside, agents, boost::make_function_output_iterator( boost::bind( &NotifyTerrainPutInsideObject, _1, boost::ref( object ) ) ) );
}

namespace
{
    template< typename P >
    void GetConcentrations( const TER_Localisation& location, P& populations, double precision )
    {
        TER_PopulationConcentration_ABC::T_PopulationConcentrationVector concentrations;
        TER_World::GetWorld().GetPopulationManager().GetConcentrationManager().GetListWithinLocalisation( location, concentrations, precision );
        for( auto it = concentrations.begin(); it != concentrations.end(); ++it )
            populations.insert( static_cast< MIL_PopulationConcentration* >( *it ) );
    }
    template< typename P >
    void GetFlows( const TER_Localisation& location, P& populations, double precision )
    {
        TER_PopulationFlow_ABC::T_PopulationFlowVector flows;
        TER_World::GetWorld().GetPopulationManager().GetFlowManager().GetListWithinLocalisation( location, flows, precision );
        for( auto it = flows.begin(); it != flows.end(); ++it )
            populations.insert( static_cast< MIL_PopulationFlow* >( *it ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction::UpdatePopulations
// Created: MCO 2013-02-13
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::UpdatePopulations( const TER_Localisation& location, double precision )
{
    T_Populations populations;
    GetConcentrations( location, populations, precision );
    GetFlows( location, populations, precision );
    populations.swap( populationsInside_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction::NotifyPopulationMovingInside
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::NotifyPopulationMovingInside( MIL_PopulationElement_ABC& population )
{
    populationsInside_.insert( &population );
    populationsMovingInside_.insert( &population );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction::NotifyPopulationMovingOutside
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::NotifyPopulationMovingOutside( MIL_PopulationElement_ABC& population )
{
    populationsInside_.erase( &population );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction::NotifyAgentMovingInside
// Created: JCR 2008-05-29
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::NotifyAgentMovingInside( MIL_Agent_ABC& agent )
{
    if( agentsInside_.insert( &agent ).second )
        agentsEntering_.insert( &agent );
    agentsMovingInside_.insert( &agent );
    agentsExiting_.erase( &agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction::NotifyAgentMovingOutside
// Created: JCR 2008-05-29
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::NotifyAgentMovingOutside( MIL_Agent_ABC& agent )
{
    if( agentsInside_.erase( &agent ) == 1 )
        agentsExiting_.insert( &agent );
    agentsDelayedEntering_.erase( &agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction::NotifyAgentPutInside
// Created: JCR 2008-05-29
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::NotifyAgentPutInside( MIL_Agent_ABC& agent )
{
    NotifyAgentMovingInside( agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction::NotifyAgentPutOutside
// Created: JCR 2008-05-29
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::NotifyAgentPutOutside( MIL_Agent_ABC& agent )
{
    if( agentsInside_.erase( &agent ) == 1 )
        agentsExiting_.insert( &agent );
    agentsDelayedEntering_.erase( &agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction::ClearInteraction
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::ClearInteraction( MIL_Object_ABC& object )
{
    T_Agents agents = agentsInside_;
    boost::for_each( agents, boost::bind( &NotifyTerrainPutOutsideObject, _1, boost::ref( object ) ) );
    agentsInside_.clear();
    agentsEntering_.clear();
    agentsMovingInside_.clear();
    agentsDelayedEntering_.clear();
    populationsInside_.clear();
    populationsMovingInside_.clear();
    ProcessInteractionEvents( object );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction::ProcessInteractionEvents
// Created: JCR 2008-05-29
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::ProcessInteractionEvents( MIL_Object_ABC& object )
{
    for( auto it = agentsInside_.begin(); it != agentsInside_.end(); ++it )
        object.PreprocessAgent( **it );

    T_Agents agents;
    for( auto it = agentsDelayedEntering_.begin(); it != agentsDelayedEntering_.end(); ++it )
        if( object.CanInteractWith( **it ) )
        {
            agents.insert( *it );
            object.ProcessAgentEntering( **it );
            if( agentsMovingInside_.find( *it ) ==  agentsMovingInside_.end() )
                object.ProcessAgentMovingInside( **it );
        }
    for( auto it = agents.begin(); it != agents.end(); ++it )
        agentsDelayedEntering_.erase( *it );

    for( auto it = agentsEntering_.begin(); it != agentsEntering_.end(); ++it )
        if( object.CanInteractWith( **it ) )
            object.ProcessAgentEntering( **it );
        else
            agentsDelayedEntering_.insert( *it );

    for( auto it = agentsExiting_.begin(); it != agentsExiting_.end(); ++it )
        object.ProcessAgentExiting( **it );

    for( auto it = agentsMovingInside_.begin(); it != agentsMovingInside_.end(); ++it )
        if( object.CanInteractWith( **it ) )
            object.ProcessAgentMovingInside( **it );

    for( auto it = agentsInside_.begin(); it != agentsInside_.end(); ++it )
        if( object.CanInteractWith( **it ) )
            object.ProcessAgentInside( **it );

    if( object.CanInteractWithEntity() )
    {
        for( auto it = populationsInside_.begin(); it != populationsInside_.end(); ++it )
            object.ProcessPopulationInside( **it );
        for( auto it = populationsMovingInside_.begin(); it != populationsMovingInside_.end(); ++it )
            (*it)->NotifyObjectCollision( object );
    }

    agentsEntering_.clear();
    agentsExiting_.clear();
    agentsMovingInside_.clear();
    populationsInside_.clear();
    populationsMovingInside_.clear();
}
