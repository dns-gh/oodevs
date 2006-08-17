// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Network/NET_RoleInterface_Dotations.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 2 $
// $Workfile: NET_RoleInterface_Dotations.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "NET_RoleInterface_Dotations.h"

BOOST_CLASS_EXPORT_GUID( NET_RoleInterface_Dotations, "NET_RoleInterface_Dotations" )

// -----------------------------------------------------------------------------
// Name: NET_RoleInterface_Dotations constructor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
NET_RoleInterface_Dotations::NET_RoleInterface_Dotations( MT_RoleContainer& role )
    : MT_Role_ABC( role )
{
}

// -----------------------------------------------------------------------------
// Name: NET_RoleInterface_Dotations constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
NET_RoleInterface_Dotations::NET_RoleInterface_Dotations()
    : MT_Role_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: NET_RoleInterface_Dotations destructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
NET_RoleInterface_Dotations::~NET_RoleInterface_Dotations()
{
}

// -----------------------------------------------------------------------------
// Name: NET_RoleInterface_Dotations::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void NET_RoleInterface_Dotations::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MT_Role_ABC >( *this );
}
