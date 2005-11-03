// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/PopulationFiring/PHY_PopulationFireResults.cpp $
// $Author: Nld $
// $Modtime: 12/04/05 17:19 $
// $Revision: 3 $
// $Workfile: PHY_PopulationFireResults.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_PopulationFireResults.h"
#include "Entities/Populations/MIL_Population.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"

MIL_MOSIDManager PHY_PopulationFireResults::idManager_;

// -----------------------------------------------------------------------------
// Name: PHY_PopulationFireResults constructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_PopulationFireResults::PHY_PopulationFireResults( const MIL_Population& firer )
    : PHY_FireResults_ABC()
    , firer_             ( firer )
    , nID_               ( idManager_.GetFreeSimID() )
{
    NET_ASN_MsgStartPopulationFire asnMsg;
    asnMsg.GetAsnMsg().oid_tir  = nID_;
    asnMsg.GetAsnMsg().oid_src  = firer_.GetID();
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: PHY_PopulationFireResults destructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_PopulationFireResults::~PHY_PopulationFireResults()
{
    NET_ASN_MsgStopPopulationFire asnMsg;
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

