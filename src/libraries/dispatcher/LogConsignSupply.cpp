// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "LogConsignSupply.h"
#include "Model.h"
#include "Automat.h"
#include "Formation.h"
#include "Agent.h"
#include "LogSupplyRecipientResourcesRequest.h"
#include "protocol/ClientPublisher_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogConsignSupply constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
LogConsignSupply::LogConsignSupply( const Model& model, const sword::LogSupplyHandlingCreation& msg )
    : SimpleEntity< >     ( msg.request().id() )
    , model_              ( model )
    , nTickCreation_      ( msg.tick() )
    , pTreatingEntity_    ( FindLogEntity( msg.supplier() ) )
    , pConvoyingEntity_   ( FindLogEntity( msg.transporters_provider() ) )
    , pConvoy_            ( 0 )
    , nState_             ( sword::LogSupplyHandlingUpdate::convoy_moving_to_loading_point )
    , currentStateEndTick_( std::numeric_limits< unsigned long >::max() )
    , requests_           ()
{
}

// -----------------------------------------------------------------------------
// Name: LogConsignSupply destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
LogConsignSupply::~LogConsignSupply()
{
    requests_.DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: LogConsignSupply::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void LogConsignSupply::Update( const sword::LogSupplyHandlingUpdate& msg )
{
    if( msg.has_convoyer() )
        pConvoy_ = ( msg.convoyer().id() == 0 ) ? 0 : &model_.Agents().Get( msg.convoyer().id() );
    if( msg.has_state() )
        nState_ = msg.state();
    if( msg.has_current_state_end_tick() )
        currentStateEndTick_ = msg.current_state_end_tick();
    else
        currentStateEndTick_ = std::numeric_limits< unsigned long >::max();
    if( msg.has_requests() )
    {
        requests_.DeleteAll();
        const auto& requests = msg.requests().requests();
        for( auto it = requests.begin(); it != requests.end(); ++it )
        {
            LogSupplyRecipientResourcesRequest* request = requests_.Find( it->recipient().id() );
            if( request )
                request->Update( *it ); //$$ useless
            else
            {
                request = new LogSupplyRecipientResourcesRequest( model_, *it );
                requests_.Register( it->recipient().id(), *request );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogConsignSupply::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void LogConsignSupply::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::LogSupplyHandlingCreation msg;
    msg().mutable_request()->set_id( GetId() );
    msg().set_tick( nTickCreation_ );
    FillLogEntityID( *msg().mutable_supplier(), pTreatingEntity_ );
    FillLogEntityID( *msg().mutable_transporters_provider(), pConvoyingEntity_ );
    msg.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LogConsignSupply::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void LogConsignSupply::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::LogSupplyHandlingUpdate msg;

    msg().mutable_request()->set_id( GetId() );
    msg().mutable_convoyer()->set_id( pConvoy_ ? pConvoy_->GetId() : 0 );
    msg().set_state( nState_ );
    if( currentStateEndTick_ != std::numeric_limits< unsigned long >::max() )
        msg().set_current_state_end_tick( currentStateEndTick_ );
    for( tools::Iterator< const LogSupplyRecipientResourcesRequest& > it = requests_.CreateIterator(); it.HasMoreElements(); )
        it.NextElement().Send( *msg().mutable_requests()->add_requests() );
    msg.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LogConsignSupply::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void LogConsignSupply::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::LogSupplyHandlingDestruction msg;
    msg().mutable_request()->set_id( GetId() );
    msg.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LogConsignSupply::Accept
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void LogConsignSupply::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: LogConsignSupply::FindLogEntity
// Created: AHC 2010-10-13
// -----------------------------------------------------------------------------
kernel::Entity_ABC* LogConsignSupply::FindLogEntity(const sword::ParentEntity& msg) const
{
    kernel::Entity_ABC* retval = 0;
    if( msg.has_automat() )
        retval = model_.Automats().Find( msg.automat().id() );
    else if( msg.has_formation() )
        retval = model_.Formations().Find( msg.formation().id() );
    return retval;
}

// -----------------------------------------------------------------------------
// Name: LogConsignSupply::FillLogEntityID
// Created: AHC 2010-10-13
// -----------------------------------------------------------------------------
void LogConsignSupply::FillLogEntityID(sword::ParentEntity& msg, const kernel::Entity_ABC* entity) const
{
    if( entity == 0 )
        msg.mutable_automat()->set_id( 0 );
    else if( dynamic_cast<const kernel::Automat_ABC*>( entity) )
        msg.mutable_automat()->set_id( entity->GetId() );
    else if( dynamic_cast<const kernel::Formation_ABC*>( entity) )
        msg.mutable_formation()->set_id( entity->GetId() );
}

// -----------------------------------------------------------------------------
// Name: LogConsignSupply::IsObsoleteForUnit
// Created: JSR 2013-03-14
// -----------------------------------------------------------------------------
bool LogConsignSupply::IsObsoleteForUnit( unsigned int id ) const
{
    return ( pTreatingEntity_ && pTreatingEntity_->GetId() == id ) ||
           ( pConvoyingEntity_ && pConvoyingEntity_->GetId() == id ) ||
           ( pConvoy_ && pConvoy_->GetId() == id );
}
