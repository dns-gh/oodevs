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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_LogMaintenanceConsign.cpp $
// $Author: Age $
// $Modtime: 6/04/05 10:54 $
// $Revision: 2 $
// $Workfile: MOS_LogMaintenanceConsign.cpp $
//
// *****************************************************************************

#include "MOS_Light2_pch.h"

#include "MOS_LogMaintenanceConsign.h"

#include "MOS_App.h"
#include "MOS_AgentManager.h"
#include "MOS_Agent.h"

// -----------------------------------------------------------------------------
// Name: MOS_LogMaintenanceConsign constructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
MOS_LogMaintenanceConsign::MOS_LogMaintenanceConsign( const ASN1T_MsgLogMaintenanceTraitementEquipementCreation& asn )
    : nID_             ( asn.oid_consigne )
    , pion_            ( *MOS_App::GetApp().GetAgentManager().FindAgent( asn.oid_pion ) )
    , pPionLogHandling_( 0 )
    , nEquipmentTypeID_( asn.type_equipement )
    , nBreakdownTypeID_( asn.type_panne )
    , nState_          ( eFinished )
{
    assert( &pion_ );
    pion_.AddConsign( *this );
}

// -----------------------------------------------------------------------------
// Name: MOS_LogMaintenanceConsign destructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
MOS_LogMaintenanceConsign::~MOS_LogMaintenanceConsign()
{
    pion_.RemoveConsign( *this );
    if( pPionLogHandling_ )
        pPionLogHandling_->TerminateConsign( *this );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MOS_LogMaintenanceConsign::OnReceiveMsgUpdate
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_LogMaintenanceConsign::OnReceiveMsgUpdate( const ASN1T_MsgLogMaintenanceTraitementEquipementUpdate& asn )
{
    assert( pion_.GetID() == asn.oid_pion );
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


    nState_ = (E_State)asn.etat;
}
