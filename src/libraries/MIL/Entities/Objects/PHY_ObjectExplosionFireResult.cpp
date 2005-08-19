// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/PHY_ObjectExplosionFireResult.cpp $
// $Author: Nld $
// $Modtime: 12/04/05 17:19 $
// $Revision: 3 $
// $Workfile: PHY_ObjectExplosionFireResult.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_ObjectExplosionFireResult.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_ObjectExplosionFireResult constructor
// Created: NLD 2004-10-27
// -----------------------------------------------------------------------------
PHY_ObjectExplosionFireResult::PHY_ObjectExplosionFireResult( const MIL_RealObject_ABC& object )
    : PHY_FireResults_ABC()
    , object_           ( object )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_ObjectExplosionFireResult destructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_ObjectExplosionFireResult::~PHY_ObjectExplosionFireResult()
{
    NET_ASN_MsgExplosion asnMsg;
    asnMsg.GetAsnMsg().oid_objet   = object_ .GetID();
    asnMsg.GetAsnMsg().resultats.n = results_.size ();

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
}

