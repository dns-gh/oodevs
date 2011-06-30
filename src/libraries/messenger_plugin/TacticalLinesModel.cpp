//*****************************************************************************
//
// $Created: NLD 2003-01-14 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/Fuseau.cpp $
// $Author: Jvt $
// $Modtime: 16/05/05 14:55 $
// $Revision: 17 $
// $Workfile: Fuseau.cpp $
//
//*****************************************************************************

#include "messenger_plugin_pch.h"
#include "TacticalLinesModel.h"
#include "TacticalLine_ABC.h"
#include "Limit.h"
#include "Lima.h"
#include "IdManager.h"
#include "protocol/ClientPublisher_ABC.h"
#include "tools/Iterator.h"
#include "protocol/Protocol.h"
#include "protocol/MessengerSenders.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::messenger;
using namespace sword;

// -----------------------------------------------------------------------------
// Name: TacticalLinesModel constructor
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
TacticalLinesModel::TacticalLinesModel( dispatcher::ClientPublisher_ABC& clients, IdManager& idManager, const kernel::CoordinateConverter_ABC& converter )
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
void TacticalLinesModel::ReadLimit( xml::xistream& xis, const TacticalLine_Diffusion& diffusion)
{
    std::auto_ptr< Limit > limit( new Limit( idManager_.NextId(), xis, diffusion, converter_ ) );
    limits_.Register( limit->GetID(), *limit );
    limit.release();
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesModel::CreateLima
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLinesModel::ReadLima( xml::xistream& xis, const TacticalLine_Diffusion& diffusion)
{
    std::auto_ptr< Lima > lima( new Lima( idManager_.NextId(), xis, diffusion, converter_ ) );
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
void TacticalLinesModel::HandleLimitRequest( dispatcher::ClientPublisher_ABC& publisher, const sword::LimitCreationRequest& asn )
{
    unsigned int nCtx = asn.has_context()? asn.context() : 0;
    plugins::messenger::LimitCreationRequestAck ack ;
    ack().set_error_code( sword::TacticalLineAck::no_error );
    std::auto_ptr< Limit > limit( new Limit( idManager_.NextId(), asn ) );
    limits_.Register( limit->GetID(), *limit );
    limit->SendCreation( clients_ );
    limit.release();
    ack().set_context( nCtx );
    ack.Send( publisher );
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
void TacticalLinesModel::HandleLimaRequest( dispatcher::ClientPublisher_ABC& publisher, const sword::PhaseLineCreationRequest& asn )
{
    unsigned int nCtx = asn.has_context()? asn.context() : 0;
    plugins::messenger::PhaseLineCreationAck ack;
    ack().set_error_code( sword::TacticalLineAck::no_error );
    std::auto_ptr< Lima > lima( new Lima( idManager_.NextId(), asn ) );
    limas_.Register( lima->GetID(), *lima );
    lima->SendCreation( clients_ );
    lima.release();
    ack().set_context( nCtx );
    ack.Send( publisher );
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
