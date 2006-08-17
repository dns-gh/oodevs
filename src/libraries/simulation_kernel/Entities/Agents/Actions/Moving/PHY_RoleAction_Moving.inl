// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.inl $
// $Author: Nld $
// $Modtime: 23/03/05 16:12 $
// $Revision: 2 $
// $Workfile: PHY_RoleAction_Moving.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::Update
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
inline
void PHY_RoleAction_Moving::Update( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::Clean
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
inline
void PHY_RoleAction_Moving::Clean()
{
    PHY_MovingEntity_ABC::Clean();

    bCurrentPathHasChanged_ = false;
    bEnvironmentHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::HasChanged
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
inline
bool PHY_RoleAction_Moving::HasChanged() const
{
    return bCurrentPathHasChanged_ || bEnvironmentHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::NotifyEnvironmentChanged
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
void PHY_RoleAction_Moving::NotifyEnvironmentChanged()
{
    bEnvironmentHasChanged_ = true;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::NotifyCurrentPathChanged
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
void PHY_RoleAction_Moving::NotifyCurrentPathChanged()
{
    bCurrentPathHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::SetSpeedModificator
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
inline
void PHY_RoleAction_Moving::SetSpeedModificator( MT_Float rFactor )
{
    rSpeedModificator_ = rFactor;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::SetMaxSpeedModificator
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
inline
void PHY_RoleAction_Moving::SetMaxSpeedModificator( MT_Float rFactor )
{
    rMaxSpeedModificator_ = rFactor;
}
