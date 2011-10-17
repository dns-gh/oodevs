// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "LogConsignFuneral.h"
#include "Model.h"
#include "Automat.h"
#include "Formation.h"
#include "Agent.h"
#include "protocol/ClientPublisher_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include <boost/foreach.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogConsignFuneral constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
LogConsignFuneral::LogConsignFuneral( const Model& model, const sword::LogFuneralHandlingCreation& msg )
    : SimpleEntity< >     ( msg.request().id() )
    , model_              ( model )
    , nTickCreation_      ( msg.tick() )
    , pRequestingUnit_    ( &model_.Agents().Get( msg.unit().id() ) )
    , pHandlingEntity_    ( 0 )
    , pConvoy_            ( 0 )
    , nState_             ( sword::LogFuneralHandlingUpdate::waiting_for_handling )
    , currentStateEndTick_( std::numeric_limits< unsigned long >::max() )
    , rank_               ( msg.rank() )
{
}

// -----------------------------------------------------------------------------
// Name: LogConsignFuneral destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
LogConsignFuneral::~LogConsignFuneral()
{
}

// -----------------------------------------------------------------------------
// Name: LogConsignFuneral::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void LogConsignFuneral::Update( const sword::LogFuneralHandlingUpdate& msg )
{
    if( msg.has_handling_unit() )
        pHandlingEntity_ = FindLogEntity( msg.handling_unit() );
    if( msg.has_convoying_unit() )
        pConvoy_ = &model_.Agents().Get( msg.convoying_unit().id() );
    if( msg.has_state() )
        nState_ = msg.state();
    if( msg.has_current_state_end_tick() )
        currentStateEndTick_ = msg.current_state_end_tick();
    if( msg.has_packaging_resource() )
        packaging_ = msg.packaging_resource().id();
}

// -----------------------------------------------------------------------------
// Name: LogConsignFuneral::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void LogConsignFuneral::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::LogFuneralHandlingCreation msg;
    msg().mutable_request()->set_id( GetId() );
    msg().mutable_unit()->set_id( pRequestingUnit_->GetId() );
    msg().set_tick( nTickCreation_ );
    msg().set_rank( rank_ );
    msg.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LogConsignFuneral::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void LogConsignFuneral::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::LogFuneralHandlingUpdate msg;
    msg().mutable_request()->set_id( GetId() );
    msg().set_state( nState_ );
    msg().set_current_state_end_tick( currentStateEndTick_ );
    if( pHandlingEntity_ )
        FillLogEntityID( *msg().mutable_handling_unit(), pHandlingEntity_ );
    if( pConvoy_ )
        msg().mutable_convoying_unit()->set_id( pConvoy_->GetId() );
    if( packaging_ )
        msg().mutable_packaging_resource()->set_id( *packaging_ );
    msg.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LogConsignFuneral::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void LogConsignFuneral::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::LogFuneralHandlingDestruction msg;
    msg().mutable_request()->set_id( GetId() );
    msg.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LogConsignFuneral::Accept
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void LogConsignFuneral::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: LogConsignFuneral::FindLogEntity
// Created: AHC 2010-10-13
// -----------------------------------------------------------------------------
kernel::Entity_ABC* LogConsignFuneral::FindLogEntity(const sword::ParentEntity& msg) const
{
    kernel::Entity_ABC* retval = 0;
    if( msg.has_automat() )
        retval = model_.Automats().Find( msg.automat().id() );
    else if( msg.has_formation() )
        retval = model_.Formations().Find( msg.formation().id() );
    return retval;
}

// -----------------------------------------------------------------------------
// Name: LogConsignFuneral::FillLogEntityID
// Created: AHC 2010-10-13
// -----------------------------------------------------------------------------
void LogConsignFuneral::FillLogEntityID(sword::ParentEntity& msg, const kernel::Entity_ABC* entity) const
{
    if( entity == 0 )
        msg.mutable_automat()->set_id( 0 );
    else if( dynamic_cast<const kernel::Automat_ABC*>( entity) )
        msg.mutable_automat()->set_id( entity->GetId() );
    else if( dynamic_cast<const kernel::Formation_ABC*>( entity) )
        msg.mutable_formation()->set_id( entity->GetId() );
}
