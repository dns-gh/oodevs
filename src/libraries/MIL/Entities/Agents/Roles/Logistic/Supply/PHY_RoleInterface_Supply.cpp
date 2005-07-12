// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_RoleInterface_Supply.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Supply.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_RoleInterface_Supply.h"

BOOST_CLASS_EXPORT_GUID( PHY_RoleInterface_Supply, "PHY_RoleInterface_Supply" )

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RoleInterface_Supply::PHY_RoleInterface_Supply( MT_RoleContainer& role )
    : MT_Role_ABC( role )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RoleInterface_Supply::PHY_RoleInterface_Supply()
    : MT_Role_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RoleInterface_Supply::~PHY_RoleInterface_Supply()
{

}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
inline
void PHY_RoleInterface_Supply::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MT_Role_ABC >( *this );
}

