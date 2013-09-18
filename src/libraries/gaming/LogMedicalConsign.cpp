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
LogMedicalConsign::LogMedicalConsign( kernel::Controller& controller, const tools::Resolver_ABC< Agent_ABC >& resolver, const Simulation& simulation, const sword::LogMedicalHandlingCreation& message )
    : LogisticsConsign_ABC( message.request().id(), controller, simulation, message.tick() )
    , resolver_           ( resolver )
    , consumer_           ( resolver_.Get( message.unit().id() ) )
    , pPionLogHandling_   ( 0 )
    , wound_              ( E_HumanWound( message.wound() ) )
    , bMentalDeceased_    ( message.mental_wound() )
    , bContaminated_      ( message.nbc_contaminated() )
    , diagnosed_          ( false )
    , nState_             ( eLogMedicalHandlingStatus_Termine )
    , rank_               ( (E_HumanRank)message.rank() )
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
    if( message.has_mental_wound() )
        bMentalDeceased_ = message.mental_wound();
    if( message.has_nbc_contaminated() )
        bContaminated_   = message.nbc_contaminated();
    if( message.has_wound() )
        wound_ = E_HumanWound( message.wound() );
    if( message.has_state() )
        nState_ = E_LogMedicalHandlingStatus( message.state() );
    if( message.has_current_state_end_tick() )
        currentStateEndTick_ = message.current_state_end_tick();
    else
        currentStateEndTick_ = std::numeric_limits< unsigned int >::max();
    if( message.has_diagnosed() )
        diagnosed_ = message.diagnosed();
    controller_.Update( *this );
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
// Name: LogMedicalConsign::RefersToAgent
// Created: LDC 2013-09-16
// -----------------------------------------------------------------------------
bool LogMedicalConsign::RefersToAgent( unsigned int id ) const
{
    return consumer_.GetId() == id || ( pPionLogHandling_ && pPionLogHandling_->GetId() == id );
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::GetRank
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
E_HumanRank LogMedicalConsign::GetRank() const
{
    return rank_;
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::GetConsumer
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
const kernel::Agent_ABC* LogMedicalConsign::GetConsumer() const
{
    return &consumer_;
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::GetHandler
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* LogMedicalConsign::GetHandler() const
{
    return pPionLogHandling_;
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::IsMental
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
bool LogMedicalConsign::IsMental() const
{
    return bMentalDeceased_;
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::IsContamined
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
bool LogMedicalConsign::IsContamined() const
{
    return bContaminated_;
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::IsDiagnosed
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
bool LogMedicalConsign::IsDiagnosed() const
{
    return diagnosed_;
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::GetWound
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
E_HumanWound LogMedicalConsign::GetWound() const
{
    return wound_;
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::GetStatus
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
E_LogMedicalHandlingStatus LogMedicalConsign::GetStatus() const
{
    return nState_;
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::GetStatusDisplay
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
QString LogMedicalConsign::GetStatusDisplay() const
{
    return tools::ToString( nState_ );
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::GetStatusDisplay
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
QString LogMedicalConsign::GetStatusDisplay( int status ) const
{
    if( 0 <= status && status < eNbrLogMedicalHandlingStatus )
        return tools::ToString( static_cast< E_LogMedicalHandlingStatus >( status ) );
    return QString();
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::GetCurrentStartedTime
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
QString LogMedicalConsign::GetCurrentStartedTime() const
{
    return GetStatusLastStarted( nState_ );
}

kernel::Entity_ABC* LogMedicalConsign::GetRequestHandler( uint32_t entityId ) const
{
    return resolver_.Find( entityId );
}
