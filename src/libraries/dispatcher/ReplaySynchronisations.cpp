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

DO_UPDATE( ASN1T_MsgUnitCreation );
DO_UPDATE( ASN1T_MsgAutomatCreation );
DO_UPDATE( ASN1T_MsgFormationCreation );
DO_UPDATE( ASN1T_MsgKnowledgeGroupCreation );
DO_UPDATE( ASN1T_MsgReport );
DO_UPDATE( ASN1T_MsgTeamCreation );
DO_UPDATE( ASN1T_MsgObjectCreation );
DO_UPDATE( ASN1T_MsgLimaCreation );
DO_UPDATE( ASN1T_MsgLimitCreation );
DO_UPDATE( ASN1T_MsgPopulationCreation );
DO_UPDATE( ASN1T_MsgPopulationConcentrationCreation );
DO_UPDATE( ASN1T_MsgPopulationFlowCreation );
DO_UPDATE( ASN1T_MsgIntelligenceCreation );
DO_UPDATE( ASN1T_MsgUnitKnowledgeCreation );
DO_UPDATE( ASN1T_MsgObjectKnowledgeCreation );
DO_UPDATE( ASN1T_MsgPopulationKnowledgeCreation );
DO_UPDATE( ASN1T_MsgPopulationConcentrationKnowledgeCreation );
DO_UPDATE( ASN1T_MsgPopulationFlowKnowledgeCreation );
DO_UPDATE( ASN1T_MsgLogMaintenanceHandlingCreation );
DO_UPDATE( ASN1T_MsgLogMedicalHandlingCreation );
DO_UPDATE( ASN1T_MsgLogSupplyHandlingCreation );
DO_UPDATE( ASN1T_MsgStartUnitFire );
DO_UPDATE( ASN1T_MsgStartFireEffect );
DO_UPDATE( ASN1T_MsgStartPopulationFire );
DO_UPDATE( ASN1T_MsgUrbanKnowledgeCreation );

// -----------------------------------------------------------------------------
// Name: ReplaySynchronisations::DoUpdate
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
void ReplaySynchronisations::DoUpdate( const ASN1T_MsgChangeDiplomacy& )
{
    FlagUpdate();
}

// -----------------------------------------------------------------------------
// Name: ReplaySynchronisations::DoUpdate
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
void ReplaySynchronisations::DoUpdate( const ASN1T_MsgChangeDiplomacyAck& )
{
    FlagUpdate();
}
