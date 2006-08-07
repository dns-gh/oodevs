// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "astec_gaming_pch.h"

#include "LogMedicalConsign.h"
#include "Agent_ABC.h"
#include "LogisticConsigns.h"
#include "Controller.h"
#include "astec_gui/Displayer_ABC.h"
#include "astec_gui/GlTools_ABC.h"
#include "Positions.h"

using namespace geometry;

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogMedicalConsign::LogMedicalConsign( Controller& controller, const Resolver_ABC< Agent_ABC >& resolver, const ASN1T_MsgLogSanteTraitementHumainCreation& message )
    : controller_      ( controller )
    , resolver_        ( resolver )
    , nID_             ( message.oid_consigne )
    , pion_            ( resolver_.Get( message.oid_pion ) )
    , pPionLogHandling_( 0 )
    , wound_           ( E_HumanWound( message.blessure ) )
    , bMentalDeceased_ ( message.blesse_mental )
    , bContaminated_   ( message.contamine_nbc )
    , diagnosed_       ( false )
    , nState_          ( eLogSanteTraitementEtat_Termine )
{
    pion_.Get< LogisticConsigns >().AddConsign( *this );
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogMedicalConsign::~LogMedicalConsign()
{
    pion_.Get< LogisticConsigns >().RemoveConsign( *this );
    if( pPionLogHandling_ )
        pPionLogHandling_->Get< LogisticConsigns >().TerminateConsign( *this );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::Update
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void LogMedicalConsign::Update( const ASN1T_MsgLogSanteTraitementHumainUpdate& message )
{
    if( message.m.oid_pion_log_traitantPresent )
    {
        if( pPionLogHandling_ )
            pPionLogHandling_->Get< LogisticConsigns >().TerminateConsign( *this );
        pPionLogHandling_ = resolver_.Find( message.oid_pion_log_traitant );
        if( pPionLogHandling_ )
            pPionLogHandling_->Get< LogisticConsigns >().HandleConsign( *this );
    }

    if( message.m.blesse_mentalPresent )
        bMentalDeceased_ = message.blesse_mental;
    if( message.m.contamine_nbcPresent )
        bContaminated_   = message.contamine_nbc;
    if( message.m.blessurePresent )
        wound_ = E_HumanWound( message.blessure );
    if( message.m.etatPresent )
        nState_ = E_LogSanteTraitementEtat( message.etat );
    if( message.m.diagnostique_effectuePresent )
        diagnosed_ = message.diagnostique_effectue;

    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogMedicalConsign::Display( Displayer_ABC& displayer ) const
{
    displayer.Display( "Consigne :", nID_ )
             .Display( "Pion demandeur :", pion_ )
             .Display( "Pion traitant :", pPionLogHandling_ )
             .Display( "Reac. mental :", bMentalDeceased_ )
             .Display( "Contamin� NBC :", bContaminated_ )
             .Display( "Etat :", nState_ );
    if( diagnosed_ )
        displayer.Display( "Blessure :", wound_ );
    else
        displayer.Display( "Blessure :", "Non diagnostiqu�e" );
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::Draw
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void LogMedicalConsign::Draw( const Point2f& where, const Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    if( ! pPionLogHandling_ || ! tools.ShouldDisplay( "RealTimeLogistic" ) )
        return;

    const Point2f from = pPionLogHandling_->Get< Positions >().GetPosition();
    const Point2f to   = pion_.Get< Positions >().GetPosition();
    if( viewport.Intersect( Rectangle2f( from, to ) ).IsEmpty() )
        return;

    glColor4d( COLOR_PINK );
    switch( nState_ )
    {
    case eLogSanteTraitementEtat_AmbulanceReleveDeplacementAller:
    case eLogSanteTraitementEtat_AmbulanceRamassageDeplacementAller:
        glLineStipple( 1, tools.StipplePattern() );
        break;
    case eLogSanteTraitementEtat_AmbulanceReleveDeplacementRetour:
        glLineStipple( 1, tools.StipplePattern(-1) );
        break;
    default:
        glLineStipple( 1, tools.StipplePattern(0) );
    }
    tools.DrawCurvedArrow( from, to, 0.4f );
}
