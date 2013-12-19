// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "HistoryLogisticsModel.h"
#include "LogisticConsignFactory_ABC.h"
#include "LogMaintenanceConsign.h"
#include "LogMedicalConsign.h"
#include "LogSupplyConsign.h"
#include "LogFuneralConsign.h"
#include "SupplyRecipientResourcesRequest.h"
#include "clients_kernel/Agent_ABC.h"
#include "protocol/Protocol.h"
#include <boost/foreach.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: HistoryLogisticsModel constructor
// Created: LGY 2013-12-13
// -----------------------------------------------------------------------------
HistoryLogisticsModel::HistoryLogisticsModel( LogisticConsignFactory_ABC& factory, const tools::Resolver_ABC< kernel::Agent_ABC >& resolver,
                                              const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver,
                                              const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver,
                                              const tools::Resolver_ABC< kernel::DotationType >& dotationResolver, kernel::Controller& controller )
    : LogisticsModel( factory, resolver, automatResolver, formationResolver, dotationResolver, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HistoryLogisticsModel destructor
// Created: LGY 2013-12-13
// -----------------------------------------------------------------------------
HistoryLogisticsModel::~HistoryLogisticsModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HistoryLogisticsModel::Fill
// Created: LGY 2013-12-12
// -----------------------------------------------------------------------------
void HistoryLogisticsModel::Fill( const sword::ListLogisticRequestsAck& message )
{
    for( int i = 0; i < message.entries().size(); ++i )
    {
        sword::LogHistoryEntry entry = message.entries( i );
        if( entry.has_funeral() && entry.funeral().has_creation() && entry.funeral().has_update() )
        {
            CreateFuneralConsign( entry.funeral().creation() );
            UpdateFuneralConsign( entry.funeral().update() );
        }
        else if( entry.has_maintenance() && entry.maintenance().has_creation() && entry.maintenance().has_update() )
        {
            CreateMaintenanceConsign( entry.maintenance().creation() );
            UpdateMaintenanceConsign( entry.maintenance().update() );
        }
        else if( entry.has_medical() && entry.medical().has_creation() && entry.medical().has_update() )
        {
            CreateMedicalConsign( entry.medical().creation() );
            UpdateMedicalConsign( entry.medical().update() );
        }
        else if( entry.has_supply() && entry.supply().has_creation() && entry.supply().has_update() )
        {
            CreateSupplyConsign( entry.supply().creation() );
            UpdateSupplyConsign( entry.supply().update() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: HistoryLogisticsModel::CreateMaintenanceConsign
// Created: LGY 2013-12-12
// -----------------------------------------------------------------------------
void HistoryLogisticsModel::CreateMaintenanceConsign( const sword::LogMaintenanceHandlingCreation& message )
{
    if( kernel::Agent_ABC* consumer = resolver_.Find( message.unit().id() ) )
        tools::Resolver< LogMaintenanceConsign >::Register( message.request().id(), *factory_.CreateMaintenanceConsign( message, *consumer ) );
}

// -----------------------------------------------------------------------------
// Name: HistoryLogisticsModel::UpdateMaintenanceConsign
// Created: LGY 2013-12-12
// -----------------------------------------------------------------------------
void HistoryLogisticsModel::UpdateMaintenanceConsign( const sword::LogMaintenanceHandlingUpdate& message )
{
    if( LogMaintenanceConsign* consign = tools::Resolver< LogMaintenanceConsign >::Find( message.request().id() ) )
    {
        kernel::Entity_ABC* handler = consign->GetHandler();
        if( message.has_provider() && ( !handler || message.provider().id() != handler->GetId() ) )
            handler = resolver_.Find( message.provider().id() );
        consign->Update( message, handler );
    }
}

// -----------------------------------------------------------------------------
// Name: HistoryLogisticsModel::CreateMedicalConsign
// Created: LGY 2013-12-12
// -----------------------------------------------------------------------------
void HistoryLogisticsModel::CreateMedicalConsign( const sword::LogMedicalHandlingCreation& message )
{
    if( kernel::Agent_ABC* consumer = resolver_.Find( message.unit().id() ) )
        tools::Resolver< LogMedicalConsign >::Register( message.request().id(), *factory_.CreateMedicalConsign( message, *consumer ) );
}

// -----------------------------------------------------------------------------
// Name: HistoryLogisticsModel::UpdateMedicalConsign
// Created: LGY 2013-12-12
// -----------------------------------------------------------------------------
void HistoryLogisticsModel::UpdateMedicalConsign( const sword::LogMedicalHandlingUpdate& message )
{
    if( LogMedicalConsign* consign = tools::Resolver< LogMedicalConsign >::Find( message.request().id() ) )
    {
        kernel::Entity_ABC* handler = consign->GetHandler();
        if( message.has_provider() && ( !handler || message.provider().id() != handler->GetId() ) )
            handler =  resolver_.Find( message.provider().id() );
        consign->Update( message, handler );
    }
}

// -----------------------------------------------------------------------------
// Name: HistoryLogisticsModel::CreateSupplyConsign
// Created: LGY 2013-12-12
// -----------------------------------------------------------------------------
void HistoryLogisticsModel::CreateSupplyConsign( const sword::LogSupplyHandlingCreation& message )
{
    tools::Resolver< LogSupplyConsign >::Register(  message.request().id(), *factory_.CreateSupplyConsign( message ) );
}

// -----------------------------------------------------------------------------
// Name: HistoryLogisticsModel::UpdateSupplyConsign
// Created: LGY 2013-12-12
// -----------------------------------------------------------------------------
void HistoryLogisticsModel::UpdateSupplyConsign( const sword::LogSupplyHandlingUpdate& message )
{
    if( LogSupplyConsign* consign = tools::Resolver< LogSupplyConsign >::Find( message.request().id() ) )
    {
        kernel::Agent_ABC* pPionLogConvoying = consign->GetConvoying();
        if( message.has_convoyer() && ( !pPionLogConvoying || message.convoyer().id() != pPionLogConvoying->GetId() ) )
            pPionLogConvoying = resolver_.Find( message.convoyer().id() );

        if( message.has_requests() )
        {
            consign->DeleteAll();
            BOOST_FOREACH( const sword::SupplyRecipientResourcesRequest& data, message.requests().requests() )
            {
                SupplyRecipientResourcesRequest* request = new SupplyRecipientResourcesRequest( dotationResolver_, automatResolver_, data );
                consign->Register( data.recipient().id(), *request );
            }
        }
        consign->Update( message, pPionLogConvoying );
    }
}

// -----------------------------------------------------------------------------
// Name: HistoryLogisticsModel::CreateFuneralConsign
// Created: LGY 2013-12-12
// -----------------------------------------------------------------------------
void HistoryLogisticsModel::CreateFuneralConsign( const sword::LogFuneralHandlingCreation& message )
{
    if( kernel::Agent_ABC* consumer = resolver_.Find( message.unit().id() ) )
        tools::Resolver< LogFuneralConsign >::Register(  message.request().id(), *factory_.CreateFuneralConsign( message, *consumer ) );
}

// -----------------------------------------------------------------------------
// Name: HistoryLogisticsModel::UpdateFuneralConsign
// Created: LGY 2013-12-13
// -----------------------------------------------------------------------------
void HistoryLogisticsModel::UpdateFuneralConsign( const sword::LogFuneralHandlingUpdate& message )
{
    if( LogFuneralConsign* consign = tools::Resolver< LogFuneralConsign >::Find( message.request().id() ) )
    {
        kernel::Entity_ABC* handler = consign->GetHandler();
        if( message.has_handling_unit() )
            handler = FindLogEntity( message.handling_unit() );

        kernel::Agent_ABC* convoy = consign->GetConvoy();
        if( message.has_convoying_unit() )
            convoy = resolver_.Find( message.convoying_unit().id() );

        consign->Update( message, handler, convoy );
    }
}
