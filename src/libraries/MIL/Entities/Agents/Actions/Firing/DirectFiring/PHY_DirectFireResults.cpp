// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_DirectFireResults.cpp $
// $Author: Nld $
// $Modtime: 12/04/05 17:19 $
// $Revision: 3 $
// $Workfile: PHY_DirectFireResults.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_DirectFireResults.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Network/NET_ASN_Messages.h"

MIL_MOSIDManager PHY_DirectFireResults::idManager_;

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireResults constructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_DirectFireResults::PHY_DirectFireResults( const MIL_AgentPion& firer, const MIL_Agent_ABC& target )
    : PHY_FireResults_ABC()
    , nID_              ( idManager_.GetFreeSimID() )
    , firer_            ( firer )
    , target_           ( target )
{
    NET_ASN_MsgStartDirectFire asnMsg;
    asnMsg.GetAsnMsg().oid_tir = nID_;
    asnMsg.GetAsnMsg().oid_src = firer_ .GetID();
    asnMsg.GetAsnMsg().oid_dst = target_.GetID();
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireResults destructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_DirectFireResults::~PHY_DirectFireResults()
{
    NET_ASN_MsgStopDirectFire asnMsg;
    asnMsg.GetAsnMsg().oid_tir = nID_;

    const PHY_AgentFireResult& fireResult = GetAgentFireResult( target_ );
    assert( results_.size() == 1 );
   
    fireResult.Serialize( target_, asnMsg.GetAsnMsg().resultat );
    asnMsg.Send();
    fireResult.CleanAfterSerialization( asnMsg.GetAsnMsg().resultat );
    idManager_.ReleaseSimID( nID_ );
}

