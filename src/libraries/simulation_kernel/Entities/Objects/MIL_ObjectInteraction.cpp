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
#include "simulation_terrain/TER_World.h"

// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
MIL_ObjectInteraction::MIL_ObjectInteraction()
    : height_ ( 0 )
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
void MIL_ObjectInteraction::load( MIL_CheckPointInArchive& file, const uint )
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
void MIL_ObjectInteraction::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << agentInsideSet_
         << agentEnteringSet_
         << agentExitingSet_
         << agentMovingInsideSet_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction::Initialize
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::Initialize( const TER_Localisation& /*location*/ )
{
    // NOTHING
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
        (**it).GetRole< PHY_RoleInterface_Location >().NotifyPutOutsideObject( object );

    intersection.clear();
    std::set_difference( newInside      .begin(), newInside      .end(), 
                         agentInsideSet_.begin(), agentInsideSet_.end(), 
                         std::insert_iterator< T_AgentSet >( intersection, intersection.end() ) );
    for( CIT_AgentSet it = intersection.begin(); it != intersection.end(); ++it )
        (**it).GetRole< PHY_RoleInterface_Location >().NotifyPutInsideObject( object );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction::NotifyAgentMovingInside
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::NotifyAgentMovingInside( MIL_PopulationElement_ABC& population )
{    
    populationInsideSet_.insert( &population );    
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction::NotifyAgentMovingOutside
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::NotifyAgentMovingOutside( MIL_PopulationElement_ABC& population )
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
    if( agentInsideSet_.erase ( &agent ) == 1 )
        agentExitingSet_.insert( &agent );   
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction::NotifyAgentPutInside
// Created: JCR 2008-05-29
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::NotifyAgentPutInside( MIL_Agent_ABC& agent )
{
    if( agentInsideSet_.insert( &agent ).second )
        agentEnteringSet_.insert( &agent );
    agentExitingSet_.erase ( &agent );
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction::NotifyAgentPutOutside
// Created: JCR 2008-05-29
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::NotifyAgentPutOutside( MIL_Agent_ABC& agent )
{
    if( agentInsideSet_ .erase( &agent ) == 1 )
        agentExitingSet_.insert( &agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction::ClearInteraction
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::ClearInteraction( MIL_Object_ABC& object )
{
    while( !agentInsideSet_.empty() )
        (**agentInsideSet_.begin()).GetRole< PHY_RoleInterface_Location >().NotifyPutOutsideObject( object );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectInteraction::ProcessInteractionEvents
// Created: JCR 2008-05-29
// -----------------------------------------------------------------------------
void MIL_ObjectInteraction::ProcessInteractionEvents( MIL_Object_ABC& object )
{
    CIT_AgentSet itAgent;
   
    for( itAgent = agentEnteringSet_.begin(); itAgent != agentEnteringSet_.end(); ++itAgent )
        object.ProcessAgentEntering( **itAgent );

    for( itAgent = agentExitingSet_.begin(); itAgent != agentExitingSet_.end(); ++itAgent )
        object.ProcessAgentExiting( **itAgent );

    for( itAgent = agentMovingInsideSet_.begin(); itAgent != agentMovingInsideSet_.end(); ++itAgent )
        object.ProcessAgentMovingInside( **itAgent );

    for( itAgent = agentInsideSet_.begin(); itAgent != agentInsideSet_.end(); ++itAgent )
        object.ProcessAgentInside( **itAgent );

    for( CIT_PopulationSet it = populationInsideSet_.begin(); it != populationInsideSet_.end(); ++it )
        object.ProcessPopulationInside( **it );

    agentEnteringSet_    .clear();
    agentExitingSet_     .clear();
    agentMovingInsideSet_.clear();
}
