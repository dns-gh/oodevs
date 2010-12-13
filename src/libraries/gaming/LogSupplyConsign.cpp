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
#include "DotationRequest.h"
#include "LogisticConsigns.h"
#include "Tools.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Viewport_ABC.h"
#include "protocol/protocol.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "PcAttributes.h"

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogSupplyConsign::LogSupplyConsign( Controller& controller, const tools::Resolver_ABC< Automat_ABC >& resolver
                                  , const tools::Resolver_ABC< Agent_ABC >& agentResolver
                                  , const tools::Resolver_ABC< Formation_ABC >&   formationResolver
                                  , const tools::Resolver_ABC< DotationType >& dotationResolver
                                  , const sword::LogSupplyHandlingCreation& message )
    : controller_           ( controller )
    , resolver_             ( resolver )
    , agentResolver_        ( agentResolver )
	, formationResolver_    ( formationResolver )
    , dotationResolver_     ( dotationResolver )
    , nID_                  ( message.request().id() )
    , consumer_             ( resolver.Get( message.consumer().id() ) )
    , pLogHandlingEntity_ ( 0 )
    , pPionLogConvoying_    ( 0 )
    , pLogProvidingConvoyResourcesEntity_( 0 )
    , nState_( eLogSupplyHandlingStatus_Termine )
{
    for( int i = 0; i < message.dotations().elem_size(); ++i )
        Register( message.dotations().elem( i ).resource().id(),
                  * new DotationRequest( dotationResolver_.Get( message.dotations().elem( i ).resource().id() ),
                                         message.dotations().elem( i ).quantite_demandee(),
                                         message.dotations().elem( i ).quantite_accordee(),
                                         message.dotations().elem( i ).quantite_en_transit() ) );
    consumer_.Get< LogSupplyConsigns >().AddConsign( *this );
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogSupplyConsign::~LogSupplyConsign()
{
    consumer_.Get< LogSupplyConsigns >().RemoveConsign( *this );
    if( pLogHandlingEntity_ )
    	pLogHandlingEntity_->Get< LogSupplyConsigns >().TerminateConsign( *this );
    if( pPionLogConvoying_ )
        pPionLogConvoying_->Get< LogSupplyConsigns >().TerminateConsign( *this );
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::OnReceiveMsgUpdate
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void LogSupplyConsign::Update( const sword::LogSupplyHandlingUpdate& message )
{
    if( message.has_supplier() && ( !pLogHandlingEntity_ || FindLogEntityID( message.supplier() ) != int( pLogHandlingEntity_ ->GetId() ) ) )
    {
        if( pLogHandlingEntity_ )
        	pLogHandlingEntity_->Get< LogSupplyConsigns >().TerminateConsign( *this );
        pLogHandlingEntity_ = FindLogEntity( message.supplier() );
        if( pLogHandlingEntity_ )
        	pLogHandlingEntity_->Get< LogSupplyConsigns >().HandleConsign( *this );
    }
    if( message.has_convoying_unit() && ( !pPionLogConvoying_ || message.convoying_unit().id() != int( pPionLogConvoying_->GetId() ) ) )
    {
        if( pPionLogConvoying_ )
            pPionLogConvoying_->Get< LogSupplyConsigns >().TerminateConsign( *this );
        pPionLogConvoying_ = agentResolver_.Find( message.convoying_unit().id() );
        if( message.convoying_unit().id() )
            pPionLogConvoying_->Get< LogSupplyConsigns >().HandleConsign( *this );
    }
    if( message.has_convoy_provider()  )
    	pLogProvidingConvoyResourcesEntity_ = FindLogEntity( message.convoy_provider() );
    if( message.has_etat()  )
        nState_ = E_LogSupplyHandlingStatus( message.etat() );
    if( message.has_dotations()  )
        for( int i = 0; i < message.dotations().elem_size(); ++i )
        {
            if( DotationRequest* request = Find( message.dotations().elem( i ).resource().id() ) )
            {
                request->requested_ = message.dotations().elem( i ).quantite_demandee();
                request->granted_   = message.dotations().elem( i ).quantite_accordee();
                request->convoyed_  = message.dotations().elem( i ).quantite_en_transit();
            }
            else
                Register( message.dotations().elem( i ).resource().id(),
                  * new DotationRequest( dotationResolver_.Get( message.dotations().elem( i ).resource().id() ),
                                         message.dotations().elem( i ).quantite_demandee(),
                                         message.dotations().elem( i ).quantite_accordee(),
                                         message.dotations().elem( i ).quantite_en_transit() ) );
        }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogSupplyConsign::Display( kernel::Displayer_ABC& displayer, kernel::Displayer_ABC& itemDisplayer ) const
{
    displayer.Display( consumer_ ).Display( nState_ );
    itemDisplayer.Display( tools::translate( "Logistic", "Instruction:" ), nID_ )
                 .Display( tools::translate( "Logistic", "Consumer:" ), consumer_ )
                 .Display( tools::translate( "Logistic", "Handler:" ), pLogHandlingEntity_ )
                 .Display( tools::translate( "Logistic", "Supplier:" ), pLogProvidingConvoyResourcesEntity_ )
                 .Display( tools::translate( "Logistic", "Convoyer:" ), pPionLogConvoying_ )
                 .Display( tools::translate( "Logistic", "State:" ), nState_ );
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::Draw
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void LogSupplyConsign::Draw( const Point2f& , const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( ! pLogHandlingEntity_ || ! tools.ShouldDisplay( "RealTimeLogistic" ) )
        return;
    const Point2f from = pLogHandlingEntity_->Get< Positions >().GetPosition();
    const Point2f to   = consumer_.Get< Positions >().GetPosition();
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

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::FindLogEntity
// Created: AHC 2010-10-13
// -----------------------------------------------------------------------------
kernel::Entity_ABC* LogSupplyConsign::FindLogEntity(const sword::ParentEntity& msg)
{
	kernel::Entity_ABC* retval = 0;
	if( msg.has_automat() )
		retval = resolver_.Find( msg.automat().id() );
	else if( msg.has_formation() )
		retval = formationResolver_.Find( msg.formation().id() );
	return retval;
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::FindLogEntityID
// Created: AHC 2010-10-13
// -----------------------------------------------------------------------------
unsigned int LogSupplyConsign::FindLogEntityID(const sword::ParentEntity& msg)
{
	unsigned int retval = 0;
	if( msg.has_automat() )
		retval = msg.automat().id() ;
	else if( msg.has_formation() )
		retval = msg.formation().id() ;
	return retval;
}
