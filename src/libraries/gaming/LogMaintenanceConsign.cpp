// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "LogMaintenanceConsign.h"
#include "LogisticConsigns.h"
#include "Simulation.h"
#include "clients_gui/DisplayExtractor.h"
#include "clients_gui/LogisticHelpers.h"
#include "clients_gui/GLOptions.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/BreakdownType.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/DisplayExtractor_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "protocol/Protocol.h"

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogMaintenanceConsign::LogMaintenanceConsign( Controller& controller, const sword::LogMaintenanceHandlingCreation& message,
                                              const kernel::EntityResolver_ABC& resolver, const tools::Resolver_ABC< ComponentType >& componentResolver,
                                              const tools::Resolver_ABC< kernel::BreakdownType >& breakdownResolver, const Simulation& simulation,
                                              kernel::Agent_ABC& consumer )
    : LogisticsConsign_ABC( message.request().id(), controller, simulation, message.tick() )
    , resolver_           ( resolver )
    , consumer_           ( consumer )
    , provider_           ( 0 )
    , equipmentType_      ( & componentResolver.Get( message.equipement().id() ) )
    , breakdownType_      ( & breakdownResolver.Get( message.breakdown().id() ) )
    , diagnosed_          ( false )
    , nState_             ( sword::LogMaintenanceHandlingUpdate::finished )
{
   // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogMaintenanceConsign::~LogMaintenanceConsign()
{
   // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::Update
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
bool LogMaintenanceConsign::Update( const sword::LogMaintenanceHandlingUpdate& message, kernel::Entity_ABC* handler )
{
    if( message.has_state() )
        nState_ = message.state();
    if( message.has_diagnosed() )
        diagnosed_ = message.diagnosed();
    if( message.has_current_state_end_tick() )
        currentStateEndTick_ = message.current_state_end_tick();
    else
        currentStateEndTick_ = std::numeric_limits< unsigned int >::max();
    provider_ = handler;
    return nState_ != sword::LogMaintenanceHandlingUpdate::finished;
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::Draw
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void LogMaintenanceConsign::Draw( const Point2f& , const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    if( ! provider_ || !tools.GetOptions().ShouldDisplay( "RealTimeLogistic" ) )
        return;
    const Point2f from = logistic_helpers::GetLogisticPosition( *provider_ );
    const Point2f to   = logistic_helpers::GetLogisticPosition( consumer_ );
    if( from.IsZero() || to.IsZero() )
        return;
    if( ! viewport.IsVisible( Rectangle2f( from, to ) ) )
        return;
    static const float color[4] = { COLOR_MAROON };
    glColor4f( color[0], color[1], color[2], tools.GetCurrentAlpha() );
    switch( nState_ )
    {
    case sword::LogMaintenanceHandlingUpdate::transporter_moving_to_supply:
        glLineStipple( 1, tools.StipplePattern() );
        break;
    case sword::LogMaintenanceHandlingUpdate::transporter_moving_back:
        glLineStipple( 1, tools.StipplePattern(-1) );
        break;
    default:
        glLineStipple( 1, tools.StipplePattern(0) );
    }
    tools.DrawCurvedArrow( from, to, 0.5f );
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::RefersToAgent
// Created: LDC 2013-09-16
// -----------------------------------------------------------------------------
bool LogMaintenanceConsign::RefersToAgent( unsigned long id ) const
{
    return consumer_.GetId() == id || ( provider_ && provider_->GetId() == id );
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::RefersToAgent
// Created: LDC 2014-01-06
// -----------------------------------------------------------------------------
bool LogMaintenanceConsign::RefersToAgent( const std::set< unsigned long >& id ) const
{
    if( id.find( consumer_.GetId() ) != id.end() )
        return true;
    return provider_ && ( id.find( provider_->GetId() ) != id.end() );
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::GetConsumer
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
kernel::Agent_ABC* LogMaintenanceConsign::GetConsumer() const
{
    return &consumer_;
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::GetHandler
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
kernel::Entity_ABC* LogMaintenanceConsign::GetHandler() const
{
    return provider_;
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::GetEquipment
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
const kernel::ComponentType* LogMaintenanceConsign::GetEquipment() const
{
    return equipmentType_;
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::GetBreakdown
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
const kernel::BreakdownType* LogMaintenanceConsign::GetBreakdown() const
{
    return breakdownType_;
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::IsDiagnosed
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
bool LogMaintenanceConsign::IsDiagnosed() const
{
    return diagnosed_;
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::GetStatus
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
sword::LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus LogMaintenanceConsign::GetStatus() const
{
    return nState_;
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::GetStatusDisplay
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
QString LogMaintenanceConsign::GetStatusDisplay() const
{
    return QString::fromStdString( ENT_Tr::ConvertFromLogMaintenanceHandlingStatus( nState_ ) );
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::GetStatusDisplay
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
QString LogMaintenanceConsign::GetStatusDisplay( int status ) const
{
    if( sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus_IsValid( status ) )
        return QString::fromStdString( ENT_Tr::ConvertFromLogMaintenanceHandlingStatus(
            static_cast< sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus >( status ) ) );
    return QString();
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::GetCurrentStartedTime
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
QString LogMaintenanceConsign::GetCurrentStartedTime() const
{
    return GetStatusLastStarted( nState_ );
}

kernel::Entity_ABC* LogMaintenanceConsign::GetRequestHandler( uint32_t entityId ) const
{
    return resolver_.FindEntity( entityId );
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::NeedResolution
// Created: LGY 2014-01-21
// -----------------------------------------------------------------------------
bool LogMaintenanceConsign::NeedResolution() const
{
    return nState_ == sword::LogMaintenanceHandlingUpdate::waiting_for_transporter_selection
        || nState_ == sword::LogMaintenanceHandlingUpdate::waiting_for_diagnosis_team_selection
        || nState_ == sword::LogMaintenanceHandlingUpdate::waiting_for_repair_team_selection;
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::GetType
// Created: LGY 2014-01-21
// -----------------------------------------------------------------------------
E_LogisticChain LogMaintenanceConsign::GetType() const
{
    return eMaintenance;
}
