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
#include "Tools.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Viewport_ABC.h"
#include "protocol/Protocol.h"

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogMaintenanceConsign::LogMaintenanceConsign( Controller& controller, const sword::LogMaintenanceHandlingCreation& message, const tools::Resolver_ABC< Agent_ABC >& resolver, const tools::Resolver_ABC< ComponentType >& componentResolver, const tools::Resolver_ABC< kernel::BreakdownType >& breakdownResolver )
    : controller_      ( controller )
    , resolver_        ( resolver )
    , nID_             ( message.request().id() )
    , consumer_        ( resolver_.Get( message.unit().id() ) )
    , pPionLogHandling_( 0 )
    , equipmentType_   ( & componentResolver.Get( message.equipement().id() ) )
    , breakdownType_   ( & breakdownResolver.Get( message.breakdown().id() ) )
    , diagnosed_       ( false )
    , nState_          ( eLogMaintenanceHandlingStatus_Termine )
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
    if( message.has_etat()  )
        nState_ = E_LogMaintenanceHandlingStatus( message.etat() );
    if( message.has_diagnostique_effectue()  )
        diagnosed_ = message.diagnostique_effectue();
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
void LogMaintenanceConsign::Display( Displayer_ABC& displayer, Displayer_ABC& itemDisplayer ) const
{
    displayer.Display( consumer_ ).Display( nState_ );
    itemDisplayer.Display( tools::translate( "Logistic", "Instruction:" ), nID_ )
                 .Display( tools::translate( "Logistic", "Consumer:" ), consumer_ )
                 .Display( tools::translate( "Logistic", "Handler:" ), pPionLogHandling_ )
                 .Display( tools::translate( "Logistic", "Equipment:" ), diagnosed_ ? equipmentType_ : 0 )
                 .Display( tools::translate( "Logistic", "Breakdown:" ), diagnosed_ ? breakdownType_ : 0 )
                 .Display( tools::translate( "Logistic", "State:" ), nState_ );
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::Draw
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void LogMaintenanceConsign::Draw( const Point2f& , const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( ! pPionLogHandling_ || ! tools.ShouldDisplay( "RealTimeLogistic" ) )
        return;
    const Point2f from = pPionLogHandling_->Get< Positions >().GetPosition();
    const Point2f to   = consumer_.Get< Positions >().GetPosition();
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
