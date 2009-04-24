
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

#include "Entities/Agents/Units/Weapons/PHY_Weapon.h"
#include "Entities/Agents/Units/Weapons/PHY_AttritionData.h"
#include "Entities/Agents/Units/Sensors/PHY_Sensor.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationGroupContainer.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory_IndirectFire_ABC.h"
#include "Entities/Agents/Units/Humans/PHY_Human.h"
#include "Entities/Agents/Units/Logistic/PHY_MaintenanceLevel.h"
#include "Entities/Agents/Units/Logistic/PHY_Breakdown.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Transported/PHY_RolePion_Transported.h"
#include "Entities/Agents/Roles/Logistic/Maintenance/PHY_MaintenanceComposanteState.h"
#include "Entities/Agents/Roles/Surrender/PHY_RolePion_Surrender.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_DirectFireData.h"
#include "Entities/Agents/Actions/Firing/IndirectFiring/PHY_IndirectFireData.h"
#include "Entities/Agents/Actions/Firing/IndirectFiring/PHY_SmokeData.h"
#include "Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.h"
#include "Entities/Agents/Actions/Transport/PHY_RoleAction_Transport.h"

#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/AttritionCapacity.h"

#include "Entities/Populations/MIL_PopulationType.h"
#include "Entities/Actions/PHY_FireDamages_Agent.h"
#include "Entities/Orders/MIL_Report.h"
#include "MIL_Singletons.h"

MT_Random PHY_ComposantePion::random_;
MT_Float  PHY_ComposantePion::rOpStateWeightHumans_ = 0.;

BOOST_CLASS_EXPORT_GUID( PHY_ComposantePion, "PHY_ComposantePion" )

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion constructor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_ComposantePion::PHY_ComposantePion( const MIL_Time_ABC& time, const PHY_ComposanteTypePion& type, PHY_RolePion_Composantes& role, uint nNbrHumanInCrew, bool bMajor, bool bLoadable, bool bCanBePartOfConvoy )
    : PHY_Composante_ABC           ( )
    , time_                        ( time )
    , pState_                      ( &PHY_ComposanteState::undamaged_ )
    , pRole_                       ( &role              )
    , pType_                       ( &type              )
    , bMajor_                      ( bMajor             )
    , bLoadable_                   ( bLoadable          )
    , bCanBePartOfConvoy_          ( bCanBePartOfConvoy )
    , bUsedForLogistic_            ( false              )
    , nAutoRepairTimeStep_         ( 0 )
    , pBreakdown_                  ( 0 )             
    , pMaintenanceState_           ( 0 )
    , nRandomBreakdownNextTimeStep_( 0 )
    , pRandomBreakdownState_       ( 0 )
    , pHumans_                     ( new PHY_HumansComposante( time, *this, nNbrHumanInCrew ) )
{
    pType_->InstanciateWeapons( std::back_inserter( weapons_ ) );   
    pType_->InstanciateSensors( std::back_inserter( sensors_ ) );

    pRole_->NotifyComposanteAdded( *this );
    assert( pHumans_->IsViable() ); //$$$ Pre-check in PHY_UnitType
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion constructor
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
PHY_ComposantePion::PHY_ComposantePion()
    : PHY_Composante_ABC           ()
    , time_                        ( MIL_Singletons::GetTime() )
    , pRole_                       ( 0 )
    , pState_                      ( 0 )
    , pType_                       ( 0 )
    , bMajor_                      ( false )
    , bLoadable_                   ( false )
    , bUsedForLogistic_            ( false )
    , bCanBePartOfConvoy_          ( false )
    , weapons_                     ()
    , sensors_                     () 
    , pHumans_                     ()
    , nAutoRepairTimeStep_         ()
    , pBreakdown_                  ()
    , pMaintenanceState_           ( 0 )
    , nRandomBreakdownNextTimeStep_()
    , pRandomBreakdownState_       ( 0 )    
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion destructor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_ComposantePion::~PHY_ComposantePion()
{
    for( CIT_WeaponVector itWeapon = weapons_.begin(); itWeapon != weapons_.end(); ++itWeapon )
        delete *itWeapon;
    weapons_.clear();

    for( CIT_SensorVector itSensor = sensors_.begin(); itSensor != sensors_.end(); ++itSensor )
        delete *itSensor;
    sensors_.clear();

    assert( pRole_ );
    pRole_->NotifyComposanteRemoved( *this );

    delete pHumans_;
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::load
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
void PHY_ComposantePion::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< PHY_Composante_ABC >( *this )
         >> pRole_;
         
    uint nID;
    file >> nID;
    pState_ = &PHY_ComposanteState::Find( nID );

    ASN1T_EquipmentType nEqID;
    file >> nEqID;
    pType_ = PHY_ComposanteTypePion::Find( nEqID );
    assert( pType_ );

    pType_->InstanciateWeapons( std::back_inserter( weapons_ ) );   
    pType_->InstanciateSensors( std::back_inserter( sensors_ ) );

    file >> const_cast< bool& >( bMajor_ )
         >> const_cast< bool& >( bLoadable_ )
         >> const_cast< bool& >( bCanBePartOfConvoy_ )
         >> bUsedForLogistic_
         >> pHumans_
         >> nAutoRepairTimeStep_
         >> const_cast< PHY_Breakdown*& >( pBreakdown_ )
         >> pMaintenanceState_
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
void PHY_ComposantePion::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    unsigned state = pState_->GetID(),
             type  = pType_->GetMosID();
    file << boost::serialization::base_object< PHY_Composante_ABC >( *this )
         << pRole_
         << state
         << type
         << bMajor_
         << bLoadable_
         << bCanBePartOfConvoy_
         << bUsedForLogistic_
         << pHumans_
         << nAutoRepairTimeStep_
         << pBreakdown_
         << pMaintenanceState_
         << nRandomBreakdownNextTimeStep_;
   
    if ( nRandomBreakdownNextTimeStep_ )
    {
        assert( pRandomBreakdownState_ );
        unsigned id = pRandomBreakdownState_->GetID();
        file << id;
    }        
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::TransferComposante
// Created: JVT 2005-01-17
// -----------------------------------------------------------------------------
void PHY_ComposantePion::TransferComposante( PHY_RolePion_Composantes& newRole )
{
    assert( pRole_ );
    assert( pHumans_ );

    // Annulation log maintenance
    if( pMaintenanceState_ )
    {
        pHumans_->NotifyComposanteBackFromMaintenance();
        assert( pRole_ );
        pRole_->NotifyComposanteBackFromMaintenance( *pMaintenanceState_ );
        pMaintenanceState_->Cancel();
        delete pMaintenanceState_;
        pMaintenanceState_ = 0;
    }

    pHumans_->NotifyComposanteTransfered( *pRole_, newRole );

    pRole_->NotifyComposanteRemoved( *this );
    pRole_ = &newRole;
    pRole_->NotifyComposanteAdded( *this );
}

// =============================================================================
// NOTIFICATIONS HUMANS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::NotifyHumanAdded
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void PHY_ComposantePion::NotifyHumanAdded( PHY_Human& human )
{
    assert( pRole_ );
    pRole_->NotifyHumanAdded( human );
}
    
// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::NotifyHumanRemoved
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void PHY_ComposantePion::NotifyHumanRemoved( PHY_Human& human )
{
    assert( pRole_ );
    pRole_->NotifyHumanRemoved( human );
}
    
// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::NotifyHumanChanged
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void PHY_ComposantePion::NotifyHumanChanged( PHY_Human& human, const PHY_Human& copyOfOldHumanState )
{
    assert( pRole_ );
    pRole_->NotifyHumanChanged( human, copyOfOldHumanState );
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ReinitializeState
// Created: NLD 2004-09-08
// Modified: JVT 2005-02-03
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ReinitializeState( const PHY_ComposanteState& tmpState )
{
    assert( pType_ );

    const PHY_ComposanteState* pNewState = &tmpState;
    
    if ( pType_->GetProtection().IsHuman() && ( *pNewState == PHY_ComposanteState::repairableWithEvacuation_ || *pNewState == PHY_ComposanteState::repairableWithoutEvacuation_ ) )
        pNewState = &PHY_ComposanteState::undamaged_;

    if ( *pState_ == *pNewState )
        return;

    const PHY_ComposanteState* pOldState = pState_;
    pState_                              = pNewState;

    if( *pState_ == PHY_ComposanteState::repairableWithEvacuation_ && !pBreakdown_ )
        pBreakdown_ = new PHY_Breakdown( pType_->GetRandomBreakdownType() );
    ManageEndMaintenance();

    assert( pRole_ );
    pRole_->NotifyComposanteChanged ( *this, *pOldState );
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
MT_Float PHY_ComposantePion::GetMaxSpeed( const TerrainData& data ) const
{
    assert( pType_ );
    
    return CanMove() ? pType_->GetMaxSpeed( data ) : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
MT_Float PHY_ComposantePion::GetMaxSpeed() const
{
    assert( pType_ );
    return CanMove() ? pType_->GetMaxSpeed() : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMaxSpeed
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
MT_Float PHY_ComposantePion::GetMaxSpeed( const MIL_Object_ABC& object ) const
{
    return CanMove() ? pType_->GetMaxSpeed( object ) : 0.;
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

// =============================================================================
// FIRE / DANGEROSITY
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyFire
// Created: NLD 2004-10-13
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ApplyFire( const PHY_AttritionData& attritionData, PHY_FireDamages_Agent& fireDamages )
{
    //$$$ FACTORISER AVEC REINITIALIZESTATE()
    assert( pRole_ );
    assert( pType_ );
    assert( CanBeFired() );
    assert( pState_ );

    const PHY_ComposanteState& oldState  = *pState_;
    const PHY_ComposanteState* pNewState = &attritionData.ComputeComposanteState();

    pRole_->WoundLoadedHumans( *this, *pNewState, fireDamages );
    ApplyHumansWounds( *pNewState, fireDamages );    

    if( pType_->GetProtection().IsHuman() && ( *pNewState == PHY_ComposanteState::repairableWithEvacuation_ || *pNewState == PHY_ComposanteState::repairableWithoutEvacuation_ ) )
        pNewState = &PHY_ComposanteState::undamaged_;

    if( *pNewState < *pState_ )
    {
        pState_ = pNewState;

        if( *pState_ == PHY_ComposanteState::repairableWithEvacuation_ && !pBreakdown_ )
            pBreakdown_ = new PHY_Breakdown( pType_->GetAttritionBreakdownType() );
        ManageEndMaintenance();
       
        pRole_->NotifyComposanteChanged( *this, oldState );
    }

    fireDamages.NotifyComposanteStateChanged( *this, oldState, *pState_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyExplosion
// Created: NLD 2004-10-13
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ApplyExplosion( const AttritionCapacity& capacity, PHY_FireDamages_Agent& fireDamages )
{
    assert( pType_ );
    ApplyFire( capacity.GetAttritionData( pType_->GetProtection() ), fireDamages );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyPopulationFire
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ApplyPopulationFire( const MIL_PopulationType& populationType, const MIL_PopulationAttitude& populationAttitude, PHY_FireDamages_Agent& fireDamages )
{
    assert( pType_ );
    ApplyFire( populationType.GetAttritionData( populationAttitude, pType_->GetProtection() ), fireDamages );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyDirectFire
// Created: NLD 2005-08-04
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ApplyDirectFire( const PHY_DotationCategory& dotationCategory, PHY_FireDamages_Agent& fireDamages )
{
    assert( pType_ );
    ApplyFire( dotationCategory.GetAttritionData( pType_->GetProtection() ), fireDamages );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyIndirectFire
// Created: NLD 2005-08-04
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ApplyIndirectFire( const PHY_DotationCategory& dotationCategory, PHY_FireDamages_Agent& fireDamages )
{
    assert( pType_ );
    assert( dotationCategory.GetIndirectFireData() );
    if( dotationCategory.GetIndirectFireData()->HasHit( pRole_->GetPion() ) )
        ApplyFire( dotationCategory.GetAttritionData( pType_->GetProtection() ), fireDamages );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyContamination
// Created: NLD 2004-10-13
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ApplyContamination( const MIL_ToxicEffectManipulator& contamination )
{
    assert( pHumans_ );
    pHumans_->ApplyContamination( contamination );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyPoisonous
// Created: NLD 2004-10-13
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ApplyPoisonous( const MIL_ToxicEffectManipulator& contamination )
{
    assert( pHumans_ );
    pHumans_->ApplyPoisonous( contamination );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetDangerosity
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
MT_Float PHY_ComposantePion::GetDangerosity( const DEC_Knowledge_AgentComposante& compTarget, MT_Float rDistBtwFirerAndTarget ) const
{
    assert( pType_ );
    assert( pRole_ );
    return CanFire() ? pType_->GetDangerosity( pRole_->GetPion(), compTarget.GetType(), rDistBtwFirerAndTarget ) : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetOnlyLoadableMaxRangeToFireOn
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
MT_Float PHY_ComposantePion::GetOnlyLoadableMaxRangeToFireOn( const DEC_Knowledge_AgentComposante& compTarget, MT_Float rWantedPH ) const
{
    assert( pType_ );
    return bLoadable_ && CanFireWhenUnloaded() ? pType_->GetMaxRangeToFireOn( pRole_->GetPion(), compTarget.GetType(), rWantedPH ) : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMaxRangeToFireOn
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
MT_Float PHY_ComposantePion::GetMaxRangeToFireOn( const DEC_Knowledge_AgentComposante& compTarget, MT_Float rWantedPH ) const
{
    assert( pType_ );
    return CanFire() ? pType_->GetMaxRangeToFireOn( pRole_->GetPion(), compTarget.GetType(), rWantedPH ) : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMinRangeToFireOn
// Created: JVT 2004-12-17
// -----------------------------------------------------------------------------
MT_Float PHY_ComposantePion::GetMinRangeToFireOn( const DEC_Knowledge_AgentComposante& compTarget, MT_Float rWantedPH ) const
{
    assert( pType_ );
    return CanFire() ? pType_->GetMinRangeToFireOn( pRole_->GetPion(), compTarget.GetType(), rWantedPH ) : std::numeric_limits< MT_Float >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMaxRangeToFireOnWithPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
MT_Float PHY_ComposantePion::GetMaxRangeToFireOnWithPosture( const DEC_Knowledge_AgentComposante& compTarget, const MIL_Agent_ABC& target, MT_Float rWantedPH ) const
{
    assert( pType_ );
    return CanFire() ? pType_->GetMaxRangeToFireOnWithPosture( pRole_->GetPion(), target, compTarget.GetType(), rWantedPH ) : 0;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMinRangeToFireOnWithPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
MT_Float PHY_ComposantePion::GetMinRangeToFireOnWithPosture( const DEC_Knowledge_AgentComposante& compTarget, const MIL_Agent_ABC& target, MT_Float rWantedPH ) const
{
    assert( pType_ );
    return CanFire() ? pType_->GetMinRangeToFireOnWithPosture( pRole_->GetPion(), target, compTarget.GetType(), rWantedPH ) : std::numeric_limits< MT_Float >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMaxRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
MT_Float PHY_ComposantePion::GetMaxRangeToIndirectFire( const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability ) const
{
    assert( pType_ );
    return CanFire() ? pType_->GetMaxRangeToIndirectFire( pRole_->GetPion(), dotationCategory, bCheckDotationsAvailability ) : -1.;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMinRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
MT_Float PHY_ComposantePion::GetMinRangeToIndirectFire( const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability ) const
{
    assert( pType_ );
    return CanFire() ? pType_->GetMinRangeToIndirectFire( pRole_->GetPion(), dotationCategory, bCheckDotationsAvailability ) : std::numeric_limits< MT_Float >::max();
}


// =============================================================================
// LOGISTIC - MEDICAL
// =============================================================================

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
// Name: PHY_ComposantePion::NotifyHumanEvacuatedByThirdParty
// Created: NLD 2005-08-01
// -----------------------------------------------------------------------------
PHY_MedicalHumanState* PHY_ComposantePion::NotifyHumanEvacuatedByThirdParty( PHY_Human& human, MIL_AutomateLOG& destinationTC2 ) const
{
    assert( pRole_ );
    return pRole_->NotifyHumanEvacuatedByThirdParty( human, destinationTC2 );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::NotifyHumanWaitingForMedical
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
PHY_MedicalHumanState* PHY_ComposantePion::NotifyHumanWaitingForMedical( PHY_Human& human ) const
{
    assert( pRole_ );
    return pRole_->NotifyHumanWaitingForMedical( human );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::NotifyHumanBackFromMedical
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_ComposantePion::NotifyHumanBackFromMedical( PHY_MedicalHumanState& humanState ) const
{
    assert( pRole_ );
    pRole_->NotifyHumanBackFromMedical( humanState );
}

// =============================================================================
// LOGISTIC - MAINTENANCE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::PreprocessRandomBreakdowns
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
void PHY_ComposantePion::PreprocessRandomBreakdowns( uint nEndDayTimeStep )
{
    assert( !pRandomBreakdownState_ && !nRandomBreakdownNextTimeStep_ );
    assert( pType_ );

    if ( pType_->GetProtection().CanRandomlyBreaksDownEva() )
    {
        pRandomBreakdownState_        = &PHY_ComposanteState::repairableWithEvacuation_;
        nRandomBreakdownNextTimeStep_ = random_.rand32_oo( time_.GetCurrentTick(), nEndDayTimeStep ); 
    }
    else if ( pType_->GetProtection().CanRandomlyBreaksDownNeva() )
    {
        pRandomBreakdownState_        = &PHY_ComposanteState::repairableWithoutEvacuation_;
        nRandomBreakdownNextTimeStep_ = random_.rand32_oo( time_.GetCurrentTick(), nEndDayTimeStep );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ManageEndMaintenance
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ManageEndMaintenance()
{
    // Appelée uniquement au changement de 'state'
    if( *pState_ != PHY_ComposanteState::maintenance_ && *pState_ != PHY_ComposanteState::repairableWithEvacuation_ )
    {
        if( pBreakdown_ )
        {
            delete pBreakdown_;
            pBreakdown_ = 0;        
        }
        
        if( pMaintenanceState_ )
        {
            assert( pHumans_ );
            pHumans_->NotifyComposanteBackFromMaintenance();
            assert( pRole_ );
            pRole_->NotifyComposanteBackFromMaintenance( *pMaintenanceState_ );
            pMaintenanceState_->Cancel();
            delete pMaintenanceState_;
            pMaintenanceState_ = 0;
        }

        if( *pState_ == PHY_ComposanteState::repairableWithoutEvacuation_ )
        {
            assert( pType_ );
            nAutoRepairTimeStep_ = std::max( nAutoRepairTimeStep_, time_.GetCurrentTick() + pType_->GetProtection().GetNeutralizationTime() );
        }
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
    MIL_Report::PostEvent( pRole_->GetPion(), MIL_Report::eReport_EquipementBackFromMaintenance, *pType_ );
    pRole_->NotifyComposanteRepaired();    
    ReinitializeState( PHY_ComposanteState::undamaged_ );    
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanBeUsed
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanBeUsed() const
{
    if( bUsedForLogistic_ )
        return false;

    assert( pRole_ );

    const PHY_RolePion_Transported& roleTransported = pRole_->GetPion().GetRole< PHY_RolePion_Transported >();
    if( roleTransported.IsTransported() || pRole_->GetPion().GetRole< PHY_RolePion_Surrender >().IsSurrendered() )
        return false;

    if( bLoadable_ )
        return !pRole_->GetPion().GetRole< PHY_RoleAction_Loading >().IsLoaded();
    else   
        return !roleTransported.HasHumanTransportersToRecover();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanBeUsedForMove
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanBeUsedForMove() const
{
    assert( pRole_ );

    const PHY_RolePion_Transported& roleTransported = pRole_->GetPion().GetRole< PHY_RolePion_Transported >();
    if( roleTransported.IsTransported() )
        return false;

    if( bLoadable_ )
        return !pRole_->GetPion().GetRole< PHY_RoleAction_Loading >().IsLoaded();
    else   
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

    return !pRole_->IsNeutralized() && pState_->IsUsable() && !pState_->IsDamaged() && bLoadable_ && !bUsedForLogistic_ && !pRole_->GetPion().GetRole< PHY_RolePion_Transported >().IsTransported();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::Repair
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void PHY_ComposantePion::Repair()
{
    if( pRole_->GetPion().GetRole< PHY_RolePion_Surrender >().IsPrisoner() )
        ReinitializeState( PHY_ComposanteState::prisoner_ );
    else
        ReinitializeState( PHY_ComposanteState::undamaged_ );
    HealAllHumans();
}

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::Update
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
void PHY_ComposantePion::Update()
{
    // Réparation automatique
    if( *pState_ == PHY_ComposanteState::repairableWithoutEvacuation_ && time_.GetCurrentTick() >= nAutoRepairTimeStep_ )
    {                
        assert( pType_ );
        MIL_Report::PostEvent( pRole_->GetPion(), MIL_Report::eReport_EquipementRepairedInPlace, *pType_ );
        pRole_->NotifyComposanteRepaired();
        ReinitializeState( PHY_ComposanteState::undamaged_ );        
    }

    // Panne aléatoire
    if( pRandomBreakdownState_ && nRandomBreakdownNextTimeStep_ == time_.GetCurrentTick()  )
    {
        if( *pState_ == PHY_ComposanteState::undamaged_ )
            ReinitializeState( *pRandomBreakdownState_ );

        nRandomBreakdownNextTimeStep_ = 0;
        pRandomBreakdownState_        = 0;
    }

    // Demande maintenance
    if( *pState_ == PHY_ComposanteState::repairableWithEvacuation_ && !pMaintenanceState_ )
    {
        assert( pBreakdown_ );
        assert( pRole_ );
        pMaintenanceState_ = pRole_->NotifyComposanteWaitingForMaintenance( *this );
    }
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
MT_Float PHY_ComposantePion::GetOperationalState() const
{
    assert( pState_ );
    if( *pState_ != PHY_ComposanteState::undamaged_ )
        return 0.;

    assert( pHumans_ );
    return ( 1. - rOpStateWeightHumans_ ) + rOpStateWeightHumans_ * pHumans_->GetOperationalState();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetNbrUsableHumans
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
uint PHY_ComposantePion::GetNbrUsableHumans() const
{
    assert( pHumans_ );
    return pHumans_->GetNbrUsableHumans();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetBreakdown
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
const PHY_Breakdown* PHY_ComposantePion::GetBreakdown() const
{
    return pBreakdown_;
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
// Name: PHY_ComposantePion::CanHaul
// Created: NLD 2005-03-29
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanHaul( const PHY_ComposanteTypePion& composanteType ) const
{
    assert( pType_ );
    return pState_->IsUsable() && CanBeUsed() && pType_->CanHaul( composanteType );
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
    return pState_->IsUsable() && CanBeUsed();
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
bool PHY_ComposantePion::CanEvacuateCasualty( const PHY_Human& human ) const
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
bool PHY_ComposantePion::CanCollectCasualty( const PHY_Human& human ) const
{
    assert( pType_ );
    return pState_->IsUsable() && CanBeUsed() && pType_->CanCollectCasualty( human );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanHealHumans
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanHealHuman( const PHY_Human& human ) const
{
    assert( pType_ );
    return pState_->IsUsable() && CanBeUsed() && pType_->CanHealHuman( human );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::Heal
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
uint PHY_ComposantePion::Heal( PHY_Human& human ) const
{
    assert( pType_ );
    assert( bUsedForLogistic_ );
    
    return pType_->Heal( human );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetHealingTime
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
uint PHY_ComposantePion::GetHealingTime( const PHY_Human& human ) const
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
    return pState_->IsUsable() && CanBeUsed() && bCanBePartOfConvoy_;
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
void PHY_ComposantePion::GetStockTransporterCapacity( MT_Float& rWeightMax, MT_Float& rVolumeMax ) const
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
    assert( bUsedForLogistic_ );
    bUsedForLogistic_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetNeutralizationTime
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------

uint PHY_ComposantePion::GetNeutralizationTime() const
{
    assert( pState_ );
 
    if ( !pState_->IsUsable() )
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
MT_Float PHY_ComposantePion::GetWeight() const
{
    assert( pType_ );
    return pType_->GetWeight();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMajorScore
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
uint PHY_ComposantePion::GetMajorScore() const
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
bool PHY_ComposantePion::CanPerceive() const
{
    assert( pState_ );
    return pState_->IsUsable() && CanBeUsed();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanMove
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanMove() const
{
    assert( pState_ );
    return pState_->IsUsable() && CanBeUsedForMove();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanBeFired
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanBeFired() const
{
    assert( pState_ );
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
// Name: PHY_ComposantePion::GetPionTransporterWeightCapacity
// Created: NLD 2006-03-23
// -----------------------------------------------------------------------------
MT_Float PHY_ComposantePion::GetPionTransporterWeightCapacity() const
{
    assert( pType_ );
    return pType_->GetPionTransporterWeightCapacity();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetPionTransporterWeightLoadedPerTimeStep
// Created: NLD 2006-03-23
// -----------------------------------------------------------------------------
MT_Float PHY_ComposantePion::GetPionTransporterWeightLoadedPerTimeStep() const
{
    assert( pType_ );
    return pType_->GetPionTransporterWeightLoadedPerTimeStep();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetPionTransporterWeightUnloadedPerTimeStep
// Created: NLD 2006-03-23
// -----------------------------------------------------------------------------
MT_Float PHY_ComposantePion::GetPionTransporterWeightUnloadedPerTimeStep() const
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
// Name: PHY_ComposantePion::CanConstruct
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanConstruct( const MIL_ObjectType_ABC& objectType ) const
{
    assert( pType_ );
    return pType_->CanConstruct( objectType ) && CanBeUsed() && pState_->IsUsable(); 
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
// Name: PHY_ComposantePion::CanMine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanMine( const MIL_ObjectType_ABC& objectType ) const
{
    assert( pType_ );
    return pType_->CanMine( objectType ) && CanBeUsed() && pState_->IsUsable(); 
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CanDemine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool PHY_ComposantePion::CanDemine( const MIL_ObjectType_ABC& objectType ) const
{
    assert( pType_ );
    return pType_->CanDemine( objectType ) && CanBeUsed() && pState_->IsUsable(); 
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
MT_Float PHY_ComposantePion::GetConstructionTime( const MIL_ObjectType_ABC& objectType, MT_Float rSizeCoef ) const
{
    if( !( CanBeUsed() && pState_->IsUsable() ) )
        return std::numeric_limits< MT_Float >::max();

    assert( pType_ );
    return pType_->GetConstructionTime( objectType, rSizeCoef );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetDestructionTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
MT_Float PHY_ComposantePion::GetDestructionTime( const MIL_ObjectType_ABC& objectType, MT_Float rSizeCoef ) const
{
    if( !( CanBeUsed() && pState_->IsUsable() ) )
        return std::numeric_limits< MT_Float >::max();

    assert( pType_ );
    return pType_->GetDestructionTime( objectType, rSizeCoef );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMiningTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
MT_Float PHY_ComposantePion::GetMiningTime( const MIL_ObjectType_ABC& objectType ) const
{
    if( !( CanBeUsed() && pState_->IsUsable() ) )
        return std::numeric_limits< MT_Float >::max();

    assert( pType_ );
    return pType_->GetMiningTime( objectType );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetDeminingTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
MT_Float PHY_ComposantePion::GetDeminingTime( const MIL_ObjectType_ABC& objectType ) const
{
    if( !( CanBeUsed() && pState_->IsUsable() ) )
        return std::numeric_limits< MT_Float >::max();

    assert( pType_ );
    return pType_->GetDeminingTime( objectType );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetBypassTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
MT_Float PHY_ComposantePion::GetBypassTime( const MIL_ObjectType_ABC& objectType, MT_Float rSizeCoef, bool bObjectIsMined ) const
{
    if( !( CanBeUsed() && pState_->IsUsable() ) )
        return std::numeric_limits< MT_Float >::max();

    assert( pType_ );
    return pType_->GetBypassTime( objectType, rSizeCoef, bObjectIsMined );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::HealAllHumans
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void PHY_ComposantePion::HealAllHumans()
{
    assert( pHumans_ );
    if( *pState_ != PHY_ComposanteState::dead_ ) 
        pHumans_->HealAllHumans();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::HealHumans
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
uint PHY_ComposantePion::HealHumans( const PHY_HumanRank& rank, uint nNbrToChange )
{
    assert( pHumans_ );
    if( *pState_ != PHY_ComposanteState::dead_ )
        return pHumans_->HealHumans( rank, nNbrToChange );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::WoundHumans
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
uint PHY_ComposantePion::WoundHumans( const PHY_HumanRank& rank, uint nNbrToChange, const PHY_HumanWound& wound )
{
    assert( pHumans_ );
    if( *pState_ != PHY_ComposanteState::dead_ )
        return pHumans_->WoundHumans( rank, nNbrToChange, wound );
    return 0;    
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
uint PHY_ComposantePion::ApproximateTravelTime( const MT_Vector2D& vSourcePos, const MT_Vector2D& vTargetPos ) const
{
    assert( pType_ );
    assert( pType_->GetMaxSpeed() != 0. );
    return (uint)( 1.439 * vSourcePos.Distance( vTargetPos ) / pType_->GetMaxSpeed() ); //$$$ Deplacer la formule magique (Cf. DEC_GeometryFunctions où elle existe aussi...)
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetRole
// Created: NLD 2005-03-09
// -----------------------------------------------------------------------------
const PHY_RolePion_Composantes& PHY_ComposantePion::GetRole() const
{
    assert( pRole_ );
    return *pRole_;
}

