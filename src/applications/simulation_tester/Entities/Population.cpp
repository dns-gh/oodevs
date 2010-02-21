// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Population.cpp $
// $Author: Sbo $
// $Modtime: 21/07/05 17:06 $
// $Revision: 38 $
// $Workfile: Population.cpp $
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Entities/Population.h"
#include "Types/TypeManager.h"
#include "Types/Entities/PopulationType.h"
#include "Types/Entities/PopulationModel.h"
#include "Tools/Position.h"
#include "Tools/PositionManager.h"
#include "Actions/Scheduler.h"
#include "Actions/Missions/Mission_Population_Type.h"
#include "Workspace.h"

using namespace TEST;

// =============================================================================
// CONCENTRATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Population::Concentration
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
Population::Concentration::Concentration( const MsgPopulationConcentrationCreation& asnMsg )
: position_ ( (const char*)asnMsg.position.data() )
{
}
    
// -----------------------------------------------------------------------------
// Name: Population::~Concentration
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
Population::Concentration::~Concentration()
{
}

// -----------------------------------------------------------------------------
// Name: Population::OnUpdate
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
void Population::Concentration::OnUpdate( const MsgPopulationConcentrationUpdate& /*asnMsg*/ )
{
}

// =============================================================================
// FLOW
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Population::Flow::Flow
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
Population::Flow::Flow( const MsgPopulationFlowCreation& /*asnMsg*/ )
: shape_ ()
{
}

// -----------------------------------------------------------------------------
// Name: Population::Flow::~Flow
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
Population::Flow::~Flow()
{
}

// -----------------------------------------------------------------------------
// Name: Population::Flow::OnUpdate
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
void Population::Flow::OnUpdate( const MsgPopulationFlowUpdate& asnMsg )
{
    if( asnMsg.has_flux()  )
    {
        shape_.clear();
        for( unsigned int i = 0; i < asnMsg.flux().vecteur_point().elem_size(); ++i )
            shape_.push_back( new Position( (const char*)asnMsg.flux.vecteur_point.elem( i ).data ) );
    }
}

//-----------------------------------------------------------------------------
// Name: Population::Population
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
Population::Population( const Workspace& workspace, const MsgPopulationCreation& asnMsg )
    : Testable_Entity ( workspace )
    , nId_            ( asnMsg.oid_population() )
    , strName_        ( asnMsg.nom() )
    , pType_          ( workspace.GetTypeManager().FindPopulationType( asnMsg.type_population() ) )
{
}

//-----------------------------------------------------------------------------
// Name: Population::~Population
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
Population::~Population()
{
}

// -----------------------------------------------------------------------------
// Name: Population::SendMagicAction
// Created: SBO 2005-09-01
// -----------------------------------------------------------------------------
void Population::SendMagicAction( int /*action*/ ) const
{
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Population::GetPosition
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
const Position& Population::GetPosition() const
{
    if( concentrations_.size() > 0 )
        return ( *concentrations_.begin() )->position_;
    return *new Position();
}

// =============================================================================
// NETWORK
// =============================================================================


// -----------------------------------------------------------------------------
// Name: Population::OnUpdate
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
void Population::OnUpdate( const MsgPopulationUpdate& /*asnMsg*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Population::OnConcentrationCreated
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
void Population::OnConcentrationCreated( const MsgPopulationConcentrationCreation& asnMsg )
{
    if( !concentrations_[ asnMsg.oid_concentration() ] )
        concentrations_[ asnMsg.oid_concentration() ] = new Concentration( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Population::OnConcentrationUpdated
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
void Population::OnConcentrationUpdated( const MsgPopulationConcentrationUpdate& asnMsg )
{
    if( concentrations_[ asnMsg.oid_concentration() ] )
        concentrations_[ asnMsg.oid_concentration() ]->OnUpdate( asnMsg );
}
    
// -----------------------------------------------------------------------------
// Name: Population::OnConcentrationDestroyed
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
void Population::OnConcentrationDestroyed( const MsgPopulationConcentrationDestruction& asnMsg )
{
    if( concentrations_[ asnMsg.oid_concentration() ] )
        delete concentrations_[ asnMsg.oid_concentration() ];
}

// -----------------------------------------------------------------------------
// Name: Population::OnFlowCreated
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
void Population::OnFlowCreated( const MsgPopulationFlowCreation& asnMsg )
{
    if( !flows_[ asnMsg.oid_flux() ] )
        flows_[ asnMsg.oid_flux() ] = new Flow( asnMsg );
}
    
// -----------------------------------------------------------------------------
// Name: Population::OnFlowUpdated
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
void Population::OnFlowUpdated( const MsgPopulationFlowUpdate& asnMsg )
{
    if( flows_[ asnMsg.oid_flux() ] )
        flows_[ asnMsg.oid_flux() ]->OnUpdate( asnMsg );
}
    
// -----------------------------------------------------------------------------
// Name: Population::OnFlowDestroyed
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
void Population::OnFlowDestroyed( const MsgPopulationFlowDestruction& asnMsg )
{
    if( flows_[ asnMsg.oid_flux() ] )
        delete flows_[ asnMsg.oid_flux() ];
}

//-----------------------------------------------------------------------------
// SCHEDULING
//-----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: Population::ScheduleAllMissions
// Created: SBO 2005-08-12
// -----------------------------------------------------------------------------
void Population::ScheduleAllMissions( Scheduler& scheduler, unsigned int nIteration /* = 1 */ )
{
    assert( pType_ );
    pType_->GetModel().ScheduleAllMissions( *this, scheduler, nIteration );
}

// -----------------------------------------------------------------------------
// Name: Population::ScheduleMission
// Created: SBO 2005-08-12
// -----------------------------------------------------------------------------
void Population::ScheduleMission( Scheduler& scheduler, const std::string& strMissionName, unsigned int nIteration /* = 1 */ )
{
    assert( pType_ );
    pType_->GetModel().ScheduleMission( *this, scheduler, strMissionName, nIteration );
}

