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
#include "protocol/Protocol.h"
#include <boost/foreach.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogisticsModel constructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LogisticsModel::LogisticsModel( LogisticConsignFactory_ABC& factory, const tools::Resolver_ABC< kernel::Agent_ABC >& resolver,
                                const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver,
                                const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver,
                                const tools::Resolver_ABC< kernel::DotationType >& dotationResolver,
                                kernel::Controller& controller )
    : factory_          ( factory )
    , resolver_         ( resolver )
    , automatResolver_  ( automatResolver )
    , formationResolver_( formationResolver )
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
    if( kernel::Agent_ABC* consumer = resolver_.Find( message.unit().id() ) )
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
    void UpdateLogisticPion( U* unit, U* newUnit, C& consign )
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
            kernel::Entity_ABC* entity = resolver_.Find( message.provider().id() );
            UpdateLogisticPion< kernel::Entity_ABC, E, C >( handler, entity, *consign );
            handler = entity;
        }
        consign->Update( message, handler );
        controller_.Update( *consign );
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
// Name: LogisticsModel::GetMaintenanceConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LogMaintenanceConsign& LogisticsModel::GetMaintenanceConsign( unsigned long id )
{
    return tools::Resolver< LogMaintenanceConsign >::Get( id );
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
// Name: LogisticsModel::GetMedicalConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LogMedicalConsign& LogisticsModel::GetMedicalConsign( unsigned long id )
{
    return tools::Resolver< LogMedicalConsign >::Get( id );
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
        kernel::Agent_ABC* pPionLogConvoying = consign->GetConvoying();
        if( message.has_convoyer() && ( !pPionLogConvoying || message.convoyer().id() != pPionLogConvoying->GetId() ) )
        {
            kernel::Agent_ABC* entity = resolver_.Find( message.convoyer().id() );
            UpdateLogisticPion< kernel::Agent_ABC, LogSupplyConsigns, LogSupplyConsign >(
                pPionLogConvoying, entity, *consign );
            pPionLogConvoying = entity;
        }

        if( message.has_requests() )
        {
            for( auto it = consign->CreateIterator(); it.HasMoreElements(); )
                it.NextElement().recipient_.Get< LogSupplyConsigns >().RemoveConsign( *consign );
            consign->DeleteAll();
            BOOST_FOREACH( const sword::SupplyRecipientResourcesRequest& data, message.requests().requests() )
            {
                SupplyRecipientResourcesRequest* request = new SupplyRecipientResourcesRequest( dotationResolver_, automatResolver_, data );
                consign->Register( data.recipient().id(), *request );
                request->recipient_.Get< LogSupplyConsigns >().AddConsign( *consign );
            }
        }
        consign->Update( message, pPionLogConvoying );
        controller_.Update( *consign );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::GetSupplyConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LogSupplyConsign& LogisticsModel::GetSupplyConsign( unsigned long id )
{
    return tools::Resolver< LogSupplyConsign >::Get( id );
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
        if(  kernel::Agent_ABC* pPionLogConvoying = consign->GetConvoying() )
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
            UpdateLogisticPion< kernel::Entity_ABC, LogFuneralConsigns, LogFuneralConsign >(
                handler, entity, *consign );
            handler = entity;
        }

        kernel::Agent_ABC* convoy = consign->GetConvoy();
        if( message.has_convoying_unit() )
        {
            kernel::Agent_ABC* entity = resolver_.Find( message.convoying_unit().id() );
            UpdateLogisticPion< kernel::Agent_ABC, LogFuneralConsigns, LogFuneralConsign >(
                convoy, entity, *consign );
            convoy = entity;
        }

        consign->Update( message, handler, convoy );
        controller_.Update( *consign );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::GetFuneralConsign
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LogFuneralConsign& LogisticsModel::GetFuneralConsign( unsigned long id )
{
    return tools::Resolver< LogFuneralConsign >::Get( id );
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
    template< typename T >
    void DeleteDeletedAgentConsigns( tools::Resolver< T >& model, unsigned long id )
    {
        DeleteAgentFunctor< T > functor( id );
        model.Apply( functor );
        for( auto it = functor.toDelete_.begin(); it!= functor.toDelete_.end(); ++it )
            model.Delete( *it );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::DestroyAgent
// Created: LDC 2013-09-16
// -----------------------------------------------------------------------------
void LogisticsModel::DestroyAgent( unsigned long id )
{
    DeleteDeletedAgentConsigns< LogMaintenanceConsign >( *this, id );
    DeleteDeletedAgentConsigns< LogMedicalConsign >( *this, id );
    DeleteDeletedAgentConsigns< LogSupplyConsign >( *this, id );
    DeleteDeletedAgentConsigns< LogFuneralConsign >( *this, id );
}

// -----------------------------------------------------------------------------
// Name: LogisticsModel::FindLogEntity
// Created: AHC 2010-10-13
// -----------------------------------------------------------------------------
kernel::Entity_ABC* LogisticsModel::FindLogEntity( const sword::ParentEntity& msg )
{
    kernel::Entity_ABC* retval = 0;
    if( msg.has_automat() )
        retval = automatResolver_.Find( msg.automat().id() );
    else if( msg.has_formation() )
        retval = formationResolver_.Find( msg.formation().id() );
    return retval;
}