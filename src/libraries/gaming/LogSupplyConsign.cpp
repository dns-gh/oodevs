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
#include "SupplyResourceRequest.h"
#include "clients_gui/DisplayExtractor.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/LogisticHelpers.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/DisplayExtractor_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Tools.h"
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

                                  , const Simulation& simulation
                                  , const sword::LogSupplyHandlingCreation& message )
    : LogisticsConsign_ABC               ( message.request().id(), controller, simulation, message.tick() )
    , resolver_                          ( resolver )
    , agentResolver_                     ( agentResolver )
    , formationResolver_                 ( formationResolver )
    , pLogHandlingEntity_                ( controller_, FindLogEntity( message.supplier() ) )
    , pPionLogConvoying_                 ( controller_ )
    , pLogProvidingConvoyResourcesEntity_( controller_, FindLogEntity( message.transporters_provider() ) )
    , nState_                            ( eLogSupplyHandlingStatus_Termine )
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
// Name: LogSupplyConsign::OnReceiveMsgUpdate
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void LogSupplyConsign::Update( const sword::LogSupplyHandlingUpdate& message, kernel::Agent_ABC* pionLogConvoying )
{
    pPionLogConvoying_ = pionLogConvoying;

    if( message.has_state()  )
        nState_ = E_LogSupplyHandlingStatus( message.state() );
    if( message.has_current_state_end_tick() )
        currentStateEndTick_ = message.current_state_end_tick();
    else
        currentStateEndTick_ = std::numeric_limits< unsigned int >::max();
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::Draw
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void LogSupplyConsign::Draw( const Point2f& , const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
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
E_LogSupplyHandlingStatus LogSupplyConsign::GetStatus() const
{
    return nState_;
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::GetStatusDisplay
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
QString LogSupplyConsign::GetStatusDisplay() const
{
    return tools::ToString( nState_ );
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::GetStatusDisplay
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
QString LogSupplyConsign::GetStatusDisplay( int status ) const
{
    if( 0 <= status && status < eNbrLogSupplyHandlingStatus )
        return tools::ToString( static_cast< E_LogSupplyHandlingStatus >( status ) );
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
