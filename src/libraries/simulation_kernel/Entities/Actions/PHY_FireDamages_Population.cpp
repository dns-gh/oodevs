// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/PHY_FireDamages_Population.cpp $
// $Author: Nld $
// $Modtime: 12/04/05 17:19 $
// $Revision: 6 $
// $Workfile: PHY_FireDamages_Population.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_FireDamages_Population.h"
#include "Entities/Populations/MIL_Population.h"
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: PHY_FireDamages_Population constructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_FireDamages_Population::PHY_FireDamages_Population()
    : nNbrKilledHumans_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_FireDamages_Population destructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_FireDamages_Population::~PHY_FireDamages_Population()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: PHY_FireDamages_Population::Serialize
// Created: JVT 04-03-29
//-----------------------------------------------------------------------------
void PHY_FireDamages_Population::Serialize( const MIL_Population& target, MsgsSimToClient::MsgPopulationFireDamages& asn ) const
{
    asn.set_target( target.GetID() );
    asn.set_dead_nbr( nNbrKilledHumans_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_FireDamages_Population::CleanAfterSerialization
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void PHY_FireDamages_Population::CleanAfterSerialization( MsgsSimToClient::MsgPopulationFireDamages& /*asn*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_FireDamages_Population::NotifyHumansKilled
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
void PHY_FireDamages_Population::NotifyHumansKilled( unsigned int nNbrHumans )
{
    nNbrKilledHumans_ += nNbrHumans;
}

// -----------------------------------------------------------------------------
// Name: PHY_FireDamages_Population::GetNbrKilledHumans
// Created: NLD 2006-04-26
// -----------------------------------------------------------------------------
unsigned int PHY_FireDamages_Population::GetNbrKilledHumans() const
{
    return nNbrKilledHumans_;
}
