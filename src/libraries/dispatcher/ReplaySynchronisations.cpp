// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ReplaySynchronisations.h"
#include "Synchroniser.h"
#include "ReplayModel_ABC.h"
#include "clients_kernel/Entity_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ReplaySynchronisations constructor
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
ReplaySynchronisations::ReplaySynchronisations( const ReplayModel_ABC& model, kernel::Entity_ABC& holder )
    : model_   ( model )
    , holder_  ( holder )
    , created_ ( false )
    , updated_ ( false )
    , synching_( false )
{
    if( model_.IsSynching() )
        StartSynchronisation( true );
}

// -----------------------------------------------------------------------------
// Name: ReplaySynchronisations destructor
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
ReplaySynchronisations::~ReplaySynchronisations()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReplaySynchronisations::StartSynchronisation
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
void ReplaySynchronisations::StartSynchronisation( bool create )
{
    synching_ = true;
    updated_ = created_ = create;
}

// -----------------------------------------------------------------------------
// Name: ReplaySynchronisations::EndSynchronisation
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
void ReplaySynchronisations::EndSynchronisation( Synchroniser& synch ) const
{
    assert( synching_ );
    if( created_ )
        synch.FlagForCreation( holder_ );
    if( updated_ )
        synch.FlagForUpdate( holder_ );
    if( ! updated_ )
        synch.FlagForDestruction( holder_ );
}

// -----------------------------------------------------------------------------
// Name: ReplaySynchronisations::StartSynchronisation
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
void ReplaySynchronisations::StartSynchronisation( kernel::Entity_ABC& next, bool create )
{
    if( synching_ )
        next.Get< ReplaySynchronisations >().StartSynchronisation( create );
}

// -----------------------------------------------------------------------------
// Name: ReplaySynchronisations::FlagUpdate
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
void ReplaySynchronisations::FlagUpdate()
{
    updated_ = true;
}

// -----------------------------------------------------------------------------
// Name: ReplaySynchronisations::DoUpdate
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
void ReplaySynchronisations::DoUpdate()
{
    if( model_.IsSynching() && !synching_ )
        StartSynchronisation( false );
    FlagUpdate();
}

#define DO_UPDATE( msg )\
    void ReplaySynchronisations::DoUpdate( const msg& )\
    {\
        DoUpdate();\
    }

DO_UPDATE( MsgsSimToClient::MsgUnitCreation );
DO_UPDATE( MsgsSimToClient::MsgAutomatCreation );
DO_UPDATE( MsgsSimToClient::MsgFormationCreation );
DO_UPDATE( MsgsSimToClient::MsgKnowledgeGroupCreation );
DO_UPDATE( MsgsSimToClient::MsgReport );
DO_UPDATE( MsgsSimToClient::MsgPartyCreation );
DO_UPDATE( MsgsSimToClient::MsgObjectCreation );
DO_UPDATE( MsgsMessengerToClient::MsgLimaCreation );
DO_UPDATE( MsgsMessengerToClient::MsgLimitCreation );
DO_UPDATE( MsgsSimToClient::MsgCrowdCreation );
DO_UPDATE( MsgsSimToClient::MsgCrowdConcentrationCreation );
DO_UPDATE( MsgsSimToClient::MsgCrowdFlowCreation );
DO_UPDATE( Common::MsgIntelligenceCreation );
DO_UPDATE( MsgsSimToClient::MsgUnitKnowledgeCreation );
DO_UPDATE( MsgsSimToClient::MsgObjectKnowledgeCreation );
DO_UPDATE( MsgsSimToClient::MsgCrowdKnowledgeCreation );
DO_UPDATE( MsgsSimToClient::MsgCrowdConcentrationKnowledgeCreation );
DO_UPDATE( MsgsSimToClient::MsgCrowdFlowKnowledgeCreation );
DO_UPDATE( MsgsSimToClient::MsgLogMaintenanceHandlingCreation );
DO_UPDATE( MsgsSimToClient::MsgLogMedicalHandlingCreation );
DO_UPDATE( MsgsSimToClient::MsgLogSupplyHandlingCreation );
DO_UPDATE( MsgsSimToClient::MsgStartUnitFire );
DO_UPDATE( MsgsSimToClient::MsgStartFireEffect );
DO_UPDATE( MsgsSimToClient::MsgStartCrowdFire );
DO_UPDATE( MsgsSimToClient::MsgUrbanCreation );
DO_UPDATE( MsgsSimToClient::MsgUrbanKnowledgeCreation );

// -----------------------------------------------------------------------------
// Name: ReplaySynchronisations::DoUpdate
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
void ReplaySynchronisations::DoUpdate( const Common::MsgChangeDiplomacy& )
{
    FlagUpdate();
}

// -----------------------------------------------------------------------------
// Name: ReplaySynchronisations::DoUpdate
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
void ReplaySynchronisations::DoUpdate( const MsgsSimToClient::MsgChangeDiplomacyAck& )
{
    FlagUpdate();
}
