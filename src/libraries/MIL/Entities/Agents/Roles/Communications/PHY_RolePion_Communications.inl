// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Roles/Communications/PHY_RolePion_Communications.inl $
// $Author: Nld $
// $Modtime: 9/11/04 10:39 $
// $Revision: 2 $
// $Workfile: PHY_RolePion_Communications.inl $
//
// *****************************************************************************

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Communications::Update( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::Clean
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Communications::Clean()
{
    bHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::HasChanged
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Communications::HasChanged() const
{
    return bHasChanged_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::ActivateBlackout
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Communications::ActivateBlackout()
{
    if( bBlackoutActivated_ )
        return;
    bBlackoutActivated_ = true;
    bHasChanged_        = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::DeactivateBlackout
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Communications::DeactivateBlackout()
{
    if( !bBlackoutActivated_ )
        return;
    bBlackoutActivated_ = false;
    bHasChanged_        = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::CanCommunicate
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Communications::CanCommunicate() const
{
    return jammers_.empty() && !bBlackoutActivated_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::ModifySpeed
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RolePion_Communications::ModifySpeed( MT_Float rSpeed ) const
{
    if( jammers_.empty() )
        return rSpeed;
    return rSpeed *= rCoefSpeedModificator_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::ModifyReloadingDuration
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RolePion_Communications::ModifyReloadingDuration( MT_Float rDuration ) const
{
    if( jammers_.empty() )
        return rDuration;
    return rDuration *= rCoefReloadingTimeModificator_;
}
