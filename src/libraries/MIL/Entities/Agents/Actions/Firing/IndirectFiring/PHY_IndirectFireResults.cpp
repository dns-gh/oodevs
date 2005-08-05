// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/IndirectFiring/PHY_IndirectFireResults.cpp $
// $Author: Nld $
// $Modtime: 12/04/05 17:19 $
// $Revision: 3 $
// $Workfile: PHY_IndirectFireResults.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_IndirectFireResults.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"

MIL_MOSIDManager PHY_IndirectFireResults::idManager_;

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireResults constructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_IndirectFireResults::PHY_IndirectFireResults( const MIL_AgentPion& firer, const MT_Vector2D& vTargetPosition, const PHY_DotationCategory& dotationCategory )
    : PHY_FireResults_ABC()
    , nID_               ( idManager_.GetFreeSimID() )
    , firer_             ( firer )
    , vTargetPosition_   ( vTargetPosition )    
{
    NET_ASN_MsgStartIndirectFire asnMsg;
    asnMsg.GetAsnMsg().oid_tir  = nID_;
    asnMsg.GetAsnMsg().oid_src  = firer_.GetID();
    asnMsg.GetAsnMsg().munition = dotationCategory.GetMosID();
    NET_ASN_Tools::WritePoint( vTargetPosition_, asnMsg.GetAsnMsg().position );
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireResults destructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_IndirectFireResults::~PHY_IndirectFireResults()
{
    NET_ASN_MsgStopIndirectFire asnMsg;
    asnMsg.GetAsnMsg().oid_tir     = nID_;
    asnMsg.GetAsnMsg().resultats.n = results_.size();

    if( !results_.empty() )
    {
        ASN1T_FireResult* pResults = new ASN1T_FireResult[ results_.size() ]; 
        uint i = 0;
        for( CIT_ResultMap itResult = results_.begin(); itResult != results_.end(); ++itResult )
            itResult->second.Serialize( *itResult->first, pResults[ i++ ] );
        asnMsg.GetAsnMsg().resultats.elem = pResults;
    }
    
    asnMsg.Send();

    for( uint n = 0; n < asnMsg.GetAsnMsg().resultats.n; ++n )  
        PHY_AgentFireResult::CleanAfterSerialization( asnMsg.GetAsnMsg().resultats.elem[ n ] );
    if( asnMsg.GetAsnMsg().resultats.n > 0 )
        delete [] asnMsg.GetAsnMsg().resultats.elem;
    idManager_.ReleaseSimID( nID_ );
}

