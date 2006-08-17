// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Perception/PHY_RoleHLA_Perceiver.cpp $
// $Author: Nld $
// $Modtime: 2/05/05 18:25 $
// $Revision: 2 $
// $Workfile: PHY_RoleHLA_Perceiver.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_RoleHLA_Perceiver.h"

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Perceiver constructor
// Created: NLD 2005-02-23
// -----------------------------------------------------------------------------
PHY_RoleHLA_Perceiver::PHY_RoleHLA_Perceiver( MT_RoleContainer& role )
    : PHY_RoleInterface_Perceiver( role )
    , bIsUsingActiveRadar_       ( false )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Perceiver destructor
// Created: NLD 2005-02-23
// -----------------------------------------------------------------------------
PHY_RoleHLA_Perceiver::~PHY_RoleHLA_Perceiver()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Perceiver::ChangeStatus
// Created: AGE 2004-12-02
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Perceiver::ChangeStatus( const std::vector< std::string >& statuses )
{
    bIsUsingActiveRadar_ = std::find( statuses.begin(), statuses.end(), "radaractif" ) != statuses.end();
}