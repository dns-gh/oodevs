// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "LogisticsModel.h"
#include "LogMaintenanceConsign.h"
#include "LogMedicalConsign.h"
#include "LogSupplyConsign.h"
#include "LogFuneralConsign.h"
#include "LogisticConsignFactory_ABC.h"
#include "LogisticConsigns.h"
#include "SupplyRecipientResourcesRequest.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "protocol/Protocol.h"
#include <boost/foreach.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogisticsModel constructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LogisticsModel::LogisticsModel( LogisticConsignFactory_ABC& factory, const kernel::EntityResolver_ABC& resolver,
                                const tools::Resolver_ABC< kernel::DotationType >& dotationResolver,
                                kernel::Controller& controller )
    : factory_          ( factory )
    , resolver_         ( resolver )
    , dotationResolver_ ( dotationResolver )
    , controller_       ( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel destructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LogisticsModel::~LogisticsModel()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::Purge
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void LogisticsModel::Purge()
{
    tools::Resolver< LogMaintenanceConsign >::DeleteAll();
    tools::Resolver< LogMedicalConsign >::DeleteAll();
    tools::Resolver< LogSupplyConsign >::DeleteAll();
    tools::Resolver< LogFuneralConsign >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::Delete
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
template< typename Type >
void LogisticsModel::Delete( unsigned long id )
{
    delete tools::Resolver< Type >::Find( id );
    tools::Resolver< Type >::Remove( id );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::Create
// Created: LGY 2013-12-13
// -----------------------------------------------------------------------------
template< typename M, typename C, typename E >
void LogisticsModel::CreateConsign( const M& message, const std::function< C*( const M&, kernel::Agent_ABC& )>& create )
{
    if( kernel::Agent_ABC* consumer = resolver_.FindAgent( message.unit().id() ) )
    {
        C* consign = create( message, *consumer );
        if( consign )
        {
            consumer->Get< E >().AddConsign( *consign );
            tools::Resolver< C >::Register( message.request().id(), *consign );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::DeleteConsign
// Created: LGY 2013-12-13
// -----------------------------------------------------------------------------
template< typename C, typename E >
void LogisticsModel::DeleteConsign( unsigned long id )
{
    if( C* consign = tools::Resolver< C >::Find( id ) )
    {
        if( kernel::Agent_ABC* consumer = consign->GetConsumer() )
            consumer->Get< E >().RemoveConsign( *consign );
        if( kernel::Entity_ABC* handler = consign->GetHandler() )
            if( E* consigns = handler->Retrieve< E >() )
                consigns->TerminateConsign( *consign );
        Delete< C >( id );
    }
}

namespace
{
    template< typename U, typename E, typename C >
    void UpdateLogistic( U* unit, U* newUnit, C& consign )
    {
        if( unit )
            unit->Get< E >().TerminateConsign( consign );
        if( newUnit )
            newUnit->Get< E >().HandleConsign( consign );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::DeleteConsign
// Created: LGY 2013-12-13
// -----------------------------------------------------------------------------
template< typename M, typename C, typename E >
void LogisticsModel::UpdateConsign( const M& message )
{
    if( C* consign = tools::Resolver< C >::Find( message.request().id() ) )
    {
        kernel::Entity_ABC* handler = consign->GetHandler();
        if( message.has_provider() && ( !handler || message.provider().id() != handler->GetId() ) )
        {
            kernel::Entity_ABC* entity = resolver_.FindEntity( message.provider().id() );
            UpdateLogistic< kernel::Entity_ABC, E, C >( handler, entity, *consign );
            handler = entity;
        }
        if( consign->Update( message, handler ) )
            controller_.Update( *consign );
        else
            DeleteConsign< C, E >( message.request().id() );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::CreateMaintenanceConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void LogisticsModel::CreateMaintenanceConsign( const sword::LogMaintenanceHandlingCreation& message )
{
   CreateConsign< sword::LogMaintenanceHandlingCreation, LogMaintenanceConsign, LogMaintenanceConsigns >( message,
       boost::bind( &LogisticConsignFactory_ABC::CreateMaintenanceConsign, &factory_, _1, _2 ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::UpdateMaintenanceConsign
// Created: LGY 2013-12-12
// -----------------------------------------------------------------------------
void LogisticsModel::UpdateMaintenanceConsign( const sword::LogMaintenanceHandlingUpdate& message )
{
    UpdateConsign< sword::LogMaintenanceHandlingUpdate, LogMaintenanceConsign, LogMaintenanceConsigns >( message );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::DeleteMaintenanceConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void LogisticsModel::DeleteMaintenanceConsign( unsigned long id )
{
    DeleteConsign< LogMaintenanceConsign, LogMaintenanceConsigns >( id );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::CreateMedicalConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void LogisticsModel::CreateMedicalConsign( const sword::LogMedicalHandlingCreation& message )
{
    CreateConsign< sword::LogMedicalHandlingCreation, LogMedicalConsign, LogMedicalConsigns >( message,
        boost::bind( &LogisticConsignFactory_ABC::CreateMedicalConsign, &factory_, _1, _2 ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::UpdateMedicalConsign
// Created: LGY 2013-12-12
// -----------------------------------------------------------------------------
void LogisticsModel::UpdateMedicalConsign( const sword::LogMedicalHandlingUpdate& message )
{
    UpdateConsign< sword::LogMedicalHandlingUpdate, LogMedicalConsign, LogMedicalConsigns >( message );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::DeleteMedicalConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void LogisticsModel::DeleteMedicalConsign( unsigned long id )
{
    DeleteConsign< LogMedicalConsign, LogMedicalConsigns >( id );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::CreateSupplyConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void LogisticsModel::CreateSupplyConsign( const sword::LogSupplyHandlingCreation& message )
{
    if( LogSupplyConsign* consign = factory_.CreateSupplyConsign( message ) )
    {
        kernel::Entity_ABC* handler = consign->GetHandler();
        if( handler )
            handler->Get< LogSupplyConsigns >().HandleConsign( *consign );
        tools::Resolver< LogSupplyConsign >::Register( message.request().id(), *consign );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::UpdateSupplyConsign
// Created: LGY 2013-12-12
// -----------------------------------------------------------------------------
void LogisticsModel::UpdateSupplyConsign( const sword::LogSupplyHandlingUpdate& message )
{
    if( LogSupplyConsign* consign = tools::Resolver< LogSupplyConsign >::Find( message.request().id() ) )
    {
        kernel::Agent_ABC* pPionLogConvoying = consign->GetConvoy();
        if( message.has_convoyer() && ( !pPionLogConvoying || message.convoyer().id() != pPionLogConvoying->GetId() ) )
        {
            kernel::Agent_ABC* entity = resolver_.FindAgent( message.convoyer().id() );
            UpdateLogistic< kernel::Agent_ABC, LogSupplyConsigns, LogSupplyConsign >(
                pPionLogConvoying, entity, *consign );
            pPionLogConvoying = entity;
        }

        if( message.has_requests() )
        {
            for( auto it = consign->CreateIterator(); it.HasMoreElements(); )
                it.NextElement().recipient_.Get< LogSupplyConsigns >().RemoveConsign( *consign );
            consign->Update( message.requests() );
            for( auto it = consign->CreateIterator(); it.HasMoreElements(); )
                it.NextElement().recipient_.Get< LogSupplyConsigns >().AddConsign( *consign );
        }
        consign->Update( message, pPionLogConvoying );
        controller_.Update( *consign );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::DeleteSupplyConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void LogisticsModel::DeleteSupplyConsign( unsigned long id )
{
    if( LogSupplyConsign* consign = tools::Resolver< LogSupplyConsign >::Find( id ) )
    {
        for( tools::Iterator< const SupplyRecipientResourcesRequest& > it = consign->CreateIterator(); it.HasMoreElements(); )
            it.NextElement().recipient_.Get< LogSupplyConsigns >().RemoveConsign( *consign );

        if( kernel::Entity_ABC* pLogHandlingEntity = consign->GetHandler() )
            pLogHandlingEntity->Get< LogSupplyConsigns >().TerminateConsign( *consign );
        if(  kernel::Agent_ABC* pPionLogConvoying = consign->GetConvoy() )
            pPionLogConvoying->Get< LogSupplyConsigns >().TerminateConsign( *consign );
        consign->DeleteAll();
        Delete< LogSupplyConsign >( id );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::CreateFuneralConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void LogisticsModel::CreateFuneralConsign( const sword::LogFuneralHandlingCreation& message )
{
    CreateConsign< sword::LogFuneralHandlingCreation, LogFuneralConsign, LogFuneralConsigns >( message,
        boost::bind( &LogisticConsignFactory_ABC::CreateFuneralConsign, &factory_, _1, _2 ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::UpdateFuneralConsign
// Created: LGY 2013-12-13
// -----------------------------------------------------------------------------
void LogisticsModel::UpdateFuneralConsign( const sword::LogFuneralHandlingUpdate& message )
{
    if( LogFuneralConsign* consign = tools::Resolver< LogFuneralConsign >::Find( message.request().id() ) )
    {
        kernel::Entity_ABC* handler = consign->GetHandler();
        if( message.has_handling_unit() )
        {
            kernel::Entity_ABC* entity = FindLogEntity( message.handling_unit() );
            UpdateLogistic< kernel::Entity_ABC, LogFuneralConsigns, LogFuneralConsign >(
                handler, entity, *consign );
            handler = entity;
        }

        kernel::Agent_ABC* convoy = consign->GetConvoy();
        if( message.has_convoying_unit() )
        {
            kernel::Agent_ABC* entity = resolver_.FindAgent( message.convoying_unit().id() );
            UpdateLogistic< kernel::Agent_ABC, LogFuneralConsigns, LogFuneralConsign >(
                convoy, entity, *consign );
            convoy = entity;
        }

        consign->Update( message, handler, convoy );
        controller_.Update( *consign );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::DeleteFuneralConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void LogisticsModel::DeleteFuneralConsign( unsigned long id )
{
    if( LogFuneralConsign* consign = tools::Resolver< LogFuneralConsign >::Find( id ) )
    {
        if( kernel::Agent_ABC* convoy = consign->GetConvoy() )
            convoy->Get< LogFuneralConsigns >().TerminateConsign( *consign );
        DeleteConsign< LogFuneralConsign,LogFuneralConsigns >( id );
    }
}

namespace
{
    template< typename T >
    struct DeleteAgentFunctor : boost::noncopyable
    {
        DeleteAgentFunctor( unsigned int id ) : id_( id ) {}
        void operator()( const T& consign ) const
        {
            if( consign.RefersToAgent( id_ ) )
                toDelete_.push_back( consign.GetId() );
        }
        unsigned long id_;
        mutable std::vector< unsigned int > toDelete_;
    };
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::DestroyAgent
// Created: LDC 2013-09-16
// -----------------------------------------------------------------------------
void LogisticsModel::DestroyAgent( unsigned long id )
{
    DeleteAgentFunctor< LogMaintenanceConsign > maintenanceFunctor( id );
    tools::Resolver< LogMaintenanceConsign >::Apply( maintenanceFunctor );
    for( auto it = maintenanceFunctor.toDelete_.begin(); it!= maintenanceFunctor.toDelete_.end(); ++it )
        DeleteMaintenanceConsign( *it );

    DeleteAgentFunctor< LogMedicalConsign > medicalFunctor( id );
    tools::Resolver< LogMedicalConsign >::Apply( medicalFunctor );
    for( auto it = medicalFunctor.toDelete_.begin(); it!= medicalFunctor.toDelete_.end(); ++it )
        DeleteMedicalConsign( *it );

    DeleteAgentFunctor< LogSupplyConsign > supplyFunctor( id );
    tools::Resolver< LogSupplyConsign >::Apply( supplyFunctor );
    for( auto it = supplyFunctor.toDelete_.begin(); it!= supplyFunctor.toDelete_.end(); ++it )
        DeleteSupplyConsign( *it );

    DeleteAgentFunctor< LogFuneralConsign > funeralFunctor( id );
    tools::Resolver< LogFuneralConsign >::Apply( funeralFunctor );
    for( auto it = funeralFunctor.toDelete_.begin(); it!= funeralFunctor.toDelete_.end(); ++it )
        DeleteFuneralConsign( *it );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::FindLogEntity
// Created: AHC 2010-10-13
// -----------------------------------------------------------------------------
kernel::Entity_ABC* LogisticsModel::FindLogEntity( const sword::ParentEntity& msg )
{
    kernel::Entity_ABC* retval = 0;
    if( msg.has_automat() )
        retval = resolver_.FindAutomat( msg.automat().id() );
    else if( msg.has_formation() )
        retval = resolver_.FindFormation( msg.formation().id() );
    return retval;
}

namespace
{
    int GetLogisticId( const sword::LogHistoryEntry& entry )
    {
        if( entry.has_funeral() )
            return entry.funeral().creation().request().id();
        if( entry.has_maintenance() )
            return entry.maintenance().creation().request().id();
        if( entry.has_medical() )
            return entry.medical().creation().request().id();
        if( entry.has_supply() )
            return entry.supply().creation().request().id();
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::Update
// Created: LGY 2013-12-12
// -----------------------------------------------------------------------------
template< typename Type >
void LogisticsModel::UpdateConsign( unsigned long id, int start, int end,
    const google::protobuf::RepeatedPtrField< sword::LogHistoryEntry >& states,
    unsigned int currentTick )
{
    Type* consign = tools::Resolver< Type >::Find( id );
    if( consign )
        consign->UpdateHistory( start, end, states, currentTick );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::UpdateLogisticHistory
// Created: LGY 2013-12-12
// -----------------------------------------------------------------------------
void LogisticsModel::UpdateLogisticHistory( int start, int end,
    const google::protobuf::RepeatedPtrField< sword::LogHistoryEntry >& states, unsigned int currentTick )
{
    if( start >= end )
        return;
    const auto& s = states.Get( start );
    const auto id = GetLogisticId( s );

    if( s.has_funeral() )
        UpdateConsign< LogFuneralConsign >( id, start, end, states, currentTick );
    else if( s.has_maintenance() )
        UpdateConsign< LogMaintenanceConsign >( id, start, end, states, currentTick );
    else if( s.has_medical() )
        UpdateConsign< LogMedicalConsign >( id, start, end, states, currentTick );
    else if( s.has_supply() )
        UpdateConsign< LogSupplyConsign >( id, start, end, states, currentTick );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::DestroyAgent
// Created: LDC 2013-09-16
// -----------------------------------------------------------------------------
void LogisticsModel::UpdateLogisticHistory( const sword::LogisticHistoryAck& message, unsigned int currentTick )
{
    int i = 0, j;
    for( j = 0; j < message.entries().size(); ++j )
    {
        if( GetLogisticId( message.entries( i ) ) == GetLogisticId( message.entries( j ) ) )
            continue;
        UpdateLogisticHistory( i, j, message.entries(), currentTick );
        i = j;
    }
    if( i != j )
        UpdateLogisticHistory( i, j, message.entries(), currentTick );
}
