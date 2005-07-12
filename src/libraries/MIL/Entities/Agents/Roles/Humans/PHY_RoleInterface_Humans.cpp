// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Humans/PHY_RoleInterface_Humans.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Humans.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_RoleInterface_Humans.h"

BOOST_CLASS_EXPORT_GUID( PHY_RoleInterface_Humans, "PHY_RoleInterface_Humans" )

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Humans constructor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_RoleInterface_Humans::PHY_RoleInterface_Humans( MT_RoleContainer& role )
    : MT_Role_ABC( role )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Humans constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_RoleInterface_Humans::PHY_RoleInterface_Humans()
    : MT_Role_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Humans destructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
PHY_RoleInterface_Humans::~PHY_RoleInterface_Humans()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Humans::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RoleInterface_Humans::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MT_Role_ABC >( *this );
}
