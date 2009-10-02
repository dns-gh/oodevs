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
#include "Entities/Agents/Roles/Transported/PHY_RoleInterface_Transported.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Network/NET_ASN_Messages.h"

#include "simulation_kernel/AlgorithmsFactories.h"
#include "TransportCapacityComputerFactory_ABC.h"
#include "TransportCapacityComputer_ABC.h"
#include "TransportPermissionComputer_ABC.h"
#include "TransportWeightComputer_ABC.h"
#include "TransportNotificationHandler_ABC.h"

#include "simulation_kernel/NetworkNotificationHandler_ABC.h"

BOOST_CLASS_EXPORT_GUID( transport::PHY_RoleAction_Transport, "PHY_RoleAction_Transport" )
BOOST_CLASS_EXPORT_GUID( transport::PHY_RoleAction_Transport::sTransportData, "PHY_RoleAction_Transport::sTransportData" )

namespace transport
{

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RoleAction_Transport* role, const unsigned int /*version*/ )
{
    const MIL_AgentPion* const pion = &role->transporter_;
    archive << pion;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RoleAction_Transport* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* pion;
    archive >> pion;
    ::new( role )PHY_RoleAction_Transport( *pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::sTransportData::sTransportData
// Created: NLD 2005-04-18
// -----------------------------------------------------------------------------
PHY_RoleAction_Transport::sTransportData::sTransportData()
    : bTransportOnlyLoadable_( false )
    , rTotalWeight_          ( 0. )
    , rRemainingWeight_      ( 0. )
    , rTransportedWeight_    ( 0. )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::sTransportData::sTransportData
// Created: NLD 2005-04-18
// -----------------------------------------------------------------------------
PHY_RoleAction_Transport::sTransportData::sTransportData( const sTransportData& rhs )
    : bTransportOnlyLoadable_( rhs.bTransportOnlyLoadable_ )
    , rTotalWeight_          ( rhs.rTotalWeight_           )
    , rRemainingWeight_      ( rhs.rRemainingWeight_       )
    , rTransportedWeight_    ( rhs.rTransportedWeight_     )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::sTransportData::sTransportData
// Created: NLD 2005-04-18
// -----------------------------------------------------------------------------
PHY_RoleAction_Transport::sTransportData::sTransportData( MT_Float rTotalWeight, bool bTransportOnlyLoadable )
    : bTransportOnlyLoadable_( bTransportOnlyLoadable )
    , rTotalWeight_          ( rTotalWeight )
    , rRemainingWeight_      ( rTotalWeight )
    , rTransportedWeight_    ( 0. )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::sTransportData::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RoleAction_Transport::sTransportData::serialize( Archive& file, const uint )
{
    file & const_cast< bool& >    ( bTransportOnlyLoadable_ )
         & const_cast< MT_Float& >( rTotalWeight_ )
         & rRemainingWeight_
         & rTransportedWeight_;
}

// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport constructor
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
PHY_RoleAction_Transport::PHY_RoleAction_Transport( MIL_AgentPion& pion )
    : transporter_             ( pion )
    , nState_                   ( eNothing )
    , bLoadUnloadHasBeenUpdated_( false )
    , rWeightTransported_       ( 0. )
    , bHasChanged_              ( true )
    , transportedPions_         ()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport destructor
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
PHY_RoleAction_Transport::~PHY_RoleAction_Transport()
{
    Cancel();
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RoleAction_Transport::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< tools::Role_ABC >( *this )
         & rWeightTransported_
         & transportedPions_;
}


// =============================================================================
// LOADING
// =============================================================================

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
    	const TransportCapacityComputer_ABC& comp = transporter_.Execute( transporter_.GetAlgorithms().transportComputerFactory_->CreateCapacityComputer() );

        if( comp.WeightLoadedPerTimeStep() == 0. )
            return eErrorNoCarriers;

        const MT_Float rWeightToLoad = std::min( comp.WeightLoadedPerTimeStep(), comp.WeightCapacity() - rWeightTransported_ );
        const MT_Float rWeightLoaded = DoLoad( rWeightToLoad );
        rWeightTransported_ += rWeightLoaded;
        if( rWeightLoaded == 0 || rWeightTransported_ >= comp.WeightCapacity() )
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
MT_Float PHY_RoleAction_Transport::DoLoad( const MT_Float rWeightToLoad )
{
    MT_Float rWeightLoaded = 0.;

    for( IT_TransportedPionMap it = transportedPions_.begin(); it != transportedPions_.end() && rWeightLoaded < rWeightToLoad; ++it )
    {
        MIL_Agent_ABC&  pion          = *it->first;
        sTransportData& transportData =  it->second;

        if( it->second.rRemainingWeight_ <= 0. )
            continue;

        if( it->second.rTransportedWeight_ <= 0. /* TODO && pion.CanBeTransported() */ ) // Filer position embarquement si bTransportOnlyLoadable_  + transporteur
            continue; // LoadForTransport fails when the 'pion' is already transported by another unit

        pion.Apply(&TransportNotificationHandler_ABC::LoadForTransport, transporter_, transportData.bTransportOnlyLoadable_ );
        const MT_Float rTmpWeight = std::min( rWeightToLoad - rWeightLoaded, it->second.rRemainingWeight_ );
        it->second.rTransportedWeight_ += rTmpWeight;
        it->second.rRemainingWeight_   -= rTmpWeight;
        rWeightLoaded                  += rTmpWeight;
    }
    return rWeightLoaded;
}

// =============================================================================
// UNLOADING
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::Unload
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
int PHY_RoleAction_Transport::Unload()
{
    bLoadUnloadHasBeenUpdated_ = true;
    if( nState_ == eNothing )
        nState_ = eUnloading;

    if( nState_ == eUnloading )
    {
    	const TransportCapacityComputer_ABC& comp = transporter_.Execute( transporter_.GetAlgorithms().transportComputerFactory_->CreateCapacityComputer() );
        if( comp.WeightUnloadedPerTimeStep() == 0. )
            return eErrorNoCarriers;

        const MT_Float rWeightUnloaded = DoUnload( comp.WeightUnloadedPerTimeStep() );
        rWeightTransported_ -= rWeightUnloaded;
        if( rWeightUnloaded == 0. || rWeightTransported_ <= 0. )
        {
            rWeightTransported_ = 0.;
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
MT_Float PHY_RoleAction_Transport::DoUnload( const MT_Float rWeightToUnload )
{
    MT_Float rWeightUnloaded = 0.;
    for( IT_TransportedPionMap it = transportedPions_.begin(); it != transportedPions_.end() && rWeightUnloaded < rWeightToUnload ; )
    {
        if( it->second.rTransportedWeight_ <= 0. )
        {
            ++it;
            continue;
        }

        const MT_Float rTmpWeight = std::min( rWeightToUnload - rWeightUnloaded, it->second.rTransportedWeight_ );
        it->second.rTransportedWeight_ -= rTmpWeight;
        rWeightUnloaded                += rTmpWeight;
        if( it->second.rTransportedWeight_ <= 0. && it->second.rRemainingWeight_ <= 0. )
        {
            bHasChanged_ = true;
            MIL_Agent_ABC& pion = *it->first;
            pion.Apply(&TransportNotificationHandler_ABC::UnloadFromTransport,  transporter_, it->second.bTransportOnlyLoadable_ );
            //assert( bOut );
            it = transportedPions_.erase( it );
        }
        else
            ++it;
    }
    return rWeightUnloaded;
}

// =============================================================================
// TRANSPORTER STATE MODIFICATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::NotifyComposanteChanged
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_RoleAction_Transport::NotifyComposanteChanged( const PHY_ComposantePion& composante )
{
    if( !composante.GetState().IsDamaged() )
        return;

    MT_Float rWeightDamaged = std::min( composante.GetPionTransporterWeightCapacity(), rWeightTransported_ );
    if( rWeightDamaged == 0 )
        return;

    for( IT_TransportedPionMap it = transportedPions_.begin(); it != transportedPions_.end() && rWeightDamaged > 0.; ++it )
    {
        if( it->second.rTransportedWeight_ )
        {
            const MT_Float rTmpWeight = std::min( rWeightDamaged, it->second.rTransportedWeight_ );
            rWeightDamaged -= rTmpWeight; 
            (*it->first).Apply(&TransportNotificationHandler_ABC::DamageTransported, it->second.rTransportedWeight_,composante.GetState(), it->second.bTransportOnlyLoadable_ );

            if( !composante.GetState().IsUsable() )
            {
                rWeightTransported_           -= rTmpWeight;
                it->second.rTransportedWeight_ = 0.; 
            }
        }
    }       
}
// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::ApplyContamination
// Created: AHC 2009-09-22
// -----------------------------------------------------------------------------
void PHY_RoleAction_Transport::ApplyContamination  ( const MIL_ToxicEffectManipulator& contamination )
{
	MT_Float rWeightDamaged = rWeightTransported_;
	for( CIT_TransportedPionMap it = transportedPions_.begin(); it != transportedPions_.end() && rWeightDamaged > 0; ++it )
	{
		if( it->second.rTransportedWeight_ )
		{
			rWeightDamaged -= it->second.rTransportedWeight_;
			(*it->first).Apply(& nbc::ToxicEffectHandler_ABC::ApplyContamination, contamination);
		}
	}
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::ApplyPoisonous
// Created: AHC 2009-09-22
// -----------------------------------------------------------------------------
void PHY_RoleAction_Transport::ApplyPoisonous( const MIL_ToxicEffectManipulator& contamination )
{
	// nothing ??
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::CheckConsistency
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_RoleAction_Transport::CheckConsistency()
{
    if( transporter_.Execute( transporter_.GetAlgorithms().transportComputerFactory_->CreateCapacityComputer() ).WeightCapacity() == 0. )
        Cancel();
}

// =============================================================================
// MAIN
// =============================================================================

namespace
{
	struct LoadableStrategy : public TransportStrategy_ABC
	{
		LoadableStrategy(bool bTransportOnlyLoadable) : bTransportOnlyLoadable_ (bTransportOnlyLoadable) {}
		bool Autorize(bool canBeLoaded) const { return  !bTransportOnlyLoadable_ || canBeLoaded ; }
	private:
		bool bTransportOnlyLoadable_;
	};
}
// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::AddPion
// Created: NLD 2005-04-18
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Transport::AddPion( MIL_Agent_ABC& transported, bool bTransportOnlyLoadable )
{
    if(    transported == transporter_
           || transportedPions_.find( &transported ) != transportedPions_.end() )
        return false;
   
    if(!transported.Execute( transporter_.GetAlgorithms().transportComputerFactory_->CreatePermissionComputer()).CanBeLoaded())
    	return false;

    const TransportWeightComputer_ABC& weightComp = transported.Execute( transporter_.GetAlgorithms().transportComputerFactory_->CreateWeightComputer(&LoadableStrategy(bTransportOnlyLoadable)));
    if( weightComp.TotalTransportedWeight() <= 0. )
        return false;

    if( !bTransportOnlyLoadable && weightComp.HeaviestTransportedWeight() >
		transporter_.Execute( transporter_.GetAlgorithms().transportComputerFactory_->CreateCapacityComputer() ).MaxComposanteTransportedWeight() )
        return false;

    transportedPions_[ &transported ].sTransportData::sTransportData( weightComp.TotalTransportedWeight(), bTransportOnlyLoadable );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::MagicLoadPion
// Created: NLD 2005-04-18
// -----------------------------------------------------------------------------
void PHY_RoleAction_Transport::MagicLoadPion( MIL_Agent_ABC& transported, bool bTransportOnlyLoadable )
{
    if(   transported == transporter_
        || transportedPions_.find( &transported ) != transportedPions_.end() )
        return;

    if(!transported.Execute( transporter_.GetAlgorithms().transportComputerFactory_->CreatePermissionComputer()).CanBeLoaded())
       	return;

    transported.Apply(&TransportNotificationHandler_ABC::LoadForTransport, transporter_, bTransportOnlyLoadable );
    sTransportData& data = transportedPions_[ &transported ].sTransportData::sTransportData(
    		transported.Execute( transporter_.GetAlgorithms().transportComputerFactory_->CreateWeightComputer(&LoadableStrategy(bTransportOnlyLoadable))).TotalTransportedWeight(),
    		bTransportOnlyLoadable );
    data.rRemainingWeight_   = 0.;
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
    IT_TransportedPionMap it = transportedPions_.find( &transported );
    if( it == transportedPions_.end() )
        return;

    transported.Apply(&TransportNotificationHandler_ABC::UnloadFromTransport, transporter_, it->second.bTransportOnlyLoadable_ );

    assert( rWeightTransported_ >= it->second.rTransportedWeight_ );
    rWeightTransported_ -= it->second.rTransportedWeight_;
    transportedPions_.erase( it );
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::Cancel
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void PHY_RoleAction_Transport::Cancel()
{
    for( IT_TransportedPionMap it = transportedPions_.begin(); it != transportedPions_.end(); ++it)
    {
        it->first->Apply(&TransportNotificationHandler_ABC::CancelTransport, transporter_);
        it->second.rTransportedWeight_ = 0;
    }
    transportedPions_.clear();
    rWeightTransported_ = 0.;
    bHasChanged_        = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::CanTransportPion
// Created: JVT 2005-02-01
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Transport::CanTransportPion( MIL_Agent_ABC& transported, bool bTransportOnlyLoadable ) const
{
    if( transporter_ == transported )
        return false;

    const TransportWeightComputer_ABC& weightComp = transported.Execute( transporter_.GetAlgorithms().transportComputerFactory_->CreateWeightComputer(&LoadableStrategy(bTransportOnlyLoadable)));
    if( weightComp.TotalTransportedWeight() <= 0. )
        return false;

    if( !bTransportOnlyLoadable && weightComp.HeaviestTransportedWeight() >
		 transporter_.Execute( transporter_.GetAlgorithms().transportComputerFactory_->CreateCapacityComputer() ).MaxComposanteTransportedWeight() )
        return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::IsLoaded
// Created: NLD 2007-02-26
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Transport::IsLoaded( const MIL_Agent_ABC& transported ) const
{
    CIT_TransportedPionMap it = transportedPions_.find( const_cast< MIL_Agent_ABC* >( &transported ) );
    if( it == transportedPions_.end() || it->second.rRemainingWeight_ > 0. )
        return false;
    return true;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::SendFullState
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
void PHY_RoleAction_Transport::SendFullState( NET_ASN_MsgUnitAttributes& msg ) const
{
    msg().m.pions_transportesPresent = 1;

    if( transportedPions_.empty() )
    {
        msg().pions_transportes.n = 0;
        return;
    }

    ASN1T_OID* pTransports = new ASN1T_OID[ transportedPions_.size() ];
    uint nNbrTransported = 0;
    for( CIT_TransportedPionMap it = transportedPions_.begin(); it != transportedPions_.end(); ++it )
    {
        if( it->second.rTransportedWeight_ > 0. )
        {            
            pTransports[ nNbrTransported ] = (*it->first).GetID();
            ++nNbrTransported;
        }
    }
    msg().pions_transportes.n    = nNbrTransported;
    msg().pions_transportes.elem = pTransports;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::SendChangedState
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
void PHY_RoleAction_Transport::SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const
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
    if( !bLoadUnloadHasBeenUpdated_ )
        nState_ = eNothing;

    if( HasChanged() )
    {
        transporter_.Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
    }
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
    return rWeightTransported_ > 0.;
}

} //namespace transport
