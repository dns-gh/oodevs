// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "LogMedicalConsign.h"
#include "LogisticConsigns.h"
#include "Simulation.h"
#include "LogConsignDisplayer_ABC.h"
#include "clients_gui/DisplayExtractor.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/DisplayExtractor_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Tools.h"
#include "protocol/SimulationSenders.h"

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogMedicalConsign::LogMedicalConsign( Controller& controller, const tools::Resolver_ABC< Agent_ABC >& resolver, const Simulation& simulation, const sword::LogMedicalHandlingCreation& message )
    : controller_         ( controller )
    , resolver_           ( resolver )
    , simulation_         ( simulation )
    , nID_                ( message.request().id() )
    , consumer_           ( resolver_.Get( message.unit().id() ) )
    , pPionLogHandling_   ( 0 )
    , wound_              ( E_HumanWound( message.wound() ) )
    , bMentalDeceased_    ( message.mental_wound() )
    , bContaminated_      ( message.nbc_contaminated() )
    , diagnosed_          ( false )
    , nState_             ( eLogMedicalHandlingStatus_Termine )
    , currentStateEndTick_( std::numeric_limits< unsigned int >::max() )
{
    consumer_.Get< LogMedicalConsigns >().AddConsign( *this );
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogMedicalConsign::~LogMedicalConsign()
{
    consumer_.Get< LogMedicalConsigns >().RemoveConsign( *this );
    if( pPionLogHandling_ )
        pPionLogHandling_->Get< LogMedicalConsigns >().TerminateConsign( *this );
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::Update
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void LogMedicalConsign::Update( const sword::LogMedicalHandlingUpdate& message )
{
    if( message.has_provider() && ( !pPionLogHandling_ || message.provider().id() != int( pPionLogHandling_->GetId() ) ) )
    {
        if( pPionLogHandling_ )
            pPionLogHandling_->Get< LogMedicalConsigns >().TerminateConsign( *this );
        pPionLogHandling_ = resolver_.Find( message.provider().id() );
        if( pPionLogHandling_ )
            pPionLogHandling_->Get< LogMedicalConsigns >().HandleConsign( *this );
    }
    if( message.has_mental_wound()  )
        bMentalDeceased_ = message.mental_wound();
    if( message.has_nbc_contaminated()  )
        bContaminated_   = message.nbc_contaminated();
    if( message.has_wound()  )
        wound_ = E_HumanWound( message.wound() );
    if( message.has_state()  )
        nState_ = E_LogMedicalHandlingStatus( message.state() );
    if( message.has_current_state_end_tick() )
        currentStateEndTick_ = message.current_state_end_tick();
    else
        currentStateEndTick_ = std::numeric_limits< unsigned int >::max();
    if( message.has_diagnosed()  )
        diagnosed_ = message.diagnosed();
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogMedicalConsign::Display( LogConsignDisplayer_ABC& displayer, kernel::DisplayExtractor_ABC& displayExtractor ) const
{
    gui::DisplayExtractor& extractor = *static_cast< gui::DisplayExtractor* >( &displayExtractor );
    displayer.DisplayTitle( consumer_.GetTooltip(), tools::ToString( nState_ ) );
    displayer.DisplayItem( tools::translate( "Logistic", "Instruction:" ), extractor.GetDisplayName( nID_ ) );
    displayer.DisplayItem( tools::translate( "Logistic", "Consumer:" ), extractor.GetDisplayName( consumer_ ) );
    if( pPionLogHandling_ )
        displayer.DisplayItem( tools::translate( "Logistic", "Handler:" ), extractor.GetDisplayName( *pPionLogHandling_ ) );
    displayer.DisplayItem( tools::translate( "Logistic", "Mentally injured:" ), extractor.GetDisplayName( bMentalDeceased_ ) );
    displayer.DisplayItem( tools::translate( "Logistic", "NBC contaminated:" ), extractor.GetDisplayName( bContaminated_ ) );
    displayer.DisplayItem( tools::translate( "Logistic", "State:" ), tools::ToString( nState_ ) );
    if( diagnosed_ )
        displayer.DisplayItem( tools::translate( "Logistic", "Injury:" ), tools::ToString( wound_ ) );
    else
        displayer.DisplayItem( tools::translate( "Logistic", "Injury:" ), tools::translate( "Logistic", "Not diagnosed" ) );
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
// Name: LogMedicalConsign::Draw
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void LogMedicalConsign::Draw( const Point2f& , const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    if( ! pPionLogHandling_ || ! tools.ShouldDisplay( "RealTimeLogistic" ) )
        return;
    const Point2f from = pPionLogHandling_->Get< Positions >().GetPosition();
    const Point2f to   = consumer_.Get< Positions >().GetPosition();
    if( ! viewport.IsVisible( Rectangle2f( from, to ) ) )
        return;
    glColor4f( COLOR_PINK );
    switch( nState_ )
    {
    case eLogMedicalHandlingStatus_AmbulanceReleveDeplacementAller:
    case eLogMedicalHandlingStatus_AmbulanceRamassageDeplacementAller:
        glLineStipple( 1, tools.StipplePattern() );
        break;
    case eLogMedicalHandlingStatus_AmbulanceReleveDeplacementRetour:
        glLineStipple( 1, tools.StipplePattern(-1) );
        break;
    default:
        glLineStipple( 1, tools.StipplePattern(0) );
    }
    tools.DrawCurvedArrow( from, to, 0.4f );
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::GetId
// Created: LDC 2013-09-16
// -----------------------------------------------------------------------------
unsigned int LogMedicalConsign::GetId() const
{
    return nID_;
}
    
// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::RefersToAgent
// Created: LDC 2013-09-16
// -----------------------------------------------------------------------------
bool LogMedicalConsign::RefersToAgent( unsigned int id ) const
{
    return consumer_.GetId() == id || ( pPionLogHandling_ && pPionLogHandling_->GetId() == id );
}
