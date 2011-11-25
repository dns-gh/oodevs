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
#include <boost/serialization/set.hpp>

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
    assert( agentInsideSet_.empty() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::load
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> agentInsideSet_
         >> agentEnteringSet_
         >> agentExitingSet_
         >> agentMovingInsideSet_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::save
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << agentInsideSet_
         << agentEnteringSet_
         << agentExitingSet_
         << agentMovingInsideSet_;
}

namespace
{
    std::set< MIL_PopulationElement_ABC* > Convert( TER_PopulationConcentration_ABC::T_PopulationConcentrationVector& vector )
    {
        std::set< MIL_PopulationElement_ABC* > elements;
        for( TER_PopulationConcentration_ABC::CIT_PopulationConcentrationVector it = vector.begin(); it != vector.end(); ++it )
            elements.insert( static_cast< MIL_PopulationConcentration* >( *it ) );
        return elements;
    }
    std::set< MIL_PopulationElement_ABC* > Convert( TER_PopulationFlow_ABC::T_PopulationFlowVector& vector )
    {
        std::set< MIL_PopulationElement_ABC* > elements;
        for( TER_PopulationFlow_ABC::CIT_PopulationFlowVector it = vector.begin(); it != vector.end(); ++it )
            elements.insert( static_cast< MIL_PopulationFlow* >( *it ) );
        return elements;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction::UpdateLocation
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::UpdateInteraction( MIL_Object_ABC& object, const TER_Localisation& location )
{
    TER_Agent_ABC::T_AgentPtrVector newInsideTmp;
    TER_World::GetWorld().GetAgentManager().GetListWithinLocalisation( location, newInsideTmp );

    T_AgentSet newInside;
    for( TER_Agent_ABC::CIT_AgentPtrVector it = newInsideTmp.begin(); it != newInsideTmp.end(); ++it )
    {
        MIL_Agent_ABC& agent = static_cast< PHY_RoleInterface_Location& >( **it ).GetAgent();
        newInside.insert( &agent );
    }

    T_AgentSet intersection;
    std::set_difference( agentInsideSet_.begin(), agentInsideSet_.end(),
                         newInside      .begin(), newInside      .end(),
                         std::insert_iterator< T_AgentSet >( intersection, intersection.end() ) );
    for( CIT_AgentSet it = intersection.begin(); it != intersection.end(); ++it )
        (**it).GetRole< PHY_RoleInterface_Location >().NotifyTerrainPutOutsideObject( object );

    intersection.clear();
    std::set_difference( newInside      .begin(), newInside      .end(),
                         agentInsideSet_.begin(), agentInsideSet_.end(),
                         std::insert_iterator< T_AgentSet >( intersection, intersection.end() ) );
    for( CIT_AgentSet it = intersection.begin(); it != intersection.end(); ++it )
        (**it).GetRole< PHY_RoleInterface_Location >().NotifyTerrainPutInsideObject( object );

    TER_PopulationConcentration_ABC::T_PopulationConcentrationVector populationsInsideObject;
    TER_World::GetWorld().GetPopulationManager().GetConcentrationManager().GetListWithinLocalisation( location, populationsInsideObject );
    TER_PopulationFlow_ABC::T_PopulationFlowVector populationsFlowInsideObject;
    TER_World::GetWorld().GetPopulationManager().GetFlowManager().GetListWithinLocalisation( location, populationsFlowInsideObject );
    Update( populationInsideSet_, Convert( populationsInsideObject ), boost::bind( &MIL_ObjectInteraction::NotifyPopulationMovingOutside, this, _1 ) );
    Update( populationInsideSet_, Convert( populationsFlowInsideObject ), boost::bind( &MIL_ObjectInteraction::NotifyPopulationMovingOutside, this, _1 ) );
    Update( Convert( populationsInsideObject ), populationInsideSet_, boost::bind( &MIL_ObjectInteraction::NotifyPopulationMovingInside, this, _1 ) );
    Update( Convert( populationsFlowInsideObject ), populationInsideSet_, boost::bind( &MIL_ObjectInteraction::NotifyPopulationMovingInside, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction::Update
// Created: LGY 2011-04-06
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::Update( const T_PopulationSet& last, const T_PopulationSet& current, boost::function< void( MIL_PopulationElement_ABC& ) > fun )
{
    T_PopulationSet result;
    std::set_difference( last.begin(), last.end(),
                         current.begin(), current.end(),
                         std::insert_iterator< T_PopulationSet >( result, result.end() ) );
    for( CIT_PopulationSet it = result.begin(); it != result.end(); ++it )
        fun( **it );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction::NotifyPopulationMovingInside
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::NotifyPopulationMovingInside( MIL_PopulationElement_ABC& population )
{
    populationInsideSet_.insert( &population );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction::NotifyPopulationMovingOutside
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::NotifyPopulationMovingOutside( MIL_PopulationElement_ABC& population )
{
    populationInsideSet_.erase( &population );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction::NotifyAgentMovingInside
// Created: JCR 2008-05-29
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::NotifyAgentMovingInside( MIL_Agent_ABC& agent )
{
    if( agentInsideSet_.insert( &agent ).second )
        agentEnteringSet_.insert( &agent );
    agentMovingInsideSet_.insert( &agent );
    agentExitingSet_.erase ( &agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction::NotifyAgentMovingOutside
// Created: JCR 2008-05-29
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::NotifyAgentMovingOutside( MIL_Agent_ABC& agent )
{
    if( agentInsideSet_.erase( &agent ) == 1 )
        agentExitingSet_.insert( &agent );
    agentDelayedEnteringSet_.erase( &agent );
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
    if( agentInsideSet_.erase( &agent ) == 1 )
        agentExitingSet_.insert( &agent );
    agentDelayedEnteringSet_.erase( &agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction::ClearInteraction
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::ClearInteraction( MIL_Object_ABC& object )
{
    while( !agentInsideSet_.empty() )
        (**agentInsideSet_.begin()).GetRole< PHY_RoleInterface_Location >().NotifyTerrainPutOutsideObject( object );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction::ProcessInteractionEvents
// Created: JCR 2008-05-29
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::ProcessInteractionEvents( MIL_Object_ABC& object )
{
    CIT_AgentSet itAgent;

    for( itAgent = agentInsideSet_.begin(); itAgent != agentInsideSet_.end(); ++itAgent )
        object.PreprocessAgent( **itAgent );

    T_AgentSet agentProcessedSet;
    for( itAgent = agentDelayedEnteringSet_.begin(); itAgent != agentDelayedEnteringSet_.end(); ++itAgent )
        if( object.CanInteractWith( **itAgent ) )
        {
            agentProcessedSet.insert( *itAgent );
            object.ProcessAgentEntering( **itAgent );
            if( agentMovingInsideSet_.find( *itAgent ) ==  agentMovingInsideSet_.end() )
                object.ProcessAgentMovingInside( **itAgent );
        }

    for( itAgent = agentProcessedSet.begin(); itAgent != agentProcessedSet.end(); ++itAgent )
        agentDelayedEnteringSet_.erase( *itAgent );

    for( itAgent = agentEnteringSet_.begin(); itAgent != agentEnteringSet_.end(); ++itAgent )
        if( object.CanInteractWith( **itAgent ) )
            object.ProcessAgentEntering( **itAgent );
        else
            agentDelayedEnteringSet_.insert( *itAgent );

    for( itAgent = agentExitingSet_.begin(); itAgent != agentExitingSet_.end(); ++itAgent )
        object.ProcessAgentExiting( **itAgent );

    for( itAgent = agentMovingInsideSet_.begin(); itAgent != agentMovingInsideSet_.end(); ++itAgent )
        if( object.CanInteractWith( **itAgent ) )
            object.ProcessAgentMovingInside( **itAgent );

    for( itAgent = agentInsideSet_.begin(); itAgent != agentInsideSet_.end(); ++itAgent )
        if( object.CanInteractWith( **itAgent ) )
            object.ProcessAgentInside( **itAgent );

    for( CIT_PopulationSet it = populationInsideSet_.begin(); it != populationInsideSet_.end(); ++it )
    {
        object.PreprocessPopulation( **it );
        if( object.CanInteractWithEntity() )
            object.ProcessPopulationInside( **it );
    }

    agentEnteringSet_.clear();
    agentExitingSet_.clear();
    agentMovingInsideSet_.clear();
    populationInsideSet_.clear();
}
