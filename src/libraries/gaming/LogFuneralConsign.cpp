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
#include "clients_gui/GLView_ABC.h"
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
                                      const Simulation& simulation, kernel::Agent_ABC& consumer )
    : LogisticsConsign_ABC( message.request().id(), controller, simulation, message.tick() )
    , automatResolver_    ( automatResolver )
    , agentResolver_      ( agentResolver )
    , formationResolver_  ( formationResolver )
    , dotationResolver_   ( dotationResolver )
    , rank_               ( (E_HumanRank)message.rank() )
    , consumer_           ( consumer )
    , handler_            ( controller )
    , convoy_             ( controller )
    , packagingResource_  ( 0 )
    , nState_             ( sword::LogFuneralHandlingUpdate::finished )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogFuneralConsign destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogFuneralConsign::~LogFuneralConsign()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogFuneralConsign::Update
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
bool LogFuneralConsign::Update( const sword::LogFuneralHandlingUpdate& message, kernel::Entity_ABC* handler, kernel::Agent_ABC* convoy )
{
    if( message.has_state() )
        nState_ = message.state();
    handler_ = handler;
    convoy_ = convoy;
    if( message.has_current_state_end_tick() )
        currentStateEndTick_ = message.current_state_end_tick();
    else
        currentStateEndTick_ = std::numeric_limits< unsigned int >::max();
    if( message.has_packaging_resource() )
        packagingResource_ = dotationResolver_.Find( message.packaging_resource().id() );
    return nState_ != sword::LogFuneralHandlingUpdate::finished;
}

// -----------------------------------------------------------------------------
// Name: LogFuneralConsign::Draw
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void LogFuneralConsign::Draw( const Point2f& , const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    if( ! handler_ || ! tools.ShouldDisplay( "RealTimeLogistic" ) )
        return;
    const Point2f from = handler_->Get< Positions >().GetPosition();
    const Point2f to   = consumer_.Get< Positions >().GetPosition();
    if( ! viewport.IsVisible( Rectangle2f( from, to ) ) )
        return;
    static const float color[4] = { COLOR_AQUA };
    glColor4f( color[0], color[1], color[2], tools.GetCurrentAlpha() );
    switch( nState_ )
    {
        case sword::LogFuneralHandlingUpdate::transporting_unpackaged:
        case sword::LogFuneralHandlingUpdate::transporting_packaged:
            glLineStipple( 1, tools.StipplePattern() );
            break;
        default:
            glLineStipple( 1, tools.StipplePattern(0) );
    }
    tools.DrawCurvedArrow( from, to, 0.5f );
}

// -----------------------------------------------------------------------------
// Name: LogFuneralConsign::RefersToAgent
// Created: LDC 2013-09-16
// -----------------------------------------------------------------------------
bool LogFuneralConsign::RefersToAgent( unsigned long id ) const
{
    return consumer_.GetId() == id
        || ( handler_ && handler_->GetId() == id )
        || ( convoy_ && convoy_->GetId() == id );
}

// -----------------------------------------------------------------------------
// Name: LogFuneralConsign::RefersToAgent
// Created: LDC 2014-01-06
// -----------------------------------------------------------------------------
bool LogFuneralConsign::RefersToAgent( const std::set< unsigned long >& id ) const
{
    return( id.find( consumer_.GetId() ) != id.end() )
       || ( handler_ && id.find( handler_->GetId() ) != id.end() )
       || ( convoy_ && id.find( convoy_->GetId() ) != id.end() );
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
kernel::Agent_ABC* LogFuneralConsign::GetConsumer() const
{
    return &consumer_;
}

// -----------------------------------------------------------------------------
// Name: LogFuneralConsign::GetHandler
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
kernel::Entity_ABC* LogFuneralConsign::GetHandler() const
{
    return handler_.ConstCast();
}

// -----------------------------------------------------------------------------
// Name: LogFuneralConsign::GetConvoy
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
kernel::Agent_ABC* LogFuneralConsign::GetConvoy() const
{
    return convoy_.ConstCast();
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
sword::LogFuneralHandlingUpdate_EnumLogFuneralHandlingStatus LogFuneralConsign::GetStatus() const
{
    return nState_;
}

// -----------------------------------------------------------------------------
// Name: LogFuneralConsign::GetStatusDisplay
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
QString LogFuneralConsign::GetStatusDisplay() const
{
    return QString::fromStdString( ENT_Tr::ConvertFromLogFuneralHandlingStatus( nState_ ) );
}

// -----------------------------------------------------------------------------
// Name: LogFuneralConsign::GetStatusDisplay
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
QString LogFuneralConsign::GetStatusDisplay( int status ) const
{
    if( sword::LogFuneralHandlingUpdate::EnumLogFuneralHandlingStatus_IsValid( status ) )
        return QString::fromStdString( ENT_Tr::ConvertFromLogFuneralHandlingStatus(
            static_cast< sword::LogFuneralHandlingUpdate::EnumLogFuneralHandlingStatus >( status ) ) );
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

E_LogisticChain LogFuneralConsign::GetType() const
{
    return eFuneral;
}
