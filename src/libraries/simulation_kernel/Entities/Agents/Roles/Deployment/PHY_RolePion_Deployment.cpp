// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RolePion_Deployment.h"
#include "NetworkNotificationHandler_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/MIL_AgentType_ABC.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_RolePion_Deployment )

namespace
{
    double GetTime( double time )
    {
        return ( time != 0 ) ? 1 / time : 1;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Deployment constructor
// Created: ABR 2011-12-15
// -----------------------------------------------------------------------------
PHY_RolePion_Deployment::PHY_RolePion_Deployment( MIL_Agent_ABC& pion )
    : owner_           ( pion )
    , eDeploymentState_( eUndeployed )
    , rDeploymentValue_( 0.f )
    , rDeploymentGap_  ( GetTime( pion.GetType().GetUnitType().GetInstallationTime() ) )
    , rUndeploymentGap_( GetTime( pion.GetType().GetUnitType().GetUninstallationTime() ) )
    , bInstallationStateHasChanged_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Deployment destructor
// Created: ABR 2011-12-15
// -----------------------------------------------------------------------------
PHY_RolePion_Deployment::~PHY_RolePion_Deployment()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Deployment::load
// Created: ABR 2011-12-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Deployment::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< PHY_RoleInterface_Deployment >( *this );
    file >> eDeploymentState_;
    file >> rDeploymentValue_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Deployment::save
// Created: ABR 2011-12-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Deployment::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< PHY_RoleInterface_Deployment >( *this );
    file << eDeploymentState_;
    file << rDeploymentValue_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Deployment::UpdateDeploymentValue
// Created: LDC 2013-09-20
// -----------------------------------------------------------------------------
bool PHY_RolePion_Deployment::UpdateDeploymentValue()
{
    rDeploymentValue_ = std::min( 1., rDeploymentValue_ + rDeploymentGap_ );
    if( rDeploymentValue_ < 1. )
        return false;
    if( eDeploymentState_ != eDeployed )
    {
        eDeploymentState_ = eDeployed;
        MIL_Report::PostEvent( owner_, report::eRC_SectionDeployee );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Deployment::UpdateUndeploymentValue
// Created: SLI 2014-02-18
// -----------------------------------------------------------------------------
bool PHY_RolePion_Deployment::UpdateUndeploymentValue()
{
    rDeploymentValue_ = std::max( 0., rDeploymentValue_ - rUndeploymentGap_ );
    if( rDeploymentValue_ > 0. )
        return false;
    if( eDeploymentState_ != eUndeployed )
    {
        eDeploymentState_ = eUndeployed;
        MIL_Report::PostEvent( owner_, report::eRC_SectionUndeployed );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Deployment::Update
// Created: ABR 2011-12-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Deployment::Update( bool /*bIsDead*/ )
{
    bool hasChanged = false;
    switch( eDeploymentState_ )
    {
    case eDeploying:
        hasChanged = UpdateDeploymentValue();
        break;
    case eUndeploying:
        hasChanged = UpdateUndeploymentValue();
        break;
    case eStartDeploying:
        hasChanged = UpdateDeploymentValue();
        eDeploymentState_ = eUndeploying;
        break;
    case eDeployed:
    case eUndeployed:
    default:
        break;
    }
    bInstallationStateHasChanged_ = hasChanged;
    if( hasChanged )
        owner_.Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Deployment::Deploy
// Created: ABR 2011-12-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Deployment::Deploy()
{
    if( eDeploymentState_ == eDeploying )
       return; 
    MIL_Report::PostEvent( owner_, report::eRC_StartDeploy );
    eDeploymentState_ = eDeploying;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Deployment::StartDeploy
// Created: LDC 2013-09-20
// -----------------------------------------------------------------------------
void PHY_RolePion_Deployment::StartDeploy()
{
    eDeploymentState_ = eStartDeploying;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Deployment::Undeploy
// Created: ABR 2011-12-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Deployment::Undeploy()
{
    if( eDeploymentState_ == eUndeploying )
       return; 
    MIL_Report::PostEvent( owner_, report::eRC_StartUndeploy );
    eDeploymentState_ = eUndeploying;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Deployment::IsDeployed
// Created: ABR 2011-12-15
// -----------------------------------------------------------------------------
bool PHY_RolePion_Deployment::IsDeployed() const
{
    return eDeploymentState_ == eDeployed;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Deployment::IsUndeployed
// Created: ABR 2011-12-15
// -----------------------------------------------------------------------------
bool PHY_RolePion_Deployment::IsUndeployed() const
{
    return eDeploymentState_ == eUndeployed;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Deployment::SendChangedState
// Created: LDC 2013-09-20
// -----------------------------------------------------------------------------
void PHY_RolePion_Deployment::SendChangedState( client::UnitAttributes& msg ) const
{    
    if( bInstallationStateHasChanged_ )
        SendFullState( msg );
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Deployment::SendFullState
// Created: LDC 2013-09-20
// -----------------------------------------------------------------------------
void PHY_RolePion_Deployment::SendFullState( client::UnitAttributes& msg ) const
{
    msg().set_installation( static_cast< unsigned int >( rDeploymentValue_ * 100. ) );
}
