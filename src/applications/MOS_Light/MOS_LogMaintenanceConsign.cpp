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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LogMaintenanceConsign.cpp $
// $Author: Nld $
// $Modtime: 5/01/05 15:57 $
// $Revision: 1 $
// $Workfile: MOS_LogMaintenanceConsign.cpp $
//
// *****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_LogMaintenanceConsign.h"

#include "MOS_App.h"
#include "MOS_AgentManager.h"
#include "MOS_Agent.h"
#include "MOS_LogMaintenanceConsign_ListView_Item.h"

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
{
    assert( &pion_ );
    pion_.AddMaintenanceConsign( *this );
}

// -----------------------------------------------------------------------------
// Name: MOS_LogMaintenanceConsign destructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
MOS_LogMaintenanceConsign::~MOS_LogMaintenanceConsign()
{
    pion_.RemoveMaintenanceConsign( *this );
    if( pPionLogHandling_ )
        pPionLogHandling_->RemoveHandledMaintenanceConsign( *this );

    assert( items_.empty() );
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
    assert( pion_.GetAgentID() == asn.oid_pion );
    if( pPionLogHandling_ )
        pPionLogHandling_->RemoveHandledMaintenanceConsign( *this );
    if( asn.oid_pion_log_traitant != 0 )
    {
        pPionLogHandling_ = MOS_App::GetApp().GetAgentManager().FindAgent( asn.oid_pion_log_traitant );
        assert( pPionLogHandling_ );
        pPionLogHandling_->AddHandledMaintenanceConsign( *this );
    }        
    else 
        pPionLogHandling_ = 0;


    nState_ = (E_State)asn.etat;


    for( CIT_ListViewItemSet it = items_.begin(); it != items_.end(); ++it )
        (**it).Update();
}

// -----------------------------------------------------------------------------
// Name: MOS_LogMaintenanceConsign::DeleteAllItems
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_LogMaintenanceConsign::DeleteAllItems()
{
    while( !items_.empty() )
        delete *items_.begin();
}
