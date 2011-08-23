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
#include "LogisticConsigns.h"
#include "Tools.h"
#include "Simulation.h"
#include "SupplyRecipientResourcesRequest.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Viewport_ABC.h"
#include "protocol/Protocol.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "PcAttributes.h"
#include <boost/foreach.hpp>

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
                                  , const Simulation& simulation
                                  , const sword::LogSupplyHandlingCreation& message )
    : controller_                        ( controller )
    , resolver_                          ( resolver )
    , agentResolver_                     ( agentResolver )
    , formationResolver_                 ( formationResolver )
    , dotationResolver_                  ( dotationResolver )
    , nID_                               ( message.request().id() )
    , pLogHandlingEntity_                ( FindLogEntity( message.supplier() ) )
    , pPionLogConvoying_                 ( 0 )
    , pLogProvidingConvoyResourcesEntity_( FindLogEntity( message.transporters_provider() ) )
    , nState_                            ( eLogSupplyHandlingStatus_Termine )
    , currentStateEndTick_               ( std::numeric_limits< unsigned int >::max() )
    , simulation_                        ( simulation )
{
    if( pLogHandlingEntity_ )
        pLogHandlingEntity_->Get< LogSupplyConsigns >().HandleConsign( *this );
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogSupplyConsign::~LogSupplyConsign()
{
    for( tools::Iterator< const SupplyRecipientResourcesRequest& > it = CreateIterator(); it.HasMoreElements(); )
        it.NextElement().recipient_.Get< LogSupplyConsigns >().RemoveConsign( *this );
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
    if( message.has_convoyer() && ( !pPionLogConvoying_ || message.convoyer().id() != int( pPionLogConvoying_->GetId() ) ) )
    {
        if( pPionLogConvoying_ )
            pPionLogConvoying_->Get< LogSupplyConsigns >().TerminateConsign( *this );
        pPionLogConvoying_ = agentResolver_.Find( message.convoyer().id() );
        if( pPionLogConvoying_ )
            pPionLogConvoying_->Get< LogSupplyConsigns >().HandleConsign( *this );
    }
    if( message.has_state()  )
        nState_ = E_LogSupplyHandlingStatus( message.state() );
    if( message.has_current_state_end_tick() )
        currentStateEndTick_ = message.current_state_end_tick();
    if( message.has_requests() )
    {
        for( tools::Iterator< const SupplyRecipientResourcesRequest& > it = CreateIterator(); it.HasMoreElements(); )
            it.NextElement().recipient_.Get< LogSupplyConsigns >().RemoveConsign( *this );
        DeleteAll();
        BOOST_FOREACH( const sword::SupplyRecipientResourcesRequest& data, message.requests().requests() )
        {
            SupplyRecipientResourcesRequest* tmp = new SupplyRecipientResourcesRequest( dotationResolver_, resolver_, data );
            Register( data.recipient().id(), *tmp );
            tmp->recipient_.Get< LogSupplyConsigns >().AddConsign( *this );
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
    unsigned nbRequests = Count();
    if( nbRequests == 0 )
        displayer.Display( tools::translate( "Logistic", "No recipients" ) ).Display( nState_ );
    else if( nbRequests == 1 )
        displayer.Display( CreateIterator().NextElement().recipient_ ).Display( nState_ );
    else
        displayer.Display( tools::translate( "Logistic", "Multiple recipients" ) ).Display( nState_ );

    itemDisplayer.Display( tools::translate( "Logistic", "Instruction:" ), nID_ );
    itemDisplayer.Display( tools::translate( "Logistic", "Transporters provider:" ), pLogProvidingConvoyResourcesEntity_ );
    itemDisplayer.Display( tools::translate( "Logistic", "Supplier:" ), pLogHandlingEntity_ );
    itemDisplayer.Display( tools::translate( "Logistic", "Convoyer:" ), pPionLogConvoying_ );
    itemDisplayer.Display( tools::translate( "Logistic", "State:" ), nState_ );

    if( currentStateEndTick_ == std::numeric_limits< unsigned int >::max() )
        itemDisplayer.Display( tools::translate( "Logistic", "Current state end in:" ), tools::translate( "Logistic", "Unknown" ) );
    else
    {
//        QTime time;
//        time.addSecs( simulation_.GetTickDuration() * ( currentStateEndTick_ - simulation_.GetCurrentTick() ) );
        itemDisplayer.Display( tools::translate( "Logistic", "Current state end in:" ), currentStateEndTick_ );
    }
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::Draw
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void LogSupplyConsign::Draw( const Point2f& , const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( !pLogHandlingEntity_ || !tools.ShouldDisplay( "RealTimeLogistic" ) )
        return;

    Point2f from = pLogHandlingEntity_->Get< Positions >().GetPosition();
    for( tools::Iterator< const SupplyRecipientResourcesRequest& > it = CreateIterator(); it.HasMoreElements(); )
    {
        const Point2f to   = it.NextElement().recipient_.Get< Positions >().GetPosition();
        if( viewport.IsVisible( Rectangle2f( from, to ) ) )
        {
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
        from = to;
    }
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
