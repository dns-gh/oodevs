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

#include "simulation_kernel_pch.h"
#include "PHY_RoleHLA_Perceiver.h"

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Perceiver constructor
// Created: NLD 2005-02-23
// -----------------------------------------------------------------------------
PHY_RoleHLA_Perceiver::PHY_RoleHLA_Perceiver()
    : bIsUsingActiveRadar_       ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Perceiver destructor
// Created: NLD 2005-02-23
// -----------------------------------------------------------------------------
PHY_RoleHLA_Perceiver::~PHY_RoleHLA_Perceiver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Perceiver::ChangeStatus
// Created: AGE 2004-12-02
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Perceiver::ChangeStatus( const std::vector< std::string >& statuses )
{
    bIsUsingActiveRadar_ = std::find( statuses.begin(), statuses.end(), "radaractif" ) != statuses.end();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Perceiver::IsUsingActiveRadar
// Created: NLD 2005-02-23
// -----------------------------------------------------------------------------
bool PHY_RoleHLA_Perceiver::IsUsingActiveRadar() const
{
    return bIsUsingActiveRadar_;
}
