// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Roles/Location/PHY_RolePion_Location.inl $
// $Author: Nld $
// $Modtime: 28/10/04 10:16 $
// $Revision: 2 $
// $Workfile: PHY_RolePion_Location.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::GetHeight
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RolePion_Location::GetHeight() const
{
    return rHeight_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::GetPosition
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& PHY_RolePion_Location::GetPosition() const
{
    return vPosition_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::GetDirection
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& PHY_RolePion_Location::GetDirection() const
{
    return vDirection_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::GetCurrentSpeed
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RolePion_Location::GetCurrentSpeed() const
{
    return rCurrentSpeed_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::HasDoneMagicMove
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Location::HasDoneMagicMove() const
{
    return bHasDoneMagicMove_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Location::Update( bool bIsDead )
{
    if( bIsDead || !bHasMove_ )
        Move( vPosition_, vDirection_, 0. );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::Clean
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Location::Clean()
{
    bHasMove_                = false;
    bHasDoneMagicMove_       = false;
    bPositionHasChanged_     = false;
    bDirectionHasChanged_    = false;
    bCurrentSpeedHasChanged_ = false;
    bHeightHasChanged_       = false;    
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::HasSpeedChanged
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Location::HasSpeedChanged() const
{
    return bCurrentSpeedHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::HasLocationChanged
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Location::HasLocationChanged() const
{
    return bPositionHasChanged_ || bDirectionHasChanged_ || bHeightHasChanged_;
}
