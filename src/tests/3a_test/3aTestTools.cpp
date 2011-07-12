// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "3a_test_pch.h"
#include "3aTestTools.h"

using namespace sword;

unsigned int TestTools::currentTick_ = 1;

// -----------------------------------------------------------------------------
// Name: TestTools::OperationalState
// Created: FPO 2011-05-06
// -----------------------------------------------------------------------------
SimToClient TestTools::OperationalState( unsigned opstate, unsigned long id )
{
    SimToClient result;
    UnitAttributes& attributes = *result.mutable_message()->mutable_unit_attributes();
    attributes.mutable_unit()->set_id( id );
    attributes.set_raw_operational_state( opstate );
    return result;
}

// -----------------------------------------------------------------------------
// Name: TestTools::BeginTick
// Created: FPO 2011-05-06
// -----------------------------------------------------------------------------
SimToClient TestTools::BeginTick()
{
    SimToClient result;
    ControlBeginTick beginTick;
    beginTick.set_current_tick( currentTick_++ );
    *result.mutable_message()->mutable_control_begin_tick() = beginTick ;
    return result;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::EndTick
// Created: FPO 2011-05-06
// -----------------------------------------------------------------------------
SimToClient TestTools::EndTick()
{
    SimToClient result;
    ControlEndTick endTick;
    *result.mutable_message()->mutable_control_end_tick() = endTick ;
    return result;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::MakeUnitCreation
// Created: FPO 2011-05-06
// -----------------------------------------------------------------------------
SimToClient TestTools::MakeUnitCreation( unsigned long id, unsigned long type_id )
{
    SimToClient result;
    UnitCreation& message = *result.mutable_message()->mutable_unit_creation();
    message.mutable_unit()->set_id( id );
    message.mutable_type()->set_id( type_id );
    message.set_name( "test" );
    message.mutable_automat()->set_id( 12 );
    message.set_pc( false );
    return result;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::CheckValue
// Created: FPO 2011-05-06
// -----------------------------------------------------------------------------
bool TestTools::CheckValue( const AarToClient& expected, const AarToClient& actual )
{
    BOOST_CHECK_EQUAL( expected.DebugString(), actual.DebugString() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::MakeEquipementVariation
// Created: FPO 2011-05-06
// -----------------------------------------------------------------------------
SimToClient TestTools::MakeEquipementVariation( int variation[5], unsigned long id, unsigned long equipmentId )
{
    SimToClient result;
    UnitAttributes& attributes = *result.mutable_message()->mutable_unit_attributes();
    attributes.mutable_unit()->set_id( id );
    EquipmentDotations_EquipmentDotation& equipment = *attributes.mutable_equipment_dotations()->add_elem();
    equipment.mutable_type()->set_id( equipmentId );
    equipment.set_available( variation[0] );
    equipment.set_unavailable( variation[1] );
    equipment.set_repairable( variation[2] );
    equipment.set_repairing( variation[3] );
    equipment.set_captured( variation[4] );
    return result;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::MakePosition
// Created: FPO 2011-05-06
// -----------------------------------------------------------------------------
sword::SimToClient TestTools::MakePosition( const char* position, unsigned long id )
{
    SimToClient result;
    UnitAttributes& attributes = *result.mutable_message()->mutable_unit_attributes();
    attributes.mutable_unit()->set_id( id );
    geocoord::MGRS mgrs( position );
    geocoord::Geodetic geodetic( mgrs );
    attributes.mutable_position()->set_latitude( geodetic.GetLatitude() * 180 / std::acos( -1. ) );
    attributes.mutable_position()->set_longitude( geodetic.GetLongitude() * 180 / std::acos( -1. ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::MakeResourceVariation
// Created: FPO 2011-05-06
// -----------------------------------------------------------------------------
sword::SimToClient TestTools::MakeResourceVariation( int variation, unsigned long id, unsigned long resourceId )
{
    SimToClient result;
    UnitAttributes& attributes = *result.mutable_message()->mutable_unit_attributes();
    attributes.mutable_unit()->set_id( id );
    ResourceDotations_ResourceDotation& resource = *attributes.mutable_resource_dotations()->add_elem();
    resource.mutable_type()->set_id( resourceId );
    resource.set_quantity( variation );
    return result;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::MakeStockVariation
// Created: FPO 2011-05-06
// -----------------------------------------------------------------------------
sword::SimToClient TestTools::MakeStockVariation( int variation, unsigned long id, unsigned long stockId )
{
    SimToClient result;
    LogSupplyState& supplystate = *result.mutable_message()->mutable_log_supply_state();
    supplystate.mutable_unit()->set_id( id );
    DotationStock& stock = *supplystate.mutable_stocks()->add_elem();
    stock.mutable_resource()->set_id( stockId );
    stock.set_quantity( variation );
    return result;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::CreateUnitDetection
// Created: FPO 2011-05-06
// -----------------------------------------------------------------------------
sword::SimToClient TestTools::CreateUnitDetection( unsigned int detector, unsigned int detected, sword::UnitVisibility::Level visibility )
{
    SimToClient result;
    sword::UnitDetection& message = *result.mutable_message()->mutable_unit_detection();
    message.mutable_observer()->set_id( detector );
    message.mutable_detected_unit()->set_id( detected );
    message.set_current_visibility( visibility );
    message.set_max_visibility( visibility );
    return result;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::MakeMounted
// Created: FPO 2011-05-06
// -----------------------------------------------------------------------------
sword::SimToClient TestTools::MakeMounted( bool mounted, unsigned long id )
{
    SimToClient result;
    UnitAttributes& attributes = *result.mutable_message()->mutable_unit_attributes();
    attributes.mutable_unit()->set_id( id );
    attributes.set_embarked( mounted );
    return result;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::MakeHumanVariation
// Created: FPO 2011-05-06
// -----------------------------------------------------------------------------
sword::SimToClient TestTools::MakeHumanVariation( int state[8], unsigned long id )
{
    SimToClient result;
    UnitAttributes& attributes = *result.mutable_message()->mutable_unit_attributes();
    attributes.mutable_unit()->set_id( id );
    HumanDotations_HumanDotation& personnel = *attributes.mutable_human_dotations()->add_elem();
    personnel.set_rank( sword::officer );
    personnel.set_total( state[0] );
    personnel.set_operational( state[1] );
    personnel.set_dead( state[2] );
    personnel.set_wounded( state[3] );
    personnel.set_mentally_wounded( state[4] );
    personnel.set_contaminated( state[5] );
    personnel.set_healing( state[6] );
    personnel.set_maintenance( state[7] );
    personnel.set_unevacuated_wounded( 0 );
    return result;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::MakeAmbulances
// Created: FPO 2011-05-06
// -----------------------------------------------------------------------------
sword::SimToClient TestTools::MakeAmbulances( int evacuationNumber, int collectionNumber, unsigned long id )
{
    SimToClient result;
    LogMedicalState& medicalstate = *result.mutable_message()->mutable_log_medical_state();
    medicalstate.mutable_unit()->set_id( id );
    LogMedicalEquipmentAvailability& evacuationEquipment = *medicalstate.mutable_evacuation_ambulances()->add_elem();
    LogMedicalEquipmentAvailability& collectionEquipment = *medicalstate.mutable_collection_ambulances()->add_elem();
    evacuationEquipment.set_total( evacuationNumber );
    collectionEquipment.set_total( collectionNumber );
    return result;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::MakeMaintenance
// Created: FPO 2011-05-06
// -----------------------------------------------------------------------------
sword::SimToClient TestTools::MakeMaintenance( int repairersNumber, int haulersNumber, unsigned long id )
{
    SimToClient result;
    LogMaintenanceState& maintenancestate = *result.mutable_message()->mutable_log_maintenance_state();
    maintenancestate.mutable_unit()->set_id( id );
    LogMaintenanceEquipmentAvailability& repairers = *maintenancestate.mutable_repairers()->add_elem();
    LogMaintenanceEquipmentAvailability& haulers = *maintenancestate.mutable_haulers()->add_elem();
    repairers.set_total( repairersNumber );
    haulers.set_total( haulersNumber );
    return result;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::UpdatePopulation
// Created: FPO 2011-05-06
// -----------------------------------------------------------------------------
sword::SimToClient TestTools::UpdatePopulationStates( int healthy, int wounded, int dead, unsigned long id )
{
    SimToClient result;
    PopulationUpdate& updatestates = *result.mutable_message()->mutable_population_update();
    updatestates.mutable_id()->set_id( id );
    updatestates.set_healthy( healthy );
    updatestates.set_wounded( wounded );
    updatestates.set_dead( dead );
    return result;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::UpdatePopulationbis
// Created: FPO 2011-05-06
// -----------------------------------------------------------------------------
sword::SimToClient TestTools::UpdatePopulationStatesbis( int healthy, int wounded, unsigned long id )
{
    SimToClient result;
    PopulationUpdate& updatestates = *result.mutable_message()->mutable_population_update();
    updatestates.mutable_id()->set_id( id );
    updatestates.set_healthy( healthy );
    updatestates.set_wounded( wounded );
    return result;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::UpdatePopulationSatisfaction
// Created: FPO 2011-05-10
// -----------------------------------------------------------------------------
sword::SimToClient TestTools::UpdatePopulationSatisfaction( float lodging, float health, float safety, unsigned long id )
{
    SimToClient result;
    PopulationUpdate& update = *result.mutable_message()->mutable_population_update();
    PopulationUpdate_Satisfaction& satisfaction = *update.mutable_satisfaction();
    update.mutable_id()->set_id( id );
    satisfaction.set_lodging( lodging );
    satisfaction.set_health( health );
    satisfaction.set_safety( safety );
    return result;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::UpdatePopulationInBlocks
// Created: FPO 2011-05-12
// -----------------------------------------------------------------------------
sword::SimToClient TestTools::UpdatePopulationInBlocks( unsigned long populationId, std::map < unsigned long, int > blocks )
{
    SimToClient result;
    PopulationUpdate& update = *result.mutable_message()->mutable_population_update();
    update.mutable_id()->set_id( populationId );
    for( std::map< unsigned long, int >::const_iterator it = blocks.begin(); it != blocks.end(); ++it )
    {
        PopulationUpdate_BlockOccupation& block = *update.add_occupations();
        block.mutable_object()->set_id( it->first );
        PopulationUpdate_BlockOccupation_UsageOccupation& occupation = *block.add_persons();
        occupation.set_number( it->second );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::UpdateCrowdDeadConcentration
// Created: FPO 2011-05-13
// -----------------------------------------------------------------------------
sword::SimToClient TestTools::UpdateCrowdDeadState( unsigned long crowdId, int dead )
{
    SimToClient result;
    CrowdUpdate& update = *result.mutable_message()->mutable_crowd_update();
    update.mutable_crowd()->set_id( crowdId );
    update.set_dead( dead );
    return result;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::CreateDirectFire
// Created: FPO 2011-05-06
// -----------------------------------------------------------------------------
sword::SimToClient TestTools::CreateDirectFire( unsigned fire_id, unsigned long firer, unsigned long target /*=42*/ )
{
    SimToClient result;
    StartUnitFire& fire = *result.mutable_message()->mutable_start_unit_fire();
    fire.mutable_fire()->set_id( fire_id );
    fire.mutable_firing_unit()->set_id( firer );
    fire.mutable_target()->mutable_unit()->set_id( target );
    fire.set_type( StartUnitFire_UnitFireType_direct );
    return result;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::CreateIndirectFire
// Created: FPO 2011-05-06
// -----------------------------------------------------------------------------
sword::SimToClient TestTools::CreateIndirectFire( unsigned fire_id, unsigned long firer, const char* position /*=""*/ )
{
    SimToClient result;
    StartUnitFire& fire = *result.mutable_message()->mutable_start_unit_fire();
    fire.mutable_fire()->set_id( fire_id );
    fire.mutable_firing_unit()->set_id( firer );
    geocoord::MGRS mgrs( position );
    geocoord::Geodetic geodetic( mgrs );
    fire.mutable_target()->mutable_position()->set_latitude( geodetic.GetLatitude() * 180 / std::acos( -1. ) );
    fire.mutable_target()->mutable_position()->set_longitude( geodetic.GetLongitude() * 180 / std::acos( -1. ) );
    fire.set_type( StartUnitFire_UnitFireType_indirect );
    return result;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::StopFire
// Created: FPO 2011-05-06
// -----------------------------------------------------------------------------
sword::SimToClient TestTools::StopFire( unsigned fire_id, unsigned int target_id, unsigned long damage_count /*= 0*/, unsigned long deadhumans_count /*= 0*/ )
{
    SimToClient result;
    StopUnitFire& fire = *result.mutable_message()->mutable_stop_unit_fire();
    fire.mutable_fire()->set_id( fire_id );
    UnitFireDamages& damage = *fire.mutable_units_damages()->add_elem();
    damage.mutable_equipments()->add_elem()->set_unavailable( damage_count );
    damage.mutable_humans()->add_elem()->set_dead( deadhumans_count );
    damage.mutable_target()->set_id( target_id );
    return result;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::MakeUnitDamages
// Created: FPO 2011-05-18
// -----------------------------------------------------------------------------
sword::SimToClient TestTools::MakeUnitDamages( unsigned int firer_id, unsigned int target_id, unsigned long damage_count /*= 0*/, unsigned long deadhumans_count /*= 0*/, unsigned int fire_id /*=0*/, bool isDirectFire /*= true*/, bool isFratricide /*= false*/ )
{
    SimToClient result;
    UnitDamagedByUnitFire& damage = *result.mutable_message()->mutable_unit_damaged_by_unit_fire();
    damage.mutable_firer()->set_id( firer_id );
    damage.mutable_fire()->set_id( fire_id );
    damage.set_direct_fire( isDirectFire );
    damage.set_fratricide( isFratricide );
    damage.mutable_equipments()->add_elem()->set_unavailable( damage_count );
    damage.mutable_humans()->add_elem()->set_dead( deadhumans_count );
    damage.mutable_unit()->set_id( target_id );
    return result;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::MakeUnitDamagesByCrowd
// Created: FPO 2011-05-26
// -----------------------------------------------------------------------------
sword::SimToClient TestTools::MakeUnitDamagesByCrowd( unsigned int crowd_id, unsigned int target_id, unsigned long damage_count /*= 0*/, unsigned long deadhumans_count /*= 0*/ )
{
    SimToClient result;
    UnitDamagedByCrowdFire& damage = *result.mutable_message()->mutable_unit_damaged_by_crowd_fire();
    damage.mutable_firer()->set_id( crowd_id );
    damage.mutable_unit()->set_id( target_id );
    damage.mutable_equipments()->add_elem()->set_unavailable( damage_count );
    damage.mutable_humans()->add_elem()->set_dead( deadhumans_count );
    return result;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::CreateConsign
// Created: FPO 2011-05-06
// -----------------------------------------------------------------------------
sword::SimToClient TestTools::CreateConsign( unsigned long id, unsigned long unit_id, unsigned long equip_id, unsigned long breakdown_id )
{
    SimToClient result;
    LogMaintenanceHandlingCreation& creation = *result.mutable_message()->mutable_log_maintenance_handling_creation();
    creation.mutable_request()->set_id( id );
    creation.mutable_unit()->set_id( unit_id );
    creation.mutable_equipement()->set_id( equip_id );
    creation.mutable_breakdown()->set_id( breakdown_id );
    return result;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::DestroyConsign
// Created: FPO 2011-05-06
// -----------------------------------------------------------------------------
sword::SimToClient TestTools::DestroyConsign( unsigned long id )
{
    SimToClient result;
    LogMaintenanceHandlingDestruction& destruction = *result.mutable_message()->mutable_log_maintenance_handling_destruction();
    destruction.mutable_request()->set_id( id );
    return result;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::IsCloseCombatPower
// Created: FPO 2011-05-06
// -----------------------------------------------------------------------------
bool TestTools::IsCloseCombatPower( const extractors::PowerExtractor_ABC& extractor )
{
    return dynamic_cast< const extractors::CloseCombatPower* >( &extractor ) != 0;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::IsDirectFirePower
// Created: FPO 2011-06-21
// -----------------------------------------------------------------------------
bool TestTools::IsDirectFirePower( const extractors::PowerExtractor_ABC& extractor )
{
    return dynamic_cast< const extractors::DirectFirePower* >( &extractor ) != 0;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::CreateMedicalConsign
// Created: FPO 2011-05-09
// -----------------------------------------------------------------------------
sword::SimToClient TestTools::CreateMedicalConsign( unsigned long unitId, unsigned long requestId, bool isWaiting )
{
    SimToClient result;
    LogMedicalHandlingUpdate& update = *result.mutable_message()->mutable_log_medical_handling_update();
    update.mutable_unit()->set_id( unitId );
    update.mutable_request()->set_id( requestId );
    if( isWaiting ) update.set_state( sword::LogMedicalHandlingUpdate::waiting_for_medical_attention );
    else update.set_state( sword::LogMedicalHandlingUpdate::diagnosing );
    return result;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::CreateFunctionalState
// Created: FPO 2011-05-16
// -----------------------------------------------------------------------------
sword::SimToClient TestTools::CreateStructuralState( unsigned long objectId, int stateValue )
{
    SimToClient result;
    UrbanUpdate& update = *result.mutable_message()->mutable_urban_update();
    update.mutable_object()->set_id( objectId );
    UrbanAttributes& attributes = *update.mutable_attributes();
    UrbanAttributes_Infrastructures_Infrastructure & infrastructure = *attributes.mutable_infrastructures()->mutable_infrastructure();
    infrastructure.set_type( "piscine" );
    infrastructure.set_active( true );
    infrastructure.set_threshold( 50 );
    attributes.mutable_structure()->set_state( stateValue );
    return result;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::UpdateFunctionalState
// Created: FPO 2011-05-16
// -----------------------------------------------------------------------------
sword::SimToClient TestTools::UpdateStructuralState( unsigned long objectId, int stateValue )
{
    SimToClient result;
    UrbanUpdate& update = *result.mutable_message()->mutable_urban_update();
    update.mutable_object()->set_id( objectId );
    UrbanAttributes& attributes = *update.mutable_attributes();
    attributes.mutable_structure()->set_state( stateValue );
    return result;
}

// -----------------------------------------------------------------------------
// Name: 3aTestTools::UpdateResourceState
// Created: FPO 2011-05-26
// -----------------------------------------------------------------------------
sword::SimToClient TestTools::UpdateResourceState( unsigned long objectId, float stateValue )
{
    SimToClient result;
    UrbanUpdate& update = *result.mutable_message()->mutable_urban_update();
    update.mutable_object()->set_id( objectId );
    UrbanAttributes& attributes = *update.mutable_attributes();
    attributes.mutable_infrastructures()->add_resource_network()->set_functional_state( stateValue );
    return result;
}
