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
#include "protocol/ClientPublisher_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "LogSupplyDotation.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogConsignSupply constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
LogConsignSupply::LogConsignSupply( const Model& model, const sword::LogSupplyHandlingCreation& msg )
    : SimpleEntity< >   ( msg.request().id() )
    , model_            ( model )
    , automat_          ( model.Automats().Get( msg.consumer().id() ) )
    , nTickCreation_    ( msg.tick_creation() )
    , pTreatingEntity_  ( 0 )
    , pConvoyingEntity_ ( 0 )
    , pConvoy_          ( 0 )
    , nState_           ( sword::LogSupplyHandlingUpdate::convoy_moving_to_loading_point )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogConsignSupply destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
LogConsignSupply::~LogConsignSupply()
{
    dotations_.DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: LogConsignSupply::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void LogConsignSupply::Update( const sword::LogSupplyHandlingUpdate& msg )
{
    if( msg.has_supplier() )
        pTreatingEntity_ = FindLogEntity( msg.supplier() );
    if( msg.has_convoy_provider() )
        pConvoyingEntity_ = FindLogEntity( msg.convoy_provider() );
    if( msg.has_convoying_unit() )
        pConvoy_ = ( msg.convoying_unit().id() == 0 ) ? 0 : &model_.Agents().Get( msg.convoying_unit().id() );
    if( msg.has_etat() )
        nState_ = msg.etat();
    if( msg.has_dotations() )
        for( int i = 0; i < msg.dotations().elem_size(); ++i )
        {
            LogSupplyDotation* pDotation = dotations_.Find( msg.dotations().elem( i ).resource().id() );
            if( pDotation )
                pDotation->Update( msg.dotations().elem( i ) );
            else
            {
                pDotation = new LogSupplyDotation( model_, msg.dotations().elem( i ) );
                dotations_.Register( msg.dotations().elem( i ).resource().id(), *pDotation );
            }
        }
}

// -----------------------------------------------------------------------------
// Name: LogConsignSupply::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void LogConsignSupply::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::LogSupplyHandlingCreation asn;

    asn().mutable_request()->set_id( GetId() );
    asn().mutable_consumer()->set_id( automat_.GetId() );
    asn().set_tick_creation( nTickCreation_ );
    {
        for( tools::Iterator< const LogSupplyDotation& > it = dotations_.CreateIterator(); it.HasMoreElements(); )
            it.NextElement().Send( *asn().mutable_dotations()->add_elem() );
    }
    asn.Send( publisher );

    if( asn().dotations().elem_size() > 0 )
        asn().mutable_dotations()->Clear();
}

// -----------------------------------------------------------------------------
// Name: LogConsignSupply::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void LogConsignSupply::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::LogSupplyHandlingUpdate asn;

    asn().mutable_request()->set_id( GetId() );
    asn().mutable_consumer()->set_id( automat_.GetId() );

//    asn().set_oid_automate_log_traitantPresent( 1 );
//    asn().set_oid_automate_log_fournissant_moyens_convoiPresent( 1 );
//    asn().set_oid_pion_convoyantPresent( 1 );
//    asn().set_etatPresent( 1 );
//    asn().set_dotationsPresent( 1 );

    FillLogEntityID( *asn().mutable_supplier(), pTreatingEntity_ );
    FillLogEntityID( *asn().mutable_convoy_provider(), pConvoyingEntity_ );
    asn().mutable_convoying_unit()->set_id( pConvoy_ ? pConvoy_->GetId() : 0 );
    asn().set_etat( nState_ );
    {
        for( tools::Iterator< const LogSupplyDotation& > it = dotations_.CreateIterator(); it.HasMoreElements(); )
            it.NextElement().Send( *asn().mutable_dotations()->add_elem() );
    }
    asn.Send( publisher );
    if( asn().dotations().elem_size() > 0 )
        asn().mutable_dotations()->Clear();
}

// -----------------------------------------------------------------------------
// Name: LogConsignSupply::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void LogConsignSupply::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::LogSupplyHandlingDestruction asn;
    asn().mutable_request()->set_id( GetId() );
    asn().mutable_consumer()->set_id( automat_.GetId() );
    asn.Send( publisher );
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
