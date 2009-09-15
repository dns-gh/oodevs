// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Surrender/PHY_RoleHLA_Surrender.cpp $
// $Author: Nld $
// $Modtime: 13/05/05 10:14 $
// $Revision: 2 $
// $Workfile: PHY_RoleHLA_Surrender.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleHLA_Surrender.h"
#include "Network/NET_ASN_Messages.h"

BOOST_CLASS_EXPORT_GUID( PHY_RoleHLA_Surrender, "PHY_RoleHLA_Surrender" )

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Surrender constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RoleHLA_Surrender::PHY_RoleHLA_Surrender()
    : bPrisoner_                 ( false )
{
    // NOTHING
}
  
// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Surrender destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RoleHLA_Surrender::~PHY_RoleHLA_Surrender()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Surrender::Capture
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
bool PHY_RoleHLA_Surrender::Capture( const MIL_AgentPion& /*pionTakingPrisoner*/ )
{
    // $$$$ _RC_ NLD 2005-03-10: 
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Surrender::Release
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
bool PHY_RoleHLA_Surrender::Release()
{
    // $$$$ _RC_ NLD 2005-03-10: 
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Surrender::Imprison
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
bool PHY_RoleHLA_Surrender::Imprison( const MIL_Object_ABC& /*camp*/ )
{
    // $$$$ _RC_ NLD 2005-03-10: 
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Surrender::IsImprisoned
// Created: NLD 2007-02-26
// -----------------------------------------------------------------------------
bool PHY_RoleHLA_Surrender::IsImprisoned( const MIL_Object_ABC& /*camp*/ )
{
    // $$$$ _RC_ NLD 2007-02-26: 
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Surrender::ChangeStatus
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Surrender::ChangeStatus( const std::vector< std::string >& statuses )
{
    bPrisoner_    = std::find( statuses.begin(), statuses.end(), "prisonnier" ) != statuses.end();
    // $$$$ NLD 2007-02-14: Surrendered state
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Surrender::IsSurrendered
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
bool PHY_RoleHLA_Surrender::IsSurrendered() const
{
    return false;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Surrender::IsPrisoner
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
bool PHY_RoleHLA_Surrender::IsPrisoner() const
{
    return bPrisoner_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Surrender::GetArmySurrenderedTo
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
const MIL_Army* PHY_RoleHLA_Surrender::GetArmySurrenderedTo() const
{
    return 0;
}
