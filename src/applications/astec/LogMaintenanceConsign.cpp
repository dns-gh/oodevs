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

#include "App.h"
#include "AgentManager.h"
#include "Agent.h"

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign constructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
LogMaintenanceConsign::LogMaintenanceConsign( const ASN1T_MsgLogMaintenanceTraitementEquipementCreation& asn )
    : nID_             ( asn.oid_consigne )
    , pion_            ( *App::GetApp().GetAgentManager().FindAgent( asn.oid_pion ) )
    , pPionLogHandling_( 0 )
    , nEquipmentTypeID_( asn.type_equipement )
    , nBreakdownTypeID_( asn.type_panne )
    , nState_          ( eFinished )
{
    assert( &pion_ );
    pion_.AddConsign( *this );
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign destructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
LogMaintenanceConsign::~LogMaintenanceConsign()
{
    pion_.RemoveConsign( *this );
    if( pPionLogHandling_ )
        pPionLogHandling_->TerminateConsign( *this );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::OnReceiveMsgUpdate
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void LogMaintenanceConsign::OnReceiveMsgUpdate( const ASN1T_MsgLogMaintenanceTraitementEquipementUpdate& asn )
{
    assert( pion_.GetID() == asn.oid_pion );
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


    nState_ = (E_State)asn.etat;
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::GetStateString
// Created: HME 2006-01-30
// -----------------------------------------------------------------------------
std::string LogMaintenanceConsign::GetStateString() const
{
    std::string strState;

    switch( nState_ )
    {        
    case eGoingFrom                   : strState = std::string( "En déplacement vers la chaine" ); break;         
        case eWaitingForCarrier       : strState = std::string( "En attente d'un remorqueur" ); break;         
        case eCarrierGoingTo          : strState = std::string( "Remorqueur en route" ); break;         
        case eCarrierLoading          : strState = std::string( "Remorqueur en cours de chargement" ); break;         
        case eCarrierGoingFrom        : strState = std::string( "Remorqueur en retour" ); break;         
        case eCarrierUnloading        : strState = std::string( "Remorqueur en cours de déchargement" ); break;         
        case eDiagnosing              : strState = std::string( "Diagnostique en cours" ); break;         
        case eSearchingForUpperLevel  : strState = std::string( "En attente de prise en charge par le niveau supérieur" ); break;         
        case eWaitingForParts         : strState = std::string( "En attente de pièces" ); break;         
        case eWaitingForRepairer      : strState = std::string( "En attente d'un réparateur" ); break;         
        case eRepairing               : strState = std::string( "En cours de réparation" ); break;         
        case eWaitingForGoingBackToWar: strState = std::string( "En attente de retour" ); break;         
        case eGoingBackToWar          : strState = std::string( "Retour en cours" ); break;         
        case eFinished                : strState = std::string( "Terminé" ); break;         
        default:
            assert( false );
    }

    return strState;
}
