// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "LogMaintenanceConsign.h"
#include "LogisticConsigns.h"
#include "Simulation.h"
#include "LogConsignDisplayer_ABC.h"
#include "clients_gui/DisplayExtractor.h"
#include "clients_gui/LogisticHelpers.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/BreakdownType.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/DisplayExtractor_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogMaintenanceConsign::LogMaintenanceConsign( Controller& controller, const sword::LogMaintenanceHandlingCreation& message, const tools::Resolver_ABC< Agent_ABC >& resolver, const tools::Resolver_ABC< ComponentType >& componentResolver, const tools::Resolver_ABC< kernel::BreakdownType >& breakdownResolver, const Simulation& simulation )
    : controller_         ( controller )
    , resolver_           ( resolver )
    , simulation_         ( simulation )
    , nID_                ( message.request().id() )
    , consumer_           ( resolver_.Get( message.unit().id() ) )
    , pPionLogHandling_   ( 0 )
    , equipmentType_      ( & componentResolver.Get( message.equipement().id() ) )
    , breakdownType_      ( & breakdownResolver.Get( message.breakdown().id() ) )
    , diagnosed_          ( false )
    , nState_             ( eLogMaintenanceHandlingStatus_Termine )
    , currentStateEndTick_( std::numeric_limits< unsigned int >::max() )
{
    consumer_.Get< LogMaintenanceConsigns >().AddConsign( *this );
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogMaintenanceConsign::~LogMaintenanceConsign()
{
    consumer_.Get< LogMaintenanceConsigns >().RemoveConsign( *this );
    if( pPionLogHandling_ )
        if( LogMaintenanceConsigns* consign = pPionLogHandling_->Retrieve< LogMaintenanceConsigns >() )
            consign->TerminateConsign( *this );
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::Update
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void LogMaintenanceConsign::Update( const sword::LogMaintenanceHandlingUpdate& message )
{
    if( message.has_state()  )
        nState_ = E_LogMaintenanceHandlingStatus( message.state() );
    if( message.has_diagnosed()  )
        diagnosed_ = message.diagnosed();
    if( message.has_current_state_end_tick() )
        currentStateEndTick_ = message.current_state_end_tick();
    if( message.has_provider() && ( !pPionLogHandling_ || message.provider().id() != int( pPionLogHandling_->GetId() ) ) )
    {
        if( pPionLogHandling_ )
            pPionLogHandling_->Get< LogMaintenanceConsigns >().TerminateConsign( *this );
        pPionLogHandling_ = resolver_.Find( message.provider().id() );
        if( pPionLogHandling_ )
            pPionLogHandling_->Get< LogMaintenanceConsigns >().HandleConsign( *this );
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogMaintenanceConsign::Display( LogConsignDisplayer_ABC& displayer, kernel::DisplayExtractor_ABC& displayExtractor ) const
{
    gui::DisplayExtractor& extractor = *static_cast< gui::DisplayExtractor* >( &displayExtractor );
    displayer.DisplayTitle( consumer_.GetTooltip(), tools::ToString( nState_ ) );
    displayer.DisplayItem( tools::translate( "Logistic", "Instruction:" ), extractor.GetDisplayName( nID_ ) );
    displayer.DisplayItem( tools::translate( "Logistic", "Consumer:" ), extractor.GetDisplayName( consumer_ ) );
    if( pPionLogHandling_ )
        displayer.DisplayItem( tools::translate( "Logistic", "Handler:" ), extractor.GetDisplayName( *pPionLogHandling_ ) );
    if( diagnosed_ && equipmentType_ )
        displayer.DisplayItem( tools::translate( "Logistic", "Equipment:" ), extractor.GetDisplayName( equipmentType_->GetName() ) );
    if( diagnosed_ && breakdownType_ )
        displayer.DisplayItem( tools::translate( "Logistic", "Breakdown:" ), extractor.GetDisplayName( breakdownType_->GetName() ) );
    displayer.DisplayItem( tools::translate( "Logistic", "State:" ), tools::ToString( nState_ ) );
    if( currentStateEndTick_ == std::numeric_limits< unsigned int >::max() )
        displayer.DisplayItem( tools::translate( "Logistic", "Current state end:" ), tools::translate( "Logistic", "Unknown" ) );
    else
    {
        unsigned int endSeconds = simulation_.GetInitialDateTime().toTime_t() + currentStateEndTick_ * simulation_.GetTickDuration();
        QDateTime endDate = QDateTime::fromTime_t( endSeconds );
        QDateTime curDate = simulation_.GetDateTime();

        QString dateDisplay;
        if( endDate.date() != curDate.date() )
            dateDisplay += endDate.date().toString() + " ";
        dateDisplay += endDate.time().toString();

        displayer.DisplayItem( tools::translate( "Logistic", "Current state end:" ), dateDisplay );
    }
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::Draw
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void LogMaintenanceConsign::Draw( const Point2f& , const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    if( ! pPionLogHandling_ || ! tools.ShouldDisplay( "RealTimeLogistic" ) )
        return;
    const Point2f from = logistic_helpers::GetLogisticPosition( *pPionLogHandling_ );
    const Point2f to   = logistic_helpers::GetLogisticPosition( consumer_ );
    if( from.IsZero() || to.IsZero() )
        return;
    if( ! viewport.IsVisible( Rectangle2f( from, to ) ) )
        return;
    glColor4f( COLOR_MAROON );
    switch( nState_ )
    {
    case eLogMaintenanceHandlingStatus_RemorqueurDeplacementAller:
        glLineStipple( 1, tools.StipplePattern() );
        break;
    case eLogMaintenanceHandlingStatus_RemorqueurDeplacementRetour:
        glLineStipple( 1, tools.StipplePattern(-1) );
        break;
    default:
        glLineStipple( 1, tools.StipplePattern(0) );
    }
    tools.DrawCurvedArrow( from, to, 0.5f );
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::GetId
// Created: LDC 2013-09-16
// -----------------------------------------------------------------------------
unsigned int LogMaintenanceConsign::GetId() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::RefersToAgent
// Created: LDC 2013-09-16
// -----------------------------------------------------------------------------
bool LogMaintenanceConsign::RefersToAgent( unsigned int id ) const
{
    return consumer_.GetId() == id;
}
