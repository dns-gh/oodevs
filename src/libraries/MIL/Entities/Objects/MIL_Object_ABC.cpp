 //*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_Object_ABC.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 15:21 $
// $Revision: 7 $
// $Workfile: MIL_Object_ABC.cpp $
//
//*****************************************************************************

#include "MIL_pch.h"

#include "MIL_Object_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "TER/TER_World.h"

#include "Entities/MIL_Army.h"

//-----------------------------------------------------------------------------
// Name: MIL_Object_ABC constructor
// Created: NLD 2002-12-12
//-----------------------------------------------------------------------------
MIL_Object_ABC::MIL_Object_ABC()
    : pArmy_                ( 0 )
    , bMarkedForDestruction_( false )
    , bReadyForDeletion_    ( false )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_Object_ABC destructor
// Created: NLD 2002-12-12
//-----------------------------------------------------------------------------
MIL_Object_ABC::~MIL_Object_ABC()
{
    assert( agentInsideSet_.empty() );
    TER_Object_ABC::Terminate(); //$$$ Ne devrait pas être appelé (Cf MarkForDestruction() )
}

// =============================================================================
// INIT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::Initialize
// Created: NLD 2004-10-26
// -----------------------------------------------------------------------------
void MIL_Object_ABC::Initialize( MIL_Army& army, const TER_Localisation& localisation )
{
    TER_Object_ABC::Initialize( localisation );    
    pArmy_ = &army;

    // Notify the agent natively inside the object that they are inside it
    TER_Agent_ABC::T_AgentPtrVector agentsInsideObject;
    TER_World::GetWorld().GetAgentManager().GetListWithinLocalisation( GetLocalisation(), agentsInsideObject );
    for( TER_Agent_ABC::CIT_AgentPtrVector itAgent = agentsInsideObject.begin(); itAgent != agentsInsideObject.end(); ++itAgent )
        static_cast< PHY_RoleInterface_Location& >( **itAgent ).NotifyPutInsideObject( *this );
}

// =============================================================================
// CheckPoints
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_Object_ABC::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< TER_Object_ABC >( *this );
    file >> pArmy_
         >> bMarkedForDestruction_
         >> bReadyForDeletion_
         >> agentInsideSet_
         >> agentEnteringSet_
         >> agentExitingSet_
         >> agentMovingInsideSet_;  
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_Object_ABC::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< TER_Object_ABC >( *this );
    file << pArmy_
         << bMarkedForDestruction_
         << bReadyForDeletion_
         << agentInsideSet_
         << agentEnteringSet_
         << agentExitingSet_
         << agentMovingInsideSet_;
}


//=============================================================================
// TOOLS
//=============================================================================
// ----------------------------------------------------------------------------
// Name: MIL_Object_ABC::CanCollideWithAgent
// Created: NLD 2003-09-02
// -----------------------------------------------------------------------------
bool MIL_Object_ABC::CanCollideWithAgent( const MIL_Agent_ABC& /*agent*/ ) const
{
    return !IsMarkedForDestruction();
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::CanInteractWithAgent
// Created: NLD 2005-09-08
// -----------------------------------------------------------------------------
bool MIL_Object_ABC::CanInteractWithAgent( const MIL_Agent_ABC& agent ) const
{
    return CanCollideWithAgent( agent );
}

// =============================================================================
// EVENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::NotifyAgentMovingInside
// Created: NLD 2004-04-29
// -----------------------------------------------------------------------------
void MIL_Object_ABC::NotifyAgentMovingInside( MIL_Agent_ABC& agent )
{
    if( !CanCollideWithAgent( agent ) )
        return;

    if( agentInsideSet_.insert( &agent ).second )
        agentEnteringSet_.insert( &agent );
    agentMovingInsideSet_.insert( &agent );
    agentExitingSet_.erase ( &agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::NotifyAgentMovingOutside
// Created: NLD 2004-04-29
// -----------------------------------------------------------------------------
void MIL_Object_ABC::NotifyAgentMovingOutside( MIL_Agent_ABC& agent )
{
    if( agentInsideSet_.erase ( &agent ) == 1 )
        agentExitingSet_.insert( &agent );   
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::NotifyAgentPutInside
// Created: NLD 2004-05-07
// -----------------------------------------------------------------------------
void MIL_Object_ABC::NotifyAgentPutInside( MIL_Agent_ABC& agent )
{
    if( !CanCollideWithAgent( agent ) )
        return;

    if( agentInsideSet_.insert( &agent ).second )
        agentEnteringSet_.insert( &agent );
    agentExitingSet_.erase ( &agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::NotifyAgentPutOutside
// Created: NLD 2004-05-07
// -----------------------------------------------------------------------------
void MIL_Object_ABC::NotifyAgentPutOutside( MIL_Agent_ABC& agent )
{
    if( agentInsideSet_ .erase( &agent ) == 1 )
        agentExitingSet_.insert( &agent );    
}

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::UpdateState
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
void MIL_Object_ABC::UpdateState()
{
    if( bMarkedForDestruction_ )
        bReadyForDeletion_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::ProcessEvents
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
void MIL_Object_ABC::ProcessEvents()
{
    CIT_AgentSet itAgent;
   
    for( itAgent = agentEnteringSet_.begin(); itAgent != agentEnteringSet_.end(); ++itAgent )
        ProcessAgentEntering( **itAgent );

    for( itAgent = agentExitingSet_.begin(); itAgent != agentExitingSet_.end(); ++itAgent )
        ProcessAgentExiting( **itAgent );

    for( itAgent = agentMovingInsideSet_.begin(); itAgent != agentMovingInsideSet_.end(); ++itAgent )
        ProcessAgentMovingInside( **itAgent );

    for( itAgent = agentInsideSet_.begin(); itAgent != agentInsideSet_.end(); ++itAgent )
        ProcessAgentInside( **itAgent );

    agentEnteringSet_    .clear();
    agentExitingSet_     .clear();
    agentMovingInsideSet_.clear();
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::UpdateLocalisation
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void MIL_Object_ABC::UpdateLocalisation( const TER_Localisation& newLocalisation )
{
    TER_Object_ABC::UpdateLocalisation( newLocalisation );
    while( !agentInsideSet_.empty() )
        (**agentInsideSet_.begin()).GetRole< PHY_RoleInterface_Location >().NotifyPutOutsideObject( *this );

    TER_Agent_ABC::T_AgentPtrVector agentsInsideObject;
    TER_World::GetWorld().GetAgentManager().GetListWithinLocalisation( GetLocalisation(), agentsInsideObject );
    for( TER_Agent_ABC::CIT_AgentPtrVector itAgent = agentsInsideObject.begin(); itAgent != agentsInsideObject.end(); ++itAgent )
        static_cast< PHY_RoleInterface_Location& >( **itAgent ).NotifyPutInsideObject( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::MarkForDestruction
// Created: NLD 2004-07-01
// -----------------------------------------------------------------------------
void MIL_Object_ABC::MarkForDestruction()
{
    bMarkedForDestruction_ = true;
    while( !agentInsideSet_.empty() )
        (**agentInsideSet_.begin()).GetRole< PHY_RoleInterface_Location >().NotifyPutOutsideObject( *this );
    TER_Object_ABC::Terminate(); // Degueu : vire l'objet du monde
}


