// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.cpp $
// $Author: Jvt $
// $Modtime: 4/05/05 14:41 $
// $Revision: 8 $
// $Workfile: PHY_RoleAction_Loading.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleAction_Loading.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Transported/PHY_RolePion_Transported.h"
#include "MIL_AgentServer.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_Agent_ABC.h"

BOOST_CLASS_EXPORT_GUID( PHY_RoleAction_Loading, "PHY_RoleAction_Loading" )

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading constructor
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
PHY_RoleAction_Loading::PHY_RoleAction_Loading( MIL_Agent_ABC& pion )
    : pPion_                  ( &pion )
    , bIsLoaded_              ( false )
    , nState_                 ( eNothing )
    , nEndTimeStep_           ( 0 )
    , bHasChanged_            ( true )
    , bHasBeenUpdated_        ( false )
{
    SetLoadedState();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RoleAction_Loading::PHY_RoleAction_Loading()
    : pPion_          ( 0 )
    , bIsLoaded_      ( false )
    , nState_         ( eNothing )
    , nEndTimeStep_   ( 0 )
    , bHasChanged_    ( true )
    , bHasBeenUpdated_( false )    
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading destructor
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
PHY_RoleAction_Loading::~PHY_RoleAction_Loading()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive > 
void PHY_RoleAction_Loading::serialize( Archive& file, const uint )
{
    file & nState_
         & bIsLoaded_
         & nEndTimeStep_
         & bHasBeenUpdated_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::SetLoadedState
// Created: NLD 2004-10-07
// Modified: JVT 2004-10-20
// -----------------------------------------------------------------------------
void PHY_RoleAction_Loading::SetLoadedState()
{
    assert( !bIsLoaded_ );
    bIsLoaded_   = true;
    bHasChanged_ = true;
    CheckConsistency();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::SetUnloadedState
// Created: NLD 2004-10-07
// Modified: JVT 2004-10-20
// -----------------------------------------------------------------------------
void PHY_RoleAction_Loading::SetUnloadedState()
{
    assert( bIsLoaded_ );
    bHasChanged_ = true;
    bIsLoaded_   = false;
    CheckConsistency();
}

namespace
{
    struct sLoadingUnloadingTimesFunctor
    {
        sLoadingUnloadingTimesFunctor()
            : nNbrHumans_                   ( 0 )
            , rNbrHumansLoadedPerTimeStep_  ( 0. )
            , rNbrHumansUnloadedPerTimeStep_( 0. )
        {
        }

        void operator() ( const PHY_ComposantePion& composante )
        {
            if( composante.CanBeLoaded() )
                nNbrHumans_ += composante.GetNbrUsableHumans();
            const PHY_ComposanteTypePion& compType = composante.GetType();
            if( composante.CanTransportHumans() )
            {
                rNbrHumansLoadedPerTimeStep_   += compType.GetNbrHumansLoadedPerTimeStep  ();
                rNbrHumansUnloadedPerTimeStep_ += compType.GetNbrHumansUnloadedPerTimeStep();
            }
        }

        uint     nNbrHumans_;
        MT_Float rNbrHumansLoadedPerTimeStep_;
        MT_Float rNbrHumansUnloadedPerTimeStep_;
    };
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::ComputeTimes
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
MT_Float PHY_RoleAction_Loading::ComputeLoadingTime() const
{
    assert( pPion_ );
    sLoadingUnloadingTimesFunctor func;
    pPion_->GetRole< PHY_RolePion_Composantes >().Apply( func );
    if( func.rNbrHumansLoadedPerTimeStep_ == 0. )
        return std::numeric_limits< MT_Float >::max();
    return func.nNbrHumans_ / func.rNbrHumansLoadedPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::ComputeUnloadingTime
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
MT_Float PHY_RoleAction_Loading::ComputeUnloadingTime() const
{
    assert( pPion_ );
    sLoadingUnloadingTimesFunctor func;
    pPion_->GetRole< PHY_RolePion_Composantes >().Apply( func );
    if( func.rNbrHumansUnloadedPerTimeStep_ == 0. )
        return std::numeric_limits< MT_Float >::max();
    return func.nNbrHumans_ / func.rNbrHumansUnloadedPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::Load
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
int PHY_RoleAction_Loading::Load()
{
    bHasBeenUpdated_ = true;

    if( bIsLoaded_ )
        return eEnd;

    if( nState_ == eNothing )
    {
        const MT_Float rLoadingTime = ComputeLoadingTime();
        if( rLoadingTime == std::numeric_limits< MT_Float >::max() )
            return eErrorNoCarriers;
        else if( rLoadingTime == 0. )
            return eErrorNoCarried;
        nEndTimeStep_ = (uint)rLoadingTime + MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
        nState_       = eLoading;
    }

    if( nState_ == eLoading )
    {
        if( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() >= nEndTimeStep_ )
        {
            nState_      = eNothing;
            SetLoadedState();
            return eEnd;
        }
        return eRunning;
    }
    return eErrorLoadUnloadInSameTime;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::Unload
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
int PHY_RoleAction_Loading::Unload()
{
    bHasBeenUpdated_ = true;

    if( !bIsLoaded_ )
        return eEnd;

    if( nState_ == eNothing )
    {
        const MT_Float rUnloadingTime = ComputeUnloadingTime();
        if( rUnloadingTime == std::numeric_limits< MT_Float >::max() )
            return eErrorNoCarriers;
        else if( rUnloadingTime == 0. )
            return eErrorNoCarried;
        nEndTimeStep_ = (uint)rUnloadingTime + MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
        nState_       = eUnloading;
    }

    if( nState_ == eUnloading )
    {
        if( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() >= nEndTimeStep_ )
        {
            nState_      = eNothing;
            SetUnloadedState();
            return eEnd;
        }
        return eRunning;
    }
    return eErrorLoadUnloadInSameTime;
}

namespace
{
    struct sLoadedStateConsistency
    {
        sLoadedStateConsistency()
            : bHasValidCarriers_( false )
            , bHasValidLoadable_( false )
        {
        }

        void operator() ( const PHY_ComposantePion& composante )
        {
            if( composante.CanTransportHumans() )
                bHasValidCarriers_ = true;

            if( composante.CanBeLoaded() )
                bHasValidLoadable_ = true;
        }

        bool bHasValidCarriers_;
        bool bHasValidLoadable_;
    };
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::CheckConsistency
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
void PHY_RoleAction_Loading::CheckConsistency()
{
    assert( pPion_ );
    sLoadedStateConsistency func;
    pPion_->GetRole< PHY_RolePion_Composantes >().Apply( func );

    if( bIsLoaded_ )
    {
        if( !func.bHasValidCarriers_ && func.bHasValidLoadable_ )
            SetUnloadedState();
    }
    else
    {
        if( !func.bHasValidLoadable_ )
            SetLoadedState();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::SendFullState
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RoleAction_Loading::SendFullState( NET_ASN_MsgUnitAttributes& msg ) const
{
    msg().m.embarquePresent = 1;
    msg().embarque = bIsLoaded_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::SendChangedState
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RoleAction_Loading::SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const
{
    if( bHasChanged_ )
        SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::ForceUnloadedState
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
void PHY_RoleAction_Loading::ForceUnloadedState()
{
    if( bIsLoaded_ )
        SetUnloadedState();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::GetLoadingTime
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
MT_Float PHY_RoleAction_Loading::GetLoadingTime() const
{
    return ComputeLoadingTime();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::GetUnloadingTime
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
MT_Float PHY_RoleAction_Loading::GetUnloadingTime() const
{
    return ComputeUnloadingTime();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::IsLoaded
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Loading::IsLoaded() const
{
    return bIsLoaded_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::GetInitialReturnCode
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
int PHY_RoleAction_Loading::GetInitialReturnCode() const
{
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::GetFinalReturnCode
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
int PHY_RoleAction_Loading::GetFinalReturnCode() const
{
    return eEnd;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::HasChanged
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Loading::HasChanged() const
{
    return bHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::LoadSuspended
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
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
void PHY_RoleAction_Loading::UnloadSuspended()
{
    if( nState_ == eUnloading )
        ++ nEndTimeStep_;
    bHasBeenUpdated_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::Update
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RoleAction_Loading::Update( bool /*bIsDead*/ )
{
    if( !bHasBeenUpdated_ )
        nState_ = eNothing;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::Clean
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RoleAction_Loading::Clean()
{
    bHasChanged_     = false;
    bHasBeenUpdated_ = false;
}
