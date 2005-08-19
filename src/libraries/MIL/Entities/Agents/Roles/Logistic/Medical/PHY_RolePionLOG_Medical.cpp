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

#include "MIL_pch.h"

#include "PHY_RolePionLOG_Medical.h"

#include "PHY_MedicalConsign_ABC.h"
#include "PHY_MedicalHumanState.h"
#include "PHY_MedicalCollectionAmbulance.h"
#include "PHY_MedicalEvacuationAmbulance.h"
#include "PHY_MedicalEvacuationConsign.h"
#include "PHY_MedicalCollectionConsign.h"
#include "PHY_MedicalSortingConsign.h"
#include "PHY_MedicalHealingConsign.h"
#include "Entities/Agents/Units/Logistic/PHY_LogWorkTime.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Agents/Units/Humans/PHY_Human.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Network/NET_ASN_Messages.h"

BOOST_CLASS_EXPORT_GUID( PHY_RolePionLOG_Medical, "PHY_RolePionLOG_Medical" )

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePionLOG_Medical::PHY_RolePionLOG_Medical( MT_RoleContainer& role, MIL_AgentPionLOG_ABC& pion )
    : PHY_RolePion_Medical ( role )
    , pPion_               ( &pion )
    , bHasChanged_         ( true )
    , bSystemEnabled_      ( false )
    , priorities_          ()
    , tacticalPriorities_  ()
    , consigns_            ()
    , evacuationAmbulances_()
    , collectionAmbulances_()
    , reservations_        ()
    , pWorkTime_           ( &PHY_LogWorkTime::time8Hours_ )
{
    priorities_.push_back( & PHY_HumanWound::woundedUE_  );
    priorities_.push_back( & PHY_HumanWound::woundedU1_  );
    priorities_.push_back( & PHY_HumanWound::woundedU2_  );
    priorities_.push_back( & PHY_HumanWound::woundedU3_  );
    priorities_.push_back( & PHY_HumanWound::notWounded_ );
    consigns_.push_back( std::make_pair( (const MIL_Automate*)0, T_MedicalConsignList() ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RolePionLOG_Medical::PHY_RolePionLOG_Medical()
    : PHY_RolePion_Medical ()
    , pPion_               ( 0 )
    , bHasChanged_         ( true )
    , bSystemEnabled_      ( false )
    , priorities_          ()
    , tacticalPriorities_  ()
    , consigns_            ()
    , evacuationAmbulances_()
    , collectionAmbulances_()
    , reservations_        ()
    , pWorkTime_           ( 0 )
{
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
        file << vector.size();
        for ( CIT_MedicalPriorityVector it = vector.begin(); it != vector.end(); ++it )
            file << (*it)->GetID();
    }

    template< typename Archive >
    void load( Archive& file, T_MedicalPriorityVector& vector, const uint )
    {
        uint nNbr;
        file >> nNbr;
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
        file << vector.size();
        for ( CIT_AutomateVector it = vector.begin(); it != vector.end(); ++it )
            file << *it;
    }
    
    template< typename Archive >
    void load( Archive& file, T_AutomateVector& vector, const uint )
    {
        uint nNbr;
        file >> nNbr;
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
        file << mmap.size();
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
        file << set.size();
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

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::load
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::load( MIL_CheckPointInArchive& file, const uint )
{
    ASN1T_EnumTempsBordee nID;

    file >> boost::serialization::base_object< PHY_RolePion_Medical >( *this )
         >> pPion_
         >> nID;
    pWorkTime_ = PHY_LogWorkTime::Find( nID );
    file >> priorities_
         >> tacticalPriorities_
         >> evacuationAmbulances_
         >> collectionAmbulances_
         >> reservations_;

    uint nNbr;
    file >> nNbr;
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
    file << boost::serialization::base_object< PHY_RolePion_Medical >( *this )
         << pPion_
         << pWorkTime_->GetAsnID()
         << priorities_
         << tacticalPriorities_
         << evacuationAmbulances_
         << collectionAmbulances_
         << reservations_;
         
    file << consigns_.size();
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

    PHY_ComposantePion* pCompAmbulance = GetRole< PHY_RolePion_Composantes >().GetAvailableEvacuationAmbulance();
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

    PHY_ComposantePion* pCompAmbulance = GetRole< PHY_RolePion_Composantes >().GetAvailableCollectionAmbulance();
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
    return GetRole< PHY_RolePion_Composantes >().GetAvailableDoctorForDiagnosing();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::GetAvailableDoctorForSorting
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RolePionLOG_Medical::GetAvailableDoctorForSorting() const
{
    return GetRole< PHY_RolePion_Composantes >().GetAvailableDoctorForSorting();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::GetAvailableDoctorForHealing
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RolePionLOG_Medical::GetAvailableDoctorForHealing( const PHY_Human& human ) const
{
    return GetRole< PHY_RolePion_Composantes >().GetAvailableDoctorForHealing( human );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::HasUsableEvacuationAmbulance
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Medical::HasUsableEvacuationAmbulance() const
{
    return GetRole< PHY_RolePion_Composantes >().HasUsableEvacuationAmbulance();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::HasUsableCollectionAmbulance
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Medical::HasUsableCollectionAmbulance() const
{
    return GetRole< PHY_RolePion_Composantes >().HasUsableCollectionAmbulance();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::HasUsableDoctorForSorting
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Medical::HasUsableDoctorForSorting() const
{
    return GetRole< PHY_RolePion_Composantes >().HasUsableDoctorForSorting();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::HasUsableDoctorForHealing
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Medical::HasUsableDoctorForHealing( const PHY_Human& human ) const
{
    return GetRole< PHY_RolePion_Composantes >().HasUsableDoctorForHealing( human );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::HasUsableDoctorForHealing
// Created: NLD 2005-01-13
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Medical::HasUsableDoctorForHealing() const
{
    return GetRole< PHY_RolePion_Composantes >().HasUsableDoctorForHealing();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::GetAutomate
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
MIL_AutomateLOG& PHY_RolePionLOG_Medical::GetAutomate() const
{
    assert( pPion_ );
    return pPion_->GetAutomate();
}

// =============================================================================
// CONSIGN MANAGEMENT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::ChangeWorkTime
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::ChangeWorkTime( const PHY_LogWorkTime& workTime )
{
    pWorkTime_   = &workTime;
    bHasChanged_ = true;
}

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

    consigns_.clear();
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
        if ( itTact->first == pAutomate )
            break;

    if ( itTact == consigns_.end() )
    {
        assert( !consigns_.empty() );
        itTact = consigns_.end() - 1;
        assert( itTact->first == 0 );
    }
    
    IT_MedicalPriorityVector itPriorityLowerBound = std::find( priorities_.begin(), priorities_.end(), &consign.GetHumanState().GetHuman().GetWound() );

    if ( itPriorityLowerBound == priorities_.end() )
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
   if( !bSystemEnabled_ || !HasUsableEvacuationAmbulance() )
        return 0;

    PHY_MedicalHumanState* pHumanState = new PHY_MedicalHumanState( pion, human );
    PHY_MedicalEvacuationConsign* pConsign = new PHY_MedicalEvacuationConsign( *this, *pHumanState );
    InsertConsign( *pConsign );
    return pHumanState;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::HandleHumanForCollection
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Medical::HandleHumanForCollection( PHY_MedicalHumanState& humanState )
{
    if( !bSystemEnabled_ || !HasUsableCollectionAmbulance() )
        return false;

    PHY_MedicalCollectionConsign* pConsign = new PHY_MedicalCollectionConsign( *this, humanState );
    InsertConsign( *pConsign );
    return true;
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
// Name: PHY_RolePionLOG_Medical::GetAvailablityScoreForSorting
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
int PHY_RolePionLOG_Medical::GetAvailablityScoreForSorting( const PHY_MedicalCollectionAmbulance& ambulance ) const
{
    if( !bSystemEnabled_ || !HasUsableDoctorForSorting() )
        return std::numeric_limits< int >::min();

    PHY_RolePion_Composantes::T_ComposanteUseMap composanteUse;
    GetRole< PHY_RolePion_Composantes >().GetDoctorsUseForSorting( composanteUse );

    uint nNbrDoctorsAvailable = 0;
    for( PHY_RolePion_Composantes::CIT_ComposanteUseMap it = composanteUse.begin(); it != composanteUse.end(); ++it )
        nNbrDoctorsAvailable += ( it->second.nNbrAvailable_ - it->second.nNbrUsed_ );

    int nScore = nNbrDoctorsAvailable - consigns_.size();
    if( HasUsableDoctorForHealing() )
    {
        if( ambulance.IsAnEmergency() )
            nScore += 100;
        else
            nScore -= 5;
    }

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
    if( !bSystemEnabled_ || !HasUsableDoctorForHealing( humanState.GetHuman() ) )
        return false;

    PHY_MedicalHealingConsign* pConsign = new PHY_MedicalHealingConsign( *this, humanState );
    InsertConsign( *pConsign );
    return true;
}

// =============================================================================
// UPDATE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::Update
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::Update( bool bIsDead )
{
    if( bIsDead )
        return;

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

    for ( IT_MedicalConsigns itConsigns = consigns_.begin(); itConsigns != consigns_.end(); ++itConsigns )
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
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::StartUsingForLogistic
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::StartUsingForLogistic( PHY_ComposantePion& composante )
{
    bHasChanged_ = true;
    composante.StartUsingForLogistic();
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
void SendComposanteUse( const PHY_RolePion_Composantes::T_ComposanteUseMap& data, ASN1T__SeqOfSanteDisponibiliteMoyens& asn )
{
    asn.n = data.size();
    if( data.empty() )
        return;

    ASN1T_SanteDisponibiliteMoyens* pData = new ASN1T_SanteDisponibiliteMoyens[ data.size() ];
    uint i = 0;
    for( PHY_RolePion_Composantes::CIT_ComposanteUseMap itData = data.begin(); itData != data.end(); ++itData )
    {
        ASN1T_SanteDisponibiliteMoyens& data = pData[ i++ ];
        data.type_equipement           = itData->first->GetMosID();
        assert( itData->second.nNbrAvailable_ );
        data.pourcentage_disponibilite = 100 - (uint)( itData->second.nNbrUsed_ * 100. / itData->second.nNbrAvailable_ );
    }
    asn.elem = pData;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::SendFullState
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::SendFullState() const
{
    NET_ASN_MsgLogSanteEtat asn;

    asn.GetAsnMsg().m.chaine_activeePresent                      = 1;
    asn.GetAsnMsg().m.temps_de_bordeePresent                     = 1;
    asn.GetAsnMsg().m.prioritesPresent                           = 1;
    asn.GetAsnMsg().m.priorites_tactiquesPresent                 = 1;
    asn.GetAsnMsg().m.disponibilites_ambulances_ramassagePresent = 1;
    asn.GetAsnMsg().m.disponibilites_ambulances_relevePresent    = 1;
    asn.GetAsnMsg().m.disponibilites_medecinsPresent             = 1;

    assert( pPion_ );
    asn.GetAsnMsg().oid_pion        = pPion_->GetID();
    asn.GetAsnMsg().chaine_activee  = bSystemEnabled_;
    asn.GetAsnMsg().temps_de_bordee = pWorkTime_->GetAsnID();

    asn.GetAsnMsg().priorites.n = priorities_.size();
    if( !priorities_.empty() )
    {
        ASN1T_EnumHumanWound* pAsnPriorities = new ASN1T_EnumHumanWound[ priorities_.size() ];
        uint i = 0 ;
        for( CIT_MedicalPriorityVector itPriority = priorities_.begin(); itPriority != priorities_.end(); ++itPriority )
            pAsnPriorities[ i++ ] = (**itPriority).GetAsnID();
        asn.GetAsnMsg().priorites.elem = pAsnPriorities;
    }

    asn.GetAsnMsg().priorites_tactiques.n = tacticalPriorities_.size();
    if( !tacticalPriorities_.empty() )
    {
        ASN1T_Automate* pAsnPriorities = new ASN1T_Automate[ tacticalPriorities_.size() ];
        uint i = 0 ;
        for( CIT_AutomateVector itPriority = tacticalPriorities_.begin(); itPriority != tacticalPriorities_.end(); ++itPriority )
            pAsnPriorities[ i++ ] = (**itPriority).GetID();
        asn.GetAsnMsg().priorites_tactiques.elem = pAsnPriorities;
    }

    PHY_RolePion_Composantes::T_ComposanteUseMap composanteUse;
    GetRole< PHY_RolePion_Composantes >().GetEvacuationAmbulancesUse( composanteUse );
    SendComposanteUse( composanteUse, asn.GetAsnMsg().disponibilites_ambulances_releve );

    composanteUse.clear();
    GetRole< PHY_RolePion_Composantes >().GetCollectionAmbulancesUse( composanteUse );
    SendComposanteUse( composanteUse, asn.GetAsnMsg().disponibilites_ambulances_ramassage );

    composanteUse.clear();
    GetRole< PHY_RolePion_Composantes >().GetDoctorsUse( composanteUse );
    SendComposanteUse( composanteUse, asn.GetAsnMsg().disponibilites_medecins );

    asn.Send();

    if( asn.GetAsnMsg().priorites.n > 0 )
        delete [] asn.GetAsnMsg().priorites.elem;
    if( asn.GetAsnMsg().priorites_tactiques.n > 0 )
        delete [] asn.GetAsnMsg().priorites_tactiques.elem;
    if( asn.GetAsnMsg().disponibilites_ambulances_ramassage.n > 0 )
        delete [] asn.GetAsnMsg().disponibilites_ambulances_ramassage.elem;
    if( asn.GetAsnMsg().disponibilites_ambulances_releve.n > 0 )
        delete [] asn.GetAsnMsg().disponibilites_ambulances_releve.elem;
    if( asn.GetAsnMsg().disponibilites_medecins.n > 0 )
        delete [] asn.GetAsnMsg().disponibilites_medecins.elem;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::SendChangedState
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Medical::SendChangedState() const
{
    if( bHasChanged_ || GetRole< PHY_RolePion_Composantes >().HasChanged() )
        SendFullState();
}
