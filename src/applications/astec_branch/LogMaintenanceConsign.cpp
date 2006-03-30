// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/LogMaintenanceConsign.cpp $
// $Author: Age $
// $Modtime: 6/04/05 10:54 $
// $Revision: 2 $
// $Workfile: LogMaintenanceConsign.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "LogMaintenanceConsign.h"
#include "Agent.h"
#include "LogisticConsigns.h"
#include "Controller.h"
#include "Displayer_ABC.h"
#include "ValuedListItem.h"
#include "GlTools_ABC.h"
#include "Positions.h"

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogMaintenanceConsign::LogMaintenanceConsign( Controller& controller, const Resolver_ABC< Agent >& resolver, const ASN1T_MsgLogMaintenanceTraitementEquipementCreation& message )
    : controller_( controller )
    , resolver_( resolver )
    , nID_             ( message.oid_consigne )
    , pion_            ( resolver_.Get( message.oid_pion ) )
    , pPionLogHandling_( 0 )
    , nEquipmentTypeID_( message.type_equipement )
    , nBreakdownTypeID_( message.type_panne )
    , diagnosed_       ( false )
    , nState_          ( eLogMaintenanceTraitementEtat_Termine )
{
    pion_.Get< LogisticConsigns >().AddConsign( *this );
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogMaintenanceConsign::~LogMaintenanceConsign()
{
    pion_.Get< LogisticConsigns >().RemoveConsign( *this );
    if( pPionLogHandling_ )
        pPionLogHandling_->Get< LogisticConsigns >().TerminateConsign( *this );
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
    if( pPionLogHandling_ )
        pPionLogHandling_->Get< LogisticConsigns >().TerminateConsign( *this );
    pPionLogHandling_ = resolver_.Find( message.oid_pion_log_traitant );
    if( pPionLogHandling_ )
        pPionLogHandling_->Get< LogisticConsigns >().HandleConsign( *this );
    if( message.m.etatPresent )
        nState_ = E_LogMaintenanceTraitementEtat( message.etat );
    if( message.m.diagnostique_effectuePresent )
        diagnosed_ = message.diagnostique_effectue;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogMaintenanceConsign::Display( Displayer_ABC& displayer ) const
{
    displayer.Display( "Consigne :", nID_ )
             .Display( "Pion demandeur :", pion_ )
             .Display( "Pion traitant :", pPionLogHandling_ )
             .Display( "Type d'équipement :", nEquipmentTypeID_ ) // $$$$ AGE 2006-03-21: only if diagnosed
             .Display( "Type de panne :", nBreakdownTypeID_ )     // $$$$ AGE 2006-03-21: only if diagnosed
             .Display( "Etat :", nState_ );
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::Draw
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void LogMaintenanceConsign::Draw( const geometry::Point2f& where, const GlTools_ABC& tools ) const
{
    if( ! pPionLogHandling_ || ! tools.ShouldDisplay( "RealTimeLogistic" ) )
        return;

    glColor4d( COLOR_MAROON );
    switch( nState_ )
    {
    case eLogMaintenanceTraitementEtat_RemorqueurDeplacementAller:
        glLineStipple( 1, tools.StipplePattern() );
        break;
    case eLogMaintenanceTraitementEtat_RemorqueurDeplacementRetour:
        glLineStipple( 1, ~tools.StipplePattern() );
        break;
    default:
        glLineStipple( 1, 0x00FF );
    }
    tools.DrawCurvedArrow( pPionLogHandling_->Get< Positions >().GetPosition(), pion_.Get< Positions >().GetPosition(), 0.5f );
}
