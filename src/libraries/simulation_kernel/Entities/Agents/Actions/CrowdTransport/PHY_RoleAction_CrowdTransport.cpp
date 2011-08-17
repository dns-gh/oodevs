// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleAction_CrowdTransport.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( crowdtransport::PHY_RoleAction_CrowdTransport )

using namespace crowdtransport;

namespace crowdtransport
{
    template< typename Archive >
    void save_construct_data( Archive& archive, const PHY_RoleAction_CrowdTransport* role, const unsigned int /*version*/ )
    {
        const MIL_AgentPion* const pion = &role->transporter_;
        archive << pion;
    }

    template< typename Archive >
    void load_construct_data( Archive& archive, PHY_RoleAction_CrowdTransport* role, const unsigned int /*version*/ )
    {
        MIL_AgentPion* pion;
        archive >> pion;
        ::new( role )PHY_RoleAction_CrowdTransport( *pion );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_CrowdTransport constructor
// Created: JSR 2011-08-09
// -----------------------------------------------------------------------------
PHY_RoleAction_CrowdTransport::PHY_RoleAction_CrowdTransport( MIL_AgentPion& pion )
    : transporter_    ( pion )
    , nState_         ( eNothing )
    , bUpdated_       ( false )
    , currentProgress_( 0 )
    , currentCrowd_   ( 0 )
    , bHasChanged_    ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_CrowdTransport destructor
// Created: JSR 2011-08-09
// -----------------------------------------------------------------------------
PHY_RoleAction_CrowdTransport::~PHY_RoleAction_CrowdTransport()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_CrowdTransport::serialize
// Created: JSR 2011-08-09
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RoleAction_CrowdTransport::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< tools::Role_ABC >( *this )
         & loadedHumans_
         & currentCrowd_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_CrowdTransport::Update
// Created: JSR 2011-08-10
// -----------------------------------------------------------------------------
void PHY_RoleAction_CrowdTransport::Update( bool /*bIsDead*/ )
{
    if( !bUpdated_ )
        nState_ = eNothing;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_CrowdTransport::Clean
// Created: JSR 2011-08-10
// -----------------------------------------------------------------------------
void PHY_RoleAction_CrowdTransport::Clean()
{
    bUpdated_ = false;
    bHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_CrowdTransport::GetInitialReturnCode
// Created: JSR 2011-08-10
// -----------------------------------------------------------------------------
int PHY_RoleAction_CrowdTransport::GetInitialReturnCode() const
{
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_CrowdTransport::GetFinalReturnCode
// Created: JSR 2011-08-10
// -----------------------------------------------------------------------------
int PHY_RoleAction_CrowdTransport::GetFinalReturnCode() const
{
    return eFinished;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_CrowdTransport::NotifyComposanteChanged
// Created: JSR 2011-08-11
// -----------------------------------------------------------------------------
void PHY_RoleAction_CrowdTransport::NotifyComposanteChanged( const PHY_ComposantePion& composante )
{
    unsigned int transported = loadedHumans_.GetAllHumans();
    unsigned int capacity = transporter_.GetRole< PHY_RolePion_Composantes >().GetCrowdTransporterCapacity();
    if( transported == 0 || capacity == 0 )
        return;
    double ratio = static_cast< double >( composante.GetCrowdTransporterCapacity() ) / capacity;
    if( ratio == 0 )
        return;
    bHasChanged_ = true;
    double composanteTransported = ratio * transported;
    const PHY_ComposanteState& state = composante.GetState();
    loadedHumans_.ApplyNumberOfDead( static_cast< unsigned int >( composanteTransported * composante.GetProtectionHumanDeadRatio( state ) ) );
    loadedHumans_.ApplyWounds( static_cast< unsigned int >( composanteTransported * composante.GetProtectionHumanWoundedRatio( state ) ) );
    if( currentCrowd_ && state == PHY_ComposanteState::dead_ )
    {
        MIL_PopulationHumans humans( loadedHumans_ );
        humans *= ratio;
        if( !humans.IsEmpty() )
        {
            loadedHumans_ -= humans;
            currentCrowd_->GetConcentration( transporter_.GetRole< PHY_RolePion_Location >().GetPosition() ).PushHumans( humans );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_CrowdTransport::LoadCrowd
// Created: JSR 2011-08-10
// -----------------------------------------------------------------------------
int PHY_RoleAction_CrowdTransport::LoadCrowd( MIL_Population& crowd, unsigned int concentrationId )
{
    bUpdated_ = true;
    MIL_PopulationConcentration* concentration = crowd.RetrieveConcentration( concentrationId );
    if( !concentration )
    {
        // TODO envoyer un CR ?
        nState_ = eNothing;
        return eImpossible;
    }

    if( nState_ == eNothing )
    {
        // TODO envoyer un CR ?
        currentProgress_ = 0;
        currentCrowd_ = &crowd;
        nState_ = eLoading;
    }
    if( nState_ == eLoading )
    {
        int capacity =  transporter_.GetRole< PHY_RolePion_Composantes >().GetCrowdTransporterCapacity() - loadedHumans_.GetAllHumans();
        double loadable  = static_cast< double >( capacity ) - currentProgress_;
        int crowdNumber = concentration->GetAllHumans();
        double toBeLoaded = std::min( static_cast< double >( crowdNumber ), transporter_.GetRole< PHY_RolePion_Composantes >().GetCrowdTransporterLoadedPerTimeStep() );
        if( loadable > toBeLoaded )
        {
            currentProgress_ += toBeLoaded;
            return eRunning;
        }
        else
        {
            // TODO envoyer un CR ?
            bHasChanged_ = true;
            currentProgress_ = 0;
            loadedHumans_ += concentration->PullHumans( std::min( crowdNumber, capacity ) );
            nState_ = eNothing;
            return eFinished;
        }
    }
    return eImpossible;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_CrowdTransport::LoadSuspended
// Created: JSR 2011-08-10
// -----------------------------------------------------------------------------
void PHY_RoleAction_CrowdTransport::LoadSuspended()
{
    bUpdated_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_CrowdTransport::UnloadCrowd
// Created: JSR 2011-08-10
// -----------------------------------------------------------------------------
int PHY_RoleAction_CrowdTransport::UnloadCrowd( MIL_Population& crowd, const MT_Vector2D& position )
{
    bUpdated_ = true;
    if( nState_ == eNothing )
    {
        // TODO envoyer un CR ?
        currentProgress_ = 0;
        nState_ = eUnloading;
    }
    if( nState_ == eUnloading )
    {
        currentProgress_ += transporter_.GetRole< PHY_RolePion_Composantes >().GetCrowdTransporterUnloadedPerTimeStep();
        if( currentProgress_ >= loadedHumans_.GetAllHumans() )
        {
            // TODO envoyer un CR ?
            bHasChanged_ = true;
            currentCrowd_ = 0;
            currentProgress_ = 0;
            crowd.GetConcentration( position ).PushHumans( loadedHumans_ );
            loadedHumans_.Empty();
            nState_ = eNothing;
            return eFinished;
        }
        else
            return eRunning;
    }
    return eImpossible;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_CrowdTransport::UnloadSuspended
// Created: JSR 2011-08-10
// -----------------------------------------------------------------------------
void PHY_RoleAction_CrowdTransport::UnloadSuspended()
{
    bUpdated_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_CrowdTransport::SendChangedState
// Created: JSR 2011-08-11
// -----------------------------------------------------------------------------
void PHY_RoleAction_CrowdTransport::SendChangedState( client::UnitAttributes& msg ) const
{
    if( bHasChanged_ )
        SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_CrowdTransport::SendFullState
// Created: JSR 2011-08-11
// -----------------------------------------------------------------------------
void PHY_RoleAction_CrowdTransport::SendFullState( client::UnitAttributes& msg ) const
{
    if( transporter_.GetRole< PHY_RolePion_Composantes >().GetCrowdTransporterCapacity() > 0 )
        msg().set_transported_crowd( loadedHumans_.GetAllHumans() );
}
