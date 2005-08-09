
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

#include "MIL_Pch.h"

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
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Transported/PHY_RolePion_Transported.h"
#include "Entities/Agents/Roles/Logistic/Maintenance/PHY_MaintenanceComposanteState.h"
#include "Entities/Agents/Roles/Surrender/PHY_RolePion_Surrender.h"
#include "Entities/Agents/Actions/Firing/PHY_AgentFireResult.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_DirectFireData.h"
#include "Entities/Agents/Actions/Firing/IndirectFiring/PHY_IndirectFireData.h"
#include "Entities/Agents/Actions/Firing/IndirectFiring/PHY_SmokeData.h"
#include "Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.h"
#include "Entities/Agents/Actions/Transport/PHY_RoleAction_Transport.h"
#include "Entities/Objects/MIL_RealObjectType.h"
#include "Entities/RC/MIL_RC.h"

MT_Random PHY_ComposantePion::random_;

BOOST_CLASS_EXPORT_GUID( PHY_ComposantePion, "PHY_ComposantePion" )

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::CheckViability
// Created: NLD 2005-08-08
// -----------------------------------------------------------------------------
void PHY_ComposantePion::CheckViability()
{
    assert( pRole_ );
    if( nNbrUsableHumans_ == 0 && !pRole_->GetPion().IsAutonomous() )
        ReinitializeState( PHY_ComposanteState::dead_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion constructor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_ComposantePion::PHY_ComposantePion( const PHY_ComposanteTypePion& type, PHY_RolePion_Composantes& role, bool bMajor, bool bLoadable )
    : PHY_Composante_ABC           ( )
    , pState_                      ( &PHY_ComposanteState::undamaged_ )
    , pRole_                       ( &role      )
    , pType_                       ( &type      )
    , bMajor_                      ( bMajor     )
    , bLoadable_                   ( bLoadable  )
    , bUsedForLogistic_            ( false      )
    , nAutoRepairTimeStep_         ( 0 )
    , pBreakdown_                  ( 0 )             
    , pMaintenanceState_           ( 0 )
    , nRandomBreakdownNextTimeStep_( 0 )
    , pRandomBreakdownState_       ( 0 )
    , nNbrUsableHumans_            ( 0 )
    , humans_                      ( *this, pType_->GetNbrHumanInCrew() )
{
    pType_->InstanciateWeapons( std::back_inserter( weapons_ ) );   
    pType_->InstanciateSensors( std::back_inserter( sensors_ ) );

    pRole_->NotifyComposanteAdded( *this );
    CheckViability();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion constructor
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
PHY_ComposantePion::PHY_ComposantePion()
    : PHY_Composante_ABC           ()
    , pRole_                       ( 0 )
    , pState_                      ( 0 )
    , pType_                       ( 0 )
    , bMajor_                      ( false )
    , bLoadable_                   ( false )
    , bUsedForLogistic_            ( false )
    , weapons_                     ()
    , sensors_                     () 
    , nNbrUsableHumans_            ()
    , humans_                      ()
    , nAutoRepairTimeStep_         ()
    , pBreakdown_                  ()
    , pMaintenanceState_           ( 0 )
    , nRandomBreakdownNextTimeStep_()
    , pRandomBreakdownState_       ( 0 )
{
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
    pState_ = &PHY_ComposanteState::FindComposanteState( nID );

    ASN1T_TypeEquipement nEqID;
    file >> nEqID;
    pType_ = PHY_ComposanteTypePion::FindComposanteType( nEqID );
    assert( pType_ );

    pType_->InstanciateWeapons( std::back_inserter( weapons_ ) );   
    pType_->InstanciateSensors( std::back_inserter( sensors_ ) );

    file >> const_cast< bool& >( bMajor_ )
         >> const_cast< bool& >( bLoadable_ )
         >> bUsedForLogistic_
         >> nNbrUsableHumans_
         >> humans_
         >> nAutoRepairTimeStep_
         >> const_cast< PHY_Breakdown*& >( pBreakdown_ )
         >> pMaintenanceState_
         >> nRandomBreakdownNextTimeStep_;
   
    if ( nRandomBreakdownNextTimeStep_ )
    {
        file >> nID;
        pRandomBreakdownState_ = &PHY_ComposanteState::FindComposanteState( nID );
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
    file << boost::serialization::base_object< PHY_Composante_ABC >( *this )
         << pRole_
         << pState_->GetID()
         << pType_->GetMosID()
         << bMajor_
         << bLoadable_
         << bUsedForLogistic_
         << nNbrUsableHumans_
         << humans_
         << nAutoRepairTimeStep_
         << pBreakdown_
         << pMaintenanceState_
         << nRandomBreakdownNextTimeStep_;
   
    if ( nRandomBreakdownNextTimeStep_ )
    {
        assert( pRandomBreakdownState_ );
        file << pRandomBreakdownState_->GetID();
    }        
}

// =============================================================================
// OPERATIONS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::TransfertComposante
// Created: JVT 2005-01-17
// -----------------------------------------------------------------------------
void PHY_ComposantePion::TransfertComposante( PHY_RolePion_Composantes& newRole )
{
    assert( pRole_ );

    // Annulation log maintenance
    if( pMaintenanceState_ )
    {
        humans_.NotifyComposanteBackFromMaintenance();
        assert( pRole_ );
        pRole_->NotifyComposanteBackFromMaintenance( *pMaintenanceState_ );
        pMaintenanceState_->Cancel();
        delete pMaintenanceState_;
        pMaintenanceState_ = 0;
    }

    humans_.NotifyComposanteTransfered( *pRole_, newRole );

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
    if( human.IsUsable() )
        ++ nNbrUsableHumans_;
    assert( pRole_ );
    pRole_->NotifyHumanAdded( human );
}
    
// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::NotifyHumanRemoved
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void PHY_ComposantePion::NotifyHumanRemoved( PHY_Human& human )
{
    if( human.IsUsable() )
    {
        assert( nNbrUsableHumans_ > 0 );
        -- nNbrUsableHumans_;
    }

    assert( pRole_ );
    pRole_->NotifyHumanRemoved( human );

    CheckViability();
}
    
// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::NotifyHumanChanged
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void PHY_ComposantePion::NotifyHumanChanged( PHY_Human& human, const PHY_Human& copyOfOldHumanState )
{
    if( copyOfOldHumanState.IsUsable() )
    {
        assert( nNbrUsableHumans_ > 0 );
        -- nNbrUsableHumans_;
    }
    if( human.IsUsable() )
        ++ nNbrUsableHumans_;
  
    assert( pRole_ );
    pRole_->NotifyHumanChanged( human, copyOfOldHumanState );

    CheckViability();
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

    if ( *pState_ == PHY_ComposanteState::repairableWithEvacuation_ && !pBreakdown_ )
            pBreakdown_ = new PHY_Breakdown( pType_->GetRandomBreakdownType() );
    else if( *pState_ == PHY_ComposanteState::dead_ )
        humans_.KillAllHumans();    
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
MT_Float PHY_ComposantePion::GetMaxSpeed( const MIL_RealObject_ABC& object ) const
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
const PHY_ConsumptionType& PHY_ComposantePion::GetConsumptionMode( const MIL_RealObjectType& objectType ) const
{
    assert( pType_ );
    return pType_->GetConsumptionMode( objectType );
}

// =============================================================================
// FIRE / DANGEROSITY
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::FillDirectFireData
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
void PHY_ComposantePion::FillDirectFireData( PHY_DirectFireData& data )
{
    for( CIT_WeaponVector itWeapon = weapons_.begin(); itWeapon != weapons_.end(); ++itWeapon )
    {
        PHY_Weapon& weapon = **itWeapon;        
        if( weapon.CanDirectFire() )
            data.AddWeapon( *this, weapon );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::FillIndirectFireData
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
void PHY_ComposantePion::FillIndirectFireData( PHY_IndirectFireData& data )
{
    for( CIT_WeaponVector itWeapon = weapons_.begin(); itWeapon != weapons_.end(); ++itWeapon )
    {
        PHY_Weapon& weapon = **itWeapon;        
        if( weapon.CanIndirectFire() )
            data.AddWeapon( *this, weapon );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::FillIndirectFireData
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
void PHY_ComposantePion::FillIndirectFireData( PHY_SmokeData& data )
{
    for( CIT_WeaponVector itWeapon = weapons_.begin(); itWeapon != weapons_.end(); ++itWeapon )
    {
        PHY_Weapon& weapon = **itWeapon;        
        if( weapon.CanIndirectFire() && data.AddWeapon( weapon ) )
            return;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyHumansWounds
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ApplyHumansWounds( const PHY_ComposanteState& composanteNewState, PHY_AgentFireResult& fireResult )
{
    const PHY_ComposanteState& oldState = *pState_;
    humans_.ApplyWounds( composanteNewState, fireResult );
    if( nNbrUsableHumans_ == 0 )
        fireResult.NotifyComposanteStateChanged( *this, oldState, PHY_ComposanteState::dead_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyFire
// Created: NLD 2004-10-13
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ApplyFire( const PHY_AttritionData& attritionData, PHY_AgentFireResult& fireResult )
{
    assert( pRole_ );
    assert( pType_ );
    assert( CanBeFired() );

    const PHY_ComposanteState& tmpState  = attritionData.ComputeComposanteState();
    const PHY_ComposanteState* pNewState = &tmpState;

    ApplyHumansWounds( *pNewState, fireResult );    
    pRole_->WoundLoadedHumans( *this, *pNewState, fireResult );

    if ( pType_->GetProtection().IsHuman() && ( *pNewState == PHY_ComposanteState::repairableWithEvacuation_ || *pNewState == PHY_ComposanteState::repairableWithoutEvacuation_ ) )
        pNewState = &PHY_ComposanteState::undamaged_;

    if ( *pNewState < *pState_ )
    {
        fireResult.NotifyComposanteStateChanged( *this, *pState_, *pNewState );
        const PHY_ComposanteState* pOldState = pState_;
        pState_                              = pNewState;

        if( *pState_ == PHY_ComposanteState::repairableWithEvacuation_ && !pBreakdown_ )
                pBreakdown_ = new PHY_Breakdown( pType_->GetAttritionBreakdownType() );
        else if( *pState_ == PHY_ComposanteState::dead_ )
            humans_.KillAllHumans( fireResult );        
        ManageEndMaintenance();
       
        pRole_->NotifyComposanteChanged ( *this, *pOldState );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyExplosion
// Created: NLD 2004-10-13
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ApplyExplosion( const MIL_RealObjectType& objectType, PHY_AgentFireResult& fireResult )
{
    assert( pType_ );
    ApplyFire( objectType.GetAttritionData( pType_->GetProtection() ), fireResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyDirectFire
// Created: NLD 2005-08-04
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ApplyDirectFire( const PHY_DotationCategory& dotationCategory, PHY_AgentFireResult& fireResult )
{
    assert( pType_ );
    ApplyFire( dotationCategory.GetAttritionData( pType_->GetProtection() ), fireResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyIndirectFire
// Created: NLD 2005-08-04
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ApplyIndirectFire( const PHY_DotationCategory& dotationCategory, PHY_AgentFireResult& fireResult )
{
    assert( pType_ );

    assert( dotationCategory.GetIndirectFireData() );
    if( dotationCategory.GetIndirectFireData()->HasHit( pRole_->GetPion() ) )
        ApplyFire( dotationCategory.GetAttritionData( pType_->GetProtection() ), fireResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyContamination
// Created: NLD 2004-10-13
// -----------------------------------------------------------------------------
void PHY_ComposantePion::ApplyContamination( const MIL_NbcAgentType& nbcAgentType )
{
    humans_.ApplyWounds( nbcAgentType );
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
// Name: PHY_ComposantePion::GetMaxRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
MT_Float PHY_ComposantePion::GetMaxRangeToIndirectFire( const PHY_IndirectFireDotationClass& ammoClass ) const
{
    assert( pType_ );
    return CanFire() ? pType_->GetMaxRangeToIndirectFire( pRole_->GetPion(), ammoClass ) : -1.;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::GetMinRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
MT_Float PHY_ComposantePion::GetMinRangeToIndirectFire( const PHY_IndirectFireDotationClass& ammoClass ) const
{
    assert( pType_ );
    return CanFire() ? pType_->GetMinRangeToIndirectFire( pRole_->GetPion(), ammoClass ) : std::numeric_limits< MT_Float >::max();
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
    return humans_.HasWoundedHumansToEvacuate();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::EvacuateWoundedHumans
// Created: NLD 2005-08-01
// -----------------------------------------------------------------------------
void PHY_ComposantePion::EvacuateWoundedHumans( MIL_AutomateLOG& destinationTC2 ) const
{
    humans_.EvacuateWoundedHumans( destinationTC2 );
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
        nRandomBreakdownNextTimeStep_ = random_.rand32_oo( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep(), nEndDayTimeStep ); 
    }
    else if ( pType_->GetProtection().CanRandomlyBreaksDownNeva() )
    {
        pRandomBreakdownState_        = &PHY_ComposanteState::repairableWithoutEvacuation_;
        nRandomBreakdownNextTimeStep_ = random_.rand32_oo( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep(), nEndDayTimeStep );
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
        if ( pBreakdown_ )
        {
            delete pBreakdown_;
            pBreakdown_ = 0;        
        }
        
        if( pMaintenanceState_ )
        {
            humans_.NotifyComposanteBackFromMaintenance();
            assert( pRole_ );
            pRole_->NotifyComposanteBackFromMaintenance( *pMaintenanceState_ );
            pMaintenanceState_->Cancel();
            delete pMaintenanceState_;
            pMaintenanceState_ = 0;
        }

        if ( *pState_ == PHY_ComposanteState::repairableWithoutEvacuation_ )
        {
            assert( pType_ );
            nAutoRepairTimeStep_ = std::max( nAutoRepairTimeStep_, MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() + pType_->GetProtection().GetNeutralizationTime() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::NotifyHandledByMaintenance
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void PHY_ComposantePion::NotifyHandledByMaintenance()
{
    ReinitializeState( PHY_ComposanteState::maintenance_ );
    humans_.NotifyComposanteHandledByMaintenance();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::NotifyRepairedByMaintenance
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_ComposantePion::NotifyRepairedByMaintenance()
{
    assert( pRole_ );
    
    ReinitializeState( PHY_ComposanteState::undamaged_ );
    MIL_RC::pRcMaterielRetourDeMaintenance_->Send( pRole_->GetPion(), MIL_RC::eRcTypeOperational );
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
    if( bUsedForLogistic_ )
        return false;

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
    if( *pState_ == PHY_ComposanteState::repairableWithoutEvacuation_ && MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() >= nAutoRepairTimeStep_ )
        ReinitializeState( PHY_ComposanteState::undamaged_ );

    // Panne aléatoire
    if( pRandomBreakdownState_ && nRandomBreakdownNextTimeStep_ == MIL_AgentServer::GetWorkspace().GetCurrentTimeStep()  )
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


