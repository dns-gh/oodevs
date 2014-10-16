//*****************************************************************************
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
#include "Simulation.h"
#include "SupplyRecipientResourcesRequest.h"
#include "clients_gui/DisplayExtractor.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/LogisticHelpers.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/DisplayExtractor_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Tools.h"
#include "protocol/MessageParameters.h"
#include "protocol/Protocol.h"
#include <boost/foreach.hpp>

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogSupplyConsign::LogSupplyConsign( Controller& controller, const tools::Resolver_ABC< Automat_ABC >& resolver
                                  , const tools::Resolver_ABC< Agent_ABC >& agentResolver
                                  , const tools::Resolver_ABC< Formation_ABC >& formationResolver
                                  , const tools::Resolver_ABC< kernel::DotationType >& dotationResolver
                                  , const Simulation& simulation
                                  , const sword::LogSupplyHandlingCreation& message )
    : LogisticsConsign_ABC               ( message.request().id(), controller, simulation, message.tick() )
    , resolver_                          ( resolver )
    , agentResolver_                     ( agentResolver )
    , formationResolver_                 ( formationResolver )
    , dotationResolver_                  ( dotationResolver )
    , pLogHandlingEntity_                ( controller_, FindLogEntity( message.supplier() ) )
    , pPionLogConvoying_                 ( controller_ )
    , pLogProvidingConvoyResourcesEntity_( controller_, FindLogEntity( message.transporters_provider() ) )
    , nState_                            ( sword::LogSupplyHandlingUpdate::convoy_finished )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogSupplyConsign::~LogSupplyConsign()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::Update
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
bool LogSupplyConsign::Update( const sword::LogSupplyHandlingUpdate& message, kernel::Agent_ABC* pionLogConvoying )
{
    pPionLogConvoying_ = pionLogConvoying;
    if( message.has_state()  )
        nState_ = message.state();
    if( message.has_current_state_end_tick() )
        currentStateEndTick_ = message.current_state_end_tick();
    else
        currentStateEndTick_ = std::numeric_limits< unsigned int >::max();
    return nState_ != sword::LogSupplyHandlingUpdate::convoy_finished;
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::Update
// Created: LGY 2014-01-28
// -----------------------------------------------------------------------------
void LogSupplyConsign::Update( const sword::SupplyRecipientResourceRequests& message )
{
    if( Count() == 0 )
    {
        BOOST_FOREACH( const sword::SupplyRecipientResourcesRequest& data, message.requests() )
        {
            SupplyRecipientResourcesRequest* request = new SupplyRecipientResourcesRequest( dotationResolver_, resolver_, data );
            Register( data.recipient().id(), *request );
        }
    }
    else
        Apply( boost::bind( &SupplyRecipientResourcesRequest::Update, _1, message ) );
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::Draw
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void LogSupplyConsign::Draw( const Point2f& , const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    if( !pLogHandlingEntity_ || !tools.ShouldDisplay( "RealTimeLogistic" ) )
        return;

    Point2f from = logistic_helpers::GetLogisticPosition( *pLogHandlingEntity_, true );
    if( from.IsZero() )
        return;
    for( tools::Iterator< const SupplyRecipientResourcesRequest& > it = CreateIterator(); it.HasMoreElements(); )
    {
        const Point2f to = logistic_helpers::GetLogisticPosition( it.NextElement().recipient_, true );
        if( to.IsZero() )
            return;
        if( viewport.IsVisible( Rectangle2f( from, to ) ) )
        {
            glColor4f( COLOR_ORANGE );
            switch( nState_ )
            {
                case sword::LogSupplyHandlingUpdate::convoy_moving_to_loading_point:
                case sword::LogSupplyHandlingUpdate::convoy_moving_to_unloading_point:
                    glLineStipple( 1, tools.StipplePattern() );
                    break;
                case sword::LogSupplyHandlingUpdate::convoy_moving_back_to_loading_point:
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
// Name: LogSupplyConsign::RefersToAgent
// Created: LDC 2013-09-16
// -----------------------------------------------------------------------------
bool LogSupplyConsign::RefersToAgent( unsigned long id ) const
{
    return ( pLogHandlingEntity_ && pLogHandlingEntity_->GetId() == id )
        || ( pPionLogConvoying_ && pPionLogConvoying_->GetId() == id )
        || ( pLogProvidingConvoyResourcesEntity_ && pLogProvidingConvoyResourcesEntity_->GetId() == id );
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::RefersToAgent
// Created: LDC 2014-01-06
// -----------------------------------------------------------------------------
bool LogSupplyConsign::RefersToAgent( const std::set< unsigned long >& id ) const
{
    return ( pLogHandlingEntity_ && id.find( pLogHandlingEntity_->GetId() ) != id.end() )
        || ( pPionLogConvoying_ && id.find( pPionLogConvoying_->GetId() ) != id.end() )
        || ( pLogProvidingConvoyResourcesEntity_ && id.find( pLogProvidingConvoyResourcesEntity_->GetId() ) != id.end() );
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::GetConsumer
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
kernel::Agent_ABC* LogSupplyConsign::GetConsumer() const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::GetHandler
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
kernel::Entity_ABC* LogSupplyConsign::GetHandler() const
{
    return pLogHandlingEntity_.ConstCast();
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::GetConvoy
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
kernel::Agent_ABC* LogSupplyConsign::GetConvoy() const
{
    return pPionLogConvoying_.ConstCast();
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::GetProviding
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* LogSupplyConsign::GetProviding() const
{
    return pLogProvidingConvoyResourcesEntity_;
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::GetStatus
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
sword::LogSupplyHandlingUpdate_EnumLogSupplyHandlingStatus LogSupplyConsign::GetStatus() const
{
    return nState_;
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::GetStatusDisplay
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
QString LogSupplyConsign::GetStatusDisplay() const
{
    return QString::fromStdString( ENT_Tr::ConvertFromLogSupplyHandlingStatus( nState_ ) );
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::GetStatusDisplay
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
QString LogSupplyConsign::GetStatusDisplay( int status ) const
{
    if( sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus_IsValid( status ) )
        return QString::fromStdString( ENT_Tr::ConvertFromLogSupplyHandlingStatus(
            static_cast< sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus >( status ) ) );
    return QString();
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::GetCurrentStartedTime
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
QString LogSupplyConsign::GetCurrentStartedTime() const
{
    return GetStatusLastStarted( nState_ );
}

kernel::Entity_ABC* LogSupplyConsign::GetRequestHandler( uint32_t entityId ) const
{
    auto handler = resolver_.Find( entityId );
    if( handler )
        return handler;
    return formationResolver_.Find( entityId );
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::UpdateHistoryState
// Created: LGY 2014-01-28
// -----------------------------------------------------------------------------
bool LogSupplyConsign::UpdateHistoryState( const sword::LogHistoryEntry& entry, HistoryState& state )
{
    if( entry.has_supply() )
    {
        const auto& sub = entry.supply();
        if( sub.has_destruction() || !sub.has_update() || !sub.has_creation() )
            return false;
        state.handler_ = GetRequestHandler(
            protocol::GetParentEntityId( sub.creation().supplier() ));

        if( sub.update().has_requests() )
            Update( sub.update().requests() );

        if( sub.update().has_state() )
            state.nStatus_ = sub.update().state();
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::GetType
// Created: LGY 2014-01-28
// -----------------------------------------------------------------------------
E_LogisticChain LogSupplyConsign::GetType() const
{
    return eSupply;
}
