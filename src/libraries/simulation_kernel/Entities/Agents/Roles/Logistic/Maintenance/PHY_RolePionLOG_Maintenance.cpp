// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Maintenance/PHY_RolePionLOG_Maintenance.cpp $
// $Author: Jvt $
// $Modtime: 4/05/05 10:53 $
// $Revision: 13 $
// $Workfile: PHY_RolePionLOG_Maintenance.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RolePionLOG_Maintenance.h"
#include "PHY_MaintenanceConsign_ABC.h"
#include "PHY_MaintenanceRepairConsign.h"
#include "PHY_MaintenanceTransportConsign.h"
#include "PHY_MaintenanceComposanteState.h"
#include "PHY_MaintenanceResourcesAlarms.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Logistic/PHY_Breakdown.h"
#include "Entities/Agents/Units/Logistic/PHY_MaintenanceWorkRate.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Dotations/PHY_RolePion_Dotations.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Composantes/PHY_ComposantePredicates.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Network/NET_ASN_Messages.h"

BOOST_CLASS_EXPORT_GUID( PHY_RolePionLOG_Maintenance, "PHY_RolePionLOG_Maintenance" )

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePionLOG_Maintenance::PHY_RolePionLOG_Maintenance( MIL_AgentPionLOG_ABC& pion )
    : pPion_                  ( &pion )
    , bHasChanged_            ( true )
    , bSystemEnabled_         ( false )
    , priorities_             ()
    , tacticalPriorities_     ()
    , consigns_               ()
    , pWorkRate_              ( &PHY_MaintenanceWorkRate::r1_ )
    , nWorkRateWarningRCTick_ ( 0 )
{
    consigns_.push_back( std::make_pair( (const MIL_Automate*)0, T_MaintenanceConsignList() ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RolePionLOG_Maintenance::PHY_RolePionLOG_Maintenance()
: pPion_                  ( 0 )
    , bHasChanged_            ( true )
    , bSystemEnabled_         ( false )
    , priorities_             ()
    , tacticalPriorities_     ()
    , consigns_               ()
    , pWorkRate_              ( 0 )
    , nWorkRateWarningRCTick_ ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePionLOG_Maintenance::~PHY_RolePionLOG_Maintenance()
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
        // T_MaintenancePriorityVector
        // =============================================================================
        template< typename Archive > 
        inline
        void serialize( Archive& file, T_MaintenancePriorityVector& vector, const uint nVersion )
        {
            split_free( file, vector, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const T_MaintenancePriorityVector& vector, const uint )
        {
            unsigned size = vector.size();
            file << size;
            for ( CIT_MaintenancePriorityVector it = vector.begin(); it != vector.end(); ++it )
            {
                ASN1T_EquipmentType id = (*it)->GetMosID();
                file << id;
            }
        }
        
        template< typename Archive >
        void load( Archive& file, T_MaintenancePriorityVector& vector, const uint )
        {
            uint nNbr;
            file >> nNbr;
            vector.reserve( nNbr );
            while ( nNbr-- )
            {
                ASN1T_EquipmentType nID;
                file >> nID;
                vector.push_back( PHY_ComposanteTypePion::Find( nID ) );
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
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::load
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> pPion_
         >> bSystemEnabled_
         >> priorities_
         >> tacticalPriorities_;
         
    ASN1T_EnumLogMaintenanceRegimeTravail nID;
    file >> nID;
    pWorkRate_ = PHY_MaintenanceWorkRate::Find( nID );
    file >> nWorkRateWarningRCTick_;

    uint nNbr;
    file >> nNbr;
    consigns_.reserve( nNbr );
    while ( nNbr-- )
    {
        MIL_Automate* pAutomate;
        file >> pAutomate;

        consigns_.push_back( std::make_pair( pAutomate, T_MaintenanceConsignList() ) );

        file >> consigns_.back().second;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::save
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    ASN1T_EnumLogMaintenanceRegimeTravail workRate = pWorkRate_->GetAsnID();

    file << pPion_
         << bSystemEnabled_
         << priorities_
         << tacticalPriorities_
         << workRate
         << nWorkRateWarningRCTick_;

    unsigned size = consigns_.size();
    file << size;
    for ( CIT_MaintenanceConsigns it = consigns_.begin(); it != consigns_.end(); ++it )
        file << it->first << it->second;

}
          
// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::GetAvailabilityRatio
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
MT_Float PHY_RolePionLOG_Maintenance::GetAvailabilityRatio( PHY_ComposanteUsePredicate& predicate, const PHY_MaintenanceWorkRate* pWorkRate ) const
{
    assert (pPion_ );
    PHY_RolePion_Composantes::T_ComposanteUseMap composanteUse;
    pPion_->GetRole< PHY_RolePion_Composantes >().GetComposantesUse( composanteUse, predicate );

    uint nNbrTotal                  = 0;
    uint nNbrAvailableAllowedToWork = 0;
    for( PHY_RolePion_Composantes::CIT_ComposanteUseMap it = composanteUse.begin(); it != composanteUse.end(); ++it )
    {
        nNbrTotal                  += it->second.nNbrTotal_;
        if( pWorkRate )
        {
            const uint nNbrAllowedToWork = pWorkRate_->GetNbrWorkerAllowedToWork( it->second.nNbrAvailable_ );
            if( nNbrAllowedToWork > it->second.nNbrUsed_ )
                nNbrAvailableAllowedToWork += ( nNbrAllowedToWork - it->second.nNbrUsed_ );
        }
        else
            nNbrAvailableAllowedToWork += ( it->second.nNbrAvailable_ - it->second.nNbrUsed_ );
    }

    if( nNbrTotal == 0 )
        return 1.;
    return (MT_Float)nNbrAvailableAllowedToWork / (MT_Float)nNbrTotal;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::StartUsingForLogistic
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::StartUsingForLogistic( PHY_ComposantePion& composante )
{
    PHY_ComposanteUsePredicate repairerUsePred( &PHY_ComposantePion::CanRepair, &PHY_ComposanteTypePion::CanRepair );
    PHY_ComposanteUsePredicate haulerUsePred  ( &PHY_ComposantePion::CanHaul  , &PHY_ComposanteTypePion::CanHaul   );    

    MT_Float rRepairerRatio = GetAvailabilityRatio( repairerUsePred, pWorkRate_ );
    MT_Float rHaulerRatio   = GetAvailabilityRatio( haulerUsePred   );

    bHasChanged_ = true;
    composante.StartUsingForLogistic();

    if( PHY_MaintenanceResourcesAlarms::IsRepairerResourcesLevelReached( rRepairerRatio, GetAvailabilityRatio( repairerUsePred, pWorkRate_ ) ) )
        MIL_Report::PostEvent( *pPion_, MIL_Report::eReport_RepairerResourcesLevelReached );
    if( PHY_MaintenanceResourcesAlarms::IsHaulerResourcesLevelReached( rHaulerRatio, GetAvailabilityRatio( haulerUsePred ) ) )
        MIL_Report::PostEvent( *pPion_, MIL_Report::eReport_HaulerResourcesLevelReached );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::StopUsingForLogistic
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::StopUsingForLogistic( PHY_ComposantePion& composante )
{
    bHasChanged_ = true;
    composante.StopUsingForLogistic();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::GetAvailableHauler
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RolePionLOG_Maintenance::GetAvailableHauler( const PHY_ComposanteTypePion& composanteType ) const
{
    assert (pPion_ );
    return pPion_->GetRole< PHY_RolePion_Composantes >().GetAvailableHauler( composanteType );
}

// -----------------------------------------------------------------------------
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Maintenance::HasUsableHauler( const PHY_ComposanteTypePion& composanteType ) const
{
    assert (pPion_ );
    PHY_ComposanteTypePredicate1< PHY_ComposanteTypePion > predicate( &PHY_ComposanteTypePion::CanHaul, composanteType );
    return pPion_->GetRole< PHY_RolePion_Composantes >().HasUsableComposante( predicate );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::GetNbrAvailableRepairersAllowedToWork
// Created: NLD 2006-03-28
// -----------------------------------------------------------------------------
uint PHY_RolePionLOG_Maintenance::GetNbrAvailableRepairersAllowedToWork( const PHY_Breakdown& breakdown ) const
{
    assert (pPion_ );
    PHY_RolePion_Composantes::T_ComposanteUseMap composanteUse;
    PHY_ComposanteUsePredicate1< PHY_Breakdown > predicate( &PHY_ComposantePion::CanRepair, &PHY_ComposanteTypePion::CanRepair, breakdown );
    pPion_->GetRole< PHY_RolePion_Composantes >().GetComposantesUse( composanteUse, predicate );   

    uint nNbrAvailableAllowedToWork = 0;
    assert( pWorkRate_ );
    for( PHY_RolePion_Composantes::CIT_ComposanteUseMap it = composanteUse.begin(); it != composanteUse.end(); ++it )
    {
        const uint nNbrAllowedToWork = pWorkRate_->GetNbrWorkerAllowedToWork( it->second.nNbrAvailable_ );
        if( nNbrAllowedToWork > it->second.nNbrUsed_ )
            nNbrAvailableAllowedToWork += ( nNbrAllowedToWork - it->second.nNbrUsed_ );
    }

    return nNbrAvailableAllowedToWork;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::GetAvailableRepairer
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RolePionLOG_Maintenance::GetAvailableRepairer( const PHY_Breakdown& breakdown ) const
{
    assert (pPion_ );
    PHY_ComposantePredicate1< PHY_Breakdown > predicate( &PHY_ComposantePion::CanRepair, breakdown );
    PHY_ComposantePion* pRepairer = pPion_->GetRole< PHY_RolePion_Composantes >().GetComposante( predicate );
    if( pRepairer && GetNbrAvailableRepairersAllowedToWork( breakdown ) > 0 )
        return pRepairer;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::HasUsableRepairer
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Maintenance::HasUsableRepairer( const PHY_Breakdown& breakdown ) const
{
    assert (pPion_ );
    PHY_ComposanteTypePredicate1< PHY_Breakdown > predicate( &PHY_ComposanteTypePion::CanRepair, breakdown );
    return pPion_->GetRole< PHY_RolePion_Composantes >().HasUsableComposante( predicate );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::ConsumePartsForBreakdown
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Maintenance::ConsumePartsForBreakdown( const PHY_Breakdown& breakdown )
{
    assert (pPion_ );
    PHY_RolePion_Dotations& roleDotations = pPion_->GetRole< PHY_RolePion_Dotations >();
    const PHY_BreakdownType::T_PartMap& parts = breakdown.GetNeededParts();
    for( PHY_BreakdownType::CIT_PartMap it = parts.begin(); it != parts.end(); ++it )
    {
        if( roleDotations.GetDotationValue( *it->first ) < it->second )
            return false;
    }

    for( PHY_BreakdownType::CIT_PartMap it = parts.begin(); it != parts.end(); ++it )
    {
        uint nOut = (uint)roleDotations.ConsumeDotation( *it->first, it->second );
        assert( nOut == it->second );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::GetPosition
// Created: NLD 2004-12-24
// -----------------------------------------------------------------------------
const MT_Vector2D& PHY_RolePionLOG_Maintenance::GetPosition() const
{
    assert (pPion_ );
    return pPion_->GetRole< PHY_RolePion_Location >().GetPosition();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::GetAutomate
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
MIL_AutomateLOG& PHY_RolePionLOG_Maintenance::GetAutomate() const
{
    assert( pPion_ );
    return pPion_->GetLogAutomate();
}
    
// =============================================================================
// CONSIGN MANAGEMENT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::ChangeWorkRate
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::ChangeWorkRate( const PHY_MaintenanceWorkRate& workRate )
{
    pWorkRate_   = &workRate;
    bHasChanged_ = true;

    if( pWorkRate_->GetDelayBeforeWarningRC() == std::numeric_limits< uint >::max() )
        nWorkRateWarningRCTick_ = 0; 
    else
        nWorkRateWarningRCTick_ = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() + pWorkRate_->GetDelayBeforeWarningRC();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::InsertConsigns
// Created: JVT 2005-05-03
// -----------------------------------------------------------------------------
inline
void PHY_RolePionLOG_Maintenance::InsertConsigns( const T_MaintenanceConsigns& oldConsigns )
{
    for ( CIT_MaintenanceConsigns it = oldConsigns.begin(); it != oldConsigns.end(); ++it )
        for ( CIT_MaintenanceConsignList it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
            InsertConsign( **it2 );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::ChangePriorities
// Created: NLD 2005-04-27
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::ChangePriorities( const T_AutomateVector& priorities )
{
    T_MaintenanceConsigns oldConsigns = consigns_;
    
    consigns_.clear(); consigns_.reserve( priorities.size() + 1 );
    for ( CIT_AutomateVector it = priorities.begin(); it != priorities.end(); ++it )
        consigns_.push_back( std::make_pair( *it, T_MaintenanceConsignList() ) );
    consigns_.push_back( std::make_pair( (const MIL_Automate*)0, T_MaintenanceConsignList() ) );

    tacticalPriorities_ = priorities;
    bHasChanged_        = true;

    InsertConsigns( oldConsigns );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::ChangePriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::ChangePriorities( const T_MaintenancePriorityVector& priorities )
{
    T_MaintenanceConsigns oldConsigns = consigns_;
    
    for ( IT_MaintenanceConsigns it = consigns_.begin(); it != consigns_.end(); ++it )
        it->second.clear();

    priorities_  = priorities;
    bHasChanged_ = true;

    InsertConsigns( oldConsigns );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::InsertConsign
// Created: JVT 2005-05-03
// -----------------------------------------------------------------------------
struct sIsPriorityEqual
{
    bool operator() ( const PHY_MaintenanceConsign_ABC* pConsign, const PHY_ComposanteTypePion* pCompType )
    {
        if( pConsign->IsFinished() )
            return false;
        return *pCompType == pConsign->GetComposanteType();
    }
};

inline
void PHY_RolePionLOG_Maintenance::InsertConsign( PHY_MaintenanceConsign_ABC& consign )
{
    IT_MaintenanceConsigns itTact = consigns_.begin();
    for( const MIL_Automate* pAutomate = &consign.GetComposanteState().GetAutomate(); itTact != consigns_.end(); ++itTact )
        if( pAutomate == itTact->first || ( pAutomate->GetTC2() && pAutomate->GetTC2() == itTact->first ) )
            break;
            
    if( itTact == consigns_.end() )
    {
        assert( !consigns_.empty() );
        itTact = consigns_.end() - 1;
        assert( itTact->first == 0 );
    }

    IT_MaintenancePriorityVector itPriorityLowerBound = std::find( priorities_.begin(), priorities_.end(), &consign.GetComposanteType() );
    if( itPriorityLowerBound == priorities_.end() )
        itTact->second.push_back( &consign );
    else
    {
        ++itPriorityLowerBound;
        T_MaintenanceConsignList::reverse_iterator itConsign = std::find_first_of( itTact->second.rbegin(), itTact->second.rend(), priorities_.begin(), itPriorityLowerBound, sIsPriorityEqual() );
        itTact->second.insert( itConsign.base(), &consign );
    }
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::HandleComposanteForTransport
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
PHY_MaintenanceComposanteState* PHY_RolePionLOG_Maintenance::HandleComposanteForTransport( MIL_AgentPion& pion, PHY_ComposantePion& composante )
{
    assert( composante.GetBreakdown() );
    
    if ( !bSystemEnabled_ || ( composante.GetBreakdown()->AffectMobility() && !HasUsableHauler( composante.GetType() ) ) )
        return 0;

    PHY_MaintenanceComposanteState* pComposanteState = new PHY_MaintenanceComposanteState( pion, composante );
    PHY_MaintenanceTransportConsign* pConsign = new PHY_MaintenanceTransportConsign( *this, *pComposanteState );
    InsertConsign( *pConsign );
    return pComposanteState;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::HandleComposanteForTransport
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Maintenance::HandleComposanteForTransport( PHY_MaintenanceComposanteState& composanteState )
{    
    if ( !bSystemEnabled_ || ( composanteState.GetComposanteBreakdown().AffectMobility() && !HasUsableHauler( composanteState.GetComposante().GetType() ) ) )
        return false;

    PHY_MaintenanceTransportConsign* pConsign = new PHY_MaintenanceTransportConsign( *this, composanteState );
    InsertConsign( *pConsign );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::GetAvailabilityScoreForTransport
// Created: NLD 2006-03-29
// -----------------------------------------------------------------------------
int PHY_RolePionLOG_Maintenance::GetAvailabilityScoreForTransport( const PHY_ComposantePion& composante )
{
    if( !bSystemEnabled_ || ( composante.GetBreakdown()->AffectMobility() && !HasUsableHauler( composante.GetType() ) ) )
        return std::numeric_limits< int >::min();

    assert (pPion_ );
    PHY_RolePion_Composantes::T_ComposanteUseMap composanteUse;
    PHY_ComposanteUsePredicate1< PHY_ComposanteTypePion > predicate( &PHY_ComposantePion::CanHaul, &PHY_ComposanteTypePion::CanHaul, composante.GetType() );
    pPion_->GetRole< PHY_RolePion_Composantes >().GetComposantesUse( composanteUse, predicate );

    uint nNbrHaulersAvailable = 0;
    for( PHY_RolePion_Composantes::CIT_ComposanteUseMap it = composanteUse.begin(); it != composanteUse.end(); ++it )
        nNbrHaulersAvailable += ( it->second.nNbrAvailable_ - it->second.nNbrUsed_ );

    return nNbrHaulersAvailable;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::HandleComposanteForRepair
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Maintenance::HandleComposanteForRepair( PHY_MaintenanceComposanteState& composanteState )
{
    if( !bSystemEnabled_ || !HasUsableRepairer( composanteState.GetComposanteBreakdown() ) )
        return false;

    PHY_MaintenanceRepairConsign* pConsign = new PHY_MaintenanceRepairConsign( *this, composanteState );
    InsertConsign( *pConsign );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::GetAvailabilityScoreForRepair
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
int PHY_RolePionLOG_Maintenance::GetAvailabilityScoreForRepair( PHY_MaintenanceComposanteState& composanteState )
{
    if( !bSystemEnabled_ || !HasUsableRepairer( composanteState.GetComposanteBreakdown() ) )
        return std::numeric_limits< int >::min();

    // Parts score
    MT_Float rRatioPartsAvailable = 0.;

    assert (pPion_ );
    const PHY_RolePion_Dotations& roleDotations = pPion_->GetRole< PHY_RolePion_Dotations >();

    const PHY_BreakdownType::T_PartMap& parts = composanteState.GetComposanteBreakdown().GetNeededParts();
    for( PHY_BreakdownType::CIT_PartMap it = parts.begin(); it != parts.end(); ++it )
    {
        // Parts never available ...
        if( roleDotations.GetDotationCapacity( *it->first ) <= 0. )
            return std::numeric_limits< int >::min();

        rRatioPartsAvailable += ( roleDotations.GetDotationValue( *it->first ) / it->second );
    }
    if( parts.empty() )
        rRatioPartsAvailable = 1.;
    else
        rRatioPartsAvailable /= parts.size();

    // Repairers score
    return (uint)( GetNbrAvailableRepairersAllowedToWork( composanteState.GetComposanteBreakdown() ) * rRatioPartsAvailable );
}

// =============================================================================
// UPDATE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::Update
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::Update( bool /*bIsDead*/ )
{
    if( nWorkRateWarningRCTick_ != 0 && MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() > nWorkRateWarningRCTick_ )
    {
        nWorkRateWarningRCTick_ = 0;
        assert( pPion_ );
        MIL_Report::PostEvent( *pPion_, MIL_Report::eReport_MaintenanceWorkRateExceeded );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::UpdateLogistic
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::UpdateLogistic( bool /*bIsDead*/ )
{
    for ( IT_MaintenanceConsigns itConsigns = consigns_.begin(); itConsigns != consigns_.end(); ++itConsigns )
        for ( IT_MaintenanceConsignList itConsign = itConsigns->second.begin(); itConsign != itConsigns->second.end(); )
            if ( (*itConsign)->Update() )
            {
                delete *itConsign;
                itConsign = itConsigns->second.erase( itConsign );
            }
            else
                ++itConsign;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::Clean
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::Clean()
{
    bHasChanged_ = false;
}

// =============================================================================
// NETWOKR
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SendComposanteUse
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
static
void SendComposanteUse( const PHY_RolePion_Composantes::T_ComposanteUseMap& data, ASN1T__SeqOfLogMaintenanceEquipmentAvailability& asn, const PHY_MaintenanceWorkRate* pWorkRate )
{
    asn.n = data.size();
    if( data.empty() )
        return;

    ASN1T_LogMaintenanceEquipmentAvailability* pData = new ASN1T_LogMaintenanceEquipmentAvailability[ data.size() ];
    uint i = 0;
    for( PHY_RolePion_Composantes::CIT_ComposanteUseMap itData = data.begin(); itData != data.end(); ++itData )
    {
        ASN1T_LogMaintenanceEquipmentAvailability& data = pData[ i++ ];
        data.type_equipement = itData->first->GetMosID();
        assert( itData->second.nNbrTotal_ );

        data.nbr_total       = itData->second.nNbrTotal_;
        data.nbr_au_travail  = itData->second.nNbrUsed_;
        data.nbr_disponibles = itData->second.nNbrAvailable_ - itData->second.nNbrUsed_; // nNbrAvailableAllowedToWork
        data.nbr_pretes      = itData->second.nNbrLent_;

        if( pWorkRate )
        {
            const uint nNbrAllowedToWork          = pWorkRate->GetNbrWorkerAllowedToWork( itData->second.nNbrAvailable_ );
            const uint nNbrAvailableAllowedToWork = nNbrAllowedToWork > itData->second.nNbrUsed_ ? nNbrAllowedToWork - itData->second.nNbrUsed_ : 0;

            data.m.nbr_au_reposPresent = 1;
            data.nbr_disponibles = nNbrAvailableAllowedToWork;
            data.nbr_au_repos    = itData->second.nNbrAvailable_ - nNbrAvailableAllowedToWork - itData->second.nNbrUsed_;
        }
    }
    asn.elem = pData;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::SendFullState
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::SendFullState() const
{
    NET_ASN_MsgLogMaintenanceState asn;

    asn().m.chaine_activeePresent             = 1;
    asn().m.regime_travailPresent             = 1;
    asn().m.prioritesPresent                  = 1;
    asn().m.priorites_tactiquesPresent        = 1;
    asn().m.disponibilites_remorqueursPresent = 1; 
    asn().m.disponibilites_reparateursPresent = 1;

    assert( pPion_ );
    asn().oid_pion        = pPion_->GetID();
    asn().chaine_activee  = bSystemEnabled_;
    asn().regime_travail  = pWorkRate_->GetAsnID();

    asn().priorites.n = priorities_.size();
    if( !priorities_.empty() )
    {
        ASN1T_EquipmentType* pAsnPriorities = new ASN1T_EquipmentType[ priorities_.size() ];
        uint i = 0 ;
        for( CIT_MaintenancePriorityVector itPriority = priorities_.begin(); itPriority != priorities_.end(); ++itPriority )
            pAsnPriorities[ i++ ] = (**itPriority).GetMosID();
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

    {
        PHY_RolePion_Composantes::T_ComposanteUseMap composanteUse;
        PHY_ComposanteUsePredicate predicate( &PHY_ComposantePion::CanHaul, &PHY_ComposanteTypePion::CanHaul );
        pPion_->GetRole< PHY_RolePion_Composantes >().GetComposantesUse( composanteUse, predicate );
        SendComposanteUse( composanteUse, asn().disponibilites_remorqueurs, 0 );
    }

    {
        PHY_RolePion_Composantes::T_ComposanteUseMap composanteUse;
        PHY_ComposanteUsePredicate predicate( &PHY_ComposantePion::CanRepair, &PHY_ComposanteTypePion::CanRepair );
        pPion_->GetRole< PHY_RolePion_Composantes >().GetComposantesUse( composanteUse, predicate );
        SendComposanteUse( composanteUse, asn().disponibilites_reparateurs, pWorkRate_ );
    }

    asn.Send();

    if( asn().priorites.n > 0 )
        delete [] asn().priorites.elem;
    if( asn().priorites_tactiques.n > 0 )
        delete [] asn().priorites_tactiques.elem;
    if( asn().disponibilites_remorqueurs.n > 0 )
        delete [] asn().disponibilites_remorqueurs.elem;
    if( asn().disponibilites_reparateurs.n > 0 )
        delete [] asn().disponibilites_reparateurs.elem;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::SendChangedState
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::SendChangedState() const
{
    assert (pPion_ );
    if( bHasChanged_ || pPion_->GetRole< PHY_RolePion_Composantes >().HasChanged() )
        SendFullState();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::GetPion
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
const MIL_AgentPionLOG_ABC& PHY_RolePionLOG_Maintenance::GetPion() const
{
    assert( pPion_ );
    return *pPion_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::EnableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::EnableSystem()
{
    bSystemEnabled_ = true;
    bHasChanged_    = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::DisableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::DisableSystem()
{
    bSystemEnabled_ = false;
    bHasChanged_    = true;
}
