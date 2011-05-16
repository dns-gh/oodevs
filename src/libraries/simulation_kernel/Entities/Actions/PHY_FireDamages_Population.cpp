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
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: PHY_FireDamages_Population constructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_FireDamages_Population::PHY_FireDamages_Population()
    : nNbrKilledHumans_ ( 0 )
    , nNbrWoundedHumans_( 0 )
    , nNbrScatteredHumans_( 0 )
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
void PHY_FireDamages_Population::Serialize( const MIL_Population& target, sword::CrowdFireDamages& msg ) const
{
    msg.mutable_target()->set_id( target.GetID() );
    DoSerializeDamages( msg );
}

// -----------------------------------------------------------------------------
// Name: PHY_FireDamages_Population::SerializeDamages
// Created: JSR 2011-05-13
// -----------------------------------------------------------------------------
void PHY_FireDamages_Population::SerializeDamages( sword::CrowdDamagedByUnitFire& msg ) const
{
    DoSerializeDamages( msg );
}

// -----------------------------------------------------------------------------
// Name: PHY_FireDamages_Population::DoSerializeDamages
// Created: JSR 2011-05-13
// -----------------------------------------------------------------------------
template< typename T >
void PHY_FireDamages_Population::DoSerializeDamages( T& msg ) const
{
    msg.set_dead( nNbrKilledHumans_ );
    msg.set_wounded( nNbrWoundedHumans_ );
    msg.set_scattered( nNbrScatteredHumans_ );
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
// Name: PHY_FireDamages_Population::NotifyHumansWounded
// Created: LDC 2011-05-09
// -----------------------------------------------------------------------------
void PHY_FireDamages_Population::NotifyHumansWounded( unsigned int nNbrHumans )
{
    nNbrWoundedHumans_ += nNbrHumans;
}

// -----------------------------------------------------------------------------
// Name: PHY_FireDamages_Population::NotifyHumansScattered
// Created: LDC 2011-05-09
// -----------------------------------------------------------------------------
void PHY_FireDamages_Population::NotifyHumansScattered( unsigned int nNbrHumans )
{
    nNbrScatteredHumans_ += nNbrHumans;
}

// -----------------------------------------------------------------------------
// Name: PHY_FireDamages_Population::GetNbrKilledHumans
// Created: NLD 2006-04-26
// -----------------------------------------------------------------------------
unsigned int PHY_FireDamages_Population::GetNbrKilledHumans() const
{
    return nNbrKilledHumans_;
}

// -----------------------------------------------------------------------------
// Name: PHY_FireDamages_Population::GetNbrWoundedHumans
// Created: LDC 2011-05-09
// -----------------------------------------------------------------------------
unsigned int PHY_FireDamages_Population::GetNbrWoundedHumans() const
{
    return nNbrWoundedHumans_;
}

// -----------------------------------------------------------------------------
// Name: PHY_FireDamages_Population::GetNbrScatteredHumans
// Created: LDC 2011-05-09
// -----------------------------------------------------------------------------
unsigned int PHY_FireDamages_Population::GetNbrScatteredHumans() const
{
    return nNbrScatteredHumans_;
}
