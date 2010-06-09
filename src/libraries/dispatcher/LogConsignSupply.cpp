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
#include "Agent.h"
#include "protocol/ClientPublisher_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "LogSupplyDotation.h"
#include "protocol/clientsenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogConsignSupply constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
LogConsignSupply::LogConsignSupply( const Model& model, const MsgsSimToClient::MsgLogSupplyHandlingCreation& msg )
    : SimpleEntity< >   ( msg.oid_consigne() )
    , model_            ( model )
    , automat_          ( model.Automats().Get( msg.oid_automate() ) )
    , nTickCreation_    ( msg.tick_creation() )
    , pTreatingAutomat_ ( 0 )
    , pConvoyingAutomat_( 0 )
    , pConvoy_          ( 0 )
    , nState_           ( MsgsSimToClient::convoi_deplacement_vers_point_chargement )
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
void LogConsignSupply::Update( const MsgsSimToClient::MsgLogSupplyHandlingUpdate& msg )
{
    if( msg.has_oid_automate_log_traitant() )
        pTreatingAutomat_ = ( msg.oid_automate_log_traitant() == 0 ) ? 0 : &model_.Automats().Get( msg.oid_automate_log_traitant() );

    if( msg.has_oid_automate_log_fournissant_moyens_convoi() )
        pConvoyingAutomat_ = ( msg.oid_automate_log_fournissant_moyens_convoi() == 0 ) ? 0 : &model_.Automats().Get( msg.oid_automate_log_fournissant_moyens_convoi() );

    if( msg.has_oid_pion_convoyant() )
        pConvoy_ = ( msg.oid_pion_convoyant() == 0 ) ? 0 : &model_.Agents().Get( msg.oid_pion_convoyant() );

    if( msg.has_etat() )
        nState_ = msg.etat();

    if( msg.has_dotations() )
        for( int i = 0; i < msg.dotations().elem_size(); ++i )
        {
            LogSupplyDotation* pDotation = dotations_.Find( msg.dotations().elem( i ).ressource_id() );
            if( pDotation )
                pDotation->Update( msg.dotations().elem( i ) );
            else
            {
                pDotation = new LogSupplyDotation( model_, msg.dotations().elem( i ) );
                dotations_.Register( msg.dotations().elem( i ).ressource_id(), *pDotation );
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

    asn().set_oid_consigne  ( GetId() );
    asn().set_oid_automate  ( automat_.GetId() );
    asn().set_tick_creation ( nTickCreation_ );
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

    asn().set_oid_consigne ( GetId() );
    asn().set_oid_automate ( automat_.GetId() );

//    asn().set_oid_automate_log_traitantPresent( 1 );
//    asn().set_oid_automate_log_fournissant_moyens_convoiPresent( 1 );
//    asn().set_oid_pion_convoyantPresent( 1 );
//    asn().set_etatPresent( 1 );
//    asn().set_dotationsPresent( 1 );

    asn().set_oid_automate_log_traitant                  ( pTreatingAutomat_ ? pTreatingAutomat_->GetId() : 0 );
    asn().set_oid_automate_log_fournissant_moyens_convoi ( pConvoyingAutomat_ ? pConvoyingAutomat_->GetId() : 0 );
    asn().set_oid_pion_convoyant                         ( pConvoy_ ? pConvoy_->GetId() : 0 );
    asn().set_etat                                       ( nState_ );
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
    asn().set_oid_consigne ( GetId() );
    asn().set_oid_automate ( automat_.GetId() );
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
