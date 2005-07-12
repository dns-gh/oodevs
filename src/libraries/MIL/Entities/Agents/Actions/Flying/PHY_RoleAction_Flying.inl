// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Flying/PHY_RoleAction_Flying.inl $
// $Author: Nld $
// $Modtime: 7/03/05 10:47 $
// $Revision: 2 $
// $Workfile: PHY_RoleAction_Flying.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying::Update
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
inline
void PHY_RoleAction_Flying::Update( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying::Clean
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
inline
void PHY_RoleAction_Flying::Clean()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying::CanMove
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
inline
bool PHY_RoleAction_Flying::CanMove() const
{
    return pActionFly_ != 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying::IsFlying
// Created: JVT 2005-02-11
// -----------------------------------------------------------------------------
inline
bool PHY_RoleAction_Flying::IsFlying() const
{
    return pActionFly_ != 0;
}
