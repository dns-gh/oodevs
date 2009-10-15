// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Medical/PHY_RolePionLOG_Medical.cpp $
// $Author: Jvt $
// $Modtime: 4/05/05 10:51 $
// $Revision: 11 $
// $Workfile: PHY_RolePionLOG_Medical.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

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
#include "Entities/Agents/Units/Humans/PHY_Human.h"
#include "Entities/Agents/Roles/Composantes/PHY_ComposantePredicates.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Network/NET_ASN_Messages.h"

#include "simulation_kernel/AlgorithmsFactories.h"
#include "simulation_kernel/OnComponentFunctorComputer_ABC.h"
#include "simulation_kernel/OnComponentFunctorComputerFactory_ABC.h"
#include "simulation_kernel/OnComponentLendedFunctorComputer_ABC.h"
#include "simulation_kernel/OnComponentLendedFunctorComputerFactory_ABC.h"

BOOST_CLASS_EXPORT_GUID( PHY_RolePionLOG_Medical, "PHY_RolePionLOG_Medical" )

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RolePionLOG_Medical* role, const unsigned int /*version*/ )
{
    MIL_AgentPionLOG_ABC* const pion = &role->pion_;
    archive << pion;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RolePionLOG_Medical* role, const unsigned int /*version*/ )
{
	MIL_AgentPionLOG_ABC* pion;
    archive >> pion;
    ::new( role )PHY_RolePionLOG_Medical( *pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePionLOG_Medical::PHY_RolePionLOG_Medical( MIL_AgentPionLOG_ABC& pion )
    : pion_                  ( pion )
    , bHasChanged_            ( true )
    , bExternalMustChangeState_ ( false )
    , bSystemEnabled_         ( false )
    , bSortingFunctionEnabled_( false )
    , bHealingFunctionEnabled_( false )
    , priorities_             ()
    , tacticalPriorities_     ()
    , consigns_               ()
    , evacuationAmbulances_   ()
    , collectionAmbulances_   ()
    , reservations_           ()
{
    priorities_.reserve( 5 );
    priorities_.push_back( & PHY_HumanWound::woundedUE_  );
    priorities_.push_back( & PHY_HumanWound::woundedU1_  );
    priorities_.push_back( & PHY_HumanWound::woundedU2_  );
    priorities_.push_back( & PHY_HumanWound::woundedU3_  );
    priorities_.push_back( & PHY_HumanWound::notWounded_ );
    consigns_.push_back( std::make_pair( (const MIL_Automate*)0, T_MedicalConsignList() ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePionLOG_Medical::~PHY_RolePionLOG_Medical()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
namespace boost
{
    namespace serialization
    {
        // =============================================================================
        // T_MedicalPriorityVector
        // =============================================================================
        template< typename Archive >
        inline
        void serialize( Archive& file, T_MedicalPriorityVector& vector, const uint nVersion )
        {
            split_free( file, vector, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const T_MedicalPriorityVector& vector, const uint )
        {
            unsigned size = vector.size();
            file << size;
            for ( CIT_MedicalPriorityVector it = vector.begin(); it != vector.end(); ++it )
            {
                unsigned id = (*it)->GetID();
                file << id;
            }
        }

        template< typename Archive >
        void load( Archive& file, T_MedicalPriorityVector& vector, const uint )
        {
            uint nNbr;
            file >> nNbr;
            vector.reserve( nNbr );
            while ( nNbr-- )
            {
                uint nID;
                file >> nID;
                vector.push_back( PHY_HumanWound::Find( nID ) );
            }
        }

        // =============================================================================
        // T_AutomateVector
        // =============================================================================
        template< typename Archive >
        inline
        void serialize( Archive& file, T_AutomateVector& vector, const uint nVersion )
        {
            split_free( file, vector, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const T_AutomateVector& vector, const uint )
        {
            unsigned size = vector.size();
            file << size;
            for ( CIT_AutomateVector it = vector.begin(); it != vector.end(); ++it )
                file << *it;
        }
        
        template< typename Archive >
        void load( Archive& file, T_AutomateVector& vector, const uint )
        {
            uint nNbr;
            file >> nNbr;
            vector.reserve( nNbr );
            while ( nNbr-- )
            {
                MIL_Automate* pAutomate;
                file >> pAutomate;
                vector.push_back( pAutomate );
            }
        }
         
        // =============================================================================
        // T_EvacuationAmbulancesMMap
        // =============================================================================
        template< typename Archive >
        inline
        void serialize( Archive& file, PHY_RolePionLOG_Medical::T_EvacuationAmbulancesMMap& mmap, const uint nVersion )
        {
            split_free( file, mmap, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const PHY_RolePionLOG_Medical::T_EvacuationAmbulancesMMap& mmap, const uint )
        {
            unsigned size = mmap.size();
            file << size;
            for ( PHY_RolePionLOG_Medical::CIT_EvacuationAmbulancesMMap it = mmap.begin(); it != mmap.end(); ++it )
            {
                file << it->first;
                file << it->second;
            }
        }
        
        template< typename Archive >
        void load( Archive& file, PHY_RolePionLOG_Medical::T_EvacuationAmbulancesMMap& mmap, const uint )
        {
            uint nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                MIL_Automate*                   pAutomate;
                PHY_MedicalEvacuationAmbulance* pAmbulance;
                
                file >> pAutomate;
                file >> pAmbulance;
                
                mmap.insert( std::make_pair( pAutomate, pAmbulance ) );
            }
        }

        // =============================================================================
        // T_CollectionAmbulancesSet
        // =============================================================================
        template< typename Archive >
        inline
        void serialize( Archive& file, PHY_RolePionLOG_Medical::T_CollectionAmbulancesSet& set, const uint nVersion )
        {
            split_free( file, set, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const PHY_RolePionLOG_Medical::T_CollectionAmbulancesSet& set, const uint )
        {
            unsigned size = set.size();
            file << size;
            for ( PHY_RolePionLOG_Medical::CIT_CollectionAmbulancesSet it = set.begin(); it != set.end(); ++it )
                file << *it;
        }
        
        template< typename Archive >
        void load( Archive& file, PHY_RolePionLOG_Medical::T_CollectionAmbulancesSet& set, const uint )
        {
            uint nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                PHY_MedicalCollectionAmbulance* pAmbulance;
                
                file >> pAmbulance;
                set.insert( pAmbulance );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::load
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< PHY_RoleInterface_Medical >( *this )
		 >> bSystemEnabled_
         >> bSortingFunctionEnabled_
         >> bHealingFunctionEnabled_
         >> priorities_
         >> tacticalPriorities_
         >> evacuationAmbulances_
         >> collectionAmbulances_
         >> reservations_;

    uint nNbr;
    file >> nNbr;
    consigns_.reserve( nNbr );
    while ( nNbr-- )
    {
        MIL_Automate* pAutomate;
        file >> pAutomate;

        consigns_.push_back( std::make_pair( pAutomate, T_MedicalConsignList() ) );

        file >> consigns_.back().second;
    }
}


// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::save
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< PHY_RoleInterface_Medical >( *this )
		 << bSystemEnabled_
         << bSortingFunctionEnabled_
         << bHealingFunctionEnabled_
         << priorities_
         << tacticalPriorities_
         << evacuationAmbulances_
         << collectionAmbulances_
         << reservations_;
         
    unsigned size = consigns_.size();
    file << size;
    for ( CIT_MedicalConsigns it = consigns_.begin(); it != consigns_.end(); ++it )
        file << it->first << it->second;
}


// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::CanCollectionAmbulanceGo
// Created: NLD 2005-01-13
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Medical::CanCollectionAmbulanceGo( const PHY_MedicalCollectionAmbulance& ambulance ) const
{
    if ( ambulance.IsAnEmergency() )
        return true;

    for ( CIT_MedicalConsigns it = consigns_.begin(); it != consigns_.end(); ++it )
        for ( CIT_MedicalConsignList it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
            if ( (*it2)->CouldNeedCollectionAmbulance() )
                return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::GetAvailableEvacuationAmbulance
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
PHY_MedicalEvacuationAmbulance* PHY_RolePionLOG_Medical::GetAvailableEvacuationAmbulance( PHY_MedicalEvacuationConsign& consign )
{
    const MIL_Automate& humanAutomate = consign.GetHumanState().GetAutomate();
    T_EvacuationAmbulancesMMapRange range = evacuationAmbulances_.equal_range( &humanAutomate );
    for( IT_EvacuationAmbulancesMMap itAmbulance = range.first; itAmbulance != range.second; ++itAmbulance )
    {
        PHY_MedicalEvacuationAmbulance& ambulance = *itAmbulance->second;
        if( ambulance.RegisterHuman( consign ) )
            return &ambulance;
    }

    PHY_ComposantePredicate1< PHY_Human > predicate( &PHY_ComposantePion::CanEvacuateCasualty, consign.GetHumanState().GetHuman() );
    GetComponentFunctor functor( predicate );
    pion_.Execute( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functor ) );
    PHY_ComposantePion* pCompAmbulance = functor.result_;
    if( !pCompAmbulance )
        return 0;

    PHY_MedicalEvacuationAmbulance* pAmbulance = new PHY_MedicalEvacuationAmbulance( *this, *pCompAmbulance );
    bool bOut = pAmbulance->RegisterHuman( consign );
    assert( bOut );
    evacuationAmbulances_.insert( std::make_pair( &humanAutomate, pAmbulance ) );
    return pAmbulance;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::GetAvailableCollectionAmbulance
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
PHY_MedicalCollectionAmbulance* PHY_RolePionLOG_Medical::GetAvailableCollectionAmbulance( PHY_MedicalCollectionConsign& consign )
{
    for( CIT_CollectionAmbulancesList itAmbulance = collectionAmbulances_.begin(); itAmbulance != collectionAmbulances_.end(); ++itAmbulance )
    {
        PHY_MedicalCollectionAmbulance& ambulance = **itAmbulance;
        if( ambulance.RegisterHuman( consign ) )
            return &ambulance;
    }


    PHY_ComposantePredicate1< PHY_Human > predicate( &PHY_ComposantePion::CanCollectCasualty, consign.GetHumanState().GetHuman() );
    GetComponentFunctor functor( predicate );
    pion_.Execute( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functor ) );
    PHY_ComposantePion* pCompAmbulance = functor.result_;
    if( !pCompAmbulance )
        return 0;

    PHY_MedicalCollectionAmbulance* pAmbulance = new PHY_MedicalCollectionAmbulance( *this, *pCompAmbulance );
    bool bOut = pAmbulance->RegisterHuman( consign );
    assert( bOut );
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
    pion_.Execute( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functor ) );
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
    pion_.Execute( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functor ) );
    return functor.result_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::GetAvailableDoctorForHealing
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RolePionLOG_Medical::GetAvailableDoctorForHealing( const PHY_Human& human ) const
{

    PHY_ComposantePredicate1< PHY_Human > predicate( &PHY_ComposantePion::CanHealHuman, human );
    GetComponentFunctor functor( predicate );
    pion_.Execute( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functor ) );
    return functor.result_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::HasUsableEvacuationAmbulance
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Medical::HasUsableEvacuationAmbulance( const PHY_Human& human ) const
{
    PHY_ComposanteTypePredicate1< PHY_Human > predicate( &PHY_ComposanteTypePion::CanEvacuateCasualty, human );
    HasUsableComponentFunctor functor( predicate );
    pion_.Execute( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functor ) );
    return functor.result_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::HasUsableCollectionAmbulance
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Medical::HasUsableCollectionAmbulance( const PHY_Human& human ) const
{

    PHY_ComposanteTypePredicate1< PHY_Human > predicate( &PHY_ComposanteTypePion::CanCollectCasualty, human );
    HasUsableComponentFunctor functor( predicate );
    pion_.Execute( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functor ) );
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
    pion_.Execute( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functor ) );
    return functor.result_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::HasUsableDoctorForHealing
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Medical::HasUsableDoctorForHealing( const PHY_Human& human, bool bBypassPriorities /* = false */ ) const
{
    if( !bBypassPriorities && human.IsWounded() && std::find( priorities_.begin(), priorities_.end(), &human.GetWound() ) == priorities_.end() )
        return false;


    PHY_ComposanteTypePredicate1< PHY_Human > predicate( &PHY_ComposanteTypePion::CanHealHuman, human );        
    HasUsableComponentFunctor functor( predicate );
    pion_.Execute( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functor ) );
    return functor.result_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::GetAutomate
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
MIL_AutomateLOG& PHY_RolePionLOG_Medical::GetAutomate() const
{

    return pion_.GetLogAutomate();
}

// =============================================================================
// CONSIGN MANAGEMENT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::InsertConsigns
// Created: JVT 2005-05-03
// -----------------------------------------------------------------------------
inline
void PHY_RolePionLOG_Medical::InsertConsigns( const T_MedicalConsigns& oldConsigns )
{
    for ( CIT_MedicalConsigns it = oldConsigns.begin(); it != oldConsigns.end(); ++it )
        for ( CIT_MedicalConsignList it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
            InsertConsign( **it2 );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::ChangePriorities
// Created: NLD 2005-04-27
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::ChangePriorities( const T_AutomateVector& priorities )
{
    T_MedicalConsigns oldConsigns = consigns_;

    consigns_.clear(); consigns_.reserve( priorities.size() + 1 );
    for ( CIT_AutomateVector it = priorities.begin(); it != priorities.end(); ++it )
        consigns_.push_back( std::make_pair( *it, T_MedicalConsignList() ) );
    consigns_.push_back( std::make_pair( (const MIL_Automate*)0, T_MedicalConsignList() ) );
    
    tacticalPriorities_ = priorities;
    bHasChanged_        = true;
    
    InsertConsigns( oldConsigns );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::ChangePriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::ChangePriorities( const T_MedicalPriorityVector& priorities )
{
    T_MedicalConsigns oldConsigns = consigns_;

    for ( IT_MedicalConsigns it = consigns_.begin(); it != consigns_.end(); ++it )
        it->second.clear();

    priorities_  = priorities;
    bHasChanged_ = true;    
    
    InsertConsigns( oldConsigns );
}

struct sIsPriorityEqual
{
    bool operator() ( const PHY_MedicalConsign_ABC* pConsign, const PHY_HumanWound* pWound )
    {
        if( pConsign->IsFinished() )
            return false;

        return *pWound == pConsign->GetHumanState().GetHuman().GetWound();
    }
};

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::InsertConsign
// Created: JVT 2005-05-03
// -----------------------------------------------------------------------------
inline
void PHY_RolePionLOG_Medical::InsertConsign( PHY_MedicalConsign_ABC& consign )
{
    IT_MedicalConsigns itTact = consigns_.begin();
    for ( const MIL_Automate* pAutomate = &consign.GetHumanState().GetAutomate(); itTact != consigns_.end(); ++itTact )
        if( pAutomate == itTact->first || ( pAutomate->GetTC2() && pAutomate->GetTC2() == itTact->first ) )
            break;

    if( itTact == consigns_.end() )
    {
        assert( !consigns_.empty() );
        itTact = consigns_.end() - 1;
        assert( itTact->first == 0 );
    }
    
    IT_MedicalPriorityVector itPriorityLowerBound = std::find( priorities_.begin(), priorities_.end(), &consign.GetHumanState().GetHuman().GetWound() );

    if( itPriorityLowerBound == priorities_.end() )
        itTact->second.push_back( &consign );
    else
    {
        ++itPriorityLowerBound;
        T_MedicalConsignList::reverse_iterator itConsign = std::find_first_of( itTact->second.rbegin(), itTact->second.rend(), priorities_.begin(), itPriorityLowerBound, sIsPriorityEqual() );
        itTact->second.insert( itConsign.base(), &consign );
    }
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::HandleHumanEvacuatedByThirdParty
// Created: NLD 2005-08-01
// -----------------------------------------------------------------------------
PHY_MedicalHumanState* PHY_RolePionLOG_Medical::HandleHumanEvacuatedByThirdParty( MIL_AgentPion& pion, PHY_Human& human )
{
    if( !bSystemEnabled_ )
        return 0;

    PHY_MedicalHumanState*        pHumanState = new PHY_MedicalHumanState       ( pion, human, true ); // true is for 'evacuated by third party'
    PHY_MedicalEvacuationConsign* pConsign    = new PHY_MedicalEvacuationConsign( *this, *pHumanState );
    InsertConsign( *pConsign );
    return pHumanState;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::HandleHumanForEvacuation
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
PHY_MedicalHumanState* PHY_RolePionLOG_Medical::HandleHumanForEvacuation( MIL_AgentPion& pion, PHY_Human& human )
{
    if( !bSystemEnabled_ || !HasUsableEvacuationAmbulance( human ) )
        return 0;

    PHY_MedicalHumanState* pHumanState = new PHY_MedicalHumanState( pion, human );
    PHY_MedicalEvacuationConsign* pConsign = new PHY_MedicalEvacuationConsign( *this, *pHumanState );
    InsertConsign( *pConsign );
    return pHumanState;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::GetAvailabilityScoreForEvacuation
// Created: NLD 2006-03-29
// -----------------------------------------------------------------------------
int PHY_RolePionLOG_Medical::GetAvailabilityScoreForEvacuation( const PHY_Human& human ) const
{
    if( !bSystemEnabled_ || !HasUsableEvacuationAmbulance( human ) )
        return std::numeric_limits< int >::min();


    PHY_Composante_ABC::T_ComposanteUseMap composanteUse;
    PHY_ComposanteUsePredicate1< PHY_Human > predicate( &PHY_ComposantePion::CanEvacuateCasualty, &PHY_ComposanteTypePion::CanEvacuateCasualty, human );
    GetComponentUseFunctor functorOnComponent( predicate, composanteUse );
    pion_.Execute( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functorOnComponent ) );
    GetComponentLendedUseFunctor functorOnLendedComponent( predicate, composanteUse );
    pion_.Execute( pion_.GetAlgorithms().onComponentLendedFunctorComputerFactory_->Create( functorOnLendedComponent ) );

    uint nNbrAvailableAllowedToWork = 0;
    for( PHY_Composante_ABC::CIT_ComposanteUseMap it = composanteUse.begin(); it != composanteUse.end(); ++it )
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

    PHY_MedicalCollectionConsign* pConsign = new PHY_MedicalCollectionConsign( *this, humanState );
    InsertConsign( *pConsign );
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
    PHY_ComposanteUsePredicate1< PHY_Human > predicate( &PHY_ComposantePion::CanCollectCasualty, &PHY_ComposanteTypePion::CanCollectCasualty, humanState.GetHuman() );
    GetComponentUseFunctor functorOnComponent( predicate, composanteUse );
    pion_.Execute( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functorOnComponent ) );
    GetComponentLendedUseFunctor functorOnLendedComponent( predicate, composanteUse );
    pion_.Execute( pion_.GetAlgorithms().onComponentLendedFunctorComputerFactory_->Create( functorOnLendedComponent ) ); 

    uint nNbrAvailableAllowedToWork = 0;
    for( PHY_Composante_ABC::CIT_ComposanteUseMap it = composanteUse.begin(); it != composanteUse.end(); ++it )
        nNbrAvailableAllowedToWork += ( it->second.nNbrAvailable_ - it->second.nNbrUsed_ );

    return nNbrAvailableAllowedToWork;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::HandleHumanForSorting
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::HandleHumanForSorting( const PHY_MedicalCollectionAmbulance& ambulance, PHY_MedicalHumanState& humanState )
{
    assert( reservations_.find( &ambulance ) != reservations_.end() );
    PHY_MedicalSortingConsign* pConsign = new PHY_MedicalSortingConsign( *this, humanState );
    InsertConsign( *pConsign );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::GetAvailabilityScoreForSorting
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
int PHY_RolePionLOG_Medical::GetAvailabilityScoreForSorting( const PHY_MedicalCollectionAmbulance& /*ambulance*/ ) const
{
    if( !bSystemEnabled_ || !bSortingFunctionEnabled_ || !HasUsableDoctorForSorting() )
        return std::numeric_limits< int >::min();

    PHY_Composante_ABC::T_ComposanteUseMap composanteUse;


    PHY_ComposanteUsePredicate predicate( &PHY_ComposantePion::CanSortHumans, &PHY_ComposanteTypePion::CanSortHumans );
    GetComponentUseFunctor functorOnComponent( predicate, composanteUse );
    pion_.Execute( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functorOnComponent ) );
    GetComponentLendedUseFunctor functorOnLendedComponent( predicate, composanteUse );
    pion_.Execute( pion_.GetAlgorithms().onComponentLendedFunctorComputerFactory_->Create( functorOnLendedComponent ) );

    uint nNbrDoctorsAvailable = 0;
    for( PHY_Composante_ABC::CIT_ComposanteUseMap it = composanteUse.begin(); it != composanteUse.end(); ++it )
        nNbrDoctorsAvailable += ( it->second.nNbrAvailable_ - it->second.nNbrUsed_ );

    int nScore = nNbrDoctorsAvailable;
    for( CIT_CollectionAmbulancesSet itReservation = reservations_.begin(); itReservation != reservations_.end(); ++itReservation )
        nScore -= (**itReservation).GetNbrHumans();

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

    PHY_MedicalHealingConsign* pConsign = new PHY_MedicalHealingConsign( *this, humanState );
    InsertConsign( *pConsign );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::GetAvailabilityScoreForHealing
// Created: NLD 2005-11-18
// -----------------------------------------------------------------------------
int PHY_RolePionLOG_Medical::GetAvailabilityScoreForHealing( const PHY_MedicalHumanState& humanState )
{
    if( !bSystemEnabled_ || !bHealingFunctionEnabled_ || !HasUsableDoctorForHealing( humanState.GetHuman() ) )
        return std::numeric_limits< int >::min();


    PHY_Composante_ABC::T_ComposanteUseMap composanteUse;
    PHY_ComposanteUsePredicate1< PHY_Human > predicate( &PHY_ComposantePion::CanHealHuman, &PHY_ComposanteTypePion::CanHealHuman, humanState.GetHuman() );
    GetComponentUseFunctor functorOnComponent( predicate, composanteUse );
    pion_.Execute( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functorOnComponent ) );
    GetComponentLendedUseFunctor functorOnLendedComponent( predicate, composanteUse );
    pion_.Execute( pion_.GetAlgorithms().onComponentLendedFunctorComputerFactory_->Create( functorOnLendedComponent ) );

    uint nNbrAllowedToWork = 0;
    for( PHY_Composante_ABC::CIT_ComposanteUseMap it = composanteUse.begin(); it != composanteUse.end(); ++it )
        nNbrAllowedToWork += ( it->second.nNbrAvailable_  - it->second.nNbrUsed_ );
    return nNbrAllowedToWork;
}

// =============================================================================
// UPDATE
// =============================================================================

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
    for( IT_EvacuationAmbulancesMMap itEvacuationAmbulance = evacuationAmbulances_.begin(); itEvacuationAmbulance != evacuationAmbulances_.end(); )
    {
        PHY_MedicalEvacuationAmbulance& ambulance = *itEvacuationAmbulance->second;
        if ( ambulance.Update() )
        {
            delete &ambulance;
            itEvacuationAmbulance = evacuationAmbulances_.erase( itEvacuationAmbulance );
        }
        else
            ++itEvacuationAmbulance;
    }

    for( IT_CollectionAmbulancesList itCollectionAmbulance = collectionAmbulances_.begin(); itCollectionAmbulance != collectionAmbulances_.end(); )
    {
        PHY_MedicalCollectionAmbulance& ambulance = **itCollectionAmbulance;
        if(  ambulance.Update() )
        {
            delete &ambulance;
            itCollectionAmbulance = collectionAmbulances_.erase( itCollectionAmbulance );
        }
        else
            ++itCollectionAmbulance;
    }

    for( IT_MedicalConsigns itConsigns = consigns_.begin(); itConsigns != consigns_.end(); ++itConsigns )
        for ( IT_MedicalConsignList itConsign = itConsigns->second.begin(); itConsign != itConsigns->second.end(); )
            if( (**itConsign).Update() )
            {
                delete *itConsign;
                itConsign = itConsigns->second.erase( itConsign );
            }
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

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::GetAvailabilityRatio
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
MT_Float PHY_RolePionLOG_Medical::GetAvailabilityRatio( PHY_ComposanteUsePredicate& predicate ) const
{

    PHY_Composante_ABC::T_ComposanteUseMap composanteUse;
    GetComponentUseFunctor functorOnComponent( predicate, composanteUse );
    pion_.Execute( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functorOnComponent ) );
    GetComponentLendedUseFunctor functorOnLendedComponent( predicate, composanteUse );
    pion_.Execute( pion_.GetAlgorithms().onComponentLendedFunctorComputerFactory_->Create( functorOnLendedComponent ) );

    uint nNbrTotal                  = 0;
    uint nNbrAvailableAllowedToWork = 0;
    for( PHY_Composante_ABC::CIT_ComposanteUseMap it = composanteUse.begin(); it != composanteUse.end(); ++it )
    {
        nNbrTotal                  += it->second.nNbrTotal_;
        nNbrAvailableAllowedToWork += ( it->second.nNbrAvailable_ - it->second.nNbrUsed_ );
    }

    if( nNbrTotal == 0 )
        return 1.;
    return (MT_Float)nNbrAvailableAllowedToWork / (MT_Float)nNbrTotal;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::StartUsingForLogistic
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::StartUsingForLogistic( PHY_ComposantePion& composante )
{
    PHY_ComposanteUsePredicate evacuationUsePred( &PHY_ComposantePion::CanEvacuateCasualties, &PHY_ComposanteTypePion::CanEvacuateCasualties );
    PHY_ComposanteUsePredicate collectionUsePred( &PHY_ComposantePion::CanCollectCasualties , &PHY_ComposanteTypePion::CanCollectCasualties  );
    PHY_ComposanteUsePredicate doctorUsePred    ( &PHY_ComposantePion::CanDiagnoseHumans    , &PHY_ComposanteTypePion::CanDiagnoseHumans     );

    MT_Float rEvacuationRatio = GetAvailabilityRatio( evacuationUsePred );
    MT_Float rCollectionRatio = GetAvailabilityRatio( collectionUsePred );
    MT_Float rDoctorsRatio    = GetAvailabilityRatio( doctorUsePred     );

    bHasChanged_ = true;
    composante.StartUsingForLogistic();

    if( PHY_MedicalResourcesAlarms::IsEvacuationResourcesLevelReached( rEvacuationRatio, GetAvailabilityRatio( evacuationUsePred ) ) )
        MIL_Report::PostEvent( pion_, MIL_Report::eReport_EvacuationResourcesLevelReached );
    if( PHY_MedicalResourcesAlarms::IsCollectionResourcesLevelReached( rCollectionRatio, GetAvailabilityRatio( collectionUsePred ) ) )
        MIL_Report::PostEvent( pion_, MIL_Report::eReport_CollectionResourcesLevelReached );
    if( PHY_MedicalResourcesAlarms::IsDoctorResourcesLevelReached( rDoctorsRatio, GetAvailabilityRatio( doctorUsePred ) ) )
        MIL_Report::PostEvent( pion_, MIL_Report::eReport_DoctorResourcesLevelReached );
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

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SendComposanteUse
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
static
void SendComposanteUse( const PHY_Composante_ABC::T_ComposanteUseMap& data, ASN1T__SeqOfLogMedicalEquipmentAvailability& asn )
{
    asn.n = data.size();
    if( data.empty() )
        return;

    ASN1T_LogMedicalEquipmentAvailability* pData = new ASN1T_LogMedicalEquipmentAvailability[ data.size() ];
    uint i = 0;
    for( PHY_Composante_ABC::CIT_ComposanteUseMap itData = data.begin(); itData != data.end(); ++itData )
    {
        ASN1T_LogMedicalEquipmentAvailability& data = pData[ i++ ];
        data.type_equipement = itData->first->GetMosID();
        assert( itData->second.nNbrTotal_ );

        data.nbr_total       = itData->second.nNbrTotal_;
        data.nbr_au_travail  = itData->second.nNbrUsed_;
        data.nbr_disponibles = itData->second.nNbrAvailable_ - itData->second.nNbrUsed_; // nNbrAvailableAllowedToWork
        data.nbr_pretes      = itData->second.nNbrLent_;
    }
    asn.elem = pData;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::SendFullState
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::SendFullState( NET_ASN_MsgUnitAttributes& asnUnit ) const
{
    UNREFERENCED_PARAMETER( asnUnit );

    NET_ASN_MsgLogMedicalState asn;

    asn().m.chaine_activeePresent                      = 1;
    asn().m.prioritesPresent                           = 1;
    asn().m.priorites_tactiquesPresent                 = 1;
    asn().m.disponibilites_ambulances_ramassagePresent = 1;
    asn().m.disponibilites_ambulances_relevePresent    = 1;
    asn().m.disponibilites_medecinsPresent             = 1;


    asn().oid_pion        = pion_.GetID();
    asn().chaine_activee  = bSystemEnabled_;

    asn().priorites.n = priorities_.size();
    if( !priorities_.empty() )
    {
        ASN1T_EnumHumanWound* pAsnPriorities = new ASN1T_EnumHumanWound[ priorities_.size() ];
        uint i = 0 ;
        for( CIT_MedicalPriorityVector itPriority = priorities_.begin(); itPriority != priorities_.end(); ++itPriority )
            pAsnPriorities[ i++ ] = (**itPriority).GetAsnID();
        asn().priorites.elem = pAsnPriorities;
    }

    asn().priorites_tactiques.n = tacticalPriorities_.size();
    if( !tacticalPriorities_.empty() )
    {
        ASN1T_Automat* pAsnPriorities = new ASN1T_Automat[ tacticalPriorities_.size() ];
        uint i = 0 ;
        for( CIT_AutomateVector itPriority = tacticalPriorities_.begin(); itPriority != tacticalPriorities_.end(); ++itPriority )
            pAsnPriorities[ i++ ] = (**itPriority).GetID();
        asn().priorites_tactiques.elem = pAsnPriorities;
    }

    PHY_Composante_ABC::T_ComposanteUseMap composanteUse;
    PHY_ComposanteUsePredicate predicate1( &PHY_ComposantePion::CanEvacuateCasualties, &PHY_ComposanteTypePion::CanEvacuateCasualties );
    GetComponentUseFunctor functorOnComponent( predicate1, composanteUse );
    pion_.Execute( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functorOnComponent ) );
    GetComponentLendedUseFunctor functorOnLendedComponent( predicate1, composanteUse );
    pion_.Execute( pion_.GetAlgorithms().onComponentLendedFunctorComputerFactory_->Create( functorOnLendedComponent ) );

    SendComposanteUse( composanteUse, asn().disponibilites_ambulances_releve );

    composanteUse.clear();
    PHY_ComposanteUsePredicate predicate2( &PHY_ComposantePion::CanCollectCasualties, &PHY_ComposanteTypePion::CanCollectCasualties );
    GetComponentUseFunctor functorOnComponent2( predicate2, composanteUse );
    pion_.Execute( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functorOnComponent2 ) );
    GetComponentLendedUseFunctor functorOnLendedComponent2( predicate2, composanteUse );
    pion_.Execute( pion_.GetAlgorithms().onComponentLendedFunctorComputerFactory_->Create( functorOnLendedComponent2 ) );

    SendComposanteUse( composanteUse, asn().disponibilites_ambulances_ramassage );

    composanteUse.clear();
    PHY_ComposanteUsePredicate predicate3( &PHY_ComposantePion::CanDiagnoseHumans, &PHY_ComposanteTypePion::CanDiagnoseHumans );
    GetComponentUseFunctor functorOnComponent3( predicate3, composanteUse );
    pion_.Execute( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functorOnComponent3 ) );
    GetComponentLendedUseFunctor functorOnLendedComponent3( predicate3, composanteUse );
    pion_.Execute( pion_.GetAlgorithms().onComponentLendedFunctorComputerFactory_->Create( functorOnLendedComponent3 ) );

    SendComposanteUse( composanteUse, asn().disponibilites_medecins );

    asn.Send();

    if( asn().priorites.n > 0 )
        delete [] asn().priorites.elem;
    if( asn().priorites_tactiques.n > 0 )
        delete [] asn().priorites_tactiques.elem;
    if( asn().disponibilites_ambulances_ramassage.n > 0 )
        delete [] asn().disponibilites_ambulances_ramassage.elem;
    if( asn().disponibilites_ambulances_releve.n > 0 )
        delete [] asn().disponibilites_ambulances_releve.elem;
    if( asn().disponibilites_medecins.n > 0 )
        delete [] asn().disponibilites_medecins.elem;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::SendChangedState
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::SendChangedState(  NET_ASN_MsgUnitAttributes& asnUnit ) const
{
    if( bHasChanged_ || bExternalMustChangeState_ )
        SendFullState( asnUnit );
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

    return pion_;
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
    bHasChanged_    = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::DisableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::DisableSystem()
{
    bSystemEnabled_ = false;
    bHasChanged_    = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::ReserveForSorting
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::ReserveForSorting( const PHY_MedicalCollectionAmbulance& ambulance )
{
    assert( reservations_.find( &ambulance ) == reservations_.end() );
    reservations_.insert( &ambulance );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::CancelReservationForSorting
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::CancelReservationForSorting( const PHY_MedicalCollectionAmbulance& ambulance )
{
    int nOut = reservations_.erase( &ambulance );
    assert( nOut == 1 );
}
