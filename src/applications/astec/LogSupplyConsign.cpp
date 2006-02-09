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
// $Archive: /MVW_v10/Build/SDK/Light2/src/LogSupplyConsign.cpp $
// $Author: Age $
// $Modtime: 1/04/05 11:14 $
// $Revision: 1 $
// $Workfile: LogSupplyConsign.cpp $
//
// *****************************************************************************

#include "astec_pch.h"

#include "LogSupplyConsign.h"

#include "App.h"
#include "AgentManager.h"
#include "Agent.h"
#include "LogSupplyConsign_ListView_Item.h"

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign constructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
LogSupplyConsign::LogSupplyConsign( const ASN1T_MsgLogRavitaillementTraitementCreation& asn )
    : nID_              ( asn.oid_consigne )
    , pion_             ( *App::GetApp().GetAgentManager().FindAgent( asn.oid_automate ) )
    , pAutomateLogHandling_ ( 0 )
    , pPionLogConvoying_( 0 )
    , pAutomateLogProvidingConvoyResources_( 0 )
    , nState_           ( eFinished )
{
    assert( &pion_ );
    pion_.AddConsign( *this );

    for( uint i = 0; i < asn.dotations.n; ++i )
    {
        sDotationData& dotationData = dotations_[ asn.dotations.elem[i].ressource_id ];
        dotationData.nNbrRequested_ = asn.dotations.elem[i].quantite_demandee;
        dotationData.nNbrReserved_  = asn.dotations.elem[i].quantite_accordee;
        dotationData.nNbrConvoyed_  = asn.dotations.elem[i].quantite_en_transit;
    }
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign destructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
LogSupplyConsign::~LogSupplyConsign()
{
    pion_.RemoveConsign( *this );
    if( pAutomateLogHandling_ )
        pAutomateLogHandling_->TerminateConsign( *this );
    if( pPionLogConvoying_ )
        pPionLogConvoying_->TerminateConsign( *this );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::OnReceiveMsgUpdate
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void LogSupplyConsign::OnReceiveMsgUpdate( const ASN1T_MsgLogRavitaillementTraitementUpdate& asn )
{
    assert( pion_.GetID() == asn.oid_automate );

    if( asn.m.oid_automate_log_traitantPresent )
    {
        if( pAutomateLogHandling_ )
            pAutomateLogHandling_->TerminateConsign( *this );
        if( asn.oid_automate_log_traitant != 0 )
        {
            pAutomateLogHandling_ = App::GetApp().GetAgentManager().FindAgent( asn.oid_automate_log_traitant );
            assert( pAutomateLogHandling_ );
            pAutomateLogHandling_->HandleConsign( *this );
        }        
        else 
            pAutomateLogHandling_ = 0;
    }
    
    if( asn.m.oid_pion_convoyantPresent )
    {
        if( pPionLogConvoying_ )
            pPionLogConvoying_->TerminateConsign( *this );
        if( asn.oid_pion_convoyant != 0 )
        {
            pPionLogConvoying_ = App::GetApp().GetAgentManager().FindAgent( asn.oid_pion_convoyant );
            assert( pPionLogConvoying_ );
            pPionLogConvoying_->HandleConsign( *this );
        }        
        else 
            pPionLogConvoying_ = 0;
    }

    if( asn.m.oid_automate_log_fournissant_moyens_convoiPresent )
    {
        if( asn.oid_automate_log_fournissant_moyens_convoi != 0 )
        {
            pAutomateLogProvidingConvoyResources_ = App::GetApp().GetAgentManager().FindAgent( asn.oid_automate_log_fournissant_moyens_convoi );
            assert( pAutomateLogProvidingConvoyResources_ );
        }
        else
            pAutomateLogProvidingConvoyResources_ = 0;
    }

    if( asn.m.etatPresent )
        nState_ = (E_State)asn.etat;

    if( asn.m.dotationsPresent )
    {
        for( uint i = 0; i < asn.dotations.n; ++i )
        {
            sDotationData& dotationData = dotations_[ asn.dotations.elem[i].ressource_id ];
            dotationData.nNbrRequested_ = asn.dotations.elem[i].quantite_demandee;
            dotationData.nNbrReserved_  = asn.dotations.elem[i].quantite_accordee;
            dotationData.nNbrConvoyed_  = asn.dotations.elem[i].quantite_en_transit;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::GetStateString
// Created: HME 2006-01-30
// -----------------------------------------------------------------------------
std::string LogSupplyConsign::GetStateString() const
{
    std::string strState;

    switch( nState_ )
    {     
    case eConvoyWaitingForCommander         : strState = std::string( "Convoi en attente d'un chef de convoi" ); break;
        case eConvoyWaitingForTransporters  : strState = std::string( "Convoi en attente de camions" ); break;
        case eConvoyForming                 : strState = std::string( "Convoi en cours de constitution" ); break;
        
        case eConvoyGoingToLoadingPoint     : strState = std::string( "Convoi en déplacement vers point de chargement" ); break;
        case eConvoyLoading                 : strState = std::string( "Convoi en cours de chargement" ); break;
        
        case eConvoyGoingToUnloadingPoint   : strState = std::string( "Convoi en déplacement vers point de déchargement" ); break;
        case eConvoyUnloading               : strState = std::string( "Convoi en cours de déchargement" ); break;
        
        case eConvoyGoingBackToFormingPoint : strState = std::string( "Convoi en retour" ); break;
        case eFinished                      : strState = std::string( "Terminé" ); break;
        default:
            assert( false );
    }

    return strState;
}
