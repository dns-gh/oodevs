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
#include "clients_kernel/Agent_ABC.h"
#include "LogisticConsigns.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/Positions.h"
#include "Tools.h"

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogMedicalConsign::LogMedicalConsign( Controller& controller, const tools::Resolver_ABC< Agent_ABC >& resolver, const ASN1T_MsgLogMedicalHandlingCreation& message )
    : controller_      ( controller )
    , resolver_        ( resolver )
    , nID_             ( message.oid_consigne )
    , pion_            ( resolver_.Get( message.oid_pion ) )
    , pPionLogHandling_( 0 )
    , wound_           ( E_HumanWound( message.blessure ) )
    , bMentalDeceased_ ( message.blesse_mental != 0 )
    , bContaminated_   ( message.contamine_nbc != 0 )
    , diagnosed_       ( false )
    , nState_          ( eLogMedicalHandlingStatus_Termine )
{
    pion_.Get< LogMedicalConsigns >().AddConsign( *this );
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogMedicalConsign::~LogMedicalConsign()
{
    pion_.Get< LogMedicalConsigns >().RemoveConsign( *this );
    if( pPionLogHandling_ )
        pPionLogHandling_->Get< LogMedicalConsigns >().TerminateConsign( *this );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::Update
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void LogMedicalConsign::Update( const ASN1T_MsgLogMedicalHandlingUpdate& message )
{
    if( message.m.oid_pion_log_traitantPresent )
    {
        if( pPionLogHandling_ )
            pPionLogHandling_->Get< LogMedicalConsigns >().TerminateConsign( *this );
        pPionLogHandling_ = resolver_.Find( message.oid_pion_log_traitant );
        if( pPionLogHandling_ )
            pPionLogHandling_->Get< LogMedicalConsigns >().HandleConsign( *this );
    }

    if( message.m.blesse_mentalPresent )
        bMentalDeceased_ = message.blesse_mental != 0;
    if( message.m.contamine_nbcPresent )
        bContaminated_   = message.contamine_nbc != 0;
    if( message.m.blessurePresent )
        wound_ = E_HumanWound( message.blessure );
    if( message.m.etatPresent )
        nState_ = E_LogMedicalHandlingStatus( message.etat );
    if( message.m.diagnostique_effectuePresent )
        diagnosed_ = message.diagnostique_effectue != 0;

    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogMedicalConsign::Display( Displayer_ABC& displayer, Displayer_ABC& itemDisplayer ) const
{
    displayer.Display( pion_ ).Display( nState_ );

    itemDisplayer.Display( tools::translate( "Logistic", "Instruction:" ), nID_ )
                 .Display( tools::translate( "Logistic", "Consumer:" ), pion_ )
                 .Display( tools::translate( "Logistic", "Handler:" ), pPionLogHandling_ )
                 .Display( tools::translate( "Logistic", "Mentally injured:" ), bMentalDeceased_ )
                 .Display( tools::translate( "Logistic", "NBC contaminated:" ), bContaminated_ )
                 .Display( tools::translate( "Logistic", "State:" ), nState_ );
    if( diagnosed_ )
        itemDisplayer.Display( tools::translate( "Logistic", "Injury:" ), wound_ );
    else
        itemDisplayer.Display( tools::translate( "Logistic", "Injury:" ), tools::translate( "Logistic", "Not diagnosed" ) );
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::Draw
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void LogMedicalConsign::Draw( const Point2f& , const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( ! pPionLogHandling_ || ! tools.ShouldDisplay( "RealTimeLogistic" ) )
        return;

    const Point2f from = pPionLogHandling_->Get< Positions >().GetPosition();
    const Point2f to   = pion_.Get< Positions >().GetPosition();
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
