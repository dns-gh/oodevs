// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Population/PHY_RolePion_Population.inl $
// $Author: Nld $
// $Modtime: 19/10/04 13:54 $
// $Revision: 1 $
// $Workfile: PHY_RolePion_Population.inl $
//
// *****************************************************************************

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Population::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Population::Update( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Population::Clean
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Population::Clean()
{
    bHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Population::HasChanged
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Population::HasChanged() const
{
    return bHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Population::EnableSlowDown
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Population::EnableSlowDown()
{
    bSlowDownEnabled_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Population::DisableSlowDown
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Population::DisableSlowDown()
{
    bSlowDownEnabled_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Population::EnableReloadingDurationModification
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Population::EnableReloadingDurationModification()
{
    bReloadingDurationModificationEnabled_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Population::DisableReloadingDurationModification
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Population::DisableReloadingDurationModification()
{
    bReloadingDurationModificationEnabled_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Population::EnableInvulnerability
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Population::EnableInvulnerability()
{
    if( ! bInvulnerabilityEnabled_ )
    {
        bHasChanged_ = true;
        bInvulnerabilityEnabled_ = true;
    }
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Population::DisableInvulnerability
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Population::DisableInvulnerability()
{
    if( bInvulnerabilityEnabled_ )
    {
        bHasChanged_ = true;
        bInvulnerabilityEnabled_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Population::IsInvulnerable
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Population::IsInvulnerable() const
{
    return bInvulnerabilityEnabled_;
}
