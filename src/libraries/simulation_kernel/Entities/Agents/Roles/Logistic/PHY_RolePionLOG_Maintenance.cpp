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
#include "PHY_MaintenanceDiagnosisConsign.h"
#include "PHY_MaintenanceTransportConsign.h"
#include "PHY_MaintenanceComposanteState.h"
#include "PHY_MaintenanceResourcesAlarms.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Logistic/PHY_Breakdown.h"
#include "Entities/Agents/Units/Logistic/PHY_MaintenanceWorkRate.h"
#include "Entities/Agents/Roles/Composantes/PHY_ComposantePredicates.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "MIL_Time_ABC.h"
#include "Network/NET_Publisher_ABC.h"
#include "simulation_kernel/OnComponentFunctor_ABC.h"
#include "simulation_kernel/DefaultComponentFunctorComputer.h"
#include "simulation_kernel/DefaultComponentLendedFunctorComputer.h"
#include "simulation_kernel/DefaultDotationComputer.h"
#include "simulation_kernel/ConsumeDotationNotificationHandler_ABC.h"
#include "simulation_kernel/NetworkNotificationHandler_ABC.h"
#include <boost/range/algorithm_ext/erase.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/make_shared.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_RolePionLOG_Maintenance )

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RolePionLOG_Maintenance* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* const pion = &role->owner_;
    archive << pion;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RolePionLOG_Maintenance* role, const unsigned int /*version*/ )
{
    MIL_AgentPionLOG_ABC* pion;
    archive >> pion;
    ::new( role )PHY_RolePionLOG_Maintenance( *pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePionLOG_Maintenance::PHY_RolePionLOG_Maintenance( MIL_AgentPionLOG_ABC& pion )
    : owner_                   ( pion )
    , bHasChanged_             ( true )
    , bExternalMustChangeState_( false )
    , bSystemEnabled_          ( false )
    , pWorkRate_               ( &PHY_MaintenanceWorkRate::r1_ )
    , nWorkRateWarningRCTick_  ( 0 )
{
    consigns_.push_back( T_MaintenanceConsigns::value_type() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePionLOG_Maintenance::~PHY_RolePionLOG_Maintenance()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::load
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< PHY_RoleInterface_Maintenance >( *this );
    file >> bSystemEnabled_;
    file >> priorities_;
    file >> tacticalPriorities_;
    sword::EnumLogMaintenanceWorkRate nID;
    file >> nID;
    pWorkRate_ = PHY_MaintenanceWorkRate::Find( nID );
    file >> nWorkRateWarningRCTick_;
    file >> consigns_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::save
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    sword::EnumLogMaintenanceWorkRate workRate = pWorkRate_->GetAsnID();
    file << boost::serialization::base_object< PHY_RoleInterface_Maintenance >( *this );
    file << bSystemEnabled_;
    file << priorities_;
    file << tacticalPriorities_;
    file << workRate;
    file << nWorkRateWarningRCTick_;
    file << consigns_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::FindLogisticManager
// Created: NLD 2011-01-11
// -----------------------------------------------------------------------------
MIL_AutomateLOG* PHY_RolePionLOG_Maintenance::FindLogisticManager() const
{
    return owner_.FindLogisticManager();
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
    DefaultComponentFunctorComputer componentComputer( functorOnComponent );
    owner_.Execute< OnComponentComputer_ABC >( componentComputer );
    GetComponentLendedUseFunctor functorOnLendedComponent( predicate, composanteUse );
    DefaultComponentLendedFunctorComputer lendedComputer( functorOnLendedComponent );
    owner_.Execute< OnComponentLendedFunctorComputer_ABC >( lendedComputer );
    for( auto it = composanteUse.begin(); it != composanteUse.end(); ++it )
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
        return 1;
    return (double)nNbrAvailableAllowedToWork / (double)nNbrTotal;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::StartUsingForLogistic
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::StartUsingForLogistic( PHY_ComposantePion& composante )
{
    PHY_ComposanteUsePredicate repairerUsePred( &PHY_ComposantePion::CanRepair, &PHY_ComposanteTypePion::CanRepair );
    PHY_ComposanteUsePredicate haulerUsePred( &PHY_ComposantePion::CanHaul, &PHY_ComposanteTypePion::CanHaul );
    double rRepairerRatio = GetAvailabilityRatio( repairerUsePred, pWorkRate_ );
    double rHaulerRatio = GetAvailabilityRatio( haulerUsePred );
    bHasChanged_ = true;
    composante.StartUsingForLogistic();
    if( PHY_MaintenanceResourcesAlarms::IsRepairerResourcesLevelReached( rRepairerRatio, GetAvailabilityRatio( repairerUsePred, pWorkRate_ ) ) )
        MIL_Report::PostEvent< MIL_Agent_ABC >( owner_, report::eRC_AlerteDisponibiliteReparateurs );
    if( PHY_MaintenanceResourcesAlarms::IsHaulerResourcesLevelReached( rHaulerRatio, GetAvailabilityRatio( haulerUsePred ) ) )
        MIL_Report::PostEvent< MIL_Agent_ABC >( owner_, report::eRC_AlerteDisponibiliteRemorqueurs );
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
    struct ComponentFunctor : OnComponentFunctor_ABC
    {
        template< typename Checker >
        ComponentFunctor( const Checker& checker )
            : checker_  ( checker )
            , component_( 0 )
        {}
        void operator()( PHY_ComposantePion& component )
        {
            if( checker_( component ) )
                component_ = &component;
        }
        std::function< bool( const PHY_ComposantePion& component ) > checker_;
        PHY_ComposantePion* component_;
    };
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::GetAvailableHauler
// Created: SLI 2014-01-31
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RolePionLOG_Maintenance::GetAvailableHauler( const PHY_ComposanteTypePion& carried, const PHY_ComposanteTypePion* type ) const
{
    double score = std::numeric_limits< double >::max();
    ComponentFunctor functor( [&]( const PHY_ComposantePion& component ) -> bool
    {
        if( type && component.GetType() != *type || !component.CanHaul1( carried ) )
            return false;
        const double newScore = component.GetType().GetHaulerWeightCapacity() - carried.GetWeight();
        if( newScore >= score )
            return false;
        score = newScore;
        return true;
    } );
    DefaultComponentFunctorComputer componentComputer( functor );
    owner_.Execute< OnComponentComputer_ABC >( componentComputer );
    return functor.component_;
}

// -----------------------------------------------------------------------------
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Maintenance::HasUsableHauler( const PHY_ComposanteTypePion& composanteType ) const
{
    PHY_ComposanteTypePredicate1< PHY_ComposanteTypePion > predicate( &PHY_ComposanteTypePion::CanHaul1, composanteType );
    HasUsableComponentFunctor functor( predicate );
    DefaultComponentFunctorComputer componentComputer( functor );
    owner_.Execute< OnComponentComputer_ABC >( componentComputer );
    return functor.result_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::GetNbrAvailableDiagnosersAllowedToWork
// Created: SLI 2014-02-12
// -----------------------------------------------------------------------------
unsigned int PHY_RolePionLOG_Maintenance::GetNbrAvailableDiagnosersAllowedToWork() const
{
    PHY_Composante_ABC::T_ComposanteUseMap composanteUse;
    PHY_ComposanteUsePredicate predicate( &PHY_ComposantePion::CanRepair, &PHY_ComposanteTypePion::CanRepair );
    GetComponentUseFunctor functorOnComponent( predicate, composanteUse );
    DefaultComponentFunctorComputer componentComputer( functorOnComponent );
    owner_.Execute< OnComponentComputer_ABC >( componentComputer );
    GetComponentLendedUseFunctor functorOnLendedComponent( predicate, composanteUse );
    DefaultComponentLendedFunctorComputer lendedComputer( functorOnLendedComponent );
    owner_.Execute< OnComponentLendedFunctorComputer_ABC >( lendedComputer );
    unsigned int nNbrAvailableAllowedToWork = 0;
    assert( pWorkRate_ );
    for( auto it = composanteUse.begin(); it != composanteUse.end(); ++it )
    {
        const unsigned int nNbrAllowedToWork = pWorkRate_->GetNbrWorkerAllowedToWork( it->second.nNbrAvailable_ );
        if( nNbrAllowedToWork > it->second.nNbrUsed_ )
            nNbrAvailableAllowedToWork += ( nNbrAllowedToWork - it->second.nNbrUsed_ );
    }
    return nNbrAvailableAllowedToWork;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::GetNbrAvailableDiagnosersAllowedToWork
// Created: LGY 2014-05-20
// -----------------------------------------------------------------------------
unsigned int PHY_RolePionLOG_Maintenance::GetNbrAvailableDiagnosersAllowedToWork( const PHY_ComposanteTypePion& type ) const
{
    unsigned int available = 0u;
    unsigned int used = 0u;
    ComponentFunctor functor( [&]( const PHY_ComposantePion& component ) -> bool
    {
        if( component.GetType() == type )
        {
            ++ available;
            if( !component.CanRepair() )
                ++ used;
        }
        return false;
    } );
    DefaultComponentFunctorComputer computer( functor );
    owner_.Execute< OnComponentComputer_ABC >( computer );

    unsigned int availableAllowedToWork = 0u;
    const unsigned int allowedToWork = pWorkRate_->GetNbrWorkerAllowedToWork( available );
    if( allowedToWork > used )
        availableAllowedToWork = allowedToWork - used;
    return availableAllowedToWork;
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
    DefaultComponentFunctorComputer componentComputer( functorOnComponent );
    owner_.Execute< OnComponentComputer_ABC >( componentComputer );
    GetComponentLendedUseFunctor functorOnLendedComponent( predicate, composanteUse );
    DefaultComponentLendedFunctorComputer lendedComputer( functorOnLendedComponent );
    owner_.Execute< OnComponentLendedFunctorComputer_ABC >( lendedComputer );
    unsigned int nNbrAvailableAllowedToWork = 0;
    assert( pWorkRate_ );
    for( auto it = composanteUse.begin(); it != composanteUse.end(); ++it )
    {
        const unsigned int nNbrAllowedToWork = pWorkRate_->GetNbrWorkerAllowedToWork( it->second.nNbrAvailable_ );
        if( nNbrAllowedToWork > it->second.nNbrUsed_ )
            nNbrAvailableAllowedToWork += ( nNbrAllowedToWork - it->second.nNbrUsed_ );
    }
    return nNbrAvailableAllowedToWork;
}

PHY_ComposantePion* PHY_RolePionLOG_Maintenance::GetAvailableDiagnoser( const PHY_ComposanteTypePion* type ) const
{
    ComponentFunctor functor( [&]( const PHY_ComposantePion& component )
    {
        return component.CanRepair() && ( !type || component.GetType() == *type );
    } );
    DefaultComponentFunctorComputer computer( functor );
    owner_.Execute< OnComponentComputer_ABC >( computer );

    if( type && GetNbrAvailableDiagnosersAllowedToWork( *type ) == 0 )
        return 0;

    return functor.component_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::GetAvailableRepairer
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RolePionLOG_Maintenance::GetAvailableRepairer( const PHY_Breakdown& breakdown, const PHY_ComposanteTypePion* type ) const
{
    ComponentFunctor functor( [&]( const PHY_ComposantePion& component )
    {
        return component.CanRepair( breakdown ) && ( !type || component.GetType() == *type );
    } );
    DefaultComponentFunctorComputer computer( functor );
    owner_.Execute< OnComponentComputer_ABC >( computer );
    if( functor.component_ && GetNbrAvailableRepairersAllowedToWork( breakdown ) > 0 )
        return functor.component_;
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
    DefaultComponentFunctorComputer computer( functor );
    owner_.Execute< OnComponentComputer_ABC >( computer );
    return functor.result_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::HasUsableDiagnoser
// Created: SLI 2014-02-12
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Maintenance::HasUsableDiagnoser() const
{
    PHY_ComposanteTypePredicate predicate( &PHY_ComposanteTypePion::CanRepair );
    HasUsableComponentFunctor functor( predicate );
    DefaultComponentFunctorComputer computer( functor );
    owner_.Execute< OnComponentComputer_ABC >( computer );
    return functor.result_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::ConsumePartsForBreakdown
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Maintenance::ConsumePartsForBreakdown( const PHY_Breakdown& breakdown )
{
    dotation::DefaultDotationComputer dotationComputer;
    owner_.Execute< dotation::DotationComputer_ABC >( dotationComputer );
    const PHY_BreakdownType::T_PartMap& parts = breakdown.GetNeededParts();
    for( auto it = parts.begin(); it != parts.end(); ++it )
        if( dotationComputer.GetDotationValue( *it->first ) < it->second )
            return false;
    for( auto it = parts.begin(); it != parts.end(); ++it )
    {
//        unsigned int nOut = (unsigned int)roleDotations.ConsumeDotation( *it->first, it->second );
//        assert( nOut == it->second );
        double dotation = it->second;
        owner_.Apply( &dotation::ConsumeDotationNotificationHandler_ABC::NotifyConsumeDotation, *it->first, dotation );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::GetWorkRate
// Created: LGY 2012-06-21
// -----------------------------------------------------------------------------
int PHY_RolePionLOG_Maintenance::GetWorkRate() const
{
    return static_cast< int >( pWorkRate_->GetAsnID() );
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
        nWorkRateWarningRCTick_ = MIL_Time_ABC::GetTime().GetCurrentTimeStep() + pWorkRate_->GetDelayBeforeWarningRC();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::InsertConsigns
// Created: JVT 2005-05-03
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::InsertConsigns( const T_MaintenanceConsigns& oldConsigns )
{
    for( auto it = oldConsigns.begin(); it != oldConsigns.end(); ++it )
        for( auto it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
            InsertConsign( *it2 );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::GetautomatePriorities
// Created: LGY 2012-06-21
// -----------------------------------------------------------------------------
PHY_RolePionLOG_Maintenance::T_AutomateVector PHY_RolePionLOG_Maintenance::GetAutomatePriorities() const
{
    T_AutomateVector result;
    for( auto it = consigns_.begin(); it != consigns_.end(); ++it )
        if( it->first )
            result.push_back( it->first );
    return result;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::ChangePriorities
// Created: NLD 2005-04-27
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::ChangePriorities( const T_AutomateVector& priorities )
{
    T_MaintenanceConsigns oldConsigns;
    consigns_.swap( oldConsigns );
    for( auto it = priorities.begin(); it != priorities.end(); ++it )
        consigns_.push_back( std::make_pair( *it, T_MaintenanceConsignList() ) );
    consigns_.push_back( T_MaintenanceConsigns::value_type() );
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
    for( auto it = consigns_.begin(); it != consigns_.end(); ++it )
        it->second.clear();
    priorities_ = priorities;
    bHasChanged_ = true;
    InsertConsigns( oldConsigns );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::GetMaintenancePriorities
// Created: LGY 2012-06-21
// -----------------------------------------------------------------------------
PHY_RolePionLOG_Maintenance::T_MaintenancePriorityVector PHY_RolePionLOG_Maintenance::GetMaintenancePriorities() const
{
    return priorities_;
}

namespace
{
    bool IsPriorityEqual( const boost::shared_ptr< PHY_MaintenanceConsign_ABC >& pConsign, const PHY_ComposanteTypePion* pCompType )
    {
        if( pConsign->IsFinished() )
            return false;
        return *pCompType == pConsign->GetComposanteType();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::InsertConsign
// Created: JVT 2005-05-03
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::InsertConsign( const boost::shared_ptr< PHY_MaintenanceConsign_ABC >& consign )
{
    if( !consign->HasValidComposanteState() )
        return;
    auto itTact = consigns_.begin();
    const MIL_Automate* pAutomate = &consign->GetComposanteState()->GetAutomate();
    for( ; itTact != consigns_.end(); ++itTact )
        if( pAutomate == itTact->first ) // TODO || ( pAutomate->GetTC2() && pAutomate->GetTC2() == itTact->first ) )
            break;
    if( itTact == consigns_.end() )
    {
        assert( !consigns_.empty() );
        itTact = consigns_.end() - 1;
        assert( itTact->first == 0 );
    }
    auto itPriority = std::find( priorities_.begin(), priorities_.end(), &consign->GetComposanteType() );
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
// Name: PHY_RolePionLOG_Maintenance::HandleComposanteForTransport
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
boost::shared_ptr< PHY_MaintenanceComposanteState > PHY_RolePionLOG_Maintenance::HandleComposanteForTransport( MIL_Agent_ABC& pion, PHY_ComposantePion& composante )
{
    assert( composante.GetBreakdown() );
    if( !bSystemEnabled_ )
        return boost::shared_ptr< PHY_MaintenanceComposanteState >();
    const auto state = boost::make_shared< PHY_MaintenanceComposanteState >( pion, composante );
    InsertConsign( boost::make_shared< PHY_MaintenanceTransportConsign >( owner_, state ) );
    return state;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::HandleComposanteForTransport
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Maintenance::HandleComposanteForTransport( const boost::shared_ptr< PHY_MaintenanceComposanteState >& state )
{
    if( !bSystemEnabled_ || state->GetComposanteBreakdown().AffectMobility() && !HasUsableHauler( state->GetComposante().GetType() ) )
        return false;
    InsertConsign( boost::make_shared< PHY_MaintenanceTransportConsign >( owner_, state ) );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::GetAvailabilityScoreForTransport
// Created: NLD 2006-03-29
// -----------------------------------------------------------------------------
int PHY_RolePionLOG_Maintenance::GetAvailabilityScoreForTransport( const PHY_ComposantePion& composante ) const
{
    if( !bSystemEnabled_ )
        return std::numeric_limits< int >::min();
    if( !composante.GetBreakdown()->AffectMobility() )
        return 0;
    PHY_Composante_ABC::T_ComposanteUseMap composanteUse;
    PHY_ComposanteUsePredicate1< PHY_ComposanteTypePion > predicate( &PHY_ComposantePion::CanHaul1, &PHY_ComposanteTypePion::CanHaul1, composante.GetType() );
    GetComponentUseFunctor functorOnComponent( predicate, composanteUse );
    DefaultComponentFunctorComputer componentComputer( functorOnComponent );
    owner_.Execute< OnComponentComputer_ABC >( componentComputer );
    GetComponentLendedUseFunctor functorOnLendedComponent( predicate, composanteUse );
    if( composanteUse.size() == 0 )
        return std::numeric_limits< int >::min();
    DefaultComponentLendedFunctorComputer lendedComputer( functorOnLendedComponent );
    owner_.Execute< OnComponentLendedFunctorComputer_ABC >( lendedComputer );
    unsigned int nNbrHaulersAvailable = 0;
    for( auto it = composanteUse.begin(); it != composanteUse.end(); ++it )
        nNbrHaulersAvailable += ( it->second.nNbrAvailable_ - it->second.nNbrUsed_ );
    return nNbrHaulersAvailable;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::HandleComposanteForRepair
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Maintenance::HandleComposanteForRepair( const boost::shared_ptr< PHY_MaintenanceComposanteState >& state )
{
    if( !bSystemEnabled_ || !HasUsableRepairer( state->GetComposanteBreakdown() ) )
        return false;
    InsertConsign( boost::make_shared< PHY_MaintenanceRepairConsign >( owner_, state ) );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::HandleComposanteForDiagnosis
// Created: SLI 2014-02-12
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Maintenance::HandleComposanteForDiagnosis( const boost::shared_ptr< PHY_MaintenanceComposanteState >& state )
{
    if( !bSystemEnabled_ || !HasUsableDiagnoser() )
        return false;
    InsertConsign( boost::make_shared< PHY_MaintenanceDiagnosisConsign >( owner_, state ) );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::GetAvailabilityScoreForDiagnosis
// Created: SLI 2014-02-12
// -----------------------------------------------------------------------------
int PHY_RolePionLOG_Maintenance::GetAvailabilityScoreForDiagnosis( const PHY_ComposanteTypePion* type /*= 0*/ ) const
{
    if( !bSystemEnabled_ || !GetAvailableDiagnoser( type ) )
        return std::numeric_limits< int >::min();
    return GetNbrAvailableDiagnosersAllowedToWork();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::GetAvailabilityScoreForRepair
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
int PHY_RolePionLOG_Maintenance::GetAvailabilityScoreForRepair( const boost::shared_ptr< PHY_MaintenanceComposanteState >& state, const PHY_ComposanteTypePion* type /*= 0*/ ) const
{
    if( !bSystemEnabled_ || !GetAvailableRepairer( state->GetComposanteBreakdown(), type ) )
        return std::numeric_limits< int >::min();
    // Parts score
    double rRatioPartsAvailable = 0.;
    dotation::DefaultDotationComputer dotationComputer;
    owner_.Execute< dotation::DotationComputer_ABC >( dotationComputer );

    const PHY_BreakdownType::T_PartMap& parts = state->GetComposanteBreakdown().GetNeededParts();
    for( auto it = parts.begin(); it != parts.end(); ++it )
    {
        // Parts never available ...
        if( dotationComputer.GetDotationCapacity( *it->first ) <= 0. )
            return std::numeric_limits< int >::min();

        rRatioPartsAvailable += ( dotationComputer.GetDotationValue( *it->first ) / it->second );
    }
    if( parts.empty() )
        rRatioPartsAvailable = 1.;
    else
        rRatioPartsAvailable /= parts.size();
    // Repairers score
    return (unsigned int)( GetNbrAvailableRepairersAllowedToWork( state->GetComposanteBreakdown() ) * rRatioPartsAvailable );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::Update
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::Update( bool /*bIsDead*/ )
{
    if( nWorkRateWarningRCTick_ != 0 && MIL_Time_ABC::GetTime().GetCurrentTimeStep() > nWorkRateWarningRCTick_ )
    {
        nWorkRateWarningRCTick_ = 0;
        MIL_Report::PostEvent< MIL_Agent_ABC >( owner_, report::eRC_RegimeMaintenanceDelaiDepasse );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::UpdateLogistic
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::UpdateLogistic( bool /*bIsDead*/ )
{
    for( auto itConsigns = consigns_.begin(); itConsigns != consigns_.end(); ++itConsigns )
        for ( auto itConsign = itConsigns->second.begin(); itConsign != itConsigns->second.end(); )
            if( (*itConsign)->Update() )
                itConsign = itConsigns->second.erase( itConsign );
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

namespace
{
    void SendComposanteUse( const PHY_Composante_ABC::T_ComposanteUseMap& data, sword::SeqOfLogMaintenanceEquipmentAvailability& asn, const PHY_MaintenanceWorkRate* pWorkRate )
    {
        for( auto it = data.begin(); it != data.end(); ++it )
        {
            sword::LogMaintenanceEquipmentAvailability& data = *asn.add_elem();
            data.mutable_equipment()->set_id( it->first->GetMosID().id() );
            assert( it->second.nNbrTotal_ );
            data.set_total( it->second.nNbrTotal_ );
            data.set_working( it->second.nNbrUsed_ );
            data.set_available( it->second.nNbrAvailable_ - it->second.nNbrUsed_ ); // nNbrAvailableAllowedToWork
            data.set_lent( it->second.nNbrLent_ );
            if( pWorkRate )
            {
                const unsigned int nNbrAllowedToWork = pWorkRate->GetNbrWorkerAllowedToWork( it->second.nNbrAvailable_ );
                const unsigned int nNbrAvailableAllowedToWork = nNbrAllowedToWork > it->second.nNbrUsed_ ? nNbrAllowedToWork - it->second.nNbrUsed_ : 0;
                data.set_available( nNbrAvailableAllowedToWork );
                data.set_resting( it->second.nNbrAvailable_ - nNbrAvailableAllowedToWork - it->second.nNbrUsed_ );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::SendFullState
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::SendFullState( unsigned int context ) const
{
    client::LogMaintenanceState asn;
    asn().mutable_unit()->set_id( owner_.GetID() );
    asn().set_chain( bSystemEnabled_ );
    asn().set_work_rate( pWorkRate_->GetAsnID() );
    asn().mutable_priorities();
    for( auto itPriority = priorities_.begin(); itPriority != priorities_.end(); ++itPriority )
        asn().mutable_priorities()->add_elem()->set_id( (**itPriority).GetMosID().id() );
    asn().mutable_tactical_priorities();
    for( auto itPriority = tacticalPriorities_.begin(); itPriority != tacticalPriorities_.end(); ++itPriority )
        asn().mutable_tactical_priorities()->add_elem()->set_id( (**itPriority).GetID() );
    {
        PHY_Composante_ABC::T_ComposanteUseMap composanteUse;
        PHY_ComposanteUsePredicate predicate( &PHY_ComposantePion::CanHaul, &PHY_ComposanteTypePion::CanHaul );
        GetComponentUseFunctor functorOnComponent( predicate, composanteUse );
        DefaultComponentFunctorComputer componentComputer( functorOnComponent );
        owner_.Execute< OnComponentComputer_ABC >( componentComputer );
        GetComponentLendedUseFunctor functorOnLendedComponent( predicate, composanteUse );
        DefaultComponentLendedFunctorComputer lendedComputer( functorOnLendedComponent );
        owner_.Execute< OnComponentLendedFunctorComputer_ABC >( lendedComputer );
        SendComposanteUse( composanteUse, *asn().mutable_haulers(), 0 );
    }
    {
        PHY_Composante_ABC::T_ComposanteUseMap composanteUse;
        PHY_ComposanteUsePredicate predicate( &PHY_ComposantePion::CanRepair, &PHY_ComposanteTypePion::CanRepair );
        GetComponentUseFunctor functorOnComponent( predicate, composanteUse );
        DefaultComponentFunctorComputer componentComputer( functorOnComponent );
        owner_.Execute< OnComponentComputer_ABC >( componentComputer );
        GetComponentLendedUseFunctor functorOnLendedComponent( predicate, composanteUse );
        DefaultComponentLendedFunctorComputer lendedComputer( functorOnLendedComponent );
        owner_.Execute< OnComponentLendedFunctorComputer_ABC >( lendedComputer );
        SendComposanteUse( composanteUse, *asn().mutable_repairers(), pWorkRate_ );
    }
    asn.Send( NET_Publisher_ABC::Publisher(), context );
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
    return owner_;
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

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::NotifyIsLoadedInVab
// Created: LDC 2014-02-19
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::NotifyIsLoadedInVab()
{
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::NotifyIsUnLoadedInVab
// Created: LDC 2014-02-19
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::NotifyIsUnLoadedInVab()
{
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::FinishAllHandlingsSuccessfullyWithoutDelay
// Created: NLD 2012-01-09
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Maintenance::FinishAllHandlingsSuccessfullyWithoutDelay()
{
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
// Name: PHY_RolePionLOG_Maintenance::ClearMaintenanceConsigns
// Created: JSR 2013-02-12
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Maintenance::ClearMaintenanceConsigns()
{
    for( auto itConsigns = consigns_.begin(); itConsigns != consigns_.end(); ++itConsigns )
        for( auto itConsign = itConsigns->second.begin(); itConsign != itConsigns->second.end(); ++itConsign )
            (*itConsign)->ClearConsign();
}
