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
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogConsignSupply constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
LogConsignSupply::LogConsignSupply( Model& model, const ASN1T_MsgLogRavitaillementTraitementCreation& msg )
    : model_            ( model )
    , nID_              ( msg.oid_consigne )
    , automat_          ( model.GetAutomats().Get( msg.oid_automate ) )
    , nTickCreation_    ( msg.tick_creation )
    , pTreatingAutomat_ ( 0 )
    , pConvoyingAutomat_( 0 )
    , pConvoy_          ( 0 )
    , nState_           ( EnumLogRavitaillementTraitementEtat::convoi_deplacement_vers_point_chargement )
    , dotations_        ()
{
}

// -----------------------------------------------------------------------------
// Name: LogConsignSupply destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
LogConsignSupply::~LogConsignSupply()
{

}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: LogConsignSupply::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void LogConsignSupply::Update( const ASN1T_MsgLogRavitaillementTraitementUpdate& msg )
{
    if( msg.m.oid_automate_log_traitantPresent )
        pTreatingAutomat_ = ( msg.oid_automate_log_traitant == 0 ) ? 0 : &model_.GetAutomats().Get( msg.oid_automate_log_traitant );

    if( msg.m.oid_automate_log_fournissant_moyens_convoiPresent )
        pConvoyingAutomat_ = ( msg.oid_automate_log_fournissant_moyens_convoi == 0 ) ? 0 : &model_.GetAutomats().Get( msg.oid_automate_log_fournissant_moyens_convoi );

    if( msg.m.oid_pion_convoyantPresent )
        pConvoy_ = ( msg.oid_pion_convoyant == 0 ) ? 0 : &model_.GetAgents().Get( msg.oid_pion_convoyant );

    if( msg.m.etatPresent )
        nState_ = msg.etat;

    if( msg.m.dotationsPresent )
    {
        for( unsigned int i = 0; i < msg.dotations.n; ++i )
        {
            LogSupplyDotation* pDotation = dotations_.Find( msg.dotations.elem[ i ].ressource_id );
            if( pDotation )
                pDotation->Update( msg.dotations.elem[ i ] );
            else
                pDotation = &dotations_.Create( model_, msg.dotations.elem[ i ].ressource_id, msg.dotations.elem[ i ] );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogConsignSupply::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void LogConsignSupply::SendCreation( Publisher_ABC& publisher ) const
{
    AsnMsgInClientLogRavitaillementTraitementCreation asn;

    asn().oid_consigne  = nID_;
    asn().oid_automate  = automat_.GetID();
    asn().tick_creation = nTickCreation_;

    dotations_.Send< ASN1T__SeqOfDemandeDotation, ASN1T_DemandeDotation >( asn().dotations );

    asn.Send( publisher );

    if( asn().dotations.n > 0 )
        delete [] asn().dotations.elem;
}

// -----------------------------------------------------------------------------
// Name: LogConsignSupply::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void LogConsignSupply::SendFullUpdate( Publisher_ABC& publisher ) const
{
    AsnMsgInClientLogRavitaillementTraitementUpdate asn;

    asn().oid_consigne = nID_;
    asn().oid_automate = automat_.GetID();

    asn().m.oid_automate_log_traitantPresent                  = 1;
    asn().m.oid_automate_log_fournissant_moyens_convoiPresent = 1;
    asn().m.oid_pion_convoyantPresent                         = 1;
    asn().m.etatPresent                                       = 1;
    asn().m.dotationsPresent                                  = 1;

    asn().oid_automate_log_traitant                  = pTreatingAutomat_ ? pTreatingAutomat_->GetID() : 0;
    asn().oid_automate_log_fournissant_moyens_convoi = pConvoyingAutomat_ ? pConvoyingAutomat_->GetID() : 0;
    asn().oid_pion_convoyant                         = pConvoy_ ? pConvoy_->GetID() : 0;
    asn().etat                                       = nState_;
    dotations_.Send< ASN1T__SeqOfDemandeDotation, ASN1T_DemandeDotation >( asn().dotations );

    asn.Send( publisher );

    if( asn().dotations.n > 0 )
        delete [] asn().dotations.elem;
}
