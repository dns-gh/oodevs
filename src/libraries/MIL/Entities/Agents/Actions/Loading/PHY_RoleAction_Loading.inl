// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.inl $
// $Author: Jvt $
// $Modtime: 31/03/05 11:21 $
// $Revision: 3 $
// $Workfile: PHY_RoleAction_Loading.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::ForceUnloadedState
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
inline
void PHY_RoleAction_Loading::ForceUnloadedState()
{
    if( bIsLoaded_ )
        SetUnloadedState();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::GetLoadingTime
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RoleAction_Loading::GetLoadingTime() const
{
    return ComputeLoadingTime();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::GetUnloadingTime
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RoleAction_Loading::GetUnloadingTime() const
{
    return ComputeUnloadingTime();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::IsLoaded
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
inline
bool PHY_RoleAction_Loading::IsLoaded() const
{
    return bIsLoaded_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::GetInitialReturnCode
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
int PHY_RoleAction_Loading::GetInitialReturnCode() const
{
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::GetFinalReturnCode
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
int PHY_RoleAction_Loading::GetFinalReturnCode() const
{
    return eEnd;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::HasChanged
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
bool PHY_RoleAction_Loading::HasChanged() const
{
    return bHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::LoadSuspended
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
void PHY_RoleAction_Loading::LoadSuspended()
{
    if( nState_ == eLoading )
        ++ nEndTimeStep_;
    bHasBeenUpdated_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::UnloadSuspended
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
void PHY_RoleAction_Loading::UnloadSuspended()
{
    if( nState_ == eUnloading )
        ++ nEndTimeStep_;
    bHasBeenUpdated_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::RecoverHumanTransportersSuspended
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
void PHY_RoleAction_Loading::RecoverHumanTransportersSuspended()
{
    if( nState_ == eRecoveringHumanTransporters )
        ++ nEndTimeStep_;
    bHasBeenUpdated_ = true;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::Update
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
void PHY_RoleAction_Loading::Update( bool /*bIsDead*/ )
{
    if( !bHasBeenUpdated_ )
        nState_ = eNothing;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::Clean
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
void PHY_RoleAction_Loading::Clean()
{
    bHasChanged_     = false;
    bHasBeenUpdated_ = false;
}

