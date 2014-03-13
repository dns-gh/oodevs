// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_RolePionLOG_Medical.cpp $
// $Author: Jvt $
// $Modtime: 4/05/05 10:51 $
// $Revision: 11 $
// $Workfile: PHY_RolePionLOG_Medical.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "protocol/Protocol.h"
#include "PHY_RolePionLOG_Medical.h"
#include "PHY_MedicalConsign_ABC.h"
#include "PHY_MedicalHumanState.h"
#include "PHY_MedicalCollectionAmbulance.h"
#include "PHY_MedicalEvacuationAmbulance.h"
#include "PHY_MedicalEvacuationConsign.h"
#include "PHY_MedicalCollectionConsign.h"
#include "PHY_MedicalSortingConsign.h"
#include "PHY_MedicalHealingConsign.h"
#include "PHY_MedicalResourcesAlarms.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Agents/Units/Humans/Human_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_ComposantePredicates.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "simulation_kernel/DefaultComponentFunctorComputer.h"
#include "simulation_kernel/DefaultComponentLendedFunctorComputer.h"
#include "simulation_kernel/NetworkNotificationHandler_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include <boost/serialization/shared_ptr.hpp>
#include <boost/ptr_container/serialize_ptr_list.hpp>
#include <boost/ptr_container/serialize_ptr_map.hpp>
#include <boost/range/algorithm_ext/erase.hpp>
#include <boost/make_shared.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_RolePionLOG_Medical )

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical constructor
// Created: JSR 2013-01-08
// -----------------------------------------------------------------------------
PHY_RolePionLOG_Medical::PHY_RolePionLOG_Medical()
    : owner_                   ( 0 )
    , bHasChanged_             ( true )
    , bExternalMustChangeState_( false )
    , bSystemEnabled_          ( false )
    , bSortingFunctionEnabled_ ( false )
    , bHealingFunctionEnabled_ ( false )
{
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePionLOG_Medical::PHY_RolePionLOG_Medical( MIL_AgentPionLOG_ABC& pion )
    : owner_                   ( &pion )
    , bHasChanged_             ( true )
    , bExternalMustChangeState_( false )
    , bSystemEnabled_          ( false )
    , bSortingFunctionEnabled_ ( false )
    , bHealingFunctionEnabled_ ( false )
{
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::Initialize
// Created: LDC 2013-01-09
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::Initialize()
{
    priorities_.push_back( &PHY_HumanWound::woundedUE_ );
    priorities_.push_back( &PHY_HumanWound::woundedU1_ );
    priorities_.push_back( &PHY_HumanWound::woundedU2_ );
    priorities_.push_back( &PHY_HumanWound::woundedU3_ );
    priorities_.push_back( &PHY_HumanWound::notWounded_ );
    consigns_.push_back( T_MedicalConsigns::value_type() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePionLOG_Medical::~PHY_RolePionLOG_Medical()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::load
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< PHY_RoleInterface_Medical >( *this );
    file >> owner_;
    file >> bSystemEnabled_;
    file >> bSortingFunctionEnabled_;
    file >> bHealingFunctionEnabled_;
    file >> priorities_;
    file >> tacticalPriorities_;
    file >> evacuationAmbulances_;
    file >> collectionAmbulances_;
    file >> reservations_;
    file >> consigns_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::save
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< PHY_RoleInterface_Medical >( *this );
    file << owner_;
    file << bSystemEnabled_;
    file << bSortingFunctionEnabled_;
    file << bHealingFunctionEnabled_;
    file << priorities_;
    file << tacticalPriorities_;
    file << evacuationAmbulances_;
    file << collectionAmbulances_;
    file << reservations_;
    file << consigns_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::FindLogisticManager
// Created: NLD 2011-01-11
// -----------------------------------------------------------------------------
MIL_AutomateLOG* PHY_RolePionLOG_Medical::FindLogisticManager() const
{
    return owner_->FindLogisticManager();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::CanCollectionAmbulanceGo
// Created: NLD 2005-01-13
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Medical::CanCollectionAmbulanceGo( const PHY_MedicalCollectionAmbulance& ambulance ) const
{
    if( ambulance.IsAnEmergency() )
        return true;
    for( auto it = consigns_.begin(); it != consigns_.end(); ++it )
        for( auto it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
            if( (*it2)->CouldNeedCollectionAmbulance() )
                return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::GetAvailableEvacuationAmbulance
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
PHY_MedicalEvacuationAmbulance* PHY_RolePionLOG_Medical::GetAvailableEvacuationAmbulance( PHY_MedicalEvacuationConsign& consign )
{
    if( !consign.HasValidHumanState() )
        return 0;
    const MIL_Automate& humanAutomate = consign.GetHumanState().GetAutomate();
    auto range = evacuationAmbulances_.equal_range( humanAutomate.GetID() );
    for( auto it = range.begin(); it != range.end(); ++it )
        if( it->second->RegisterHuman( consign ) )
            return it->second;
    PHY_ComposantePredicate1< Human_ABC > predicate( &PHY_ComposantePion::CanEvacuateCasualty, consign.GetHumanState().GetHuman() );
    GetComponentFunctor functor( predicate );
    DefaultComponentFunctorComputer computer( functor );
    owner_->Execute< OnComponentComputer_ABC >( computer );
    PHY_ComposantePion* pCompAmbulance = functor.result_;
    if( !pCompAmbulance )
        return 0;
    std::auto_ptr< PHY_MedicalEvacuationAmbulance > pAmbulance( new PHY_MedicalEvacuationAmbulance( *this, *pCompAmbulance ) );
    if( ! pAmbulance->RegisterHuman( consign ) )
        MT_LOG_ERROR_MSG( __FUNCTION__ << ": Register failed" );
    return evacuationAmbulances_.insert( humanAutomate.GetID(), pAmbulance )->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::GetAvailableCollectionAmbulance
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
PHY_MedicalCollectionAmbulance* PHY_RolePionLOG_Medical::GetAvailableCollectionAmbulance( PHY_MedicalCollectionConsign& consign )
{
    for( auto it = collectionAmbulances_.begin(); it != collectionAmbulances_.end(); ++it )
        if( it->RegisterHuman( consign ) )
            return &*it;
    if( !consign.HasValidHumanState() )
        return 0;
    PHY_ComposantePredicate1< Human_ABC > predicate( &PHY_ComposantePion::CanCollectCasualty, consign.GetHumanState().GetHuman() );
    GetComponentFunctor functor( predicate );
    DefaultComponentFunctorComputer computer( functor );
    owner_->Execute< OnComponentComputer_ABC >( computer );
    PHY_ComposantePion* pCompAmbulance = functor.result_;
    if( !pCompAmbulance )
        return 0;
    PHY_MedicalCollectionAmbulance* pAmbulance = new PHY_MedicalCollectionAmbulance( *this, *pCompAmbulance );
    if( ! pAmbulance->RegisterHuman( consign ) )
        MT_LOG_ERROR_MSG( __FUNCTION__ << ": Register failed" );
    collectionAmbulances_.push_back( pAmbulance );
    return pAmbulance;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::GetAvailableDoctorForDiagnosing
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RolePionLOG_Medical::GetAvailableDoctorForDiagnosing() const
{
    PHY_ComposantePredicate predicate( &PHY_ComposantePion::CanDiagnoseHumans );
    GetComponentFunctor functor( predicate );
    DefaultComponentFunctorComputer computer( functor );
    owner_->Execute< OnComponentComputer_ABC >( computer );
    return functor.result_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::GetAvailableDoctorForSorting
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RolePionLOG_Medical::GetAvailableDoctorForSorting() const
{
    PHY_ComposantePredicate predicate( &PHY_ComposantePion::CanSortHumans );
    GetComponentFunctor functor( predicate );
    DefaultComponentFunctorComputer computer( functor );
    owner_->Execute< OnComponentComputer_ABC >( computer );
    return functor.result_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::GetAvailableDoctorForHealing
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RolePionLOG_Medical::GetAvailableDoctorForHealing( const Human_ABC& human ) const
{
    PHY_ComposantePredicate1< Human_ABC > predicate( &PHY_ComposantePion::CanHealHuman, human );
    GetComponentFunctor functor( predicate );
    DefaultComponentFunctorComputer computer( functor );
    owner_->Execute< OnComponentComputer_ABC >( computer );
    return functor.result_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::HasUsableEvacuationAmbulance
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Medical::HasUsableEvacuationAmbulance( const Human_ABC& human ) const
{
    PHY_ComposanteTypePredicate1< Human_ABC > predicate( &PHY_ComposanteTypePion::CanEvacuateCasualty, human );
    HasUsableComponentFunctor functor( predicate );
    DefaultComponentFunctorComputer computer( functor );
    owner_->Execute< OnComponentComputer_ABC >( computer );
    return functor.result_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::HasUsableCollectionAmbulance
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Medical::HasUsableCollectionAmbulance( const Human_ABC& human ) const
{
    PHY_ComposanteTypePredicate1< Human_ABC > predicate( &PHY_ComposanteTypePion::CanCollectCasualty, human );
    HasUsableComponentFunctor functor( predicate );
    DefaultComponentFunctorComputer computer( functor );
    owner_->Execute< OnComponentComputer_ABC >( computer );
    return functor.result_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::HasUsableDoctorForSorting
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Medical::HasUsableDoctorForSorting() const
{
    PHY_ComposanteTypePredicate predicate( &PHY_ComposanteTypePion::CanSortHumans );
    HasUsableComponentFunctor functor( predicate );
    DefaultComponentFunctorComputer computer( functor );
    owner_->Execute< OnComponentComputer_ABC >( computer );
    return functor.result_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::HasUsableDoctorForHealing
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Medical::HasUsableDoctorForHealing( const Human_ABC& human, bool bBypassPriorities /* = false */ ) const
{
    if( !bBypassPriorities && human.IsWounded() && std::find( priorities_.begin(), priorities_.end(), &human.GetWound() ) == priorities_.end() )
        return false;
    PHY_ComposanteTypePredicate1< Human_ABC > predicate( &PHY_ComposanteTypePion::CanHealHuman, human );
    HasUsableComponentFunctor functor( predicate );
    DefaultComponentFunctorComputer computer( functor );
    owner_->Execute< OnComponentComputer_ABC >( computer );
    return functor.result_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::InsertConsigns
// Created: JVT 2005-05-03
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::InsertConsigns( const T_MedicalConsigns& oldConsigns )
{
    for ( auto it = oldConsigns.begin(); it != oldConsigns.end(); ++it )
        for ( auto it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
            InsertConsign( *it2 );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::ChangePriorities
// Created: NLD 2005-04-27
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::ChangePriorities( const T_AutomateVector& priorities )
{
    T_MedicalConsigns oldConsigns = consigns_;
    consigns_.clear(); consigns_.reserve( priorities.size() + 1 );
    for( auto it = priorities.begin(); it != priorities.end(); ++it )
        consigns_.push_back( std::make_pair( *it, T_MedicalConsignList() ) );
    consigns_.push_back( std::make_pair( (const MIL_Automate*)0, T_MedicalConsignList() ) );
    tacticalPriorities_ = priorities;
    bHasChanged_ = true;
    InsertConsigns( oldConsigns );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::GetAutomatePriorities
// Created: LGY 2012-06-21
// -----------------------------------------------------------------------------
PHY_RolePionLOG_Medical::T_AutomateVector PHY_RolePionLOG_Medical::GetAutomatePriorities() const
{
    return tacticalPriorities_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::ChangePriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::ChangePriorities( const T_MedicalPriorityVector& priorities )
{
    T_MedicalConsigns oldConsigns = consigns_;
    for ( auto it = consigns_.begin(); it != consigns_.end(); ++it )
        it->second.clear();
    priorities_ = priorities;
    bHasChanged_ = true;
    InsertConsigns( oldConsigns );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::GetMedicalPriorities
// Created: LGY 2012-06-21
// -----------------------------------------------------------------------------
PHY_RolePionLOG_Medical::T_MedicalPriorityVector PHY_RolePionLOG_Medical::GetMedicalPriorities() const
{
    return priorities_;
}

namespace
{
    bool IsPriorityEqual( const boost::shared_ptr< PHY_MedicalConsign_ABC >& consign, const PHY_HumanWound* pWound )
    {
        if( consign->IsFinished() || ! consign->HasValidHumanState() )
            return false;
        return *pWound == consign->GetHumanState().GetHuman().GetWound();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::InsertConsign
// Created: JVT 2005-05-03
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::InsertConsign( const boost::shared_ptr< PHY_MedicalConsign_ABC >& consign )
{
    if( !consign->HasValidHumanState() )
        return;
    auto itTact = consigns_.begin();
    const MIL_Automate& automate = consign->GetHumanState().GetAutomate();
    for( ; itTact != consigns_.end(); ++itTact )
        if( &automate == itTact->first ) // TODO AHC || ( pAutomate->GetTC2() && pAutomate->GetTC2() == itTact->first ) )
            break;
    if( itTact == consigns_.end() )
    {
        assert( !consigns_.empty() );
        itTact = consigns_.end() - 1;
        assert( itTact->first == 0 );
    }
    auto itPriority = std::find( priorities_.begin(), priorities_.end(), &consign->GetHumanState().GetHuman().GetWound() );
    if( itPriority == priorities_.end() )
        itTact->second.push_back( consign );
    else
    {
        ++itPriority;
        auto itConsign = std::find_first_of( itTact->second.rbegin(), itTact->second.rend(), priorities_.begin(), itPriority, &IsPriorityEqual );
        itTact->second.insert( itConsign.base(), consign );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::HandleHumanEvacuatedByThirdParty
// Created: NLD 2005-08-01
// -----------------------------------------------------------------------------
boost::shared_ptr< PHY_MedicalHumanState > PHY_RolePionLOG_Medical::HandleHumanEvacuatedByThirdParty( MIL_AgentPion& pion, Human_ABC& human )
{
    if( !bSystemEnabled_ )
        return boost::shared_ptr< PHY_MedicalHumanState >();
    auto humanState = boost::make_shared< PHY_MedicalHumanState >( boost::ref( pion ), boost::ref( human ), true ); // true is for 'evacuated by third party'
    InsertConsign( boost::make_shared< PHY_MedicalEvacuationConsign >( boost::ref( *owner_ ), boost::ref( *humanState ) ) );
    return humanState;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::HandleHumanForEvacuation
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
boost::shared_ptr< PHY_MedicalHumanState > PHY_RolePionLOG_Medical::HandleHumanForEvacuation( MIL_AgentPion& pion, Human_ABC& human )
{
    if( !bSystemEnabled_ || !HasUsableEvacuationAmbulance( human ) )
        return boost::shared_ptr< PHY_MedicalHumanState >();
    auto humanState = boost::make_shared< PHY_MedicalHumanState >( boost::ref( pion ), boost::ref( human ) );
    InsertConsign( boost::make_shared< PHY_MedicalEvacuationConsign >( boost::ref( *owner_ ), boost::ref( *humanState ) ) );
    return humanState;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::HandleHumanForEvacuation
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Medical::HandleHumanForEvacuation( PHY_MedicalHumanState& humanState )
{
    if( !bSystemEnabled_ || !HasUsableEvacuationAmbulance( humanState.GetHuman() ) )
        return false;
    InsertConsign( boost::make_shared< PHY_MedicalEvacuationConsign >( boost::ref( *owner_ ), boost::ref( humanState ) ) );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::ExecuteOnComponentsAndLendedComponents
// Created: LDC 2009-12-18
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::ExecuteOnComponentsAndLendedComponents( ComposanteUsePredicate_ABC& predicate, PHY_Composante_ABC::T_ComposanteUseMap& composanteUse ) const
{
    GetComponentUseFunctor functorOnComponent( predicate, composanteUse );
    DefaultComponentFunctorComputer componentComputer( functorOnComponent );
    owner_->Execute< OnComponentComputer_ABC >( componentComputer );
    GetComponentLendedUseFunctor functorOnLendedComponent( predicate, composanteUse );
    DefaultComponentLendedFunctorComputer lendedComputer( functorOnLendedComponent );
    owner_->Execute< OnComponentLendedFunctorComputer_ABC >( lendedComputer );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::GetAvailabilityScoreForEvacuation
// Created: NLD 2006-03-29
// -----------------------------------------------------------------------------
int PHY_RolePionLOG_Medical::GetAvailabilityScoreForEvacuation( const Human_ABC& human ) const
{
    if( !bSystemEnabled_ || !HasUsableEvacuationAmbulance( human ) )
        return std::numeric_limits< int >::min();
    PHY_Composante_ABC::T_ComposanteUseMap composanteUse;
    PHY_ComposanteUsePredicate1< Human_ABC > predicate( &PHY_ComposantePion::CanEvacuateCasualty, &PHY_ComposanteTypePion::CanEvacuateCasualty, human );
    ExecuteOnComponentsAndLendedComponents( predicate, composanteUse );
    if( composanteUse.size() == 0 )
        return std::numeric_limits< int >::min();
    unsigned int nNbrAvailableAllowedToWork = 0;
    for( auto it = composanteUse.begin(); it != composanteUse.end(); ++it )
        nNbrAvailableAllowedToWork += ( it->second.nNbrAvailable_ - it->second.nNbrUsed_ );
    return nNbrAvailableAllowedToWork;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::HandleHumanForCollection
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Medical::HandleHumanForCollection( PHY_MedicalHumanState& humanState )
{
    if( !bSystemEnabled_ || !HasUsableCollectionAmbulance( humanState.GetHuman() ) )
        return false;
    InsertConsign( boost::make_shared< PHY_MedicalCollectionConsign >( boost::ref( *owner_ ), boost::ref( humanState ) ) );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::GetAvailabilityScoreForCollection
// Created: NLD 2006-03-29
// -----------------------------------------------------------------------------
int PHY_RolePionLOG_Medical::GetAvailabilityScoreForCollection( const PHY_MedicalHumanState& humanState ) const
{
    if( !bSystemEnabled_ || !HasUsableCollectionAmbulance( humanState.GetHuman() ) )
        return std::numeric_limits< int >::min();
    PHY_Composante_ABC::T_ComposanteUseMap composanteUse;
    PHY_ComposanteUsePredicate1< Human_ABC > predicate( &PHY_ComposantePion::CanCollectCasualty, &PHY_ComposanteTypePion::CanCollectCasualty, humanState.GetHuman() );
    ExecuteOnComponentsAndLendedComponents( predicate, composanteUse );
    unsigned int nNbrAvailableAllowedToWork = 0;
    if( composanteUse.size() == 0 )
        return std::numeric_limits< int >::min();
    for( auto it = composanteUse.begin(); it != composanteUse.end(); ++it )
        nNbrAvailableAllowedToWork += ( it->second.nNbrAvailable_ - it->second.nNbrUsed_ );
    return nNbrAvailableAllowedToWork;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::HandleHumanForSorting
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::HandleHumanForSorting( PHY_MedicalHumanState& humanState )
{
    InsertConsign( boost::make_shared< PHY_MedicalSortingConsign >( boost::ref( *owner_ ), boost::ref( humanState ) ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::GetAvailabilityScoreForSorting
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
int PHY_RolePionLOG_Medical::GetAvailabilityScoreForSorting() const
{
    if( !bSystemEnabled_ || !bSortingFunctionEnabled_ || !HasUsableDoctorForSorting() )
        return std::numeric_limits< int >::min();
    PHY_Composante_ABC::T_ComposanteUseMap composanteUse;
    PHY_ComposanteUsePredicate predicate( &PHY_ComposantePion::CanSortHumans, &PHY_ComposanteTypePion::CanSortHumans );
    ExecuteOnComponentsAndLendedComponents( predicate, composanteUse );
    if( composanteUse.size() == 0 )
        return std::numeric_limits< int >::min();
    unsigned int nNbrDoctorsAvailable = 0;
    for( auto it = composanteUse.begin(); it != composanteUse.end(); ++it )
        nNbrDoctorsAvailable += ( it->second.nNbrAvailable_ - it->second.nNbrUsed_ );
    int nScore = nNbrDoctorsAvailable;
    for( auto it = reservations_.begin(); it != reservations_.end(); ++it )
        nScore -= (**it).GetNbrHumans();
    return nScore;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::HandleHumanForHealing
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Medical::HandleHumanForHealing( PHY_MedicalHumanState& humanState )
{
    if( !bSystemEnabled_ || !bHealingFunctionEnabled_ || !HasUsableDoctorForHealing( humanState.GetHuman() ) )
        return false;
    InsertConsign( boost::make_shared< PHY_MedicalHealingConsign >( boost::ref( *owner_ ), boost::ref( humanState ) ) );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::GetAvailabilityScoreForHealing
// Created: NLD 2005-11-18
// -----------------------------------------------------------------------------
int PHY_RolePionLOG_Medical::GetAvailabilityScoreForHealing( const PHY_MedicalHumanState& humanState ) const
{
    if( !bSystemEnabled_ || !bHealingFunctionEnabled_ || !HasUsableDoctorForHealing( humanState.GetHuman() ) )
        return std::numeric_limits< int >::min();
    PHY_Composante_ABC::T_ComposanteUseMap composanteUse;
    PHY_ComposanteUsePredicate1< Human_ABC > predicate( &PHY_ComposantePion::CanHealHuman, &PHY_ComposanteTypePion::CanHealHuman, humanState.GetHuman() );
    ExecuteOnComponentsAndLendedComponents( predicate, composanteUse );
    if( composanteUse.size() == 0 )
        return std::numeric_limits< int >::min();
    unsigned int nNbrAllowedToWork = 0;
    for( auto it = composanteUse.begin(); it != composanteUse.end(); ++it )
        nNbrAllowedToWork += ( it->second.nNbrAvailable_ - it->second.nNbrUsed_ );
    return nNbrAllowedToWork;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::Update
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::Update( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::UpdateLogistic
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::UpdateLogistic( bool /*bIsDead*/ )
{
    for( auto it = evacuationAmbulances_.begin(); it != evacuationAmbulances_.end(); )
        if( it->second->Update() )
            it = evacuationAmbulances_.erase( it );
        else
            ++it;
    for( auto it = collectionAmbulances_.begin(); it != collectionAmbulances_.end(); )
        if( it->Update() )
            it = collectionAmbulances_.erase( it );
        else
            ++it;
    for( auto itConsigns = consigns_.begin(); itConsigns != consigns_.end(); ++itConsigns )
        for ( auto itConsign = itConsigns->second.begin(); itConsign != itConsigns->second.end(); )
            if( (*itConsign)->Update() )
                itConsign = itConsigns->second.erase( itConsign );
            else
                ++itConsign;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::Clean
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::Clean()
{
    bHasChanged_ = false;
    bExternalMustChangeState_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::GetAvailabilityRatio
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
double PHY_RolePionLOG_Medical::GetAvailabilityRatio( PHY_ComposanteUsePredicate& predicate ) const
{
    unsigned int nNbrTotal = 0;
    unsigned int nNbrAvailableAllowedToWork = 0;
    PHY_Composante_ABC::T_ComposanteUseMap composanteUse;
    ExecuteOnComponentsAndLendedComponents( predicate, composanteUse );
    for( auto it = composanteUse.begin(); it != composanteUse.end(); ++it )
    {
        nNbrTotal += it->second.nNbrTotal_;
        nNbrAvailableAllowedToWork += ( it->second.nNbrAvailable_ - it->second.nNbrUsed_ );
    }
    if( nNbrTotal == 0 )
        return 1;
    return (double)nNbrAvailableAllowedToWork / (double)nNbrTotal;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::StartUsingForLogistic
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::StartUsingForLogistic( PHY_ComposantePion& composante )
{
    PHY_ComposanteUsePredicate evacuationUsePred( &PHY_ComposantePion::CanEvacuateCasualties, &PHY_ComposanteTypePion::CanEvacuateCasualties );
    PHY_ComposanteUsePredicate collectionUsePred( &PHY_ComposantePion::CanCollectCasualties , &PHY_ComposanteTypePion::CanCollectCasualties  );
    PHY_ComposanteUsePredicate doctorUsePred( &PHY_ComposantePion::CanDiagnoseHumans    , &PHY_ComposanteTypePion::CanDiagnoseHumans     );
    double rEvacuationRatio = GetAvailabilityRatio( evacuationUsePred );
    double rCollectionRatio = GetAvailabilityRatio( collectionUsePred );
    double rDoctorsRatio = GetAvailabilityRatio( doctorUsePred );
    bHasChanged_ = true;
    composante.StartUsingForLogistic();
    if( PHY_MedicalResourcesAlarms::IsEvacuationResourcesLevelReached( rEvacuationRatio, GetAvailabilityRatio( evacuationUsePred ) ) )
        MIL_Report::PostEvent<MIL_Agent_ABC>( *owner_, report::eRC_AlerteDisponibiliteMoyensReleve );
    if( PHY_MedicalResourcesAlarms::IsCollectionResourcesLevelReached( rCollectionRatio, GetAvailabilityRatio( collectionUsePred ) ) )
        MIL_Report::PostEvent<MIL_Agent_ABC>( *owner_, report::eRC_AlerteDisponibiliteMoyensRamassage );
    if( PHY_MedicalResourcesAlarms::IsDoctorResourcesLevelReached( rDoctorsRatio, GetAvailabilityRatio( doctorUsePred ) ) )
        MIL_Report::PostEvent<MIL_Agent_ABC>( *owner_, report::eRC_AlerteDisponibiliteMedecins );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::StopUsingForLogistic
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::StopUsingForLogistic( PHY_ComposantePion& composante )
{
    bHasChanged_ = true;
    composante.StopUsingForLogistic();
}

namespace
{
    void SendComposanteUse( const PHY_Composante_ABC::T_ComposanteUseMap& data, sword::SeqOfLogMedicalEquipmentAvailability& asn )
    {
        for( auto itData = data.begin(); itData != data.end(); ++itData )
        {
            sword::LogMedicalEquipmentAvailability& data = *asn.add_elem();
            data.mutable_equipment()->set_id( itData->first->GetMosID().id() );
            assert( itData->second.nNbrTotal_ );
            data.set_total( itData->second.nNbrTotal_ );
            data.set_working( itData->second.nNbrUsed_ );
            data.set_available( itData->second.nNbrAvailable_ - itData->second.nNbrUsed_ ); // nNbrAvailableAllowedToWork
            data.set_lent( itData->second.nNbrLent_ );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::SendFullState
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::SendFullState( unsigned int context ) const
{
    client::LogMedicalState asn;
    asn().mutable_unit()->set_id( owner_->GetID() );
    asn().set_chain( bSystemEnabled_ );
    for( auto itPriority = priorities_.begin(); itPriority != priorities_.end(); ++itPriority )
        asn().mutable_priorities()->add_elem( (**itPriority).GetAsnID() );
    for( auto itPriority = tacticalPriorities_.begin(); itPriority != tacticalPriorities_.end(); ++itPriority )
        asn().mutable_tactical_priorities()->add_elem()->set_id( (**itPriority).GetID() );
    PHY_Composante_ABC::T_ComposanteUseMap composanteUse;
    PHY_ComposanteUsePredicate predicate1( &PHY_ComposantePion::CanEvacuateCasualties, &PHY_ComposanteTypePion::CanEvacuateCasualties );
    ExecuteOnComponentsAndLendedComponents( predicate1, composanteUse );
    SendComposanteUse( composanteUse, *asn().mutable_evacuation_ambulances() );
    composanteUse.clear();
    PHY_ComposanteUsePredicate predicate2( &PHY_ComposantePion::CanCollectCasualties, &PHY_ComposanteTypePion::CanCollectCasualties );
    ExecuteOnComponentsAndLendedComponents( predicate2, composanteUse );
    SendComposanteUse( composanteUse, *asn().mutable_collection_ambulances() );
    composanteUse.clear();
    PHY_ComposanteUsePredicate predicate3( &PHY_ComposantePion::CanDiagnoseHumans, &PHY_ComposanteTypePion::CanDiagnoseHumans );
    ExecuteOnComponentsAndLendedComponents( predicate3, composanteUse );
    SendComposanteUse( composanteUse, *asn().mutable_doctors() );
    asn.Send( NET_Publisher_ABC::Publisher(), context );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::SendChangedState
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::SendChangedState() const
{
    if( bHasChanged_ || bExternalMustChangeState_ )
        SendFullState();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::NotifyHasChanged
// Created: MGD 2009-09-29
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::NotifyComponentHasChanged()
{
    bExternalMustChangeState_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::GetPion
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
const MIL_AgentPionLOG_ABC& PHY_RolePionLOG_Medical::GetPion() const
{
    return *owner_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::EnableHealingFunction
// Created: NLD 2006-08-01
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::EnableHealingFunction()
{
    bHealingFunctionEnabled_ = true;
    // bHasChanged_    = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::DisableHealingFunction
// Created: NLD 2006-08-01
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::DisableHealingFunction()
{
    bHealingFunctionEnabled_ = false;
    // bHasChanged_    = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::EnableSortingFunction
// Created: NLD 2006-08-01
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::EnableSortingFunction()
{
    bSortingFunctionEnabled_ = true;
    // bHasChanged_    = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::DisableSortingFunction
// Created: NLD 2006-08-01
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::DisableSortingFunction()
{
    bSortingFunctionEnabled_ = false;
    // bHasChanged_    = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::EnableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::EnableSystem()
{
    bSystemEnabled_ = true;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::DisableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::DisableSystem()
{
    bSystemEnabled_ = false;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::ReserveForSorting
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::ReserveForSorting( const PHY_MedicalCollectionAmbulance& ambulance )
{
    reservations_.push_back( &ambulance );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::CancelReservationForSorting
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::CancelReservationForSorting( const PHY_MedicalCollectionAmbulance& ambulance )
{
    boost::remove_erase( reservations_, &ambulance );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::FinishAllHandlingsSuccessfullyWithoutDelay
// Created: NLD 2012-01-09
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Medical::FinishAllHandlingsSuccessfullyWithoutDelay()
{
    for( auto it = evacuationAmbulances_.begin(); it != evacuationAmbulances_.end(); ++it )
        it->second->Cancel();
    for( auto it = collectionAmbulances_.begin(); it != collectionAmbulances_.end(); ++it )
        it->Cancel();
    bool handlings = false;
    for( auto itConsigns = consigns_.begin(); itConsigns != consigns_.end(); ++itConsigns )
        for( auto itConsign = itConsigns->second.begin(); itConsign != itConsigns->second.end(); ++itConsign )
        {
            (*itConsign)->FinishSuccessfullyWithoutDelay();
            handlings = true;
        }
    return handlings;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::ClearMedicalConsigns
// Created: JSR 2013-02-13
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::ClearMedicalConsigns()
{
    for( auto itConsigns = consigns_.begin(); itConsigns != consigns_.end(); ++itConsigns )
        for( auto itConsign = itConsigns->second.begin(); itConsign != itConsigns->second.end(); ++itConsign )
            ( *itConsign )->ClearConsign();
}
