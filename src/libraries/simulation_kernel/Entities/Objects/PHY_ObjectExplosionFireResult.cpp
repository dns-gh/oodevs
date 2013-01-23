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
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"

// -----------------------------------------------------------------------------
// Name: PHY_ObjectExplosionFireResult constructor
// Created: NLD 2004-10-27
// -----------------------------------------------------------------------------
PHY_ObjectExplosionFireResult::PHY_ObjectExplosionFireResult( const MIL_Object_ABC& object )
    : PHY_FireResults_ABC()
    , object_ ( object )
    , hits_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ObjectExplosionFireResult destructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_ObjectExplosionFireResult::~PHY_ObjectExplosionFireResult()
{
    {
        client::Explosion asnMsg;
        asnMsg().mutable_object()->set_id( object_.GetID() );
        Serialize( *asnMsg().mutable_units_damages ()      );
        Serialize( *asnMsg().mutable_crowds_damages() );
        asnMsg.Send( NET_Publisher_ABC::Publisher() );
    }

    const T_PopulationDamagesMap& populationDamages = GetPopulationDamages();
    for( auto it = populationDamages.begin(); it != populationDamages.end(); ++it )
    {
        const MIL_Population&               population = *it->first;
        const PHY_FireDamages_Population&   damages    =  it->second;
        int dead = damages.GetNbrKilledHumans();
        int wounded = damages.GetNbrWoundedHumans();
        int scattered = damages.GetNbrScatteredHumans();

        if( dead + wounded + scattered > 0 )
            MIL_Report::PostEvent( population, MIL_Report::eRC_PopulationVictimeExplosionMines, dead, wounded );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ObjectExplosionFireResult::Hit
// Created: JCR 2008-08-08
// -----------------------------------------------------------------------------
void PHY_ObjectExplosionFireResult::Hit( unsigned int hits)
{
    hits_ += hits;
}

// -----------------------------------------------------------------------------
// Name: PHY_ObjectExplosionFireResult::GetHits
// Created: JCR 2008-08-08
// -----------------------------------------------------------------------------
unsigned int PHY_ObjectExplosionFireResult::GetHits() const
{
    return hits_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ObjectExplosionFireResult::GetSourceName
// Created: JSR 2013-01-23
// -----------------------------------------------------------------------------
const std::string& PHY_ObjectExplosionFireResult::GetSourceName() const
{
    return object_.GetType().GetRealName();
}