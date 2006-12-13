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

#include "simulation_kernel_pch.h"

#include "PHY_ObjectExplosionFireResult.h"

#include "Entities/Objects/MIL_RealObject_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Populations/MIL_Population.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_ObjectExplosionFireResult constructor
// Created: NLD 2004-10-27
// -----------------------------------------------------------------------------
PHY_ObjectExplosionFireResult::PHY_ObjectExplosionFireResult( const MIL_RealObject_ABC& object )
    : PHY_FireResults_ABC()
    , object_            ( object )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_ObjectExplosionFireResult destructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_ObjectExplosionFireResult::~PHY_ObjectExplosionFireResult()
{
    NET_ASN_MsgExplosion asnMsg;
    asnMsg().oid_objet   = object_ .GetID();

    Serialize( asnMsg().degats_pions       );
    Serialize( asnMsg().degats_populations );
    
    asnMsg.Send();

    CleanAfterSerialization( asnMsg().degats_pions       );
    CleanAfterSerialization( asnMsg().degats_populations );

    // $$$ Merde pour VABF Popu
    static MT_Random randomGenerator;
    const T_PopulationDamagesMap& populationDamages = GetPopulationDamages();
    for( CIT_PopulationDamagesMap it = populationDamages.begin(); it != populationDamages.end(); ++it )
    {
        const MIL_Population&               population = *it->first;
        const PHY_FireDamages_Population&   damages    =  it->second;

        MIL_Report::PostEvent( population, MIL_Report::eReport_MineExplosionOnPopulation, damages.GetNbrKilledHumans(), damages.GetNbrKilledHumans() * randomGenerator.rand_ii( 0.6, 0.75 ) );
    }
}

