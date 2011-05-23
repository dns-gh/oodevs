// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_RolePionLOG_Maintenance.cpp $
// $Author: Jvt $
// $Modtime: 4/05/05 10:53 $
// $Revision: 13 $
// $Workfile: PHY_RolePionLOG_Maintenance.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RolePionLOG_Maintenance.h"
#include "protocol/ClientSenders.h"
#include "PHY_MaintenanceConsign_ABC.h"
#include "PHY_MaintenanceRepairConsign.h"
#include "PHY_MaintenanceTransportConsign.h"
#include "PHY_MaintenanceComposanteState.h"
#include "PHY_MaintenanceResourcesAlarms.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Logistic/PHY_Breakdown.h"
#include "Entities/Agents/Units/Logistic/PHY_MaintenanceWorkRate.h"
#include "Entities/Agents/Roles/Composantes/PHY_ComposantePredicates.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Network/NET_Publisher_ABC.h"
#include "simulation_kernel/AlgorithmsFactories.h"
#include "simulation_kernel/OnComponentFunctor_ABC.h"
#include "simulation_kernel/OnComponentFunctorComputer_ABC.h"
#include "simulation_kernel/OnComponentFunctorComputerFactory_ABC.h"
#include "simulation_kernel/OnComponentLendedFunctorComputer_ABC.h"
#include "simulation_kernel/OnComponentLendedFunctorComputerFactory_ABC.h"
#include "simulation_kernel/DotationComputer_ABC.h"
#include "simulation_kernel/DotationComputerFactory_ABC.h"
#include "simulation_kernel/ConsumeDotationNotificationHandler_ABC.h"
#include "simulation_kernel/NetworkNotificationHandler_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_RolePionLOG_Maintenance )

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RolePionLOG_Maintenance* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* const pion = &role->pion_;
    archive << pion;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RolePionLOG_Maintenance* role, const unsigned int /*version*/ )
{
    MIL_AgentPionLOG_ABC* pion;
    archive >> pion;
    ::new( role )PHY_RolePionLOG_Maintenance( *pion );
    pion->RegisterRole( *role );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePionLOG_Maintenance::PHY_RolePionLOG_Maintenance( MIL_AgentPionLOG_ABC& pion )
    : pion_                    ( pion )
    , bHasChanged_             ( true )
    , bExternalMustChangeState_( false )
    , bSystemEnabled_          ( false )
    , pWorkRate_               ( &PHY_MaintenanceWorkRate::r1_ )
    , nWorkRateWarningRCTick_  ( 0 )
{
    consigns_.push_back( std::make_pair( (const MIL_Automate*)0, T_MaintenanceConsignList() ) );
}


// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePionLOG_Maintenance::~PHY_RolePionLOG_Maintenance()
{
    // NOTHING
}

namespace boost
{
    namespace serialization
    {
        typedef std::vector< const MIL_Automate* > T_AutomateVector; // $$$$ _RC_ LGY 2010-07-15: à remanier
        typedef T_AutomateVector::const_iterator CIT_AutomateVector;

        typedef std::vector< const PHY_ComposanteTypePion* >  T_MaintenancePriorityVector;
        typedef T_MaintenancePriorityVector::const_iterator CIT_MaintenancePriorityVector;

        // =============================================================================
        // T_MaintenancePriorityVector
        // =============================================================================
        template< typename Archive >
        inline
        void serialize( Archive& file, T_MaintenancePriorityVector& vector, const unsigned int nVersion )
        {
            split_free( file, vector, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const T_MaintenancePriorityVector& vector, const unsigned int )
        {
            std::size_t size = vector.size();
            file << size;
            for ( CIT_MaintenancePriorityVector it = vector.begin(); it != vector.end(); ++it )
            {
                sword::EquipmentType id = (*it)->GetMosID();
                int equipment = id.id();
                file << equipment;
            }
        }

        template< typename Archive >
        void load( Archive& file, T_MaintenancePriorityVector& vector, const unsigned int )
        {
            std::size_t nNbr;
            file >> nNbr;
            vector.reserve( nNbr );
            while ( nNbr-- )
            {
                sword::EquipmentType nID;
                int equipment;
                file >> equipment;
                nID.set_id( equipment );
                vector.push_back( PHY_ComposanteTypePion::Find( nID ) );
            }
        }

        // =============================================================================
        // T_AutomateVector
        // =============================================================================
        template< typename Archive >
        inline
        void serialize( Archive& file, T_AutomateVector& vector, const unsigned int nVersion )
        {
            split_free( file, vector, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const T_AutomateVector& vector, const unsigned int )
        {
            std::size_t size = vector.size();
            file << size;
            for ( CIT_AutomateVector it = vector.begin(); it != vector.end(); ++it )
                file << *it;
        }

        template< typename Archive >
        void load( Archive& file, T_AutomateVector& vector, const unsigned int )
        {
            std::size_t nNbr;
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
void PHY_RolePionLOG_Maintenance::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< PHY_RoleInterface_Maintenance >( *this )
         >> bSystemEnabled_
         >> priorities_
         >> tacticalPriorities_;
    sword::EnumLogMaintenanceWorkRate nID;
    file >> nID;
    pWorkRate_ = PHY_MaintenanceWorkRate::Find( nID );
    file >> nWorkRateWarningRCTick_;
    std::size_t nNbr;
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
void PHY_RolePionLOG_Maintenance::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    sword::EnumLogMaintenanceWorkRate workRate = pWorkRate_->GetAsnID();
    file << boost::serialization::base_object< PHY_RoleInterface_Maintenance >( *this )
         << bSystemEnabled_
         << priorities_
         << tacticalPriorities_
         << workRate
         << nWorkRateWarningRCTick_;
    std::size_t size = consigns_.size();
    file << size;
    for ( CIT_MaintenanceConsigns it = consigns_.begin(); it != consigns_.end(); ++it )
        file << it->first << it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::GetAvailabilityRatio
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
double PHY_RolePionLOG_Maintenance::GetAvailabilityRatio( PHY_ComposanteUsePredicate& predicate, const PHY_MaintenanceWorkRate* pWorkRate ) const
{
    unsigned int nNbrTotal = 0;
    unsigned int nNbrAvailableAllowedToWork = 0;
    PHY_Composante_ABC::T_ComposanteUseMap composanteUse;
    GetComponentUseFunctor functorOnComponent( predicate, composanteUse );
    std::auto_ptr< OnComponentComputer_ABC > componentComputer( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functorOnComponent ) );
    pion_.Execute( *componentComputer );
    GetComponentLendedUseFunctor functorOnLendedComponent( predicate, composanteUse );
    std::auto_ptr< OnComponentLendedFunctorComputer_ABC > lendedComponent( pion_.GetAlgorithms().onComponentLendedFunctorComputerFactory_->Create( functorOnLendedComponent ) );
    pion_.Execute( *lendedComponent );
    for( PHY_Composante_ABC::CIT_ComposanteUseMap it = composanteUse.begin(); it != composanteUse.end(); ++it )
    {
        nNbrTotal += it->second.nNbrTotal_;
        if( pWorkRate )
        {
            const unsigned int nNbrAllowedToWork = pWorkRate_->GetNbrWorkerAllowedToWork( it->second.nNbrAvailable_ );
            if( nNbrAllowedToWork > it->second.nNbrUsed_ )
                nNbrAvailableAllowedToWork += ( nNbrAllowedToWork - it->second.nNbrUsed_ );
        }
        else
            nNbrAvailableAllowedToWork += ( it->second.nNbrAvailable_ - it->second.nNbrUsed_ );
    }
    if( nNbrTotal == 0 )
        return 1.;
    return (double)nNbrAvailableAllowedToWork / (double)nNbrTotal;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::StartUsingForLogistic
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::StartUsingForLogistic( PHY_ComposantePion& composante )
{
    PHY_ComposanteUsePredicate repairerUsePred( &PHY_ComposantePion::CanRepair, &PHY_ComposanteTypePion::CanRepair );
    PHY_ComposanteUsePredicate haulerUsePred( &PHY_ComposantePion::CanHaul  , &PHY_ComposanteTypePion::CanHaul );
    double rRepairerRatio = GetAvailabilityRatio( repairerUsePred, pWorkRate_ );
    double rHaulerRatio = GetAvailabilityRatio( haulerUsePred   );
    bHasChanged_ = true;
    composante.StartUsingForLogistic();
    if( PHY_MaintenanceResourcesAlarms::IsRepairerResourcesLevelReached( rRepairerRatio, GetAvailabilityRatio( repairerUsePred, pWorkRate_ ) ) )
        MIL_Report::PostEvent( pion_, MIL_Report::eReport_RepairerResourcesLevelReached );
    if( PHY_MaintenanceResourcesAlarms::IsHaulerResourcesLevelReached( rHaulerRatio, GetAvailabilityRatio( haulerUsePred ) ) )
        MIL_Report::PostEvent( pion_, MIL_Report::eReport_HaulerResourcesLevelReached );
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

namespace
{
    class AvailableHaulerComputer : public OnComponentFunctor_ABC // $$$$ MGD MERGE all Logictic OnComponentFunctor in one file
    {
    public:
        AvailableHaulerComputer( const PHY_ComposanteTypePion& composanteType )
            : rScore_( std::numeric_limits< double >::max() )
            , composanteType_( composanteType )
            , pSelectedHauler_( 0 )
        {
        }
        void operator() ( PHY_ComposantePion& composante )
        {
            if( !composante.CanHaul( composanteType_ ) )
                return;

            double rNewScore = composante.GetType().GetHaulerWeightCapacity() - composanteType_.GetWeight();
            assert( rNewScore >= 0. );
            if( rNewScore < rScore_ )
            {
                rScore_ = rNewScore;
                pSelectedHauler_ = &composante;
            }
        }
        double rScore_;
        const PHY_ComposanteTypePion& composanteType_;
        PHY_ComposantePion* pSelectedHauler_;
    };
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::GetAvailableHauler
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RolePionLOG_Maintenance::GetAvailableHauler( const PHY_ComposanteTypePion& composanteType ) const
{
    AvailableHaulerComputer functor( composanteType );
    std::auto_ptr< OnComponentComputer_ABC > componentComputer( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functor ) );
    pion_.Execute( *componentComputer );
    return functor.pSelectedHauler_;
}

// -----------------------------------------------------------------------------
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Maintenance::HasUsableHauler( const PHY_ComposanteTypePion& composanteType ) const
{
    PHY_ComposanteTypePredicate1< PHY_ComposanteTypePion > predicate( &PHY_ComposanteTypePion::CanHaul, composanteType );
    HasUsableComponentFunctor functor( predicate );
    std::auto_ptr< OnComponentComputer_ABC > computer( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functor ) );
    pion_.Execute( *computer );
    return functor.result_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::GetNbrAvailableRepairersAllowedToWork
// Created: NLD 2006-03-28
// -----------------------------------------------------------------------------
unsigned int PHY_RolePionLOG_Maintenance::GetNbrAvailableRepairersAllowedToWork( const PHY_Breakdown& breakdown ) const
{
    PHY_Composante_ABC::T_ComposanteUseMap composanteUse;
    PHY_ComposanteUsePredicate1< PHY_Breakdown > predicate( &PHY_ComposantePion::CanRepair, &PHY_ComposanteTypePion::CanRepair, breakdown );
    GetComponentUseFunctor functorOnComponent( predicate, composanteUse );
    std::auto_ptr< OnComponentComputer_ABC > componentComputer( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functorOnComponent ) );
    pion_.Execute( *componentComputer );
    GetComponentLendedUseFunctor functorOnLendedComponent( predicate, composanteUse );
    std::auto_ptr< OnComponentLendedFunctorComputer_ABC > lendedComputer( pion_.GetAlgorithms().onComponentLendedFunctorComputerFactory_->Create( functorOnLendedComponent ) );
    pion_.Execute( *lendedComputer );
    unsigned int nNbrAvailableAllowedToWork = 0;
    assert( pWorkRate_ );
    for( PHY_Composante_ABC::CIT_ComposanteUseMap it = composanteUse.begin(); it != composanteUse.end(); ++it )
    {
        const unsigned int nNbrAllowedToWork = pWorkRate_->GetNbrWorkerAllowedToWork( it->second.nNbrAvailable_ );
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
    PHY_ComposantePredicate1< PHY_Breakdown > predicate( &PHY_ComposantePion::CanRepair, breakdown );
    GetComponentFunctor functor( predicate );
    std::auto_ptr< OnComponentComputer_ABC > computer( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functor ) );
    pion_.Execute( *computer );
    PHY_ComposantePion* pRepairer = functor.result_;
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
    PHY_ComposanteTypePredicate1< PHY_Breakdown > predicate( &PHY_ComposanteTypePion::CanRepair, breakdown );
    HasUsableComponentFunctor functor( predicate );
    std::auto_ptr< OnComponentComputer_ABC > computer( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functor ) );
    pion_.Execute( *computer );
    return functor.result_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::ConsumePartsForBreakdown
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Maintenance::ConsumePartsForBreakdown( const PHY_Breakdown& breakdown )
{
    std::auto_ptr< dotation::DotationComputer_ABC > dotationComputer( pion_.GetAlgorithms().dotationComputerFactory_->Create() );
    pion_.Execute( *dotationComputer );//@TODO MGD move execute for all computer in factories create
    const PHY_BreakdownType::T_PartMap& parts = breakdown.GetNeededParts();
    for( PHY_BreakdownType::CIT_PartMap it = parts.begin(); it != parts.end(); ++it )
    {
        if( dotationComputer->GetDotationValue( *it->first ) < it->second )
            return false;
    }
    for( PHY_BreakdownType::CIT_PartMap it = parts.begin(); it != parts.end(); ++it )
    {
//        unsigned int nOut = (unsigned int)roleDotations.ConsumeDotation( *it->first, it->second );
//        assert( nOut == it->second );
        pion_.Apply( &dotation::ConsumeDotationNotificationHandler_ABC::NotifyConsumeDotation, *it->first, it->second );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::ChangeWorkRate
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::ChangeWorkRate( const PHY_MaintenanceWorkRate& workRate )
{
    pWorkRate_ = &workRate;
    bHasChanged_ = true;
    if( pWorkRate_->GetDelayBeforeWarningRC() == std::numeric_limits< unsigned int >::max() )
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
    bHasChanged_ = true;
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
    priorities_ = priorities;
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
        if( pAutomate == itTact->first ) // TODO || ( pAutomate->GetTC2() && pAutomate->GetTC2() == itTact->first ) )
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

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::HandleComposanteForTransport
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
PHY_MaintenanceComposanteState* PHY_RolePionLOG_Maintenance::HandleComposanteForTransport( MIL_Agent_ABC& pion, PHY_ComposantePion& composante )
{
    assert( composante.GetBreakdown() );
    if( !bSystemEnabled_ || ( composante.GetBreakdown()->AffectMobility() && !HasUsableHauler( composante.GetType() ) ) )
        return 0;
    PHY_MaintenanceComposanteState* pComposanteState = new PHY_MaintenanceComposanteState( pion, composante );
    PHY_MaintenanceTransportConsign* pConsign = new PHY_MaintenanceTransportConsign( pion_, *pComposanteState );
    InsertConsign( *pConsign );
    return pComposanteState;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::HandleComposanteForTransport
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Maintenance::HandleComposanteForTransport( PHY_MaintenanceComposanteState& composanteState )
{
    if( !bSystemEnabled_ || ( composanteState.GetComposanteBreakdown().AffectMobility() && !HasUsableHauler( composanteState.GetComposante().GetType() ) ) )
        return false;
    PHY_MaintenanceTransportConsign* pConsign = new PHY_MaintenanceTransportConsign( pion_, composanteState );
    InsertConsign( *pConsign );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::GetAvailabilityScoreForTransport
// Created: NLD 2006-03-29
// -----------------------------------------------------------------------------
int PHY_RolePionLOG_Maintenance::GetAvailabilityScoreForTransport( const PHY_ComposantePion& composante ) const
{
    if( !bSystemEnabled_ || ( composante.GetBreakdown()->AffectMobility() && !HasUsableHauler( composante.GetType() ) ) )
        return std::numeric_limits< int >::min();
    PHY_Composante_ABC::T_ComposanteUseMap composanteUse;
    PHY_ComposanteUsePredicate1< PHY_ComposanteTypePion > predicate( &PHY_ComposantePion::CanHaul, &PHY_ComposanteTypePion::CanHaul, composante.GetType() );
    GetComponentUseFunctor functorOnComponent( predicate, composanteUse );
    std::auto_ptr< OnComponentComputer_ABC > componentComputer( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functorOnComponent ) );
    pion_.Execute( *componentComputer );
    GetComponentLendedUseFunctor functorOnLendedComponent( predicate, composanteUse );
    std::auto_ptr< OnComponentLendedFunctorComputer_ABC > lendedComputer( pion_.GetAlgorithms().onComponentLendedFunctorComputerFactory_->Create( functorOnLendedComponent ) );
    pion_.Execute( *lendedComputer );
    unsigned int nNbrHaulersAvailable = 0;
    for( PHY_Composante_ABC::CIT_ComposanteUseMap it = composanteUse.begin(); it != composanteUse.end(); ++it )
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
    PHY_MaintenanceRepairConsign* pConsign = new PHY_MaintenanceRepairConsign( pion_, composanteState );
    InsertConsign( *pConsign );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::GetAvailabilityScoreForRepair
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
int PHY_RolePionLOG_Maintenance::GetAvailabilityScoreForRepair( const PHY_MaintenanceComposanteState& composanteState ) const
{
    if( !bSystemEnabled_ || !HasUsableRepairer( composanteState.GetComposanteBreakdown() ) )
        return std::numeric_limits< int >::min();
    // Parts score
    double rRatioPartsAvailable = 0.;
    std::auto_ptr< dotation::DotationComputer_ABC > dotationComputer( pion_.GetAlgorithms().dotationComputerFactory_->Create() );
    pion_.Execute( *dotationComputer );

    const PHY_BreakdownType::T_PartMap& parts = composanteState.GetComposanteBreakdown().GetNeededParts();
    for( PHY_BreakdownType::CIT_PartMap it = parts.begin(); it != parts.end(); ++it )
    {
        // Parts never available ...
        if( dotationComputer->GetDotationCapacity( *it->first ) <= 0. )
            return std::numeric_limits< int >::min();

        rRatioPartsAvailable += ( dotationComputer->GetDotationValue( *it->first ) / it->second );
    }
    if( parts.empty() )
        rRatioPartsAvailable = 1.;
    else
        rRatioPartsAvailable /= parts.size();
    // Repairers score
    return (unsigned int)( GetNbrAvailableRepairersAllowedToWork( composanteState.GetComposanteBreakdown() ) * rRatioPartsAvailable );
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
        MIL_Report::PostEvent( pion_, MIL_Report::eReport_MaintenanceWorkRateExceeded );
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
            if( (*itConsign)->Update() )
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
    bExternalMustChangeState_ = false;
}

// -----------------------------------------------------------------------------
// Name: SendComposanteUse
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
static
void SendComposanteUse( const PHY_Composante_ABC::T_ComposanteUseMap& data, sword::SeqOfLogMaintenanceEquipmentAvailability& asn, const PHY_MaintenanceWorkRate* pWorkRate )
{
    if( data.empty() )
        return;

    for( PHY_Composante_ABC::CIT_ComposanteUseMap itData = data.begin(); itData != data.end(); ++itData )
    {
        sword::LogMaintenanceEquipmentAvailability& data = *asn.add_elem();
        data.mutable_equipment()->set_id( itData->first->GetMosID().id() );
        assert( itData->second.nNbrTotal_ );
        data.set_total( itData->second.nNbrTotal_ );
        data.set_working( itData->second.nNbrUsed_ );
        data.set_available( itData->second.nNbrAvailable_ - itData->second.nNbrUsed_ ); // nNbrAvailableAllowedToWork
        data.set_lent( itData->second.nNbrLent_ );
        if( pWorkRate )
        {
            const unsigned int nNbrAllowedToWork = pWorkRate->GetNbrWorkerAllowedToWork( itData->second.nNbrAvailable_ );
            const unsigned int nNbrAvailableAllowedToWork = nNbrAllowedToWork > itData->second.nNbrUsed_ ? nNbrAllowedToWork - itData->second.nNbrUsed_ : 0;
            data.set_available( nNbrAvailableAllowedToWork );
            data.set_resting( itData->second.nNbrAvailable_ - nNbrAvailableAllowedToWork - itData->second.nNbrUsed_ );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::SendFullState
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::SendFullState() const
{
    client::LogMaintenanceState asn;
    asn().mutable_unit()->set_id( pion_.GetID() );
    asn().set_chain( bSystemEnabled_ );
    asn().set_work_rate( pWorkRate_->GetAsnID() );
    asn().mutable_priorities();
    for( CIT_MaintenancePriorityVector itPriority = priorities_.begin(); itPriority != priorities_.end(); ++itPriority )
        asn().mutable_priorities()->add_elem()->set_id( (**itPriority).GetMosID().id() );
    asn().mutable_tactical_priorities();
    for( CIT_AutomateVector itPriority = tacticalPriorities_.begin(); itPriority != tacticalPriorities_.end(); ++itPriority )
        asn().mutable_tactical_priorities()->add_elem()->set_id( (**itPriority).GetID() );
    {
        PHY_Composante_ABC::T_ComposanteUseMap composanteUse;
        PHY_ComposanteUsePredicate predicate( &PHY_ComposantePion::CanHaul, &PHY_ComposanteTypePion::CanHaul );
        GetComponentUseFunctor functorOnComponent( predicate, composanteUse );
        std::auto_ptr< OnComponentComputer_ABC > componentComputer( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functorOnComponent ) );
        pion_.Execute( *componentComputer );
        GetComponentLendedUseFunctor functorOnLendedComponent( predicate, composanteUse );
        std::auto_ptr< OnComponentLendedFunctorComputer_ABC > lendedComputer( pion_.GetAlgorithms().onComponentLendedFunctorComputerFactory_->Create( functorOnLendedComponent ) );
        pion_.Execute( *lendedComputer );
        SendComposanteUse( composanteUse, *asn().mutable_haulers(), 0 );
    }
    {
        PHY_Composante_ABC::T_ComposanteUseMap composanteUse;
        PHY_ComposanteUsePredicate predicate( &PHY_ComposantePion::CanRepair, &PHY_ComposanteTypePion::CanRepair );
        GetComponentUseFunctor functorOnComponent( predicate, composanteUse );
        std::auto_ptr< OnComponentComputer_ABC > componentComputer( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( functorOnComponent ) );
        pion_.Execute( *componentComputer );
        GetComponentLendedUseFunctor functorOnLendedComponent( predicate, composanteUse );
        std::auto_ptr< OnComponentLendedFunctorComputer_ABC > lendedComputer( pion_.GetAlgorithms().onComponentLendedFunctorComputerFactory_->Create( functorOnLendedComponent ) );
        pion_.Execute( *lendedComputer );

        SendComposanteUse( composanteUse, *asn().mutable_repairers(), pWorkRate_ );
    }
    asn.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::SendChangedState
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::SendChangedState() const
{
    if( bHasChanged_ || bExternalMustChangeState_ )
        SendFullState();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::GetPion
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
const MIL_AgentPionLOG_ABC& PHY_RolePionLOG_Maintenance::GetPion() const
{

    return pion_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::EnableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::EnableSystem()
{
    bSystemEnabled_ = true;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::DisableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::DisableSystem()
{
    bSystemEnabled_ = false;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::NotifyHasChanged
// Created: MGD 2009-09-29
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::NotifyComponentHasChanged()
{
    bExternalMustChangeState_ = true;
}
