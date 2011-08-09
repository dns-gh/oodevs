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
#include "AlgorithmsFactories.h"
#include "PostureComputer_ABC.h"
#include "HumanLoadingTimeComputer_ABC.h"
#include "LoadedStateConsistencyComputer_ABC.h"
#include "LoadingComputerFactory_ABC.h"
#include "NetworkNotificationHandler_ABC.h"
#include "VisionConeNotificationHandler_ABC.h"
#include "LoadingChangeNotificationHandler_ABC.h"
#include "MIL_AgentServer.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( transport::PHY_RoleAction_Loading )

namespace transport
{

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RoleAction_Loading* role, const unsigned int /*version*/ )
{
    MIL_Agent_ABC* const pion = &role->pion_;
    archive << pion;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RoleAction_Loading* role, const unsigned int /*version*/ )
{
    MIL_Agent_ABC* pion;
    archive >> pion;
    ::new( role )PHY_RoleAction_Loading( *pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading constructor
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
PHY_RoleAction_Loading::PHY_RoleAction_Loading( MIL_Agent_ABC& pion )
    : pion_           ( pion )
    , bIsLoaded_      ( false )
    , nState_         ( eNothing )
    , nEndTimeStep_   ( 0 )
    , bHasChanged_    ( true )
    , bHasBeenUpdated_( false )
{
    SetLoadedState();
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
void PHY_RoleAction_Loading::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< tools::Role_ABC >( *this )
         & nState_
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
    pion_.Apply( &transport::LoadingChangeNotificationHandler_ABC::NotifyIsLoadedInVab );
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
    pion_.Apply( &transport::LoadingChangeNotificationHandler_ABC::NotifyIsUnLoadedInVab );
    CheckConsistency();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::ComputeTimes
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
double PHY_RoleAction_Loading::ComputeLoadingTime() const
{
    std::auto_ptr< HumanLoadingTimeComputer_ABC > loadingTimeComputer = pion_.GetAlgorithms().loadingComputerFactory_->CreateHumanLoadingTimeComputer();
    pion_.Execute( *loadingTimeComputer );
    if( loadingTimeComputer->GetHumansLoadedPerTimeStep() == 0. )
        return std::numeric_limits< double >::max();
    return loadingTimeComputer->GetHumansCount() / loadingTimeComputer->GetHumansLoadedPerTimeStep();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::ComputeUnloadingTime
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
double PHY_RoleAction_Loading::ComputeUnloadingTime() const
{
    std::auto_ptr< HumanLoadingTimeComputer_ABC > loadingTimeComputer = pion_.GetAlgorithms().loadingComputerFactory_->CreateHumanLoadingTimeComputer();
    pion_.Execute( *loadingTimeComputer );
    if( loadingTimeComputer->GetHumansUnloadedPerTimeStep() == 0. )
        return std::numeric_limits< double >::max();
    return loadingTimeComputer->GetHumansCount() / loadingTimeComputer->GetHumansUnloadedPerTimeStep();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::Load
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
int PHY_RoleAction_Loading::Load()
{
    bHasBeenUpdated_ = true;

    if( nState_ == eUnloading )
    {
        MIL_Report::PostEvent( pion_, MIL_Report::eReport_DisembarkmentInterrupted );
        nState_ = eNothing;
    }
    if( bIsLoaded_ )
        return eEnd;

    if( nState_ == eNothing )
    {
        const double rLoadingTime = ComputeLoadingTime();
        if( rLoadingTime == std::numeric_limits< double >::max() )
            return eErrorNoCarriers;
        else if( rLoadingTime == 0. )
            return eErrorNoCarried;
        nEndTimeStep_ = (unsigned int)rLoadingTime + MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
        nState_       = eLoading;
        MIL_Report::PostEvent( pion_, MIL_Report::eReport_EmbarkmentStarted );    }

    if( nState_ == eLoading )
    {
        if( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() >= nEndTimeStep_ )
        {
            nState_      = eNothing;
            MIL_Report::PostEvent( pion_, MIL_Report::eReport_EmbarkmentFinished );            SetLoadedState();
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

    if( nState_ == eLoading )
    {
        MIL_Report::PostEvent( pion_, MIL_Report::eReport_EmbarkmentInterrupted );
        nState_ = eNothing;
    }
    if( !bIsLoaded_ )
        return eEnd;

    if( nState_ == eNothing )
    {
        const double rUnloadingTime = ComputeUnloadingTime();
        if( rUnloadingTime == std::numeric_limits< double >::max() )
            return eErrorNoCarriers;
        else if( rUnloadingTime == 0. )
            return eErrorNoCarried;
        nEndTimeStep_ = (unsigned int)rUnloadingTime + MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
        nState_       = eUnloading;
        MIL_Report::PostEvent( pion_, MIL_Report::eReport_DisembarkmentStarted );
    }

    if( nState_ == eUnloading )
    {
        if( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() >= nEndTimeStep_ )
        {
            nState_      = eNothing;
            MIL_Report::PostEvent( pion_, MIL_Report::eReport_DisembarkmentFinished );            SetUnloadedState();
            return eEnd;
        }
        return eRunning;
    }
    return eErrorLoadUnloadInSameTime;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::CheckConsistency
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
void PHY_RoleAction_Loading::CheckConsistency()
{
    std::auto_ptr< LoadedStateConsistencyComputer_ABC > comp = pion_.GetAlgorithms().loadingComputerFactory_->CreateLoadedStateConsistencyComputer();
    pion_.Execute( *comp );

    if( bIsLoaded_ )
    {
        if( !comp->HasValidCarrier() && comp->HasValidLoadable() )
            SetUnloadedState();
    }
    else
    {
        if( !comp->HasValidLoadable() )
            SetLoadedState();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::SendFullState
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RoleAction_Loading::SendFullState( client::UnitAttributes& msg ) const
{
    msg().set_embarked( bIsLoaded_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::SendChangedState
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RoleAction_Loading::SendChangedState( client::UnitAttributes& msg ) const
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
double PHY_RoleAction_Loading::GetLoadingTime() const
{
    return ComputeLoadingTime();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::GetUnloadingTime
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
double PHY_RoleAction_Loading::GetUnloadingTime() const
{
    return ComputeUnloadingTime();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::IsLoaded
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Loading::IsLoaded() const
{
    return bIsLoaded_ && ( nState_ == eNothing );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::IsUnloaded
// Created: LDC 2010-11-02
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Loading::IsUnloaded() const
{
    return !bIsLoaded_ && ( nState_ == eNothing );
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

    if( HasChanged() )
    {
        pion_.Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
        pion_.Apply( &network::VisionConeNotificationHandler_ABC::NotifyVisionConeDataHasChanged );
    }
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

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::Execute
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void PHY_RoleAction_Loading::Execute( posture::PostureComputer_ABC& algorithm ) const
{
    algorithm.NotifyLoaded();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::LoadForTransport
// Created: AHC 2009-09-24
// -----------------------------------------------------------------------------
void PHY_RoleAction_Loading::LoadForTransport   ( const MIL_Agent_ABC& /*transporter*/, bool /*bTransportOnlyLoadable*/,  bool& /*bTransportedByAnother*/  )
{
    ForceUnloadedState();
}
// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::UnloadFromTransport
// Created: AHC 2009-09-24
// -----------------------------------------------------------------------------
void PHY_RoleAction_Loading::UnloadFromTransport( const MIL_Agent_ABC& /*transporter*/, bool /*bTransportOnlyLoadable*/, MT_Vector2D* /*position*/ )
{
    ForceUnloadedState();
}

} // namespace transport


