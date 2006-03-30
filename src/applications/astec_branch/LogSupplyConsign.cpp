// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/LogSupplyConsign.cpp $
// $Author: Age $
// $Modtime: 1/04/05 11:14 $
// $Revision: 1 $
// $Workfile: LogSupplyConsign.cpp $
//
// *****************************************************************************

#include "astec_pch.h"

#include "LogSupplyConsign.h"
#include "Agent.h"
#include "Controller.h"
#include "LogisticConsigns.h"
#include "Displayer_ABC.h"
#include "DotationRequest.h"
#include "GlTools_ABC.h"
#include "Positions.h"

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogSupplyConsign::LogSupplyConsign( Controller& controller, const Resolver_ABC< Agent >& resolver, const Resolver_ABC< DotationType >& dotationResolver, const ASN1T_MsgLogRavitaillementTraitementCreation& message )
    : controller_           ( controller )
    , resolver_             ( resolver )
    , dotationResolver_     ( dotationResolver )
    , nID_                  ( message.oid_consigne )
    , pion_                 ( resolver.Get( message.oid_automate ) )
    , pAutomateLogHandling_ ( 0 )
    , pPionLogConvoying_    ( 0 )
    , pAutomateLogProvidingConvoyResources_( 0 )
    , nState_( eLogRavitaillementTraitementEtat_Termine )
{
    for( uint i = 0; i < message.dotations.n; ++i )
        Register( message.dotations.elem[i].ressource_id,
                  * new DotationRequest( dotationResolver_.Get( message.dotations.elem[i].ressource_id ),
                                         message.dotations.elem[i].quantite_demandee,
                                         message.dotations.elem[i].quantite_accordee,
                                         message.dotations.elem[i].quantite_en_transit ) );
    pion_.Get< LogisticConsigns >().AddConsign( *this );
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogSupplyConsign::~LogSupplyConsign()
{
    pion_.Get< LogisticConsigns >().RemoveConsign( *this );
    if( pAutomateLogHandling_ )
        pAutomateLogHandling_->Get< LogisticConsigns >().TerminateConsign( *this );
    if( pPionLogConvoying_ )
        pPionLogConvoying_->Get< LogisticConsigns >().TerminateConsign( *this );
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::OnReceiveMsgUpdate
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void LogSupplyConsign::Update( const ASN1T_MsgLogRavitaillementTraitementUpdate& message )
{
    if( message.m.oid_automate_log_traitantPresent )
    {
        if( pAutomateLogHandling_ )
            pAutomateLogHandling_->Get< LogisticConsigns >().TerminateConsign( *this );
        pAutomateLogHandling_ = resolver_.Find( message.oid_automate_log_traitant );
        if( pAutomateLogHandling_ )
            pAutomateLogHandling_->Get< LogisticConsigns >().HandleConsign( *this );
    }

    if( message.m.oid_pion_convoyantPresent )
    {
        if( pPionLogConvoying_ )
            pPionLogConvoying_->Get< LogisticConsigns >().TerminateConsign( *this );
        pPionLogConvoying_ = resolver_.Find(message.oid_pion_convoyant );
        if( message.oid_pion_convoyant )
            pPionLogConvoying_->Get< LogisticConsigns >().HandleConsign( *this );
    }

    if( message.m.oid_automate_log_fournissant_moyens_convoiPresent )
        pAutomateLogProvidingConvoyResources_ = resolver_.Find( message.oid_automate_log_fournissant_moyens_convoi );

    if( message.m.etatPresent )
        nState_ = E_LogRavitaillementTraitementEtat( message.etat );

    if( message.m.dotationsPresent )
    {
        for( uint i = 0; i < message.dotations.n; ++i )
        {
            DotationRequest* request = Find( message.dotations.elem[i].ressource_id );
            if( request )
            {
                request->requested_ = message.dotations.elem[i].quantite_demandee;
                request->granted_   = message.dotations.elem[i].quantite_accordee;
                request->convoyed_  = message.dotations.elem[i].quantite_en_transit;
            }
            else
                Register( message.dotations.elem[i].ressource_id,
                  * new DotationRequest( dotationResolver_.Get( message.dotations.elem[i].ressource_id ),
                                         message.dotations.elem[i].quantite_demandee,
                                         message.dotations.elem[i].quantite_accordee,
                                         message.dotations.elem[i].quantite_en_transit ) );
        }
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogSupplyConsign::Display( Displayer_ABC& displayer ) const
{
    displayer.Display( "Consigne :", nID_ )
             .Display( "Pion demandeur :", pion_ )
             .Display( "Pion traitant :", pAutomateLogHandling_ )
             .Display( "Pion fournissant les moyens :", pAutomateLogProvidingConvoyResources_ )
             .Display( "Pion convoyant :", pPionLogConvoying_ )
             .Display( "Etat :", nState_ );
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::Draw
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void LogSupplyConsign::Draw( const geometry::Point2f& where, const GlTools_ABC& tools ) const
{
    if( ! pAutomateLogHandling_ || ! tools.ShouldDisplay( "RealTimeLogistic" ) )
        return;

    glColor4d( COLOR_ORANGE );
    switch( nState_ )
    {
    case eLogRavitaillementTraitementEtat_ConvoiDeplacementVersPointChargement:
    case eLogRavitaillementTraitementEtat_ConvoiDeplacementVersPointDechargement:
        glLineStipple( 1, tools.StipplePattern() );
        break;
    case eLogRavitaillementTraitementEtat_ConvoiDeplacementRetour:
        glLineStipple( 1, ~tools.StipplePattern() );
        break;
    default:
        glLineStipple( 1, 0x00FF );
    }
    tools.DrawCurvedArrow( pAutomateLogHandling_->Get< Positions >().GetPosition(), pion_.Get< Positions >().GetPosition(), 0.6f );
}
