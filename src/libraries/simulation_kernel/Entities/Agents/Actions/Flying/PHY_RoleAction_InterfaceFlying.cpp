// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Flying/PHY_RoleAction_InterfaceFlying.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 2 $
// $Workfile: PHY_RoleAction_InterfaceFlying.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleAction_InterfaceFlying.h"

BOOST_CLASS_EXPORT_GUID( PHY_RoleAction_InterfaceFlying, "PHY_RoleAction_InterfaceFlying" )

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_InterfaceFlying constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RoleAction_InterfaceFlying::PHY_RoleAction_InterfaceFlying()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_InterfaceFlying destructor
// Created: NLD 2004-09-InterfaceFlying
// -----------------------------------------------------------------------------
PHY_RoleAction_InterfaceFlying::~PHY_RoleAction_InterfaceFlying()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_InterfaceFlying::Update
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_RoleAction_InterfaceFlying::Update( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_InterfaceFlying::Clean
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_RoleAction_InterfaceFlying::Clean()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_InterfaceFlying::Fly
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_RoleAction_InterfaceFlying::Fly()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_InterfaceFlying::SetFlyingHeight
// Created: JVT 2004-11-02
// -----------------------------------------------------------------------------
void PHY_RoleAction_InterfaceFlying::SetFlyingHeight( MT_Float /*rHeight*/)
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_InterfaceFlying::IsFlying
// Created: JVT 2005-02-11
// -----------------------------------------------------------------------------
bool PHY_RoleAction_InterfaceFlying::IsFlying() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_InterfaceFlying::Apply
// Created: NLD 2005-02-18
// -----------------------------------------------------------------------------
void PHY_RoleAction_InterfaceFlying::Apply( MT_Float /*rHeight*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_InterfaceFlying::CanMove
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
bool PHY_RoleAction_InterfaceFlying::CanMove() const
{
    return true;
}
