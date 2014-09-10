// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Helpers.h"

#include <stdint.h>

using namespace protocol;

#define COUNT_OF(X) (sizeof(X)/sizeof*(X))

const mapping::MagicUnitAction mapping::MagicUnitAction::data_[] = {
    { "automat_and_units_creation",         sword::UnitMagicAction::automat_and_units_creation },
    { "automat_creation",                   sword::UnitMagicAction::automat_creation },
    { "cancel_surrender",                   sword::UnitMagicAction::cancel_surrender },
    { "change_automat_superior",            sword::UnitMagicAction::change_automat_superior },
    { "change_brain_debug",                 sword::UnitMagicAction::change_brain_debug },
    { "change_critical_intelligence",       sword::UnitMagicAction::change_critical_intelligence },
    { "change_dotation",                    sword::UnitMagicAction::change_dotation },
    { "change_equipment_human_size",        sword::UnitMagicAction::change_equipment_human_size },
    { "change_equipment_state",             sword::UnitMagicAction::change_equipment_state },
    { "change_extension",                   sword::UnitMagicAction::change_extension },
    { "change_formation_superior",          sword::UnitMagicAction::change_formation_superior },
    { "change_human_factors",               sword::UnitMagicAction::change_human_factors },
    { "change_human_state",                 sword::UnitMagicAction::change_human_state },
    { "change_knowledge_group",             sword::UnitMagicAction::change_knowledge_group },
    { "change_logistic_links",              sword::UnitMagicAction::change_logistic_links },
    { "change_posture",                     sword::UnitMagicAction::change_posture },
    { "create_basic_load_supply_request",   sword::UnitMagicAction::create_basic_load_supply_request },
    { "create_breakdowns",                  sword::UnitMagicAction::create_breakdowns },
    { "create_direct_fire_order",           sword::UnitMagicAction::create_direct_fire_order },
    { "create_stock_supply_request",        sword::UnitMagicAction::create_stock_supply_request },
    { "create_wound",                       sword::UnitMagicAction::create_wound },
    { "create_wounds",                      sword::UnitMagicAction::create_wounds },
    { "crowd_change_affinities",            sword::UnitMagicAction::crowd_change_affinities },
    { "crowd_change_armed_individuals",     sword::UnitMagicAction::crowd_change_armed_individuals },
    { "crowd_change_attitude",              sword::UnitMagicAction::crowd_change_attitude },
    { "crowd_change_health_state",          sword::UnitMagicAction::crowd_change_health_state },
    { "crowd_creation",                     sword::UnitMagicAction::crowd_creation },
    { "crowd_total_destruction",            sword::UnitMagicAction::crowd_total_destruction },
    { "delete_unit",                        sword::UnitMagicAction::delete_unit },
    { "destroy_all",                        sword::UnitMagicAction::destroy_all },
    { "destroy_component",                  sword::UnitMagicAction::destroy_component },
    { "exec_script",                        sword::UnitMagicAction::exec_script },
    { "fire_order",                         sword::UnitMagicAction::create_fire_order },
    { "formation_creation",                 sword::UnitMagicAction::formation_creation },
    { "inhabitant_change_affinities",       sword::UnitMagicAction::inhabitant_change_affinities },
    { "inhabitant_change_alerted_state",    sword::UnitMagicAction::inhabitant_change_alerted_state },
    { "inhabitant_change_confined_state",   sword::UnitMagicAction::inhabitant_change_confined_state },
    { "inhabitant_change_health_state",     sword::UnitMagicAction::inhabitant_change_health_state },
    { "knowledge_group_update",             sword::UnitMagicAction::knowledge_group_update },
    { "load_unit",                          sword::UnitMagicAction::load_unit },
    { "log_finish_handlings",               sword::UnitMagicAction::log_finish_handlings },
    { "log_maintenance_set_manual",         sword::UnitMagicAction::log_maintenance_set_manual },
    { "log_supply_change_quotas",           sword::UnitMagicAction::log_supply_change_quotas },
    { "log_supply_pull_flow",               sword::UnitMagicAction::log_supply_pull_flow },
    { "log_supply_push_flow",               sword::UnitMagicAction::log_supply_push_flow },
    { "log_supply_set_manual",              sword::UnitMagicAction::log_supply_set_manual },
    { "partial_recovery",                   sword::UnitMagicAction::partial_recovery },
    { "recover_all",                        sword::UnitMagicAction::recover_all },
    { "recover_all_except_log",             sword::UnitMagicAction::recover_all_except_log },
    { "recover_equipments",                 sword::UnitMagicAction::recover_equipments },
    { "recover_equipments_except_log",      sword::UnitMagicAction::recover_equipments_except_log },
    { "recover_resources",                  sword::UnitMagicAction::recover_resources },
    { "recover_resources_except_log",       sword::UnitMagicAction::recover_resources_except_log },
    { "recover_transporters",               sword::UnitMagicAction::recover_transporters },
    { "recover_troops",                     sword::UnitMagicAction::recover_troops },
    { "recover_troops_except_log",          sword::UnitMagicAction::recover_troops_except_log },
    { "reload_brain",                       sword::UnitMagicAction::reload_brain },
    { "rename",                             sword::UnitMagicAction::rename },
    { "surrender",                          sword::UnitMagicAction::surrender_to },
    { "teleport",                           sword::UnitMagicAction::move_to },
    { "transfer_equipment",                 sword::UnitMagicAction::transfer_equipment },
    { "unit_change_affinities",             sword::UnitMagicAction::unit_change_affinities },
    { "unit_change_superior",               sword::UnitMagicAction::unit_change_superior },
    { "unit_creation",                      sword::UnitMagicAction::unit_creation },
    { "unload_unit",                        sword::UnitMagicAction::unload_unit },
};
const size_t mapping::MagicUnitAction::size_ = COUNT_OF( mapping::MagicUnitAction::data_ );
static_assert( sword::UnitMagicAction::Type_ARRAYSIZE == mapping::MagicUnitAction::size_, "missing magic unit action descriptors" );

const mapping::MagicKnowledgeAction mapping::MagicKnowledgeAction::data_[] = {
    { "knowledge_group_add_knowledge",      sword::KnowledgeMagicAction::add_knowledge },
    { "knowledge_group_enable",             sword::KnowledgeMagicAction::enable },
    { "knowledge_group_rename",             sword::KnowledgeMagicAction::rename },
    { "knowledge_group_update_side",        sword::KnowledgeMagicAction::update_party },
    { "knowledge_group_update_side_parent", sword::KnowledgeMagicAction::update_party_parent },
    { "knowledge_group_update_type",        sword::KnowledgeMagicAction::update_type },
};

const size_t mapping::MagicKnowledgeAction::size_ = COUNT_OF( mapping::MagicKnowledgeAction::data_ );
static_assert( sword::KnowledgeMagicAction::Type_ARRAYSIZE == mapping::MagicKnowledgeAction::size_, "missing magic knowledge action descriptors" );

const mapping::MagicObjectAction mapping::MagicObjectAction::data_[] = {
    { "create_object",  sword::ObjectMagicAction::create },
    { "destroy_object", sword::ObjectMagicAction::destroy },
    { "rename_object",  sword::ObjectMagicAction::rename },
    { "update_object",  sword::ObjectMagicAction::update },
};

const size_t mapping::MagicObjectAction::size_ = COUNT_OF( mapping::MagicObjectAction::data_ );
static_assert( sword::ObjectMagicAction::Type_ARRAYSIZE == mapping::MagicObjectAction::size_, "missing magic object action descriptors" );

const mapping::MagicAction mapping::MagicAction::data_[] = {
    { "change_diplomacy",               sword::MagicAction::change_diplomacy },
    { "change_resource_links",          sword::MagicAction::change_resource_network_properties },
    { "create_knowledge_group",         sword::MagicAction::create_knowledge_group },
    { "fire_order_on_location",         sword::MagicAction::create_fire_order_on_location },
    { "global_weather",                 sword::MagicAction::global_weather },
    { "local_weather",                  sword::MagicAction::local_weather },
    { "local_weather_destruction",      sword::MagicAction::local_weather_destruction },
    { "debug_internal",                 sword::MagicAction::debug_internal},
    { "pathfind_creation",              sword::MagicAction::pathfind_creation },
    { "pathfind_destruction",           sword::MagicAction::pathfind_destruction },
    { "pathfind_update",                sword::MagicAction::pathfind_update },
    { "select_diagnosis_team",          sword::MagicAction::select_diagnosis_team},
    { "select_maintenance_transporter", sword::MagicAction::select_maintenance_transporter},
    { "select_new_logistic_state",      sword::MagicAction::select_new_logistic_state},
    { "select_repair_team",             sword::MagicAction::select_repair_team},
    { "transfer_to_logistic_superior",  sword::MagicAction::transfer_to_logistic_superior},
};

const size_t mapping::MagicAction::size_ = COUNT_OF( mapping::MagicAction::data_ );
static_assert( sword::MagicAction::Type_ARRAYSIZE == mapping::MagicAction::size_, "missing magic action descriptors" );

const mapping::PhaseLineType mapping::PhaseLineType::data_[] = {
    { "LCA",  sword::PhaseLineOrder::attitude_change_line },
    { "LD",   sword::PhaseLineOrder::line_of_departure },
    { "LC",   sword::PhaseLineOrder::coordination_line },
    { "LI",   sword::PhaseLineOrder::denial_line },
    { "LO",   sword::PhaseLineOrder::objective_line },
    { "LCAR", sword::PhaseLineOrder::blocking_line },
    { "LR",   sword::PhaseLineOrder::handover_line },
    { "LDM",  sword::PhaseLineOrder::start_of_mission_line },
    { "LFM",  sword::PhaseLineOrder::end_of_mission_line },
    { "LIA",  sword::PhaseLineOrder::recognition_and_reception_line },
};

const size_t mapping::PhaseLineType::size_ = COUNT_OF( mapping::PhaseLineType::data_ );
static_assert( sword::PhaseLineOrder::Function_ARRAYSIZE == mapping::PhaseLineType::size_, "missing phase line order descriptors" );

const mapping::Service mapping::Service::data_[] = {
    { "struct aar::Service",                sword::service_aar },
    { "struct authentication::Service",     sword::service_authentication },
    { "struct plugins::messenger::Service", sword::service_messenger },
    { "struct simulation::Service",         sword::service_simulation },
    { "struct replay::Service",             sword::service_replay },
};

const size_t mapping::Service::size_ = COUNT_OF( mapping::Service::data_ );
static_assert( sword::EnumService_ARRAYSIZE == mapping::Service::size_, "missing service descriptor" );
static_assert( sizeof sword::EnumService == sizeof uint32_t, "invalid EnumService size" );
