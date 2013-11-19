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
#include "Simulation.h"
#include "clients_gui/DisplayExtractor.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/DisplayExtractor_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogFuneralConsign constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogFuneralConsign::LogFuneralConsign( Controller& controller, const sword::LogFuneralHandlingCreation& message, const tools::Resolver_ABC< Automat_ABC >& automatResolver,
                                      const tools::Resolver_ABC< Agent_ABC >& agentResolver,
                                      const tools::Resolver_ABC< Formation_ABC >& formationResolver,
                                      const tools::Resolver_ABC< DotationType >& dotationResolver,
                                      const Simulation& simulation )
    : LogisticsConsign_ABC( message.request().id(), controller, simulation, message.tick() )
    , automatResolver_    ( automatResolver )
    , agentResolver_      ( agentResolver )
    , formationResolver_  ( formationResolver )
    , dotationResolver_   ( dotationResolver )
    , rank_               ( (E_HumanRank)message.rank() )
    , consumer_           ( agentResolver_.Get( message.unit().id() ) )
    , handler_            ( controller )
    , convoy_             ( controller )
    , packagingResource_  ( 0 )
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
        if( LogFuneralConsigns* consign = handler_.ConstCast()->Retrieve< LogFuneralConsigns >() )
            consign->TerminateConsign( *this );
    }
    if( convoy_ )
        convoy_.ConstCast()->Get< LogFuneralConsigns >().TerminateConsign( *this );
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
            handler_.ConstCast()->Get< LogFuneralConsigns >().TerminateConsign( *this );
        handler_ = FindLogEntity( message.handling_unit() );
        if( handler_ )
            handler_.ConstCast()->Get< LogFuneralConsigns >().HandleConsign( *this );
    }
    if( message.has_convoying_unit() )
    {
        if( convoy_ )
            convoy_.ConstCast()->Get< LogFuneralConsigns >().TerminateConsign( *this );
        convoy_ = agentResolver_.Find( message.convoying_unit().id() );
        if( convoy_ )
            convoy_.ConstCast()->Get< LogFuneralConsigns >().HandleConsign( *this );
    }
    if( message.has_current_state_end_tick() )
        currentStateEndTick_ = message.current_state_end_tick();
    else
        currentStateEndTick_ = std::numeric_limits< unsigned int >::max();
    if( message.has_packaging_resource() )
        packagingResource_ = dotationResolver_.Find( message.packaging_resource().id() );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogFuneralConsign::Draw
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void LogFuneralConsign::Draw( const Point2f& , const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
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

// -----------------------------------------------------------------------------
// Name: LogFuneralConsign::RefersToAgent
// Created: LDC 2013-09-16
// -----------------------------------------------------------------------------
bool LogFuneralConsign::RefersToAgent( unsigned int id ) const
{
    return consumer_.GetId() == id
        || ( handler_ && handler_->GetId() == id )
        || ( convoy_ && convoy_->GetId() == id );
}

// -----------------------------------------------------------------------------
// Name: LogFuneralConsign::GetRank
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
E_HumanRank LogFuneralConsign::GetRank() const
{
    return rank_;
}

// -----------------------------------------------------------------------------
// Name: LogFuneralConsign::GetConsumer
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
const kernel::Agent_ABC* LogFuneralConsign::GetConsumer() const
{
    return &consumer_;
}

// -----------------------------------------------------------------------------
// Name: LogFuneralConsign::GetHandler
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* LogFuneralConsign::GetHandler() const
{
    return handler_;
}

// -----------------------------------------------------------------------------
// Name: LogFuneralConsign::GetConvoy
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
const kernel::Agent_ABC* LogFuneralConsign::GetConvoy() const
{
    return convoy_;
}

// -----------------------------------------------------------------------------
// Name: LogFuneralConsign::GetPackagingResource
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
const kernel::DotationType* LogFuneralConsign::GetPackagingResource() const
{
    return packagingResource_;
}

// -----------------------------------------------------------------------------
// Name: LogFuneralConsign::GetStatus
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
E_LogFuneralHandlingStatus LogFuneralConsign::GetStatus() const
{
    return nState_;
}

// -----------------------------------------------------------------------------
// Name: LogFuneralConsign::GetStatusDisplay
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
QString LogFuneralConsign::GetStatusDisplay() const
{
    return tools::ToString( nState_ );
}

// -----------------------------------------------------------------------------
// Name: LogFuneralConsign::GetStatusDisplay
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
QString LogFuneralConsign::GetStatusDisplay( int status ) const
{
    if( 0 <= status && status < eNbrLogFuneralHandlingStatus )
        return tools::ToString( static_cast< E_LogFuneralHandlingStatus >( status ) );
    return QString();
}

// -----------------------------------------------------------------------------
// Name: LogFuneralConsign::GetCurrentStartedTime
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
QString LogFuneralConsign::GetCurrentStartedTime() const
{
    return GetStatusLastStarted( nState_ );
}

kernel::Entity_ABC* LogFuneralConsign::GetRequestHandler( uint32_t entityId ) const
{
    auto handler = automatResolver_.Find( entityId );
    if( handler )
        return handler;
    return formationResolver_.Find( entityId );
}
