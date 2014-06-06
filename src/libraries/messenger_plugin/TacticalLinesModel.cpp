// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "messenger_plugin_pch.h"
#include "TacticalLinesModel.h"
#include "Limit.h"
#include "Lima.h"
#include "tools/IdManager.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/MessengerSenders.h"
#include <tools/Iterator.h>
#include <xeumeuleu/xml.hpp>

using namespace plugins::messenger;

// -----------------------------------------------------------------------------
// Name: TacticalLinesModel constructor
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
TacticalLinesModel::TacticalLinesModel( dispatcher::ClientPublisher_ABC& clients, tools::IdManager& idManager, const kernel::CoordinateConverter_ABC& converter )
    : clients_  ( clients )
    , idManager_( idManager )
    , converter_( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesModel destructor
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
TacticalLinesModel::~TacticalLinesModel()
{
    limas_.DeleteAll();
    limits_.DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesModel::CreateLimit
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLinesModel::ReadLimit( xml::xistream& xis, const sword::Diffusion& diffusion)
{
    std::unique_ptr< Limit > limit( new Limit( idManager_.GetNextId(), xis, diffusion, converter_ ) );
    limits_.Register( limit->GetID(), *limit );
    limit.release();
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesModel::CreateLima
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLinesModel::ReadLima( xml::xistream& xis, const sword::Diffusion& diffusion)
{
    std::unique_ptr< Lima > lima( new Lima( idManager_.GetNextId(), xis, diffusion, converter_ ) );
    limas_.Register( lima->GetID(), *lima );
    lima.release();
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesModel::Write
// Created: RDS 2008-04-09
// -----------------------------------------------------------------------------
void TacticalLinesModel::Write( xml::xostream& xos ) const
{
    xos << xml::start( "tacticalLines" );
    {
        tools::Iterator< const Limit& > it( limits_.CreateIterator() );
        while( it.HasMoreElements() )
            it.NextElement().Write( xos, converter_ );
    }
    {
        tools::Iterator< const Lima& > it( limas_.CreateIterator() );
        while( it.HasMoreElements() )
            it.NextElement().Write( xos, converter_ );
    }
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesModel::HandleLimitRequest
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void TacticalLinesModel::HandleLimitRequest( dispatcher::ClientPublisher_ABC& publisher, const sword::LimitCreationRequest& asn, unsigned int context )
{
    plugins::messenger::LimitCreationRequestAck ack;
    ack().set_error_code( sword::TacticalLineAck::no_error );
    std::unique_ptr< Limit > limit( new Limit( idManager_.GetNextId(), asn ) );
    const auto id = limit->GetID();
    ack().set_id( id );
    limits_.Register( id, *limit );
    limit->SendCreation( clients_ );
    limit.release();
    ack.Send( publisher, context );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesModel::HandleLimitRequest
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void TacticalLinesModel::HandleLimitRequest( dispatcher::ClientPublisher_ABC& publisher, const sword::LimitUpdateRequest& asn )
{
    plugins::messenger::LimitUpdateRequestAck ack;
    ack().set_error_code( sword::TacticalLineAck::no_error );
    Limit* limit = limits_.Find( asn.id().id() );
    if( limit )
    {
        limit->Update( asn );
        limit->SendUpdate( clients_ );
        ack().set_id( limit->GetID() );
    }
    else
        ack().set_error_code( sword::TacticalLineAck::error_invalid_id );
    ack.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesModel::HandleLimitRequest
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void TacticalLinesModel::HandleLimitRequest( dispatcher::ClientPublisher_ABC& publisher, const sword::LimitDestructionRequest& asn )
{
    plugins::messenger::LimitDestructionRequestAck ack;
    ack().set_error_code( sword::TacticalLineAck::no_error );
    Limit* limit = limits_.Find( asn.id().id() );
    if( limit )
    {
        ack().set_id( limit->GetID() );
        limit->SendDestruction( clients_ );
        delete limit;
        limits_.Remove( asn.id().id() );
    }
    else
        ack().set_error_code( sword::TacticalLineAck::error_invalid_id );
    ack.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesModel::HandleLimaRequest
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void TacticalLinesModel::HandleLimaRequest( dispatcher::ClientPublisher_ABC& publisher, const sword::PhaseLineCreationRequest& asn, unsigned int context )
{
    plugins::messenger::PhaseLineCreationAck ack;
    ack().set_error_code( sword::TacticalLineAck::no_error );
    std::unique_ptr< Lima > lima( new Lima( idManager_.GetNextId(), asn ) );
    const auto id = lima->GetID();
    ack().set_id( id );
    limas_.Register( id, *lima );
    lima->SendCreation( clients_ );
    lima.release();
    ack.Send( publisher, context );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesModel::HandleLimaRequest
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void TacticalLinesModel::HandleLimaRequest( dispatcher::ClientPublisher_ABC& publisher, const sword::PhaseLineUpdateRequest& asn )
{
    plugins::messenger::PhaseLineUpdateRequestAck ack ;
    ack().set_error_code( sword::TacticalLineAck::no_error );
    Lima* lima = limas_.Find( asn.id().id() );
    if( lima )
    {
        lima->Update( asn );
        lima->SendUpdate( clients_ );
        ack().set_id( lima->GetID() );
    }
    else
        ack().set_error_code( sword::TacticalLineAck::error_invalid_id );
    ack.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesModel::HandleLimaRequest
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void TacticalLinesModel::HandleLimaRequest( dispatcher::ClientPublisher_ABC& publisher, const sword::PhaseLineDestructionRequest& asn)
{
    plugins::messenger::PhaseLineDestructionRequestAck ack ;
    ack().set_error_code( sword::TacticalLineAck::no_error );
    Lima* lima = limas_.Find( asn.id().id() );
    if( lima )
    {
        ack().set_id( lima->GetID() );
        lima->SendDestruction( clients_ );
        delete lima;
        limas_.Remove( asn.id().id() );
    }
    else
        ack().set_error_code( sword::TacticalLineAck::error_invalid_id );
    ack.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesModel::SendStateToNewClient
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void TacticalLinesModel::SendStateToNewClient( dispatcher::ClientPublisher_ABC& client ) const
{
    {
        tools::Iterator< const Limit& > it( limits_.CreateIterator() );
        while( it.HasMoreElements() )
            it.NextElement().SendFullState( client );
    }
    {
        tools::Iterator< const Lima& > it( limas_.CreateIterator() );
        while( it.HasMoreElements() )
            it.NextElement().SendFullState( client );
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesModel::CollectFormations
// Created: RDS 2008-04-09
// -----------------------------------------------------------------------------
void TacticalLinesModel::CollectFormations( T_FormationMap& formations )
{
    {
        tools::Iterator< const Limit& > it( limits_.CreateIterator() );
        while( it.HasMoreElements() )
        {
            const Limit& limit = it.NextElement();
            if( limit.GetDiffusion().has_formation() )
                formations[ limit.GetDiffusion().formation().id() ].insert( &limit );
        }
    }
    {
        tools::Iterator< const Lima& > it( limas_.CreateIterator() );
        while( it.HasMoreElements() )
        {
            const Lima& lima = it.NextElement();
            if( lima.GetDiffusion().has_formation() )
                formations[ lima.GetDiffusion().formation().id() ].insert( &lima );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesModel::CollectAutomats
// Created: RDS 2008-04-09
// -----------------------------------------------------------------------------
void TacticalLinesModel::CollectAutomats( T_AutomatMap& automats )
{
    {
        tools::Iterator< const Limit& > it( limits_.CreateIterator() );
        while( it.HasMoreElements() )
        {
            const Limit& limit = it.NextElement();
            if( limit.GetDiffusion().has_automat() )
                automats[ limit.GetDiffusion().automat().id() ].insert( &limit );
        }
    }
    {
        tools::Iterator< const Lima& > it( limas_.CreateIterator() );
        while( it.HasMoreElements() )
        {
            const Lima& lima = it.NextElement();
            if( lima.GetDiffusion().has_automat() )
                automats[ lima.GetDiffusion().automat().id() ].insert( &lima );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesModel::CollectUnits
// Created: JSR 2013-05-30
// -----------------------------------------------------------------------------
void TacticalLinesModel::CollectUnits( T_UnitMap& units )
{
    {
        tools::Iterator< const Limit& > it( limits_.CreateIterator() );
        while( it.HasMoreElements() )
        {
            const Limit& limit = it.NextElement();
            if( limit.GetDiffusion().has_unit() )
                units[ limit.GetDiffusion().unit().id() ].insert( &limit );
        }
    }
    {
        tools::Iterator< const Lima& > it( limas_.CreateIterator() );
        while( it.HasMoreElements() )
        {
            const Lima& lima = it.NextElement();
            if( lima.GetDiffusion().has_unit() )
                units[ lima.GetDiffusion().unit().id() ].insert( &lima );
        }
    }
}

namespace
{
    template< typename T >
    void DeleteFromCollection( unsigned int id, tools::Resolver< T >& collection, dispatcher::ClientPublisher_ABC& clients )
    {
        std::vector< unsigned int > idToDelete;
        auto functor = [&] ( TacticalLine_ABC& line )
        {
            if( line.GetDiffusion().unit().id() == id )
                idToDelete.push_back( line.GetID() );
        };    
        collection.Apply( functor );
        for( auto it = idToDelete.begin(); it != idToDelete.end(); ++it )
        {
            collection.Get( *it ).SendDestruction( clients );
            collection.Delete( *it );
        }
        collection.Apply( functor );
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesModel::DeleteUnit
// Created: LDC 2013-11-13
// -----------------------------------------------------------------------------
void TacticalLinesModel::DeleteUnit( int32_t id )
{
    DeleteFromCollection( unsigned int( id ), limits_, clients_ );
    DeleteFromCollection( unsigned int( id ), limas_, clients_ );
}
