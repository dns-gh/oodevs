// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "Helpers.h"

#include <boost/static_assert.hpp>

using namespace protocol;

#define COUNT_OF(X) (sizeof(X)/sizeof*(X))

const mapping::MagicUnitAction mapping::MagicUnitAction::data_[] = {
    { "automat_creation",                   sword::UnitMagicAction::automat_creation },
    { "automat_log_supply_change_quotas",   sword::UnitMagicAction::log_supply_change_quotas },
    { "automat_log_supply_pull_flow",       sword::UnitMagicAction::log_supply_pull_flow },
    { "automat_log_supply_push_flow",       sword::UnitMagicAction::log_supply_push_flow },
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
    { "create_breakdowns",                  sword::UnitMagicAction::create_breakdowns },
    { "create_direct_fire_order",           sword::UnitMagicAction::create_direct_fire_order },
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
    { "fire_order",                         sword::UnitMagicAction::create_fire_order },
    { "formation_creation",                 sword::UnitMagicAction::formation_creation },
    { "formation_log_supply_change_quotas", sword::UnitMagicAction::log_supply_change_quotas },
    { "formation_log_supply_pull_flow",     sword::UnitMagicAction::log_supply_pull_flow },
    { "formation_log_supply_push_flow",     sword::UnitMagicAction::log_supply_push_flow },
    { "inhabitant_change_affinities",       sword::UnitMagicAction::inhabitant_change_affinities },
    { "inhabitant_change_alerted_state",    sword::UnitMagicAction::inhabitant_change_alerted_state },
    { "inhabitant_change_confined_state",   sword::UnitMagicAction::inhabitant_change_confined_state },
    { "inhabitant_change_health_state",     sword::UnitMagicAction::inhabitant_change_health_state },
    { "knowledge_group_update",             sword::UnitMagicAction::knowledge_group_update },
    { "load_unit",                          sword::UnitMagicAction::load_unit },
    { "log_finish_handlings",               sword::UnitMagicAction::log_finish_handlings },
    { "partial_recovery",                   sword::UnitMagicAction::partial_recovery },
    { "recover_all",                        sword::UnitMagicAction::recover_all },
    { "recover_equipments",                 sword::UnitMagicAction::recover_equipments },
    { "recover_resources",                  sword::UnitMagicAction::recover_resources },
    { "recover_transporters",               sword::UnitMagicAction::recover_transporters },
    { "recover_troops",                     sword::UnitMagicAction::recover_troops },
    { "recover_all_except_log",             sword::UnitMagicAction::recover_all_except_log },
    { "recover_equipments_except_log",      sword::UnitMagicAction::recover_equipments_except_log },
    { "recover_resources_except_log",       sword::UnitMagicAction::recover_resources_except_log },
    { "recover_troops_except_log",          sword::UnitMagicAction::recover_troops_except_log },
    { "reload_brain",                       sword::UnitMagicAction::reload_brain },
    { "surrender",                          sword::UnitMagicAction::surrender_to },
    { "teleport",                           sword::UnitMagicAction::move_to },
    { "transfer_equipment",                 sword::UnitMagicAction::transfer_equipment },
    { "unit_change_affinities",             sword::UnitMagicAction::unit_change_affinities },
    { "unit_change_superior",               sword::UnitMagicAction::unit_change_superior },
    { "unit_creation",                      sword::UnitMagicAction::unit_creation },
    { "unload_unit",                        sword::UnitMagicAction::unload_unit },
    { "change_posture",                     sword::UnitMagicAction::change_posture },
    { "exec_script",                        sword::UnitMagicAction::exec_script },
};
const size_t mapping::MagicUnitAction::size_ = COUNT_OF( mapping::MagicUnitAction::data_ );

const mapping::MagicKnowledgeAction mapping::MagicKnowledgeAction::data_[] = {
    { "knowledge_group_add_knowledge",      sword::KnowledgeMagicAction::add_knowledge },
    { "knowledge_group_enable",             sword::KnowledgeMagicAction::enable },
    { "knowledge_group_update_side",        sword::KnowledgeMagicAction::update_party },
    { "knowledge_group_update_side_parent", sword::KnowledgeMagicAction::update_party_parent },
    { "knowledge_group_update_type",        sword::KnowledgeMagicAction::update_type },
};

const size_t mapping::MagicKnowledgeAction::size_ = COUNT_OF( mapping::MagicKnowledgeAction::data_ );
BOOST_STATIC_ASSERT( sword::KnowledgeMagicAction::Type_ARRAYSIZE == mapping::MagicKnowledgeAction::size_ );

const mapping::MagicObjectAction mapping::MagicObjectAction::data_[] = {
    { "create_object",  sword::ObjectMagicAction::create },
    { "destroy_object", sword::ObjectMagicAction::destroy },
    { "update_object",  sword::ObjectMagicAction::update },
};

const size_t mapping::MagicObjectAction::size_ = COUNT_OF( mapping::MagicObjectAction::data_ );
BOOST_STATIC_ASSERT( sword::ObjectMagicAction::Type_ARRAYSIZE == mapping::MagicObjectAction::size_ );

const mapping::MagicAction mapping::MagicAction::data_[] = {
    { "change_diplomacy",          sword::MagicAction::change_diplomacy },
    { "change_resource_links",     sword::MagicAction::change_resource_network_properties },
    { "create_knowledge_group",    sword::MagicAction::create_knowledge_group },
    { "fire_order_on_location",    sword::MagicAction::create_fire_order_on_location },
    { "global_weather",            sword::MagicAction::global_weather },
    { "local_weather",             sword::MagicAction::local_weather },
    { "local_weather_destruction", sword::MagicAction::local_weather_destruction },
    { "debug_internal",            sword::MagicAction::debug_internal},
};

const size_t mapping::MagicAction::size_ = COUNT_OF( mapping::MagicAction::data_ );
BOOST_STATIC_ASSERT( sword::MagicAction::Type_ARRAYSIZE == mapping::MagicAction::size_ );

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
BOOST_STATIC_ASSERT( sword::PhaseLineOrder::Function_ARRAYSIZE == mapping::PhaseLineType::size_ );

const mapping::Service mapping::Service::data_[] = {
    { "struct aar::Service",                sword::service_aar },
    { "struct authentication::Service",     sword::service_authentication },
    { "struct plugins::messenger::Service", sword::service_messenger },
    { "struct simulation::Service",         sword::service_simulation },
    { "struct replay::Service",             sword::service_replay },
};

const size_t mapping::Service::size_ = COUNT_OF( mapping::Service::data_ );
BOOST_STATIC_ASSERT( sword::Service_ARRAYSIZE == mapping::Service::size_ );
