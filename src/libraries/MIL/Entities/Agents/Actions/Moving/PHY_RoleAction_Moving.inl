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
    if( !bHasMoved_ )
        bForcePathCheck_ = true;;
    bHasChanged_ = false;
    bHasMoved_   = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::HasChanged
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
inline
bool PHY_RoleAction_Moving::HasChanged() const
{
    return bHasChanged_;
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
