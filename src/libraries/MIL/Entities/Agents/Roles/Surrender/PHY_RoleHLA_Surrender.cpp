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

#include "MIL_Pch.h"

#include "PHY_RoleHLA_Surrender.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Surrender constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RoleHLA_Surrender::PHY_RoleHLA_Surrender( MT_RoleContainer& role )
    : PHY_RoleInterface_Surrender( role )
    , bSurrendered_              ( false )
    , bPrisoner_                 ( false )
{

}
  
// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Surrender destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RoleHLA_Surrender::~PHY_RoleHLA_Surrender()
{

}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Surrender::TakePrisoner
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
bool PHY_RoleHLA_Surrender::TakePrisoner( const MIL_AgentPion& /*pionTakingPrisoner*/ )
{
    // $$$$ _RC_ NLD 2005-03-10: 
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Surrender::NotifyInsidePrisonerCamp
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Surrender::NotifyInsidePrisonerCamp( const MIL_CampPrisonniers& /*camp*/ )
{
    // $$$$ _RC_ NLD 2005-03-10: 
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Surrender::NotifyOutsidePrisonerCamp
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Surrender::NotifyOutsidePrisonerCamp( const MIL_CampPrisonniers& /*camp*/ )
{
    // $$$$ _RC_ NLD 2005-03-10: 
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Surrender::ChangeStatus
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Surrender::ChangeStatus( const std::vector< std::string >& statuses )
{
    bPrisoner_    = std::find( statuses.begin(), statuses.end(), "prisonnier" ) != statuses.end();
    bSurrendered_ = std::find( statuses.begin(), statuses.end(), "rendu" ) != statuses.end();
}
