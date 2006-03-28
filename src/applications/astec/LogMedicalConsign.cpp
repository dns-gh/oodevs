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
// $Archive: /MVW_v10/Build/SDK/Light2/src/LogMedicalConsign.cpp $
// $Author: Age $
// $Modtime: 6/04/05 10:54 $
// $Revision: 2 $
// $Workfile: LogMedicalConsign.cpp $
//
// *****************************************************************************

#include "astec_pch.h"

#include "LogMedicalConsign.h"

#include "App.h"
#include "AgentManager.h"
#include "Agent.h"
#include "LogMedicalConsign_ListView_Item.h"

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign constructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
LogMedicalConsign::LogMedicalConsign( const ASN1T_MsgLogSanteTraitementHumainCreation& asn )
    : nID_             ( asn.oid_consigne )
    , pion_            ( *App::GetApp().GetAgentManager().FindAgent( asn.oid_pion ) )
    , pPionLogHandling_( 0 )
    , wound_           ( (E_Wound)asn.blessure )
    , bMentalDeceased_ ( asn.blesse_mental )
    , bContaminated_   ( asn.contamine_nbc )
    , nState_          ( eFinished )
    , bDiagnosed_      ( false )
{
    assert( &pion_ );
    pion_.AddConsign( *this );
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign destructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
LogMedicalConsign::~LogMedicalConsign()
{
    pion_.RemoveConsign( *this );
    if( pPionLogHandling_ )
        pPionLogHandling_->TerminateConsign( *this );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::OnReceiveMsgUpdate
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void LogMedicalConsign::OnReceiveMsgUpdate( const ASN1T_MsgLogSanteTraitementHumainUpdate& asn )
{
    assert( pion_.GetID() == asn.oid_pion );
    if( asn.m.oid_pion_log_traitantPresent )
    {
        if( pPionLogHandling_ )
            pPionLogHandling_->TerminateConsign( *this );
        if( asn.oid_pion_log_traitant != 0 )
        {
            pPionLogHandling_ = App::GetApp().GetAgentManager().FindAgent( asn.oid_pion_log_traitant );
            assert( pPionLogHandling_ );
            pPionLogHandling_->HandleConsign( *this );
        }        
        else 
            pPionLogHandling_ = 0;
    }

    if( asn.m.blesse_mentalPresent )
        bMentalDeceased_ = asn.blesse_mental;
    if( asn.m.contamine_nbcPresent )
        bContaminated_   = asn.contamine_nbc;
    if( asn.m.blessurePresent )
        wound_ = (E_Wound)asn.blessure;

    if( asn.m.etatPresent )
        nState_ = (E_State)asn.etat;
    if( asn.m.diagnostique_effectuePresent )
        bDiagnosed_ = asn.diagnostique_effectue;
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::GetWoundAsString
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
std::string LogMedicalConsign::GetWoundAsString() const
{
    switch( wound_ ) 
    {
        case eNotWounded: return "Non blessé";
        case eKilled    : return "Mort";
        case eWoundedU1 : return "U1";
        case eWoundedU2 : return "U2";
        case eWoundedU3 : return "U3";
        case eWoundedUE : return "UE";
        default:
            assert( false );
            return "Fussoir";
    }
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::GetStateString
// Created: HME 2006-01-30
// -----------------------------------------------------------------------------
std::string LogMedicalConsign::GetStateString() const
{
    std::string strState;

    switch( nState_ )
    {        
        case eWaitingForEvacuation               : strState = std::string( "Relève - En attente d'une ambulance" )                      ; break;
        case eEvacuationGoingTo                  : strState = std::string( "Relève - Ambulance en route" )                              ; break;
        case eEvacuationLoading                  : strState = std::string( "Relève - Embarquement dans ambulance" )                     ; break;
        case eEvacuationWaitingForFullLoading    : strState = std::string( "Relève - En attente chargement complet de l'ambulance" )    ; break;
        case eEvacuationGoingFrom                : strState = std::string( "Relève - Ambulance en retour" )                             ; break;
        case eEvacuationUnloading                : strState = std::string( "Relève - Débarquement de l'ambulance" )                     ; break;
        case eWaitingForDiagnostic               : strState = std::string( "En attente de diagnostique" )                               ; break;
        case eDiagnosing                         : strState = std::string( "Diagnostique en cours" )                                    ; break;
        case eSearchingForSortingArea            : strState = std::string( "Recherche d'un secteur de tri" )                            ; break;
        case eWaitingForSorting                  : strState = std::string( "En attente de tri" )                                        ; break;
        case eSorting                            : strState = std::string( "Tri en cours" )                                             ; break;
        case eSearchingForHealingArea            : strState = std::string( "Recherche d'un secteur de soin" )                           ; break;
        case eWaitingForHealing                  : strState = std::string( "En attente de soins" )                                      ; break;
        case eHealing                            : strState = std::string( "Soins en cours" )                                           ; break;
        case eWaitingForCollection               : strState = std::string( "Ramassage - En attente d'une ambulance" )                   ; break;
        case eCollectionLoading                  : strState = std::string( "Ramassage - Embarquement dans ambulance" )                  ; break;
        case eCollectionWaitingForFullLoading    : strState = std::string( "Ramassage - En attente chargement complet de l'ambulance" ) ; break;
        case eCollectionGoingTo                  : strState = std::string( "Ramassage - Ambulance en route" )                           ; break;
        case eCollectionUnloading                : strState = std::string( "Ramassage - Débarquement de l'ambulance" )                  ; break;
        case eFinished                           : strState = std::string( "Terminé" )                                                  ; break;
        default:
            assert( false );
    }

    return strState;
}
