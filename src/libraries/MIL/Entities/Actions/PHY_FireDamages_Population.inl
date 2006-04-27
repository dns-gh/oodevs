// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Firing/PHY_FireDamages_Population.inl $
// $Author: Nld $
// $Modtime: 19/10/04 13:54 $
// $Revision: 1 $
// $Workfile: PHY_FireDamages_Population.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_FireDamages_Population::NotifyHumansKilled
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
inline
void PHY_FireDamages_Population::NotifyHumansKilled( uint nNbrHumans )
{
    nNbrKilledHumans_ += nNbrHumans;
}

// -----------------------------------------------------------------------------
// Name: PHY_FireDamages_Population::GetNbrKilledHumans
// Created: NLD 2006-04-26
// -----------------------------------------------------------------------------
inline
uint PHY_FireDamages_Population::GetNbrKilledHumans() const
{
    return nNbrKilledHumans_;
}
