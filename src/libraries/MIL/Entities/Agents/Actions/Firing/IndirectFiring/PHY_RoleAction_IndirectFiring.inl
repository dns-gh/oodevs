// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/IndirectFiring/PHY_RoleAction_IndirectFiring.inl $
// $Author: Jvt $
// $Modtime: 31/03/05 11:26 $
// $Revision: 2 $
// $Workfile: PHY_RoleAction_IndirectFiring.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_IndirectFiring::GetInitialReturnCode
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
inline
int PHY_RoleAction_IndirectFiring::GetInitialReturnCode() const
{
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_IndirectFiring::GetFinalReturnCode
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
inline
int PHY_RoleAction_IndirectFiring::GetFinalReturnCode() const
{
    return eFinished;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_IndirectFiring::Update
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
inline
void PHY_RoleAction_IndirectFiring::Update( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_IndirectFiring::Clean
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
inline
void PHY_RoleAction_IndirectFiring::Clean()
{
    // NOTHING
}

