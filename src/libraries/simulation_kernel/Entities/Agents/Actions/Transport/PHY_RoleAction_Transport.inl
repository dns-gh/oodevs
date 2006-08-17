// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Transport/PHY_RoleAction_Transport.inl $
// $Author: Nld $
// $Modtime: 19/04/05 16:34 $
// $Revision: 3 $
// $Workfile: PHY_RoleAction_Transport.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::GetInitialReturnCode
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
inline
int PHY_RoleAction_Transport::GetInitialReturnCode() const
{
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::GetFinalReturnCode
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
inline
int PHY_RoleAction_Transport::GetFinalReturnCode() const
{
    return eFinished;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::Update
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
inline
void PHY_RoleAction_Transport::Update( bool /*bIsDead*/ )
{
    if( !bLoadUnloadHasBeenUpdated_ )
        nState_ = eNothing;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::Clean
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
inline
void PHY_RoleAction_Transport::Clean()
{
    bLoadUnloadHasBeenUpdated_ = false;
    bHasChanged_               = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::HasChanged
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
inline
bool PHY_RoleAction_Transport::HasChanged() const
{
    return bHasChanged_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::LoadSuspended
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
void PHY_RoleAction_Transport::LoadSuspended()
{
    bLoadUnloadHasBeenUpdated_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::UnloadSuspended
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
void PHY_RoleAction_Transport::UnloadSuspended()
{
    bLoadUnloadHasBeenUpdated_ = true;
}


// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::IsFinished
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
inline
bool PHY_RoleAction_Transport::IsFinished() const
{
    return transportedPions_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::IsTransporting
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
inline
bool PHY_RoleAction_Transport::IsTransporting() const
{
    return rWeightTransported_ > 0.;
}
