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

#include "MIL_pch.h"

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
// Name: PHY_RoleHLA_Refugee::OrientateRefugee
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
bool PHY_RoleHLA_Refugee::OrientateRefugee( const MIL_CampRefugies& /*camp*/ )
{
    // $$$$ _RC_ NLD 2005-03-10: 
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Refugee::NotifyInsideRefugeeCamp
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Refugee::NotifyInsideRefugeeCamp( const MIL_CampRefugies& /*camp*/ )
{
    // $$$$ _RC_ NLD 2005-03-10: 
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Refugee::NotifyOutsideRefugeeCamp
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Refugee::NotifyOutsideRefugeeCamp( const MIL_CampRefugies& /*camp*/ )
{
    // $$$$ _RC_ NLD 2005-03-10: 
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
