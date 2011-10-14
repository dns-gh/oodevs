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

DO_UPDATE( sword::AutomatCreation );
DO_UPDATE( sword::CrowdConcentrationCreation );
DO_UPDATE( sword::CrowdConcentrationKnowledgeCreation );
DO_UPDATE( sword::CrowdCreation );
DO_UPDATE( sword::CrowdFlowCreation );
DO_UPDATE( sword::CrowdFlowKnowledgeCreation );
DO_UPDATE( sword::CrowdKnowledgeCreation );
DO_UPDATE( sword::FormationCreation );
DO_UPDATE( sword::KnowledgeGroupCreation );
DO_UPDATE( sword::LimitCreation );
DO_UPDATE( sword::LogMaintenanceHandlingCreation );
DO_UPDATE( sword::LogMedicalHandlingCreation );
DO_UPDATE( sword::LogSupplyHandlingCreation );
DO_UPDATE( sword::ObjectCreation );
DO_UPDATE( sword::ObjectKnowledgeCreation );
DO_UPDATE( sword::PartyCreation );
DO_UPDATE( sword::PhaseLineCreation );
DO_UPDATE( sword::PopulationCreation );
DO_UPDATE( sword::Report );
DO_UPDATE( sword::StartCrowdFire );
DO_UPDATE( sword::StartFireEffect );
DO_UPDATE( sword::StartUnitFire );
DO_UPDATE( sword::UnitCreation );
DO_UPDATE( sword::UnitKnowledgeCreation );
DO_UPDATE( sword::UrbanCreation );
DO_UPDATE( sword::UrbanKnowledgeCreation );

// -----------------------------------------------------------------------------
// Name: ReplaySynchronisations::DoUpdate
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
void ReplaySynchronisations::DoUpdate( const sword::ChangeDiplomacy& )
{
    FlagUpdate();
}

// -----------------------------------------------------------------------------
// Name: ReplaySynchronisations::DoUpdate
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
void ReplaySynchronisations::DoUpdate( const sword::ChangeDiplomacyAck& )
{
    FlagUpdate();
}
