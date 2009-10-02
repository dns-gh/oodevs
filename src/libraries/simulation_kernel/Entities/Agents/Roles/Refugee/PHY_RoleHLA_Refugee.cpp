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

BOOST_CLASS_EXPORT_GUID( PHY_RoleHLA_Refugee, "PHY_RoleHLA_Refugee" )

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Refugee constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RoleHLA_Refugee::PHY_RoleHLA_Refugee()
    : bManaged_                ( false )
{
    // NOTHING
}
  
// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Refugee destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RoleHLA_Refugee::~PHY_RoleHLA_Refugee()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Refugee::IsManaged
// Created: NLD 2007-02-26
// -----------------------------------------------------------------------------
bool PHY_RoleHLA_Refugee::IsManaged( const MIL_Object_ABC& /*camp*/ ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Refugee::ChangeStatus
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Refugee::ChangeStatus( const std::vector< std::string >& statuses )
{
    bManaged_ = std::find( statuses.begin(), statuses.end(), "refugie_prisencompte" ) != statuses.end();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Refugee::IsManaged
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
inline
bool PHY_RoleHLA_Refugee::IsManaged() const
{
    return bManaged_;
}
