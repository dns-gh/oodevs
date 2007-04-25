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
#include "clients_kernel/Agent_ABC.h"
#include "LogisticConsigns.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/Positions.h"
#include "Tools.h"

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogMaintenanceConsign::LogMaintenanceConsign( Controller& controller, const ASN1T_MsgLogMaintenanceTraitementEquipementCreation& message, const Resolver_ABC< Agent_ABC >& resolver, const Resolver_ABC< ComponentType >& componentResolver, const Resolver_ABC< BreakdownType >& breakdownResolver )
    : controller_      ( controller )
    , resolver_        ( resolver )
    , nID_             ( message.oid_consigne )
    , pion_            ( resolver_.Get( message.oid_pion ) )
    , pPionLogHandling_( 0 )
    , equipmentType_   ( & componentResolver.Get( message.type_equipement ) )
    , breakdownType_   ( & breakdownResolver.Get( message.type_panne ) )
    , diagnosed_       ( false )
    , nState_          ( eLogMaintenanceTraitementEtat_Termine )
{
    if( LogMaintenanceConsigns* consign = pion_.Retrieve< LogMaintenanceConsigns >() )
        consign->AddConsign( *this );
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogMaintenanceConsign::~LogMaintenanceConsign()
{
    if( LogMaintenanceConsigns* consign = pion_.Retrieve< LogMaintenanceConsigns >() )
        consign->RemoveConsign( *this );
    if( pPionLogHandling_ )
        if( LogMaintenanceConsigns* consign = pPionLogHandling_->Retrieve< LogMaintenanceConsigns >() )
            consign->TerminateConsign( *this );
}


// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::Update
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void LogMaintenanceConsign::Update( const ASN1T_MsgLogMaintenanceTraitementEquipementUpdate& message )
{
    if( message.m.etatPresent )
        nState_ = E_LogMaintenanceTraitementEtat( message.etat );
    if( message.m.diagnostique_effectuePresent )
        diagnosed_ = message.diagnostique_effectue;

    if( pPionLogHandling_ )
        pPionLogHandling_->Get< LogMaintenanceConsigns >().TerminateConsign( *this );
    pPionLogHandling_ = resolver_.Find( message.oid_pion_log_traitant );
    if( pPionLogHandling_ )
        pPionLogHandling_->Get< LogMaintenanceConsigns >().HandleConsign( *this );
    
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogMaintenanceConsign::Display( Displayer_ABC& displayer, Displayer_ABC& itemDisplayer ) const
{
    displayer.Display( pion_ ).Display( nState_ );

    itemDisplayer.Display( tools::translate( "Logistic", "Instruction:" ), nID_ )
                 .Display( tools::translate( "Logistic", "Consumer:" ), pion_ )
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
    const Point2f to   = pion_.Get< Positions >().GetPosition();
    if( ! viewport.IsVisible( Rectangle2f( from, to ) ) )
        return;

    glColor4f( COLOR_MAROON );
    switch( nState_ )
    {
    case eLogMaintenanceTraitementEtat_RemorqueurDeplacementAller:
        glLineStipple( 1, tools.StipplePattern() );
        break;
    case eLogMaintenanceTraitementEtat_RemorqueurDeplacementRetour:
        glLineStipple( 1, tools.StipplePattern(-1) );
        break;
    default:
        glLineStipple( 1, tools.StipplePattern(0) );
    }
    tools.DrawCurvedArrow( from, to, 0.5f );
}
