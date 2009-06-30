// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/PHY_FireResults_ABC.cpp $
// $Author: Nld $
// $Modtime: 12/04/05 17:19 $
// $Revision: 3 $
// $Workfile: PHY_FireResults_ABC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_FireResults_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_ABC constructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_FireResults_ABC::PHY_FireResults_ABC()
    : nNbrRefs_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_ABC destructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_FireResults_ABC::~PHY_FireResults_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_ABC::Serialize
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
void PHY_FireResults_ABC::Serialize( ASN1T_UnitsFireDamages& asn ) const
{
    asn.n = agentsDamages_.size();
    if( !agentsDamages_.empty() )
    {
        ASN1T_UnitFireDamages* pDamages = new ASN1T_UnitFireDamages[ agentsDamages_.size() ]; 
        uint i = 0;
        for( CIT_AgentDamagesMap it = agentsDamages_.begin(); it != agentsDamages_.end(); ++it )
            it->second.Serialize( *it->first, pDamages[ i++ ] );
        asn.elem = pDamages;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_ABC::CleanAfterSerialization
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
void PHY_FireResults_ABC::CleanAfterSerialization( ASN1T_UnitsFireDamages& asn )
{
    for( uint n = 0; n < asn.n; ++n )  
        PHY_FireDamages_Agent::CleanAfterSerialization( asn.elem[ n ] );
    if( asn.n > 0 )
        delete [] asn.elem;
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_ABC::Serialize
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
void PHY_FireResults_ABC::Serialize( ASN1T_PopulationsFireDamages& asn ) const
{
    asn.n = populationsDamages_.size();
    if( !populationsDamages_.empty() )
    {
        ASN1T_PopulationFireDamages* pDamages = new ASN1T_PopulationFireDamages[ populationsDamages_.size() ]; 
        uint i = 0;
        for( CIT_PopulationDamagesMap it = populationsDamages_.begin(); it != populationsDamages_.end(); ++it )
            it->second.Serialize( *it->first, pDamages[ i++ ] );
        asn.elem = pDamages;
    }
}
    
// -----------------------------------------------------------------------------
// Name: PHY_FireResults_ABC::CleanAfterSerialization
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
void PHY_FireResults_ABC::CleanAfterSerialization( ASN1T_PopulationsFireDamages& asn )
{
    for( uint n = 0; n < asn.n; ++n )  
        PHY_FireDamages_Population::CleanAfterSerialization( asn.elem[ n ] );
    if( asn.n > 0 )
        delete [] asn.elem;
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_ABC::GetDamages
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
PHY_FireDamages_Agent& PHY_FireResults_ABC::GetDamages( const MIL_Agent_ABC& target )
{
    return agentsDamages_[ &target ];
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_ABC::GetDamages
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
PHY_FireDamages_Population& PHY_FireResults_ABC::GetDamages( const MIL_Population& target )
{
    return populationsDamages_[ &target ];
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_ABC::IncRef
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void PHY_FireResults_ABC::IncRef()
{
    ++nNbrRefs_;
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_ABC::DecRef
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void PHY_FireResults_ABC::DecRef()
{
    assert( nNbrRefs_ > 0 );
    --nNbrRefs_;
    if( nNbrRefs_ == 0 )
        delete this;
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_ABC::GetPopulationDamages
// Created: NLD 2006-04-26
// -----------------------------------------------------------------------------
const PHY_FireResults_ABC::T_PopulationDamagesMap& PHY_FireResults_ABC::GetPopulationDamages() const
{
    return populationsDamages_;
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_ABC::GetPopulationDamages
// Created: NLD 2006-04-26
// -----------------------------------------------------------------------------
void PHY_FireResults_ABC::Hit()
{
    // NOTHING
}

