// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_RoleAction_DirectFiring.inl $
// $Author: Jvt $
// $Modtime: 31/03/05 11:19 $
// $Revision: 2 $
// $Workfile: PHY_RoleAction_DirectFiring.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::GetInitialReturnCode
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
inline
int PHY_RoleAction_DirectFiring::GetInitialReturnCode() const
{
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::GetFinalReturnCode
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
inline
int PHY_RoleAction_DirectFiring::GetFinalReturnCode() const
{
    return eFinished;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::Update
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
inline
void PHY_RoleAction_DirectFiring::Update( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::Clean
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
inline
void PHY_RoleAction_DirectFiring::Clean()
{
    // NOTHING
}

