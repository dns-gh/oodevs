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
#include "ClientPublisher_ABC.h"
#include "ModelVisitor_ABC.h"
#include "LogSupplyDotation.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogConsignSupply constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
LogConsignSupply::LogConsignSupply( const Model& model, const ASN1T_MsgLogSupplyHandlingCreation& msg )
    : SimpleEntity< >   ( msg.oid_consigne )
    , model_            ( model )
    , automat_          ( model.automats_.Get( msg.oid_automate ) )
    , nTickCreation_    ( msg.tick_creation )
    , pTreatingAutomat_ ( 0 )
    , pConvoyingAutomat_( 0 )
    , pConvoy_          ( 0 )
    , nState_           ( EnumLogSupplyHandlingStatus::convoi_deplacement_vers_point_chargement )
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
void LogConsignSupply::Update( const ASN1T_MsgLogSupplyHandlingUpdate& msg )
{
    if( msg.m.oid_automate_log_traitantPresent )
        pTreatingAutomat_ = ( msg.oid_automate_log_traitant == 0 ) ? 0 : &model_.automats_.Get( msg.oid_automate_log_traitant );

    if( msg.m.oid_automate_log_fournissant_moyens_convoiPresent )
        pConvoyingAutomat_ = ( msg.oid_automate_log_fournissant_moyens_convoi == 0 ) ? 0 : &model_.automats_.Get( msg.oid_automate_log_fournissant_moyens_convoi );

    if( msg.m.oid_pion_convoyantPresent )
        pConvoy_ = ( msg.oid_pion_convoyant == 0 ) ? 0 : &model_.agents_.Get( msg.oid_pion_convoyant );

    if( msg.m.etatPresent )
        nState_ = msg.etat;

    if( msg.m.dotationsPresent )
        for( unsigned int i = 0; i < msg.dotations.n; ++i )
        {
            LogSupplyDotation* pDotation = dotations_.Find( msg.dotations.elem[ i ].ressource_id );
            if( pDotation )
                pDotation->Update( msg.dotations.elem[ i ] );
            else
            {
                pDotation = new LogSupplyDotation( model_, msg.dotations.elem[ i ] );
                dotations_.Register( msg.dotations.elem[ i ].ressource_id, *pDotation );
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

    asn().oid_consigne  = GetId();
    asn().oid_automate  = automat_.GetId();
    asn().tick_creation = nTickCreation_;
    {
        asn().dotations.n = dotations_.Count();
        asn().dotations.elem = asn().dotations.n > 0 ? new ASN1T_DotationQuery[ asn().dotations.n ] : 0;
        unsigned int i = 0;
        for( kernel::Iterator< const LogSupplyDotation& > it = dotations_.CreateIterator(); it.HasMoreElements(); )
            it.NextElement().Send( asn().dotations.elem[i++] );
    }
    asn.Send( publisher );

    if( asn().dotations.n > 0 )
        delete [] asn().dotations.elem;
}

// -----------------------------------------------------------------------------
// Name: LogConsignSupply::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void LogConsignSupply::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::LogSupplyHandlingUpdate asn;

    asn().oid_consigne = GetId();
    asn().oid_automate = automat_.GetId();

    asn().m.oid_automate_log_traitantPresent                  = 1;
    asn().m.oid_automate_log_fournissant_moyens_convoiPresent = 1;
    asn().m.oid_pion_convoyantPresent                         = 1;
    asn().m.etatPresent                                       = 1;
    asn().m.dotationsPresent                                  = 1;

    asn().oid_automate_log_traitant                  = pTreatingAutomat_ ? pTreatingAutomat_->GetId() : 0;
    asn().oid_automate_log_fournissant_moyens_convoi = pConvoyingAutomat_ ? pConvoyingAutomat_->GetId() : 0;
    asn().oid_pion_convoyant                         = pConvoy_ ? pConvoy_->GetId() : 0;
    asn().etat                                       = nState_;
    {
        asn().dotations.n = dotations_.Count();
        asn().dotations.elem = asn().dotations.n > 0 ? new ASN1T_DotationQuery[ asn().dotations.n ] : 0;
        unsigned int i = 0;
        for( kernel::Iterator< const LogSupplyDotation& > it = dotations_.CreateIterator(); it.HasMoreElements(); )
            it.NextElement().Send( asn().dotations.elem[i++] );
    }
    asn.Send( publisher );

    if( asn().dotations.n > 0 )
        delete [] asn().dotations.elem;
}

// -----------------------------------------------------------------------------
// Name: LogConsignSupply::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void LogConsignSupply::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::LogSupplyHandlingDestruction asn;
    asn().oid_consigne = GetId();
    asn().oid_automate = automat_.GetId();
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LogConsignSupply::Accept
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void LogConsignSupply::Accept( ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
