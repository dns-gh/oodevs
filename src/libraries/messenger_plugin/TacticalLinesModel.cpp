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
#include "protocol/protocol.h"
#include "protocol/messengersenders.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::messenger;
using namespace Common;
using namespace MsgsClientToMessenger;

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
void TacticalLinesModel::ReadLimit( xml::xistream& xis, const MsgTacticalLine_Diffusion& diffusion)
{
    std::auto_ptr< Limit > limit( new Limit( idManager_.NextId(), xis, diffusion, converter_ ) );
    limits_.Register( limit->GetID(), *limit );
    limit.release();
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesModel::CreateLima
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLinesModel::ReadLima( xml::xistream& xis, const MsgTacticalLine_Diffusion& diffusion)
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
void TacticalLinesModel::HandleLimitRequest( dispatcher::ClientPublisher_ABC& publisher, const MsgsClientToMessenger::MsgLimitCreationRequest& asn )
{
    plugins::messenger::LimitCreationRequestAck ack ;
    ack().set_error_code( MsgsMessengerToClient::TacticalLineAck_ErrorCode_no_error );
    std::auto_ptr< Limit > limit( new Limit( idManager_.NextId(), asn ) );
    limits_.Register( limit->GetID(), *limit );
    limit->SendCreation( clients_ );
    limit.release();
    ack.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesModel::HandleLimitRequest
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void TacticalLinesModel::HandleLimitRequest( dispatcher::ClientPublisher_ABC& publisher, const MsgLimitUpdateRequest& asn )
{
    plugins::messenger::LimitUpdateRequestAck ack;
    ack().set_error_code( MsgsMessengerToClient::TacticalLineAck_ErrorCode_no_error );
    Limit* limit = limits_.Find( asn.oid() );
    if( limit )
    {
        limit->Update( asn );
        limit->SendUpdate( clients_ );
    }
    else
        ack().set_error_code( MsgsMessengerToClient::TacticalLineAck_ErrorCode_error_invalid_id );
    ack.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesModel::HandleLimitRequest
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void TacticalLinesModel::HandleLimitRequest( dispatcher::ClientPublisher_ABC& publisher, const MsgLimitDestructionRequest& asn )
{
    plugins::messenger::LimitDestructionRequestAck ack;
    ack().set_error_code( MsgsMessengerToClient::TacticalLineAck_ErrorCode_no_error );
    Limit* limit = limits_.Find( asn.oid() );
    if( limit )
    {
        limit->SendDestruction( clients_ );
        delete limit;
        limits_.Remove( asn.oid() );
    }
    else
        ack().set_error_code( MsgsMessengerToClient::TacticalLineAck_ErrorCode_error_invalid_id );
    ack.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesModel::HandleLimaRequest
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void TacticalLinesModel::HandleLimaRequest( dispatcher::ClientPublisher_ABC& publisher, const MsgsClientToMessenger::MsgLimaCreationRequest& asn )
{
    plugins::messenger::LimaCreationRequestAck ack ;
    ack().set_error_code( MsgsMessengerToClient::TacticalLineAck_ErrorCode_no_error );
    std::auto_ptr< Lima > lima( new Lima( idManager_.NextId(), asn ) );
    limas_.Register( lima->GetID(), *lima );
    lima->SendCreation( clients_ );
    lima.release();
    ack.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesModel::HandleLimaRequest
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void TacticalLinesModel::HandleLimaRequest( dispatcher::ClientPublisher_ABC& publisher, const MsgLimaUpdateRequest& asn )
{
    plugins::messenger::LimaUpdateRequestAck ack ;
    ack().set_error_code( MsgsMessengerToClient::TacticalLineAck_ErrorCode_no_error );
    Lima* lima = limas_.Find( asn.oid() );
    if( lima )
    {
        lima->Update( asn );
        lima->SendUpdate( clients_ );
    }
    else
        ack().set_error_code( MsgsMessengerToClient::TacticalLineAck_ErrorCode_error_invalid_id );
    ack.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinesModel::HandleLimaRequest
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void TacticalLinesModel::HandleLimaRequest( dispatcher::ClientPublisher_ABC& publisher, const MsgLimaDestructionRequest& asn)
{
    plugins::messenger::LimaDestructionRequestAck ack ;
    ack().set_error_code( MsgsMessengerToClient::TacticalLineAck_ErrorCode_no_error );
    Lima* lima = limas_.Find( asn.oid() );
    if( lima )
    {
        lima->SendDestruction( clients_ );
        delete lima;
        limas_.Remove( asn.oid() );
    }
    else
        ack().set_error_code( MsgsMessengerToClient::TacticalLineAck_ErrorCode_error_invalid_id );
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
                formations[ limit.GetDiffusion().formation() ].insert( &limit );
        }
    }
    {
        tools::Iterator< const Lima& > it( limas_.CreateIterator() );
        while( it.HasMoreElements() )
        {
            const Lima& lima = it.NextElement();
            if( lima.GetDiffusion().has_formation() )
                formations[ lima.GetDiffusion().formation() ].insert( &lima );
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
                automats[ limit.GetDiffusion().automat() ].insert( &limit );
        }
    }
    {
        tools::Iterator< const Lima& > it( limas_.CreateIterator() );
        while( it.HasMoreElements() )
        {
            const Lima& lima = it.NextElement();
            if( lima.GetDiffusion().has_automat() )
                automats[ lima.GetDiffusion().automat() ].insert( &lima );
        }
    }
}
