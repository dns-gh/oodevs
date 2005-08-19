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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_LogMedicalConsign.cpp $
// $Author: Age $
// $Modtime: 6/04/05 10:54 $
// $Revision: 2 $
// $Workfile: MOS_LogMedicalConsign.cpp $
//
// *****************************************************************************

#include "MOS_Light2_pch.h"

#include "MOS_LogMedicalConsign.h"

#include "MOS_App.h"
#include "MOS_AgentManager.h"
#include "MOS_Agent.h"
#include "MOS_LogMedicalConsign_ListView_Item.h"

// -----------------------------------------------------------------------------
// Name: MOS_LogMedicalConsign constructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
MOS_LogMedicalConsign::MOS_LogMedicalConsign( const ASN1T_MsgLogSanteTraitementHumainCreation& asn )
    : nID_             ( asn.oid_consigne )
    , pion_            ( *MOS_App::GetApp().GetAgentManager().FindAgent( asn.oid_pion ) )
    , pPionLogHandling_( 0 )
    , wound_           ( (E_Wound)asn.blessure )
    , bMentalDeceased_ ( asn.blesse_mental )
    , bContaminated_   ( asn.contamine_nbc )
    , nState_          ( eFinished )
{
    assert( &pion_ );
    pion_.AddConsign( *this );
}

// -----------------------------------------------------------------------------
// Name: MOS_LogMedicalConsign destructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
MOS_LogMedicalConsign::~MOS_LogMedicalConsign()
{
    pion_.RemoveConsign( *this );
    if( pPionLogHandling_ )
        pPionLogHandling_->TerminateConsign( *this );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MOS_LogMedicalConsign::OnReceiveMsgUpdate
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_LogMedicalConsign::OnReceiveMsgUpdate( const ASN1T_MsgLogSanteTraitementHumainUpdate& asn )
{
    assert( pion_.GetAgentID() == asn.oid_pion );
    if( asn.m.oid_pion_log_traitantPresent )
    {
        if( pPionLogHandling_ )
            pPionLogHandling_->TerminateConsign( *this );
        if( asn.oid_pion_log_traitant != 0 )
        {
            pPionLogHandling_ = MOS_App::GetApp().GetAgentManager().FindAgent( asn.oid_pion_log_traitant );
            assert( pPionLogHandling_ );
            pPionLogHandling_->HandleConsign( *this );
        }        
        else 
            pPionLogHandling_ = 0;
    }

    if ( asn.m.blesse_mentalPresent )
        bMentalDeceased_ = asn.blesse_mental;
    if ( asn.m.contamine_nbcPresent )
        bContaminated_   = asn.contamine_nbc;
    if ( asn.m.blessurePresent )
        wound_ = (E_Wound)asn.blessure;

    if( asn.m.etatPresent )
        nState_ = (E_State)asn.etat;
}

// -----------------------------------------------------------------------------
// Name: MOS_LogMedicalConsign::GetWoundAsString
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
std::string MOS_LogMedicalConsign::GetWoundAsString() const
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
