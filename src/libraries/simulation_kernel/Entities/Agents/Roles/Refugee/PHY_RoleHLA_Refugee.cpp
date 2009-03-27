// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Refugee/PHY_RoleHLA_Refugee.cpp $
// $Author: Jvt $
// $Modtime: 12/04/05 14:26 $
// $Revision: 2 $
// $Workfile: PHY_RoleHLA_Refugee.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_RoleHLA_Refugee.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Refugee constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RoleHLA_Refugee::PHY_RoleHLA_Refugee( MT_RoleContainer& role )
    : PHY_RoleInterface_Refugee( role )
    , bManaged_                ( false )
{
}
  
// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Refugee destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RoleHLA_Refugee::~PHY_RoleHLA_Refugee()
{

}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Refugee::Orientate
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
bool PHY_RoleHLA_Refugee::Orientate( const MIL_AgentPion& /*pionManaging*/ )
{
     // $$$$ NLD 2007-02-15: 
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Refugee::Release
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
bool PHY_RoleHLA_Refugee::Release()
{
    // $$$$ NLD 2007-02-15: 
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Refugee::Release
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
bool PHY_RoleHLA_Refugee::Release( const MIL_Object_ABC& /*camp*/ )
{
    // $$$$ NLD 2007-02-15: 
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Refugee::IsManaged
// Created: NLD 2007-02-26
// -----------------------------------------------------------------------------
bool PHY_RoleHLA_Refugee::IsManaged( const MIL_Object_ABC& /*camp*/ ) const
{
    return false;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Refugee::ChangeStatus
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Refugee::ChangeStatus( const std::vector< std::string >& statuses )
{
    bManaged_ = std::find( statuses.begin(), statuses.end(), "refugie_prisencompte" ) != statuses.end();
}
