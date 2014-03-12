// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Transport/PHY_RoleAction_Transport.cpp $
// $Author: Nld $
// $Modtime: 22/06/05 18:30 $
// $Revision: 6 $
// $Workfile: PHY_RoleAction_Transport.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleAction_Transport.h"
#include "OnComponentComputer_ABC.h"
#include "TransportWeightComputer_ABC.h"
#include "TransportPermissionComputer_ABC.h"
#include "LocationActionNotificationHandler_ABC.h"
#include "TransportNotificationHandler_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Transported/PHY_RoleInterface_Transported.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "protocol/ClientSenders.h"
#include "simulation_kernel/NetworkNotificationHandler_ABC.h"
#include <boost/serialization/export.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( transport::PHY_RoleAction_Transport )

using namespace transport;

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::sTransportData::sTransportData
// Created: NLD 2005-04-18
// -----------------------------------------------------------------------------
PHY_RoleAction_Transport::sTransportData::sTransportData()
    : bTransportOnlyLoadable_( false )
    , bMagicLoad_            ( false )
    , rTotalWeight_          ( 0 )
    , rRemainingWeight_      ( 0 )
    , rTransportedWeight_    ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::sTransportData::sTransportData
// Created: NLD 2005-04-18
// -----------------------------------------------------------------------------
PHY_RoleAction_Transport::sTransportData::sTransportData( double rTotalWeight,
                                                          bool   bTransportOnlyLoadable,
                                                          bool   bMagicLoad )
    : bTransportOnlyLoadable_( bTransportOnlyLoadable )
    , bMagicLoad_            ( bMagicLoad )
    , rTotalWeight_          ( rTotalWeight )
    , rRemainingWeight_      ( rTotalWeight )
    , rTransportedWeight_    ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport constructor
// Created: LDC 2013-01-09
// -----------------------------------------------------------------------------
PHY_RoleAction_Transport::PHY_RoleAction_Transport()
    : owner_                    ( 0 )
    , nState_                   ( eNothing )
    , bLoadUnloadHasBeenUpdated_( false )
    , bHasChanged_              ( true )
    , rWeightTransported_       ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport constructor
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
PHY_RoleAction_Transport::PHY_RoleAction_Transport( MIL_AgentPion& pion )
    : owner_                    ( &pion )
    , nState_                   ( eNothing )
    , bLoadUnloadHasBeenUpdated_( false )
    , bHasChanged_              ( true )
    , rWeightTransported_       ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport destructor
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
PHY_RoleAction_Transport::~PHY_RoleAction_Transport()
{
    Cancel();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RoleAction_Transport::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< tools::Role_ABC >( *this );
    file & owner_;
    file & rWeightTransported_;
    file & transportedPions_;
}

namespace
{
    struct TransportCapacityComputer : OnComponentComputer_ABC
    {
        TransportCapacityComputer()
            : rWeightCapacity_                ( 0 )
            , rWeightLoadedPerTimeStep_       ( 0 )
            , rWeightUnloadedPerTimeStep_     ( 0 )
            , rMaxComposanteTransportedWeight_( 0 )
        {}

        virtual void ApplyOnComponent( PHY_ComposantePion& composante )
        {
            if( composante.CanTransportPion() )
            {
                rWeightCapacity_                 += composante.GetPionTransporterWeightCapacity           ();
                rWeightLoadedPerTimeStep_        += composante.GetPionTransporterWeightLoadedPerTimeStep  ();
                rWeightUnloadedPerTimeStep_      += composante.GetPionTransporterWeightUnloadedPerTimeStep();
                rMaxComposanteTransportedWeight_ = std::max( rMaxComposanteTransportedWeight_, composante.GetPionTransporterWeightCapacity() );
            }
        }

        double rWeightCapacity_;
        double rWeightLoadedPerTimeStep_;
        double rWeightUnloadedPerTimeStep_;
        double rMaxComposanteTransportedWeight_;
    };
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::Load
// Created: NLD 2004-11-18
// -----------------------------------------------------------------------------
int PHY_RoleAction_Transport::Load()
{
    bLoadUnloadHasBeenUpdated_ = true;
    if( nState_ == eNothing )
        nState_ = eLoading;

    if( nState_ == eLoading )
    {
        TransportCapacityComputer computer;
        owner_->Execute< OnComponentComputer_ABC >( computer );

        if( computer.rWeightLoadedPerTimeStep_ == 0 )
            return eErrorNoCarriers;

        const double rWeightToLoad = std::min( computer.rWeightLoadedPerTimeStep_, computer.rWeightCapacity_ - rWeightTransported_ );
        const double rWeightLoaded = DoLoad( rWeightToLoad );
        rWeightTransported_ += rWeightLoaded;
        if( rWeightLoaded == 0 || rWeightTransported_ >= computer.rWeightCapacity_ )
        {
            nState_ = eNothing;
            return eFinished;
        }
        return eRunning;
    }
    return eImpossible;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::DoLoad
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
double PHY_RoleAction_Transport::DoLoad( const double rWeightToLoad )
{
    double rWeightLoaded = 0;
    bool bTransportedByAnother = false;

    for( auto it = transportedPions_.begin(); it != transportedPions_.end() && rWeightLoaded < rWeightToLoad; ++it )
    {
        MIL_Agent_ABC&  pion          = *it->first;
        sTransportData& transportData =  it->second;

        if( it->second.rRemainingWeight_ <= 0 )
            continue;

        pion.Apply(&TransportNotificationHandler_ABC::LoadForTransport, *owner_, transportData.bTransportOnlyLoadable_, bTransportedByAnother );

        if( it->second.rTransportedWeight_ <= 0 && bTransportedByAnother /* TODO && pion.CanBeTransported() */ ) // Filer position embarkedment si bTransportOnlyLoadable_  + transporteur
            continue; // LoadForTransport fails when the 'pion' is already transported by another unit

        const double rTmpWeight = std::min( rWeightToLoad - rWeightLoaded, it->second.rRemainingWeight_ );
        it->second.rTransportedWeight_ += rTmpWeight;
        it->second.rRemainingWeight_   -= rTmpWeight;
        rWeightLoaded                  += rTmpWeight;
    }
    if( rWeightLoaded )
        bHasChanged_ = true;
    return rWeightLoaded;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::Unload
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
int PHY_RoleAction_Transport::Unload( MT_Vector2D* position )
{
    bLoadUnloadHasBeenUpdated_ = true;
    if( nState_ == eNothing )
        nState_ = eUnloading;

    if( nState_ == eUnloading )
    {
        TransportCapacityComputer computer;
        owner_->Execute< OnComponentComputer_ABC >( computer );
        if( computer.rWeightUnloadedPerTimeStep_ == 0 )
            return eErrorNoCarriers;

        const double rWeightUnloaded = DoUnload( computer.rWeightUnloadedPerTimeStep_, position );
        rWeightTransported_ -= rWeightUnloaded;
        if( rWeightUnloaded == 0 || rWeightTransported_ <= 0 )
        {
            rWeightTransported_ = 0;
            nState_ = eNothing;
            return eFinished;
        }
        return eRunning;
    }
    return eImpossible;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::DoUnload
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
double PHY_RoleAction_Transport::DoUnload( const double rWeightToUnload, MT_Vector2D* position )
{
    double rWeightUnloaded = 0;
    for( auto it = transportedPions_.begin(); it != transportedPions_.end() && rWeightUnloaded < rWeightToUnload; )
    {
        if( it->second.rTransportedWeight_ <= 0 )
        {
            ++it;
            continue;
        }
        const double rTmpWeight = std::min( rWeightToUnload - rWeightUnloaded, it->second.rTransportedWeight_ );
        it->second.rTransportedWeight_ -= rTmpWeight;
        rWeightUnloaded                += rTmpWeight;
        if( it->second.rTransportedWeight_ <= 0 && it->second.rRemainingWeight_ <= 0 )
        {
            bHasChanged_ = true;
            MIL_Agent_ABC& pion = *it->first;
            pion.Apply(&TransportNotificationHandler_ABC::UnloadFromTransport,  *owner_, it->second.bTransportOnlyLoadable_, position );
            it = transportedPions_.erase( it );
        }
        else
            ++it;
    }
    if( rWeightUnloaded )
        bHasChanged_ = true;
    return rWeightUnloaded;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::NotifyComposanteChanged
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_RoleAction_Transport::NotifyComposanteChanged( const PHY_ComposantePion& composante )
{
    if( !composante.GetState().IsDamaged() )
        return;

    double rWeightDamaged = std::min( composante.GetPionTransporterWeightCapacity(), rWeightTransported_ );
    if( rWeightDamaged == 0 )
        return;

    for( auto it = transportedPions_.begin(); it != transportedPions_.end() && rWeightDamaged > 0; ++it )
    {
        if( it->second.rTransportedWeight_ )
        {
            const double rTmpWeight = std::min( rWeightDamaged, it->second.rTransportedWeight_ );
            rWeightDamaged -= rTmpWeight;
            (*it->first).Apply(&TransportNotificationHandler_ABC::DamageTransported, it->second.rTransportedWeight_,composante.GetState(), it->second.bTransportOnlyLoadable_ );

            if( !composante.GetState().IsUsable() )
            {
                rWeightTransported_           -= rTmpWeight;
                it->second.rTransportedWeight_ = 0;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::ApplyContamination
// Created: AHC 2009-09-22
// -----------------------------------------------------------------------------
void PHY_RoleAction_Transport::ApplyContamination()
{
    double rWeightDamaged = rWeightTransported_;
    for( auto it = transportedPions_.begin(); it != transportedPions_.end() && rWeightDamaged > 0; ++it )
    {
        if( it->second.rTransportedWeight_ )
        {
            rWeightDamaged -= it->second.rTransportedWeight_;
            (*it->first).GetRole< PHY_RoleInterface_Composantes >().ApplyContamination();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::CheckConsistency
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_RoleAction_Transport::CheckConsistency()
{
    TransportCapacityComputer computer;
    owner_->Execute< OnComponentComputer_ABC >( computer );
    if( computer.rWeightCapacity_ == 0 )
        Cancel();
}

namespace
{
    struct TransportWeightComputer : TransportWeightComputer_ABC
    {
        TransportWeightComputer( bool bTransportOnlyLoadable, bool bCanTransportDestroyed )
            : bTransportOnlyLoadable_( bTransportOnlyLoadable )
            , bCanTransportDestroyed_( bCanTransportDestroyed )
            , totalTransportedWeight_( 0 )
            , heaviestTransportedWeight_( 0 )
        {}

        virtual void AddTransportedWeight( double weight, bool canBeLoaded )
        {
            if( !bTransportOnlyLoadable_ || canBeLoaded )
            {
                totalTransportedWeight_ += weight;
                heaviestTransportedWeight_ = std::max( heaviestTransportedWeight_, weight );
            }
        }

        virtual bool CanTransportDestroyed() const
        {
            return bCanTransportDestroyed_;
        }

        bool bTransportOnlyLoadable_;
        bool bCanTransportDestroyed_;
        double totalTransportedWeight_;
        double heaviestTransportedWeight_;
    };

    struct TransportPermissionComputer : TransportPermissionComputer_ABC
    {
        TransportPermissionComputer()
            : allow_( true )
        {}

        virtual void AllowLoading( bool doAllow )
        {
            allow_ &= doAllow;
        }

        bool allow_;
    };
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::AddPion
// Created: NLD 2005-04-18
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Transport::AddPion( MIL_Agent_ABC& transported, bool bTransportOnlyLoadable )
{
    if( transported == *owner_
           || transportedPions_.find( &transported ) != transportedPions_.end() )
        return false;

    TransportPermissionComputer permissionComputer;
    transported.Execute< TransportPermissionComputer_ABC >( permissionComputer );
    if( !permissionComputer.allow_ )
        return false;

    TransportWeightComputer weightComp( bTransportOnlyLoadable, owner_->CanTransportDestroyed() );
    transported.Execute< TransportWeightComputer_ABC >( weightComp );
    if( weightComp.totalTransportedWeight_ <= 0 )
        return false;

    TransportCapacityComputer capacityComputer;
    owner_->Execute< OnComponentComputer_ABC >( capacityComputer );
    if( !bTransportOnlyLoadable &&
        weightComp.heaviestTransportedWeight_ > capacityComputer.rMaxComposanteTransportedWeight_ )
        return false;

    transportedPions_[ &transported ] = sTransportData( weightComp.totalTransportedWeight_, bTransportOnlyLoadable, false );

    bHasChanged_ = true;
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::MagicLoadPion
// Created: NLD 2005-04-18
// -----------------------------------------------------------------------------
void PHY_RoleAction_Transport::MagicLoadPion( MIL_Agent_ABC& transported, bool bTransportOnlyLoadable )
{
    bool bTransportedByAnother = false;
    if( transported == *owner_
        || transportedPions_.find( &transported ) != transportedPions_.end() )
        return;

    TransportPermissionComputer permissionComputer;
    transported.Execute< TransportPermissionComputer_ABC >( permissionComputer );
    if( !permissionComputer.allow_ )
         return;

    transported.Apply( &TransportNotificationHandler_ABC::LoadForTransport, *owner_, bTransportOnlyLoadable, bTransportedByAnother );
    TransportWeightComputer weightComputer( bTransportOnlyLoadable, owner_->CanTransportDestroyed() );
    transported.Execute< TransportWeightComputer_ABC >( weightComputer );
    sTransportData& data = transportedPions_[ &transported ] =
        sTransportData(
            weightComputer.totalTransportedWeight_,
            bTransportOnlyLoadable, true );
    data.rRemainingWeight_   = 0;
    data.rTransportedWeight_ = data.rTotalWeight_;
    rWeightTransported_ += data.rTotalWeight_;

    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::MagicUnloadPion
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void PHY_RoleAction_Transport::MagicUnloadPion( MIL_Agent_ABC& transported )
{
    auto it = transportedPions_.find( &transported );
    if( it == transportedPions_.end() )
        return;

    MT_Vector2D* positionDummy( 0 );
    transported.Apply(&TransportNotificationHandler_ABC::UnloadFromTransport, *owner_, it->second.bTransportOnlyLoadable_, positionDummy );

    rWeightTransported_ = std::max( 0., rWeightTransported_ - it->second.rTransportedWeight_ );
    transportedPions_.erase( it );
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::Cancel
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void PHY_RoleAction_Transport::Cancel()
{
    for( auto it = transportedPions_.begin(); it != transportedPions_.end(); ++it)
    {
        it->first->Apply(&TransportNotificationHandler_ABC::CancelTransport, *owner_);
        it->second.rTransportedWeight_ = 0;
        if( it->second.bMagicLoad_ )
            it->first->Apply( &location::LocationActionNotificationHandler_ABC::Follow, *owner_ );
    }
    transportedPions_.clear();
    rWeightTransported_ = 0;
    bHasChanged_        = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::CanTransportPion
// Created: JVT 2005-02-01
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Transport::CanTransportPion( MIL_Agent_ABC& transported, bool bTransportOnlyLoadable ) const
{
    if( *owner_ == transported || transported.IsMarkedForDestruction() )
        return false;

    TransportWeightComputer weightComp( bTransportOnlyLoadable, owner_->CanTransportDestroyed() );
    transported.Execute< TransportWeightComputer_ABC >( weightComp );
    if( weightComp.totalTransportedWeight_ <= 0 )
        return false;

    TransportCapacityComputer capacityComputer;
    owner_->Execute< OnComponentComputer_ABC >( capacityComputer );
    if( capacityComputer.rWeightLoadedPerTimeStep_ == 0 ||
        !bTransportOnlyLoadable && weightComp.heaviestTransportedWeight_ > capacityComputer.rMaxComposanteTransportedWeight_ )
        return false;

    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::GetNumberOfRoundTripToTransportPion
// Created: GGE 2013-04-03
// -----------------------------------------------------------------------------
double PHY_RoleAction_Transport::GetNumberOfRoundTripToTransportPion( MIL_Agent_ABC& transported, bool bTransportOnlyLoadable ) const
{
    if( *owner_ == transported )
        return 0;

    TransportWeightComputer weightComp( bTransportOnlyLoadable, owner_->CanTransportDestroyed() );
    transported.Execute< TransportWeightComputer_ABC >( weightComp );

    if( weightComp.totalTransportedWeight_ <= 0 )
        return 0;

    TransportCapacityComputer capacityComputer;
    owner_->Execute< OnComponentComputer_ABC >( capacityComputer );

    return weightComp.totalTransportedWeight_ / capacityComputer.rWeightCapacity_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::IsLoaded
// Created: NLD 2007-02-26
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Transport::IsLoaded( const MIL_Agent_ABC& transported ) const
{
    auto it = transportedPions_.find( const_cast< MIL_Agent_ABC* >( &transported ) );
    if( it == transportedPions_.end() || it->second.rRemainingWeight_ > 0 )
        return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::SendFullState
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
void PHY_RoleAction_Transport::SendFullState( client::UnitAttributes& msg ) const
{
    msg().mutable_transported_units(); // Force sending of a list, even if empty
    for( auto it = transportedPions_.begin(); it != transportedPions_.end(); ++it )
        if( it->second.rTransportedWeight_ > 0 )
            msg().mutable_transported_units()->add_elem()->set_id( (*it->first).GetID() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::SendChangedState
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
void PHY_RoleAction_Transport::SendChangedState( client::UnitAttributes& msg ) const
{
    if( bHasChanged_ )
        SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::GetInitialReturnCode
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
int PHY_RoleAction_Transport::GetInitialReturnCode() const
{
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::GetFinalReturnCode
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
int PHY_RoleAction_Transport::GetFinalReturnCode() const
{
    return eFinished;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::Update
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
void PHY_RoleAction_Transport::Update( bool /*bIsDead*/ )
{
    std::vector< MIL_Agent_ABC* > deletedUnits;
    for( auto it = transportedPions_.begin(); it != transportedPions_.end(); ++it )
        if( it->first->IsMarkedForDestruction() )
            deletedUnits.push_back( it->first );
    for( auto it = deletedUnits.begin(); it != deletedUnits.end(); ++it )
        MagicUnloadPion( **it );
    if( !bLoadUnloadHasBeenUpdated_ )
        nState_ = eNothing;

    if( HasChanged() )
        owner_->Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::Clean
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
void PHY_RoleAction_Transport::Clean()
{
    bLoadUnloadHasBeenUpdated_ = false;
    bHasChanged_               = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::HasChanged
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Transport::HasChanged() const
{
    return bHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::LoadSuspended
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RoleAction_Transport::LoadSuspended()
{
    bLoadUnloadHasBeenUpdated_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Loading::UnloadSuspended
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RoleAction_Transport::UnloadSuspended()
{
    bLoadUnloadHasBeenUpdated_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::IsFinished
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Transport::IsFinished() const
{
    return transportedPions_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::IsTransporting
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Transport::IsTransporting() const
{
    return rWeightTransported_ > 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::RemainingWeight
// Created: JSR 2012-04-12
// -----------------------------------------------------------------------------
double PHY_RoleAction_Transport::RemainingWeight( MIL_Agent_ABC& pion ) const
{
    auto it = transportedPions_.find( &pion );
    if( it == transportedPions_.end() )
        return 0;
    return it->second.rRemainingWeight_;
}
