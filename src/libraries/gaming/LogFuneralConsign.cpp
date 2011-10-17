// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "LogFuneralConsign.h"
#include "LogisticConsigns.h"
#include "Tools.h"
#include "Simulation.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Viewport_ABC.h"
#include "protocol/Protocol.h"

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogFuneralConsign constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogFuneralConsign::LogFuneralConsign( Controller& controller, const sword::LogFuneralHandlingCreation& message, const tools::Resolver_ABC< Automat_ABC >& automatResolver,
                                      const tools::Resolver_ABC< Agent_ABC >& agentResolver, 
                                      const tools::Resolver_ABC< Formation_ABC >&   formationResolver, 
                                      const tools::Resolver_ABC< DotationType >& dotationResolver,
                                      const Simulation& simulation )
    : controller_         ( controller )
    , automatResolver_    ( automatResolver )
    , agentResolver_      ( agentResolver )
    , formationResolver_  ( formationResolver )
    , dotationResolver_   ( dotationResolver )
    , simulation_         ( simulation )
    , nID_                ( message.request().id() )
    , rank_               ( (E_HumanRank)message.rank() )
    , consumer_           ( agentResolver_.Get( message.unit().id() ) )
    , handler_            ( 0 )
    , convoy_             ( 0 )
    , packagingResource_  ( 0 )
    , currentStateEndTick_( std::numeric_limits< unsigned int >::max() )
    , nState_             ( eLogFuneralHandlingStatus_Finished )
{
    consumer_.Get< LogFuneralConsigns >().AddConsign( *this );
}

// -----------------------------------------------------------------------------
// Name: LogFuneralConsign destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogFuneralConsign::~LogFuneralConsign()
{
    consumer_.Get< LogFuneralConsigns >().RemoveConsign( *this );
    if( handler_ )
    {
        if( LogFuneralConsigns* consign = handler_->Retrieve< LogFuneralConsigns >() )
            consign->TerminateConsign( *this );
    }
    if( convoy_ )
        convoy_->Get< LogFuneralConsigns >().TerminateConsign( *this );
}

// -----------------------------------------------------------------------------
// Name: LogFuneralConsign::Update
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void LogFuneralConsign::Update( const sword::LogFuneralHandlingUpdate& message )
{
    if( message.has_state() )
        nState_ = E_LogFuneralHandlingStatus( message.state() );
    if( message.has_handling_unit() )
    {
        if( handler_ )
            handler_->Get< LogFuneralConsigns >().TerminateConsign( *this );
        handler_ = FindLogEntity( message.handling_unit() );
        if( handler_ )
            handler_->Get< LogFuneralConsigns >().HandleConsign( *this );
    }
    if( message.has_convoying_unit() )
    {
        if( convoy_ )
            convoy_->Get< LogFuneralConsigns >().TerminateConsign( *this );
        convoy_ = agentResolver_.Find( message.convoying_unit().id() );
        if( convoy_ )
            convoy_->Get< LogFuneralConsigns >().HandleConsign( *this );
    }
    if( message.has_current_state_end_tick() )
        currentStateEndTick_ = message.current_state_end_tick();
    if( message.has_packaging_resource() )
        packagingResource_ = dotationResolver_.Find( message.packaging_resource().id() );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogFuneralConsign::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogFuneralConsign::Display( Displayer_ABC& displayer, Displayer_ABC& itemDisplayer ) const
{
    displayer.Display( consumer_ ).Display( nState_ );
    itemDisplayer.Display( tools::translate( "Logistic", "Instruction:" ), nID_ )
                 .Display( tools::translate( "Logistic", "Consumer:" ), consumer_ )
                 .Display( tools::translate( "Logistic", "Handler:" ), handler_ )
                 .Display( tools::translate( "Logistic", "Convoy:" ), convoy_ )
                 .Display( tools::translate( "Logistic", "Rank:" ), rank_ )
                 .Display( tools::translate( "Logistic", "Current packaging:" ), packagingResource_ )
                 .Display( tools::translate( "Logistic", "State:" ), nState_ );
    if( currentStateEndTick_ == std::numeric_limits< unsigned int >::max() )
        itemDisplayer.Display( tools::translate( "Logistic", "Current state end:" ), tools::translate( "Logistic", "Unknown" ) );
    else
    {
        unsigned int endSeconds = simulation_.GetInitialDateTime().toTime_t() + currentStateEndTick_ * simulation_.GetTickDuration();
        QDateTime endDate = QDateTime::	fromTime_t( endSeconds );
        QDateTime curDate = simulation_.GetDateTime();

        QString dateDisplay;
        if ( endDate.date() != curDate.date() )
            dateDisplay += endDate.date().toString() + " ";
        dateDisplay += endDate.time().toString();

        itemDisplayer.Display( tools::translate( "Logistic", "Current state end:" ), dateDisplay );
    }
}

// -----------------------------------------------------------------------------
// Name: LogFuneralConsign::Draw
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void LogFuneralConsign::Draw( const Point2f& , const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( ! handler_ || ! tools.ShouldDisplay( "RealTimeLogistic" ) )
        return;
    const Point2f from = handler_->Get< Positions >().GetPosition();
    const Point2f to   = consumer_.Get< Positions >().GetPosition();
    if( ! viewport.IsVisible( Rectangle2f( from, to ) ) )
        return;
    glColor4f( COLOR_AQUA );
    switch( nState_ )
    {
        case eLogFuneralHandlingStatus_TransportingUnpackaged:
        case eLogFuneralHandlingStatus_TransportingPackaged:
            glLineStipple( 1, tools.StipplePattern() );
            break;
        default:
            glLineStipple( 1, tools.StipplePattern(0) );
    }
    tools.DrawCurvedArrow( from, to, 0.5f );
}

// -----------------------------------------------------------------------------
// Name: LogFuneralConsign::FindLogEntity
// Created: AHC 2010-10-13
// -----------------------------------------------------------------------------
kernel::Entity_ABC* LogFuneralConsign::FindLogEntity(const sword::ParentEntity& msg)
{
    kernel::Entity_ABC* retval = 0;
    if( msg.has_automat() )
        retval = automatResolver_.Find( msg.automat().id() );
    else if( msg.has_formation() )
        retval = formationResolver_.Find( msg.formation().id() );
    return retval;
}