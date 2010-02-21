// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
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
LogSupplyConsign::LogSupplyConsign( Controller& controller, const tools::Resolver_ABC< Automat_ABC >& resolver, const tools::Resolver_ABC< Agent_ABC >& agentResolver, const tools::Resolver_ABC< DotationType >& dotationResolver, const MsgsSimToClient::MsgLogSupplyHandlingCreation& message )
    : controller_           ( controller )
    , resolver_             ( resolver )
    , agentResolver_        ( agentResolver )
    , dotationResolver_     ( dotationResolver )
    , nID_                  ( message.oid_consigne() )
    , pion_                 ( resolver.Get( message.oid_automate() ) )
    , pAutomateLogHandling_ ( 0 )
    , pPionLogConvoying_    ( 0 )
    , pAutomateLogProvidingConvoyResources_( 0 )
    , nState_( eLogSupplyHandlingStatus_Termine )
{
    for( int i = 0; i < message.dotations().elem_size(); ++i )
        Register( message.dotations().elem( i ).ressource_id() ,
                  * new DotationRequest( dotationResolver_.Get( message.dotations().elem( i ).ressource_id() ),
                                         message.dotations().elem( i ).quantite_demandee(),
                                         message.dotations().elem( i ).quantite_accordee(),
                                         message.dotations().elem( i ).quantite_en_transit() ) );
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
void LogSupplyConsign::Update( const MsgsSimToClient::MsgLogSupplyHandlingUpdate& message )
{
    if( message.has_oid_automate_log_traitant()  )
    {
        if( pAutomateLogHandling_ )
            pAutomateLogHandling_->Get< LogSupplyConsigns >().TerminateConsign( *this );
        pAutomateLogHandling_ = resolver_.Find( message.oid_automate_log_traitant() );
        if( pAutomateLogHandling_ )
            pAutomateLogHandling_->Get< LogSupplyConsigns >().HandleConsign( *this );
    }

    if( message.has_oid_pion_convoyant()  )
    {
        if( pPionLogConvoying_ )
            pPionLogConvoying_->Get< LogSupplyConsigns >().TerminateConsign( *this );
        pPionLogConvoying_ = agentResolver_.Find( message.oid_pion_convoyant() );
        if( message.oid_pion_convoyant() )
            pPionLogConvoying_->Get< LogSupplyConsigns >().HandleConsign( *this );
    }

    if( message.has_oid_automate_log_fournissant_moyens_convoi()  )
        pAutomateLogProvidingConvoyResources_ = resolver_.Find( message.oid_automate_log_fournissant_moyens_convoi() );

    if( message.has_etat()  )
        nState_ = E_LogSupplyHandlingStatus( message.etat() );

    if( message.has_dotations()  )
    {
        for( int i = 0; i < message.dotations().elem_size(); ++i )
        {
            DotationRequest* request = Find( message.dotations().elem( i ).ressource_id() );
            if( request )
            {
                request->requested_ = message.dotations().elem( i ).quantite_demandee();
                request->granted_   = message.dotations().elem( i ).quantite_accordee();
                request->convoyed_  = message.dotations().elem( i ).quantite_en_transit();
            }
            else
                Register( message.dotations().elem( i ).ressource_id(),
                  * new DotationRequest( dotationResolver_.Get( message.dotations().elem( i ).ressource_id() ),
                                         message.dotations().elem( i ).quantite_demandee(),
                                         message.dotations().elem( i ).quantite_accordee(),
                                         message.dotations().elem( i ).quantite_en_transit() ) );
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
    case eLogSupplyHandlingStatus_ConvoiDeplacementVersPointChargement:
    case eLogSupplyHandlingStatus_ConvoiDeplacementVersPointDechargement:
        glLineStipple( 1, tools.StipplePattern() );
        break;
    case eLogSupplyHandlingStatus_ConvoiDeplacementRetour:
        glLineStipple( 1, tools.StipplePattern(-1) );
        break;
    default:
        glLineStipple( 1, tools.StipplePattern(0) );
    }
    tools.DrawCurvedArrow( from, to, 0.6f );
}
