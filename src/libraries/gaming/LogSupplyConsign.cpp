// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"

#include "LogSupplyConsign.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Controller.h"
#include "LogisticConsigns.h"
#include "clients_kernel/Displayer_ABC.h"
#include "DotationRequest.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Viewport_ABC.h"
#include "Tools.h"

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogSupplyConsign::LogSupplyConsign( Controller& controller, const Resolver_ABC< Automat_ABC >& resolver, const Resolver_ABC< Agent_ABC >& agentResolver, const Resolver_ABC< DotationType >& dotationResolver, const ASN1T_MsgLogRavitaillementTraitementCreation& message )
    : controller_           ( controller )
    , resolver_             ( resolver )
    , agentResolver_        ( agentResolver )
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
    pion_.Get< LogSupplyConsigns >().AddConsign( *this );
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogSupplyConsign::~LogSupplyConsign()
{
    pion_.Get< LogSupplyConsigns >().RemoveConsign( *this );
    if( pAutomateLogHandling_ )
        pAutomateLogHandling_->Get< LogSupplyConsigns >().TerminateConsign( *this );
    if( pPionLogConvoying_ )
        pPionLogConvoying_->Get< LogSupplyConsigns >().TerminateConsign( *this );
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
            pAutomateLogHandling_->Get< LogSupplyConsigns >().TerminateConsign( *this );
        pAutomateLogHandling_ = resolver_.Find( message.oid_automate_log_traitant );
        if( pAutomateLogHandling_ )
            pAutomateLogHandling_->Get< LogSupplyConsigns >().HandleConsign( *this );
    }

    if( message.m.oid_pion_convoyantPresent )
    {
        if( pPionLogConvoying_ )
            pPionLogConvoying_->Get< LogSupplyConsigns >().TerminateConsign( *this );
        pPionLogConvoying_ = agentResolver_.Find( message.oid_pion_convoyant );
        if( message.oid_pion_convoyant )
            pPionLogConvoying_->Get< LogSupplyConsigns >().HandleConsign( *this );
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
void LogSupplyConsign::Display( kernel::Displayer_ABC& displayer, kernel::Displayer_ABC& itemDisplayer ) const
{
    displayer.Display( pion_ ).Display( nState_ );

    itemDisplayer.Display( tools::translate( "Logistic", "Instruction:" ), nID_ )
                 .Display( tools::translate( "Logistic", "Consumer:" ), pion_ )
                 .Display( tools::translate( "Logistic", "Handler:" ), pAutomateLogHandling_ )
                 .Display( tools::translate( "Logistic", "Supplier:" ), pAutomateLogProvidingConvoyResources_ )
                 .Display( tools::translate( "Logistic", "Convoyer:" ), pPionLogConvoying_ )
                 .Display( tools::translate( "Logistic", "State:" ), nState_ );
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::Draw
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void LogSupplyConsign::Draw( const Point2f& , const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( ! pAutomateLogHandling_ || ! tools.ShouldDisplay( "RealTimeLogistic" ) )
        return;

    const Point2f from = pAutomateLogHandling_->Get< Positions >().GetPosition();
    const Point2f to   = pion_.Get< Positions >().GetPosition();
    if( ! viewport.IsVisible( Rectangle2f( from, to ) ) )
        return;

    glColor4f( COLOR_ORANGE );
    switch( nState_ )
    {
    case eLogRavitaillementTraitementEtat_ConvoiDeplacementVersPointChargement:
    case eLogRavitaillementTraitementEtat_ConvoiDeplacementVersPointDechargement:
        glLineStipple( 1, tools.StipplePattern() );
        break;
    case eLogRavitaillementTraitementEtat_ConvoiDeplacementRetour:
        glLineStipple( 1, tools.StipplePattern(-1) );
        break;
    default:
        glLineStipple( 1, tools.StipplePattern(0) );
    }
    tools.DrawCurvedArrow( from, to, 0.6f );
}
