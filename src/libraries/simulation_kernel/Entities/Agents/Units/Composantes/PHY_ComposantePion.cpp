// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Composantes/PHY_ComposantePion.cpp $
// $Author: Jvt $
// $Modtime: 11/05/05 14:49 $
// $Revision: 28 $
// $Workfile: PHY_ComposantePion.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ComposantePion.h"
#include "MIL_Time_ABC.h"
#include "WeaponAvailabilityComputer_ABC.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Units/Categories/PHY_Protection.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory_IndirectFire_ABC.h"
#include "Entities/Agents/Units/Humans/PHY_HumansComposante.h"
#include "Entities/Agents/Units/Logistic/PHY_Breakdown.h"
#include "Entities/Agents/Units/Sensors/PHY_Sensor.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorType.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeAgent.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeObject.h"
#include "Entities/Agents/Units/Weapons/PHY_Weapon.h"
#include "Entities/Agents/Units/Weapons/PHY_AttritionData.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Agents/Roles/Transported/PHY_RoleInterface_Transported.h"
#include "Entities/Agents/Roles/Logistic/PHY_MaintenanceComposanteState.h"
#include "Entities/Agents/Roles/Logistic/PHY_MaintenanceTransportConsign.h"
#include "Entities/Agents/Roles/Surrender/PHY_RoleInterface_Surrender.h"
#include "Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.h"
#include "Entities/Agents/Actions/Underground/PHY_RoleAction_MovingUnderground.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/AttritionCapacity.h"
#include "Entities/Objects/StructuralCapacity.h"
#include "Entities/Populations/MIL_PopulationType.h"
#include "Entities/Actions/PHY_FireDamages_Agent.h"
#include "Entities/Orders/MIL_Report.h"
#include "Knowledge/DEC_Knowledge_AgentComposante.h"
#include "CheckPoints/SerializationTools.h"
#include "Tools/NET_AsnException.h"

double PHY_ComposantePion::rOpStateWeightHumans_ = 0.;

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_ComposantePion )

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion constructor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_ComposantePion::PHY_ComposantePion( const MIL_Time_ABC& time, const PHY_ComposanteTypePion& type, PHY_RolePion_Composantes& role, unsigned int nNbrHumanInCrew, bool bMajor, bool bLoadable, bool bCanBePartOfConvoy )
    : time_( time )
    , pRole_( &role )
    , pState_( &PHY_ComposanteState::undamaged_ )
    , pType_( &type )
    , bMajor_( bMajor )
    , bLoadable_( bLoadable )
    , bCanBePartOfConvoy_( bCanBePartOfConvoy )
    , bUsedForLogistic_( false )
    , bRepairEvacuationNoMeansChecked_( false )
    , pHumans_( new PHY_HumansComposante( time, *this, nNbrHumanInCrew ) )
    , nAutoRepairTimeStep_( 0 )
    , nRandomBreakdownNextTimeStep_( 0 )
    , pRandomBreakdownState_( 0 )
{
    pType_->InstanciateWeapons( std::back_inserter( weapons_ ) );
    pType_->InstanciateSensors( std::back_inserter( sensors_ ) );
    pRole_->NotifyComposanteAdded( *this, 0 );
    assert( pHumans_->IsViable() ); //$$$ Pre-check in PHY_UnitType
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion constructor
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
PHY_ComposantePion::PHY_ComposantePion()
    : time_( MIL_Time_ABC::GetTime() )
    , pRole_( 0 )
    , pState_( 0 )
    , pType_( 0 )
    , bMajor_( false )
    , bLoadable_( false )
    , bCanBePartOfConvoy_( false )
    , bUsedForLogistic_( false )
    , bRepairEvacuationNoMeansChecked_( false )
    , pHumans_( 0 )
    , nAutoRepairTimeStep_( 0 )
    , nRandomBreakdownNextTimeStep_( 0 )
    , pRandomBreakdownState_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion destructor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_ComposantePion::~PHY_ComposantePion()
{
    for( auto it = weapons_.begin(); it != weapons_.end(); ++it )
        delete *it;
    for( auto it = sensors_.begin(); it != sensors_.end(); ++it )
        delete *it;
    assert( pRole_ );
    pRole_->NotifyComposanteRemoved( *this );
    delete pHumans_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::load
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
void PHY_ComposantePion::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< PHY_Composante_ABC >( *this )
         >> pRole_;
    unsigned int nID;
    file >> nID;
    pState_ = &PHY_ComposanteState::Find( nID );
    file >> pType_;
    pType_->InstanciateWeapons( std::back_inserter( weapons_ ) );
    pType_->InstanciateSensors( std::back_inserter( sensors_ ) );
    file >> const_cast< bool& >( bMajor_ )
         >> const_cast< bool& >( bLoadable_ )
         >> const_cast< bool& >( bCanBePartOfConvoy_ )
         >> bUsedForLogistic_
         >> pHumans_
         >> nAutoRepairTimeStep_
         >> pBreakdown_
         >> pMaintenanceState_
         >> bRepairEvacuationNoMeansChecked_
         >> nRandomBreakdownNextTimeStep_;
    if( nRandomBreakdownNextTimeStep_ )
    {
        file >> nID;
        pRandomBreakdownState_ = &PHY_ComposanteState::Find( nID );
    }
    else
        pRandomBreakdownState_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::save
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
void PHY_ComposantePion::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    unsigned int state = pState_->GetID();
    file << boost::serialization::base_object< PHY_Composante_ABC >( *this )
         << pRole_
         << state
         << pType_
         << bMajor_
         << bLoadable_
         << bCanBePartOfConvoy_
         << bUsedForLogistic_
         << pHumans_
         << nAutoRepairTimeStep_
         << pBreakdown_
         << pMaintenanceState_
         << bRepairEvacuationNoMeansChecked_
         << nRandomBreakdownNextTimeStep_;
    if( nRandomBreakdownNextTimeStep_ )
    {
        assert( pRandomBreakdownState_ );
        unsigned int id = pRandomBreakdownState_->GetID();
        file << id;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::TransferComposante
// Created: JVT 2005-01-17
// -----------------------------------------------------------------------------
void PHY_ComposantePion::TransferComposante( PHY_RoleInterface_Composantes& newRole )
{
    assert( pRole_ );
    assert( pHumans_ );
    // Annulation log maintenance
    if( pMaintenanceState_ )
    {
        DeleteMaintenanceState();
        bRepairEvacuationNoMeansChecked_ = false;
    }
    pHumans_->NotifyComposanteTransfered( *pRole_, newRole );
    auto dotationsRemoved = pRole_->NotifyComposanteRemoved( *this );
    pRole_ = &newRole;
    pRole_->NotifyComposanteAdded( *this, &dotationsRemoved );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ReinitializeState
// Created: NLD 2004-09-08
// Modified: ABR 2011-03-07
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ReinitializeState( const PHY_ComposanteState& state, const PHY_BreakdownType* breakdownType /*= 0*/ )
{
    assert( pType_ );
    const PHY_ComposanteState* pState = &state;
    if( pType_->GetProtection().IsHuman() && ( state == PHY_ComposanteState::repairableWithEvacuation_ || state == PHY_ComposanteState::repairableWithoutEvacuation_ ) )
        pState = &PHY_ComposanteState::undamaged_;
    if( *pState_ == *pState && ( !pBreakdown_ || &pBreakdown_->GetType() == breakdownType ) )
        return;
    if( breakdownType )
    {
        if( !pType_->CanHaveBreakdown( breakdownType ) )
            return;
        pBreakdown_.reset( new PHY_Breakdown( breakdownType ) );
    }
    else if( *pState == PHY_ComposanteState::repairableWithEvacuation_ && !pBreakdown_ )
        pBreakdown_.reset( new PHY_Breakdown( pType_->GetRandomBreakdownType() ) );
    bRepairEvacuationNoMeansChecked_ = false;
    std::swap( pState_, pState );
    ManageEndMaintenance();
    pRole_->NotifyComposanteChanged( *this, *pState );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetMaxSpeed( const TerrainData& data ) const
{
    assert( pType_ );
    return pType_->GetMaxSpeed( data );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetMaxSpeed() const
{
    assert( pType_ );
    return pType_->GetMaxSpeed();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMaxSpeed
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetMaxSpeed( const MIL_Object_ABC& object ) const
{
    return pType_->GetMaxSpeed( object );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetConsumptionsDotations
// Created: NLD 2004-09-30
// -----------------------------------------------------------------------------
const PHY_DotationConsumptions* PHY_ComposantePion::GetDotationConsumptions( const PHY_ConsumptionType& consumptionMode ) const
{
    assert( pState_ );
    assert( pType_ );
    return pState_->IsUsable() ? pType_->GetDotationConsumptions( consumptionMode ) : 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetConsumptionMode
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
const PHY_ConsumptionType& PHY_ComposantePion::GetConsumptionMode( const MIL_ObjectType_ABC& objectType ) const
{
    assert( pType_ );
    return pType_->GetConsumptionMode( objectType );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyFire
// Created: NLD 2004-10-13
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ApplyFire( const PHY_AttritionData& attritionData, double urbanProtection, PHY_FireDamages_Agent& fireDamages )
{
    //$$$ FACTORISER AVEC REINITIALIZESTATE()
    assert( pRole_ );
    assert( pType_ );
    assert( CanBeFired() );
    assert( pState_ );
    const PHY_ComposanteState& oldState  = *pState_;
    const PHY_ComposanteState* pNewState = &attritionData.ComputeComposanteState( urbanProtection );
    pRole_->WoundLoadedHumans( *this, *pNewState, fireDamages );
    ApplyHumansWounds( *pNewState, fireDamages );
    if( pType_->GetProtection().IsHuman() && ( *pNewState == PHY_ComposanteState::repairableWithEvacuation_ || *pNewState == PHY_ComposanteState::repairableWithoutEvacuation_ ) )
        pNewState = &PHY_ComposanteState::undamaged_;
    ApplyNewComposanteState( *pNewState, oldState );
    fireDamages.NotifyComposanteStateChanged( *this, oldState, *pState_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyExplosion
// Created: NLD 2004-10-13
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ApplyExplosion( const AttritionCapacity& capacity, PHY_FireDamages_Agent& fireDamages )
{
    assert( pType_ );
    ApplyFire( capacity.GetAttritionData( pType_->GetProtection() ), 0, fireDamages );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyPopulationFire
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ApplyPopulationFire( const MIL_PopulationType& populationType, const MIL_PopulationAttitude& populationAttitude, PHY_FireDamages_Agent& fireDamages, double armedIndividuals )
{
    assert( pType_ );
    ApplyFire( populationType.GetAttritionData( populationAttitude, pType_->GetProtection(), armedIndividuals ), 0, fireDamages );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyDirectFire
// Created: NLD 2005-08-04
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ApplyDirectFire( const PHY_DotationCategory& dotationCategory, PHY_FireDamages_Agent& fireDamages )
{
    assert( pType_ );
    double urbanProtection = pRole_->GetPion().GetRole< PHY_RoleInterface_UrbanLocation >().ComputeUrbanProtection( dotationCategory );
    ApplyFire( dotationCategory.GetAttritionData( pType_->GetProtection() ), urbanProtection, fireDamages );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyUrbanObjectCrumbling
// Created: SLG 2010-07-01
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ApplyUrbanObjectCrumbling( const MIL_Object_ABC& object )
{
    //$$$ FACTORISER AVEC REINITIALIZESTATE()
    assert( pRole_ );
    assert( pType_ );
    assert( CanBeFired() );
    assert( pState_ );
    const PHY_ComposanteState& oldState  = *pState_;
    const PHY_ComposanteState* pNewState = pState_;
    StructuralCapacity* capacity = const_cast< MIL_Object_ABC& >( object ).Retrieve< StructuralCapacity >();
    if( capacity )
        pNewState = &capacity->ComputeComposanteState( object, pType_->GetProtection() );
    PHY_FireDamages_Agent fireDamages;
    pRole_->WoundLoadedHumans( *this, *pNewState, fireDamages );
    ApplyHumansWounds( *pNewState, fireDamages );
    if( pType_->GetProtection().IsHuman() && ( *pNewState == PHY_ComposanteState::repairableWithEvacuation_ || *pNewState == PHY_ComposanteState::repairableWithoutEvacuation_ ) )
        pNewState = &PHY_ComposanteState::undamaged_;
    ApplyNewComposanteState( *pNewState, oldState );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyNewComposanteState
// Created: SLG 2010-07-02
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ApplyNewComposanteState( const PHY_ComposanteState& pNewState, const PHY_ComposanteState& oldState )
{
    if( pNewState < *pState_ )
    {
        pState_ = &pNewState;
        if( *pState_ == PHY_ComposanteState::repairableWithEvacuation_ && !pBreakdown_ )
            pBreakdown_.reset( new PHY_Breakdown( pType_->GetAttritionBreakdownType() ) );
        ManageEndMaintenance();
        pRole_->NotifyComposanteChanged( *this, oldState );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyIndirectFire
// Created: NLD 2005-08-04
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ApplyIndirectFire( const PHY_DotationCategory& dotationCategory, PHY_FireDamages_Agent& fireDamages, double ratio )
{
    assert( pType_ );
    const PHY_DotationCategory::T_IndirectFireEffects& effects = dotationCategory.GetIndirectFireEffects();
    for( auto it = effects.begin(); it != effects.end(); ++it )
        if( (*it)->HasHit( pRole_->GetPion(), ratio ) )
        {
            double urbanProtection = pRole_->GetPion().GetRole< PHY_RoleInterface_UrbanLocation >().ComputeUrbanProtection( dotationCategory );
            ApplyFire( dotationCategory.GetAttritionData( pType_->GetProtection() ), urbanProtection, fireDamages );
        }
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyContamination
// Created: NLD 2004-10-13
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ApplyContamination()
{
    assert( pHumans_ );
    pHumans_->ApplyContamination();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyEffect
// Created: LGY 2012-12-13
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ApplyEffect( const WoundEffects_ABC& effects )
{
    assert( pHumans_ );
    pHumans_->ApplyEffect( effects );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetProtectionHumanDeadRatio
// Created: JSR 2011-08-11
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetProtectionHumanDeadRatio( const PHY_ComposanteState& state ) const
{
    assert( pType_ );
    return pType_->GetProtection().GetHumanDeadRatio( state );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetProtectionHumanWoundedRatio
// Created: JSR 2011-08-11
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetProtectionHumanWoundedRatio( const PHY_ComposanteState& state ) const
{
    assert( pType_ );
    return pType_->GetProtection().GetHumanWoundedRatio( state );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetDangerosity
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetDangerosity( const DEC_Knowledge_AgentComposante& compTarget, double rDistBtwFirerAndTarget, bool bUseAmmo ) const
{
    assert( pType_ );
    assert( pRole_ );
    return CanFire() ? pType_->GetDangerosity( pRole_->GetPion(), compTarget.GetType(), rDistBtwFirerAndTarget, bUseAmmo ) : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetOnlyLoadableMaxRangeToFireOn
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetOnlyLoadableMaxRangeToFireOn( const DEC_Knowledge_AgentComposante& compTarget, double rWantedPH ) const
{
    assert( pType_ );
    return CanFireWhenUnloaded() ? pType_->GetMaxRangeToFireOn( pRole_->GetPion(), compTarget.GetType(), rWantedPH, 0 ) : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMaxRangeToFireOn
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetMaxRangeToFireOn( const DEC_Knowledge_AgentComposante& compTarget, double rWantedPH, const PHY_DotationCategory* dotation ) const
{
    assert( pType_ );
    return CanFire() ? pType_->GetMaxRangeToFireOn( pRole_->GetPion(), compTarget.GetType(), rWantedPH, dotation ) : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMinRangeToFireOn
// Created: JVT 2004-12-17
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetMinRangeToFireOn( const DEC_Knowledge_AgentComposante& compTarget, double rWantedPH ) const
{
    assert( pType_ );
    return CanFire() ? pType_->GetMinRangeToFireOn( pRole_->GetPion(), compTarget.GetType(), rWantedPH ) : std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMaxRangeToFireOnWithPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetMaxRangeToFireOnWithPosture( const DEC_Knowledge_AgentComposante& compTarget, const MIL_Agent_ABC& target, double rWantedPH ) const
{
    assert( pType_ );
    return CanFire() ? pType_->GetMaxRangeToFireOnWithPosture( pRole_->GetPion(), target, compTarget.GetType(), rWantedPH ) : 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMinRangeToFireOnWithPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetMinRangeToFireOnWithPosture( const DEC_Knowledge_AgentComposante& compTarget, const MIL_Agent_ABC& target, double rWantedPH ) const
{
    assert( pType_ );
    return CanFire() ? pType_->GetMinRangeToFireOnWithPosture( pRole_->GetPion(), target, compTarget.GetType(), rWantedPH ) : std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMaxRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetMaxRangeToIndirectFire( const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability, bool theoric ) const
{
    assert( pType_ );
    return ( CanFire() || theoric ) ? pType_->GetMaxRangeToIndirectFire( pRole_->GetPion(), dotationCategory, bCheckDotationsAvailability ) : -1.;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMaxRangeToDirectFire
// Created: MCO 2012-11-23
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetMaxRangeToDirectFire() const
{
    assert( pType_ );
    return pType_->GetMaxRangeToDirectFire();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMaxRangeToIndirectFire
// Created: GGE 2012-07-25
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetMaxRangeToIndirectFire( ) const
{
    assert( pType_ );
    return pType_->GetMaxRangeToIndirectFire( pRole_->GetPion() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMinRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetMinRangeToIndirectFire( const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability, bool theoric ) const
{
    assert( pType_ );
    return ( CanFire() || theoric ) ? pType_->GetMinRangeToIndirectFire( pRole_->GetPion(), dotationCategory, bCheckDotationsAvailability ) : std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMaxRangeToFire
// Created: DDA 2010-05-03
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetMaxRangeToFire(const MIL_Agent_ABC& pion, double rWantedPH ) const
{
    assert( pType_ );
    return pType_->GetMaxRangeToFire( pion, rWantedPH );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::HasWoundedHumansToEvacuate
// Created: NLD 2005-08-08
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::HasWoundedHumansToEvacuate() const
{
    assert( pHumans_ );
    return pHumans_->HasWoundedHumansToEvacuate();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::EvacuateWoundedHumans
// Created: NLD 2005-08-01
// -----------------------------------------------------------------------------
void PHY_ComposantePion::EvacuateWoundedHumans( MIL_AutomateLOG& destinationTC2 ) const
{
    assert( pHumans_ );
    pHumans_->EvacuateWoundedHumans( destinationTC2 );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::PreprocessRandomBreakdowns
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
void PHY_ComposantePion::PreprocessRandomBreakdowns( unsigned int nEndDayTimeStep )
{
    assert( !pRandomBreakdownState_ && !nRandomBreakdownNextTimeStep_ );
    assert( pType_ );
    if( pType_->GetProtection().CanRandomlyBreaksDownEva() )
    {
        pRandomBreakdownState_ = &PHY_ComposanteState::repairableWithEvacuation_;
        nRandomBreakdownNextTimeStep_ = MIL_Random::rand32_oo( time_.GetCurrentTimeStep(), nEndDayTimeStep );
    }
    else if( pType_->GetProtection().CanRandomlyBreaksDownNeva() )
    {
        pRandomBreakdownState_ = &PHY_ComposanteState::repairableWithoutEvacuation_;
        nRandomBreakdownNextTimeStep_ = MIL_Random::rand32_oo( time_.GetCurrentTimeStep(), nEndDayTimeStep );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ManageEndMaintenance
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ManageEndMaintenance()
{
    if( *pState_ == PHY_ComposanteState::maintenance_ )
        return;
    DeleteMaintenanceState();
    if( *pState_ == PHY_ComposanteState::repairableWithEvacuation_ )
        return;
    pBreakdown_.reset();
    if( *pState_ == PHY_ComposanteState::repairableWithoutEvacuation_ )
    {
        assert( pType_ );
        nAutoRepairTimeStep_ = std::max( nAutoRepairTimeStep_, time_.GetCurrentTimeStep() + pType_->GetProtection().GetNeutralizationTime() );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::NotifyHandledByMaintenance
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void PHY_ComposantePion::NotifyHandledByMaintenance()
{
    assert( pHumans_ );
    ReinitializeState( PHY_ComposanteState::maintenance_ );
    pHumans_->NotifyComposanteHandledByMaintenance();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::NotifyRepairedByMaintenance
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_ComposantePion::NotifyRepairedByMaintenance()
{
    assert( pRole_ );
    assert( pType_ );
    MIL_Report::PostEvent( pRole_->GetPion(), report::eRC_MaterielRetourDeMaintenance, *pType_ );
    pRole_->NotifyComposanteRepaired();
    ReinitializeState( PHY_ComposanteState::undamaged_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanBeUsed
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanBeUsed( bool bWithLoaded ) const
{
    const transport::PHY_RoleAction_Loading* roleLoading = ( bLoadable_ && !bWithLoaded ) ? pRole_->GetPion().RetrieveRole< transport::PHY_RoleAction_Loading >() : 0;
    if( !CanComponentBeUsed( roleLoading, bWithLoaded ) )
        return false;
    assert( pRole_ );
    // $$$$ LDC: All this should be rewritten with GetRole/RetrieveRole's but using Apply.
    const transport::PHY_RoleInterface_Transported* roleTransported = pRole_->GetPion().RetrieveRole< transport::PHY_RoleInterface_Transported >();
    if( roleTransported && roleTransported->IsTransported() )
        return false;
    const surrender::PHY_RoleInterface_Surrender* roleSurrendered = pRole_->GetPion().RetrieveRole< surrender::PHY_RoleInterface_Surrender >();
    if( roleSurrendered && roleSurrendered->IsSurrendered() )
        return false;
    if( bLoadable_ )
        return true;
    return !roleTransported || !roleTransported->HasHumanTransportersToRecover();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::IsAway
// Created: LGY 2013-09-24
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::IsAway() const
{
    const auto roleTransported = pRole_->GetPion().RetrieveRole< transport::PHY_RoleInterface_Transported >();
    return !bLoadable_ && roleTransported && roleTransported->HasHumanTransportersToRecover();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanComponentBeUsed
// Created: LDC 2012-01-04
// Same as CanBeUsed() but doesn't check stuff that can be used at pion level (performance)
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanComponentBeUsed( const transport::PHY_RoleAction_Loading* roleLoading, bool bWithLoaded ) const
{
    if( bUsedForLogistic_ )
        return false;
    if( bLoadable_ && !bWithLoaded )
        return !roleLoading || roleLoading->IsUnloaded();
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanBeUsedForMove
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanBeUsedForMove() const
{
    assert( pRole_ );
    const transport::PHY_RoleInterface_Transported& roleTransported = pRole_->GetPion().GetRole< transport::PHY_RoleInterface_Transported >();
    if( roleTransported.IsTransported() )
        return false;
    if( pRole_->GetPion().GetRole< PHY_RoleAction_MovingUnderground >().IsUnderground() )
        return false;
    if( bLoadable_ )
        return !pRole_->GetPion().GetRole< transport::PHY_RoleAction_Loading >().IsLoaded();
    return !roleTransported.HasHumanTransportersToRecover();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanFire
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanFire() const
{
    assert( pState_ );
    assert( pRole_ );
    if( pRole_->GetPion().GetRole< PHY_RoleAction_MovingUnderground >().IsUnderground() )
        return false;
    return !pRole_->IsNeutralized() && pState_->IsUsable() && !pState_->IsDamaged() && CanBeUsed();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanFireWhenUnloaded
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanFireWhenUnloaded() const
{
    assert( pState_ );
    assert( pRole_ );
    return !pRole_->IsNeutralized() && pState_->IsUsable() && !pState_->IsDamaged() && bLoadable_ && !bUsedForLogistic_ && !pRole_->GetPion().GetRole< transport::PHY_RoleInterface_Transported >().IsTransported();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::Repair
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void PHY_ComposantePion::Repair( bool withLog )
{
    if( pRole_->GetPion().GetRole< surrender::PHY_RoleInterface_Surrender >().IsPrisoner() )
        ReinitializeState( PHY_ComposanteState::prisoner_ );
    else
        if( !withLog || *pState_ != PHY_ComposanteState::maintenance_ )
            ReinitializeState( PHY_ComposanteState::undamaged_ );
    HealAllHumans( withLog );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::Update
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
void PHY_ComposantePion::Update()
{
    // Réparation automatique
    if( *pState_ == PHY_ComposanteState::repairableWithoutEvacuation_ && time_.GetCurrentTimeStep() >= nAutoRepairTimeStep_ )
    {
        assert( pType_ );
        MIL_Report::PostEvent( pRole_->GetPion(), report::eRC_MaterielRepareSurPlace, *pType_ );
        pRole_->NotifyComposanteRepaired();
        ReinitializeState( PHY_ComposanteState::undamaged_ );
    }
    // Panne aléatoire
    if( pRandomBreakdownState_ && nRandomBreakdownNextTimeStep_ == time_.GetCurrentTimeStep()  )
    {
        if( *pState_ == PHY_ComposanteState::undamaged_ )
            ReinitializeState( *pRandomBreakdownState_ );
        nRandomBreakdownNextTimeStep_ = 0;
        pRandomBreakdownState_ = 0;
    }
    // Demande maintenance
    if( *pState_ == PHY_ComposanteState::repairableWithEvacuation_ && !pMaintenanceState_ )
    {
        assert( pBreakdown_ );
        assert( pRole_ );
        pMaintenanceState_.reset( pRole_->NotifyComposanteWaitingForMaintenance( *this ) );
    }
    if( pRole_->GetPion().IsJammed() ||
        pRole_->GetPion().IsLogisticJammed() ||
        bRepairEvacuationNoMeansChecked_ ||
        *pState_ != PHY_ComposanteState::repairableWithEvacuation_ && pState_->IsUsable() )
        return;
    bRepairEvacuationNoMeansChecked_ = HasRepairEvacuationMeans();
    if( bRepairEvacuationNoMeansChecked_ )
        MIL_Report::PostEvent( pRole_->GetPion(), report::eRC_RepairEvacuationNoMeans, *pType_ );
}

bool PHY_ComposantePion::HasRepairEvacuationMeans() const
{
    if( *pState_ == PHY_ComposanteState::repairableWithEvacuation_ && !pMaintenanceState_ )
        return true;
    if( !pMaintenanceState_ )
        return false;
    const PHY_MaintenanceConsign_ABC* consign = pMaintenanceState_->GetConsign();
    return consign && consign->SearchForUpperLevelNotFound();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyHumansWounds
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ApplyHumansWounds( const PHY_ComposanteState& composanteNewState, PHY_FireDamages_Agent& fireDamages )
{
    assert( pHumans_ );
    pHumans_->ApplyWounds( composanteNewState, fireDamages );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetOperationalState
// Created: NLD 2006-02-09
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetOperationalState() const
{
    assert( pState_ );
    if( *pState_ != PHY_ComposanteState::undamaged_ )
        return 0.;
    assert( pHumans_ );
    if( pHumans_->IsEmpty() )
        return 1.;
    return ( 1. - rOpStateWeightHumans_ ) + rOpStateWeightHumans_ * pHumans_->GetOperationalState();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetNbrUsableHumans
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
unsigned int PHY_ComposantePion::GetNbrUsableHumans() const
{
    assert( pHumans_ );
    return pHumans_->GetNbrUsableHumans();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetNbrHealthyHumans
// Created: LDC 2012-10-26
// -----------------------------------------------------------------------------
unsigned int PHY_ComposantePion::GetNbrHealthyHumans( const PHY_HumanRank& rank ) const
{
    return pHumans_->GetNbrHealthyHumans( rank );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetBreakdown
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
const PHY_Breakdown* PHY_ComposantePion::GetBreakdown() const
{
    return pBreakdown_.get();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanHaul
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanHaul() const
{
    assert( pType_ );
    return pState_->IsUsable() && CanBeUsed() && pType_->CanHaul();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanHaul1
// Created: NLD 2005-03-29
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanHaul1( const PHY_ComposanteTypePion& composanteType ) const
{
    assert( pType_ );
    return pState_->IsUsable() && CanBeUsed() && pType_->CanHaul1( composanteType );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanRepair
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanRepair( const PHY_Breakdown& breakdown ) const
{
    assert( pType_ );
    return pState_->IsUsable() && CanBeUsed() && pType_->CanRepair( breakdown );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanRepair
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanRepair() const
{
    assert( pType_ );
    return pState_->IsUsable() && CanBeUsed() && pType_->CanRepair();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanBeLent
// Created: JVT 2005-01-17
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanBeLent() const
{
    return pState_->IsUsable() && CanBeUsed( true );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanEvacuateCasualties
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanEvacuateCasualties() const
{
    assert( pType_ );
    return pState_->IsUsable() && CanBeUsed() && pType_->CanEvacuateCasualties();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanEvacuateCasualty
// Created: NLD 2007-02-05
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanEvacuateCasualty( const Human_ABC& human ) const
{
    assert( pType_ );
    return pState_->IsUsable() && CanBeUsed() && pType_->CanEvacuateCasualty( human );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanCollectCasualties
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanCollectCasualties() const
{
    assert( pType_ );
    return pState_->IsUsable() && CanBeUsed() && pType_->CanCollectCasualties();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanCollectCasualty
// Created: NLD 2007-02-05
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanCollectCasualty( const Human_ABC& human ) const
{
    assert( pType_ );
    return pState_->IsUsable() && CanBeUsed() && pType_->CanCollectCasualty( human );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanHealHumans
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanHealHuman( const Human_ABC& human ) const
{
    assert( pType_ );
    return pState_->IsUsable() && CanBeUsed() && pType_->CanHealHuman( human );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::Heal
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
unsigned int PHY_ComposantePion::Heal( Human_ABC& human ) const
{
    assert( pType_ );
    assert( bUsedForLogistic_ );
    return pType_->Heal( human );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetHealingTime
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
unsigned int PHY_ComposantePion::GetHealingTime( const Human_ABC& human ) const
{
    assert( pType_ );
    assert( bUsedForLogistic_ );
    return pType_->GetHealingTime( human );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanDiagnoseHumans
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanDiagnoseHumans() const
{
    assert( pType_ );
    return pState_->IsUsable() && CanBeUsed() && pType_->CanDiagnoseHumans();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanSortHumans
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanSortHumans() const
{
    assert( pType_ );
    return pState_->IsUsable() && CanBeUsed() && pType_->CanSortHumans();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanBePartOfConvoy
// Created: NLD 2006-03-23
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanBePartOfConvoy() const
{
    assert( pType_ );
    return pState_->IsUsable() && CanBeUsed() && CanMove() && bCanBePartOfConvoy_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CouldBePartOfConvoy
// Created: NLD 2006-07-04
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CouldBePartOfConvoy() const
{
    return bCanBePartOfConvoy_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanTransportStock
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanTransportStock( const PHY_DotationCategory& dotationCategory ) const
{
    assert( pType_ );
    return pState_->IsUsable() && CanBeUsed() && pType_->CanTransportStock( dotationCategory );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetStockTransporterCapacity
// Created: NLD 2005-07-18
// -----------------------------------------------------------------------------
void PHY_ComposantePion::GetStockTransporterCapacity( double& rWeightMax, double& rVolumeMax ) const
{
    assert( pType_ );
    return pType_->GetStockTransporterCapacity( rWeightMax, rVolumeMax );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::StartUsingForLogistic
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_ComposantePion::StartUsingForLogistic()
{
    assert( !bUsedForLogistic_ );
    bUsedForLogistic_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::StopUsingForLogistic
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_ComposantePion::StopUsingForLogistic()
{
    bUsedForLogistic_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetNeutralizationTime
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
unsigned int PHY_ComposantePion::GetNeutralizationTime() const
{
    assert( pState_ );
    if( !pState_->IsUsable() )
        return 0;
    assert( pType_ );
    return pType_->GetProtection().GetNeutralizationTime();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetType
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
const PHY_ComposanteTypePion& PHY_ComposantePion::GetType() const
{
    assert( pType_ );
    return *pType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetIdentificationMaxRange
// Created: DDA 2010-03-24
// -----------------------------------------------------------------------------
float PHY_ComposantePion::GetIdentificationMaxRange() const
{
    double distance = 0.;
    for( auto it = sensors_.begin(); it != sensors_.end(); ++it )
    {
        const PHY_SensorTypeAgent* pTypeAgent = (*it)->GetType().GetTypeAgent();
        if( pTypeAgent )
            distance = std::max( distance, pTypeAgent->IdentificationDistance() );
    }
    return static_cast< float >( distance );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetReconnoissanceMaxRange
// Created: GGE & PSN 10-04-20
// -----------------------------------------------------------------------------
float PHY_ComposantePion::GetReconnoissanceMaxRange() const
{
    double distance = std::numeric_limits< double >::max();
    for( auto it = sensors_.begin(); it != sensors_.end(); ++it )
    {
        const PHY_SensorTypeAgent* pTypeAgent = ( *it )->GetType().GetTypeAgent();
        if( pTypeAgent )
            distance = std::min( distance, pTypeAgent->ReconnoissanceDistance() );
    }
    return static_cast< float >( distance );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetState
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
const PHY_ComposanteState& PHY_ComposantePion::GetState() const
{
    assert( pState_ );
    return *pState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::IsMajor
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::IsMajor() const
{
    return bMajor_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetWeight
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetWeight() const
{
    assert( pType_ );
    return pType_->GetWeight();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMajorScore
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
unsigned int PHY_ComposantePion::GetMajorScore() const
{
    if( !pState_->IsUsable() && CanBeUsed() ) //$$$ C'est de la merde
        return 0;
    if( IsMajor() )
    {
        if( !pState_->IsDamaged() )
            return 4;
        return 3;
    }
    if( !pState_->IsDamaged() )
        return 2;
    return 1;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanPerceive
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanPerceive( const transport::PHY_RoleAction_Loading* roleLoading ) const
{
    assert( pState_ );
    if( !pState_->IsUsable() )
        return false;
    if( !CanComponentBeUsed( roleLoading, false ) )
        return false;
    const transport::PHY_RoleInterface_Transported* roleTransported = pRole_->GetPion().RetrieveRole< transport::PHY_RoleInterface_Transported >();
    return bLoadable_ || !roleTransported || !roleTransported->HasHumanTransportersToRecover();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanMove
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanMove() const
{
    assert( pState_ );
    return pState_->IsUsable() && !pState_->IsDamaged() && CanBeUsedForMove();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::IsUsable
// Created: LMT 2010-05-04
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::IsUsable() const
{
    assert( pState_ );
    return pState_->IsUsable();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::IsLoadable
// Created: LMT 2010-05-04
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::IsLoadable() const
{
    return bLoadable_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanBeFired
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanBeFired() const
{
    assert( pState_ );
    if( pRole_->GetPion().GetRole< PHY_RoleAction_MovingUnderground >().IsUnderground() )
        return false;
    return pState_->IsUsable() && CanBeUsed();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanTransportPion
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanTransportPion() const
{
    assert( pType_ );
    return pState_->CanTransport() && pType_->CanTransportPion() && CanBeUsed();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanTransportCrowd
// Created: JSR 2011-08-09
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanTransportCrowd() const
{
    assert( pType_ && pState_ );
    return pState_->CanTransport() && pType_->CanTransportCrowd() && CanBeUsed();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetCrowdTransporterCapacity
// Created: JSR 2011-08-10
// -----------------------------------------------------------------------------
unsigned int PHY_ComposantePion::GetCrowdTransporterCapacity() const
{
    assert( pType_ );
    return pType_->GetCrowdTransporterCapacity();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetPionTransporterWeightCapacity
// Created: NLD 2006-03-23
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetPionTransporterWeightCapacity() const
{
    assert( pType_ );
    return pType_->GetPionTransporterWeightCapacity();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetCrowdTransporterLoadedPerTimeStep
// Created: JSR 2011-08-10
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetCrowdTransporterLoadedPerTimeStep() const
{
    assert( pType_ );
    return pType_->GetCrowdTransporterLoadedPerTimeStep();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetCrowdTransporterUnloadedPerTimeStep
// Created: JSR 2011-08-10
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetCrowdTransporterUnloadedPerTimeStep() const
{
    assert( pType_ );
    return pType_->GetCrowdTransporterUnloadedPerTimeStep();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetPionTransporterWeightLoadedPerTimeStep
// Created: NLD 2006-03-23
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetPionTransporterWeightLoadedPerTimeStep() const
{
    assert( pType_ );
    return pType_->GetPionTransporterWeightLoadedPerTimeStep();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetPionTransporterWeightUnloadedPerTimeStep
// Created: NLD 2006-03-23
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetPionTransporterWeightUnloadedPerTimeStep() const
{
    assert( pType_ );
    return pType_->GetPionTransporterWeightUnloadedPerTimeStep();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanTransportHumans
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanTransportHumans() const
{
    assert( pType_ );
    return !bLoadable_ && pState_->CanTransport() && pType_->CanTransportHumans() && CanBeUsed();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanBeLoaded
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanBeLoaded() const
{
    return pState_->IsUsable() && bLoadable_; //$$$$ CanBeUsed() ??
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanBeTransported
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanBeTransported() const
{
    return pState_->IsUsable(); //$$$$ CanBeUsed() ??
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::HasConstructionSpeeds
// Created: JSR 2012-02-07
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::HasConstructionSpeeds() const
{
    assert( pType_ );
    return pType_->HasConstructionSpeeds();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetConstructionSpeed
// Created: JSR 2012-02-03
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetConstructionSpeed( const TerrainData& data ) const
{
    assert( pType_ );
    return pType_->GetConstructionSpeed( data );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanConstruct
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanConstruct( const MIL_ObjectType_ABC& objectType, bool bWithLoaded ) const
{
    assert( pType_ );
    return pType_->CanConstruct( objectType ) && CanBeUsed( bWithLoaded ) && pState_->IsUsable();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanDestroy
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanDestroy( const MIL_ObjectType_ABC& objectType ) const
{
    assert( pType_ );
    return pType_->CanDestroy( objectType ) && CanBeUsed() && pState_->IsUsable();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanRemoveFromPath
// Created: BCI 2011-06-20
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanRemoveFromPath( const MIL_ObjectType_ABC& objectType, bool isObjectMined ) const
{
    assert( pType_ );
    return pType_->CanRemoveFromPath( objectType, isObjectMined ) && CanBeUsed() && pState_->IsUsable();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanMine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanMine( const MIL_ObjectType_ABC& objectType ) const
{
    assert( pType_ );
    return pType_->CanMine( objectType ) && CanBeUsed() && pState_->IsUsable();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanExtinguish
// Created: BCI 2011-01-25
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanExtinguish( const MIL_ObjectType_ABC& objectType ) const
{
    assert( pType_ );
    return pType_->CanExtinguish( objectType ) && CanBeUsed() && pState_->IsUsable();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanDemine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanDemine( const MIL_ObjectType_ABC& objectType, bool bWithLoaded ) const
{
    assert( pType_ );
    return pType_->CanDemine( objectType ) && CanBeUsed( bWithLoaded ) && pState_->IsUsable();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanBypass
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanBypass( const MIL_ObjectType_ABC& objectType, bool bObjectIsMined ) const
{
    assert( pType_ );
    return pType_->CanBypass( objectType, bObjectIsMined ) && CanBeUsed() && pState_->IsUsable();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetConstructionTime
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetConstructionTime( const MIL_ObjectType_ABC& objectType, double rSizeCoef ) const
{
    if( !( CanBeUsed() && pState_->IsUsable() ) )
        return std::numeric_limits< double >::max();
    assert( pType_ );
    return pType_->GetConstructionTime( objectType, rSizeCoef );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetDestructionTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetDestructionTime( const MIL_ObjectType_ABC& objectType, double rSizeCoef ) const
{
    if( !( CanBeUsed() && pState_->IsUsable() ) )
        return std::numeric_limits< double >::max();
    assert( pType_ );
    return pType_->GetDestructionTime( objectType, rSizeCoef );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMiningTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetMiningTime( const MIL_ObjectType_ABC& objectType ) const
{
    if( !( CanBeUsed() && pState_->IsUsable() ) )
        return std::numeric_limits< double >::max();
    assert( pType_ );
    return pType_->GetMiningTime( objectType );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetExtinguishingTime
// Created: BCI 2011-01-25
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetExtinguishingTime( const MIL_ObjectType_ABC& objectType ) const
{
    if( !( CanBeUsed() && pState_->IsUsable() ) )
        return std::numeric_limits< double >::max();
    assert( pType_ );
    return pType_->GetExtinguishingTime( objectType );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetDeminingTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetDeminingTime( const MIL_ObjectType_ABC& objectType ) const
{
    if( !( CanBeUsed() && pState_->IsUsable() ) )
        return std::numeric_limits< double >::max();
    assert( pType_ );
    return pType_->GetDeminingTime( objectType );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetBypassTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetBypassTime( const MIL_ObjectType_ABC& objectType, double rSizeCoef, bool bObjectIsMined ) const
{
    if( !( CanBeUsed() && pState_->IsUsable() ) )
        return std::numeric_limits< double >::max();
    assert( pType_ );
    return pType_->GetBypassTime( objectType, rSizeCoef, bObjectIsMined );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::HealAllHumans
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void PHY_ComposantePion::HealAllHumans( bool withLog )
{
    assert( pHumans_ );
    pHumans_->HealAllHumans( withLog );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::KillAllHumans
// Created: ABR 2011-12-07
// -----------------------------------------------------------------------------
void PHY_ComposantePion::KillAllHumans()
{
    assert( pHumans_ );
    pHumans_->KillAllHumans();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::HealHumans
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
unsigned int PHY_ComposantePion::HealHumans( const PHY_HumanRank& rank, unsigned int nNbrToChange )
{
    assert( pHumans_ );
    return pHumans_->HealHumans( rank, nNbrToChange );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::OverloadHumans
// Created: ABR 2011-03-07
// -----------------------------------------------------------------------------
unsigned int PHY_ComposantePion::OverloadHumans( const PHY_HumanRank& rank, unsigned int nNbrToChange, const PHY_HumanWound& wound, bool psyop /*= false*/, bool contaminated /*= false*/ )
{
    assert( pHumans_ );
    return pHumans_->OverloadHumans( rank, nNbrToChange, wound, psyop, contaminated );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::WoundHumans
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
unsigned int PHY_ComposantePion::WoundHumans( const PHY_HumanRank& rank, unsigned int nNbrToChange, const PHY_HumanWound& wound )
{
    assert( pHumans_ );
    return pHumans_->WoundHumans( rank, nNbrToChange, wound );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ChangeHumanRank
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::ChangeHumanRank( const PHY_HumanRank& oldRank, const PHY_HumanRank& newRank, const PHY_HumanWound& wound )
{
    assert( pHumans_ );
    return pHumans_->ChangeHumanRank( oldRank, newRank, wound );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApproximateTravelTime
// Created: NLD 2004-12-24
// -----------------------------------------------------------------------------
unsigned int PHY_ComposantePion::ApproximateTravelTime( const MT_Vector2D& vSourcePos, const MT_Vector2D& vTargetPos ) const
{
    assert( pType_ );
    assert( pType_->GetMaxSpeed() != 0. );
    return static_cast< unsigned int >( 1.439 * vSourcePos.Distance( vTargetPos ) / pType_->GetMaxSpeed() ); //$$$ Deplacer la formule magique (Cf. DEC_GeometryFunctions où elle existe aussi...)
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::DeleteMaintenanceState
// Created: JSR 2013-02-12
// -----------------------------------------------------------------------------
void PHY_ComposantePion::DeleteMaintenanceState()
{
    if( pMaintenanceState_ )
    {
        pHumans_->NotifyComposanteBackFromMaintenance();
        pRole_->NotifyComposanteBackFromMaintenance( *pMaintenanceState_ );
        pMaintenanceState_->Cancel();
        pMaintenanceState_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CancelLogisticRequests
// Created: JSR 2013-02-22
// -----------------------------------------------------------------------------
void PHY_ComposantePion::CancelLogisticRequests()
{
    pHumans_->CancelLogisticRequests();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetRole
// Created: NLD 2005-03-09
// -----------------------------------------------------------------------------
const PHY_RoleInterface_Composantes& PHY_ComposantePion::GetRole() const
{
    assert( pRole_ );
    return *pRole_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::Execute
// Created: MGD 2009-09-15
// -----------------------------------------------------------------------------
void PHY_ComposantePion::Execute( firing::WeaponAvailabilityComputer_ABC& algorithm ) const
{
    for( auto it = weapons_.begin(); it != weapons_.end(); ++it )
        algorithm.ApplyOnWeapon( *this, **it );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::UseAmmunition
// Created: LDC 2010-01-07
// -----------------------------------------------------------------------------
void PHY_ComposantePion::UseAmmunition( const PHY_DotationCategory& category, MIL_Agent_ABC& pion )
{
    pType_->UseAmmunition( category, pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetPHModifier
// Created: LDC 2010-01-07
// -----------------------------------------------------------------------------
double PHY_ComposantePion::GetPHModifier( const PHY_DotationCategory& category, MIL_Agent_ABC& pion ) const
{
    return pType_->GetPHModifier( category, pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CounterIndirectFire
// Created: LDC 2010-01-07
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CounterIndirectFire( const PHY_DotationCategory& category, MIL_Agent_ABC& pion ) const
{
    return pType_->CounterIndirectFire( category, pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::DestroyIndirectFire
// Created: LDC 2010-01-07
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::DestroyIndirectFire( const PHY_DotationCategory& category, MIL_Agent_ABC& pion ) const
{
    return pType_->DestroyIndirectFire( category, pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ChangeHumanState
// Created: ABR 2011-08-12
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ChangeHumanState( sword::MissionParameters& msg )
{
    assert( pHumans_ );
    pHumans_->ChangeHumanState( msg );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::RemoveHealthyHumans
// Created: JSR 2012-10-26
// -----------------------------------------------------------------------------
void PHY_ComposantePion::RemoveHealthyHumans( const PHY_HumanRank& rank, unsigned int humansToRemove )
{
    pHumans_->RemoveHealthyHumans( rank, humansToRemove );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::OnNoAvailableHuman
// Created: LDC 2013-10-02
// -----------------------------------------------------------------------------
void PHY_ComposantePion::OnNoAvailableHuman()
{
    ApplyNewComposanteState( PHY_ComposanteState::dead_, *pState_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanPerceive
// Created: MMC 2013-01-11
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanPerceive( const MIL_ObjectType_ABC& objectType ) const
{
    if( pState_->IsUsable() )
        for( auto it = sensors_.begin(); it != sensors_.end(); ++it )
        {
            if( *it )
            {
                const PHY_SensorTypeObject* pSensorTypeObject = (*it)->GetType().GetTypeObject();
                if( pSensorTypeObject && pSensorTypeObject->CanPerceive( objectType ) )
                    return true;
            }
        }
    return false;
}
