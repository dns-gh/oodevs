// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathematiques Appliquees SA (MASA)
//
// *****************************************************************************
#include "Actions.h"
#include "Simulation.h"
#include "ENT/ENT_Enums.h"
#include <tools/Helpers.h>

#define ENUM( name, ... )                                                               \
    const int name##_values[] = { __VA_ARGS__ };                                        \
    const ActionEnum name##_enum = { #name, name##_values, COUNT_OF( name##_values ) };

namespace protocol
{
namespace mapping
{
namespace
{

ENUM( diplomacy,
    sword::unknown,
    sword::friendly,
    sword::enemy,
    sword::neutral
);

ENUM( tiredness,
    eUnitTiredness_Normal,
    eUnitTiredness_Fatigue,
    eUnitTiredness_Epuise
);

ENUM( moral,
    eUnitMorale_Fanatique,
    eUnitMorale_Bon,
    eUnitMorale_Moyen,
    eUnitMorale_Mauvais
);

ENUM( experience,
    eUnitExperience_Veteran,
    eUnitExperience_Experimente,
    eUnitExperience_Conscrit
);

ENUM( stress,
    eUnitStress_Calm,
    eUnitStress_Worried,
    eUnitStress_Stressed
);

ENUM( attitude,
    ePopulationAttitude_Calme,
    ePopulationAttitude_Agitee,
    ePopulationAttitude_Excitee,
    ePopulationAttitude_Agressive
);

ENUM( weather,
    eWeatherType_None,
    eWeatherType_SandStorm,
    eWeatherType_Fog,
    eWeatherType_Drizzle,
    eWeatherType_Rain,
    eWeatherType_Snow,
    eWeatherType_Smoke
);

ENUM( identification,
    sword::UnitIdentification::detected,
    sword::UnitIdentification::recognized,
    sword::UnitIdentification::identified
);

}  // namespace

const ActionEnum* enums[] =
{
    &attitude_enum,
    &diplomacy_enum,
    &experience_enum,
    &identification_enum,
    &moral_enum,
    &stress_enum,
    &tiredness_enum,
    &weather_enum,
};

const size_t enumsCount = COUNT_OF( enums );

namespace
{

const ActionParam automat_and_units_creation_params[] =
{
    { "AutomatType", "identifier", 0 },
    { "Location", "point", 0 },
    { "KnowledgeGroup", "identifier", 0 },
};

const ActionParam automat_creation_params[] =
{
    { "AutomatType", "identifier", 0 },
    { "KnowledgeGroup", "identifier", 0 },
};

const ActionParam change_automat_superior_params[] =
{
    { "Automat", "automat", 0 },
};

const ActionParam change_brain_debug_params[] =
{
    { "Enable", "boolean", 0 },
};

const ActionParam change_critical_intelligence_params[] =
{
    { "Name", "string", 0 },
};

const ActionParam change_diplomacy_params[] =
{
    { "Camp1", "identifier", 0 },
    { "Camp2", "identifier", 0 },
    { "Diplomacy", "enumeration", &diplomacy_enum },
};

const ActionParam change_dotation_params[] =
{
    { "Dotations", "list", 0 },
};

const ActionParam change_equipment_state_params[] =
{
    { "EquipmentStates", "list", 0 },
};

const ActionParam change_extension_params[] =
{
    { "Extensions", "extensionlist", 0 },
};

const ActionParam change_formation_superior_params[] =
{
    { "Formation", "formation", 0 },
};

const ActionParam change_human_factors_params[] =
{
    { "Tiredness", "enumeration", &tiredness_enum },
    { "Morale", "enumeration", &moral_enum },
    { "Experience", "enumeration", &experience_enum },
    { "Stress", "enumeration", &stress_enum },
};

const ActionParam change_human_state_params[] =
{
    { "HumanStates", "list", 0 },
};

const ActionParam change_knowledge_group_params[] =
{
    { "Group", "knowledgegroup", 0 },
    { "Camp", "army", 0 },
};

const ActionParam change_logistic_links_params[] =
{
    { "Nominal", "identifier", 0 },
    { "Current", "identifier", 0 },
};

const ActionParam change_resource_links_params[] =
{
    { "Target", "identifier", 0 },
    { "Urban", "boolean", 0 },
    { "Nodes", "list", 0 },
};

const ActionParam change_trafficability_params[] =
{
    { "Trafficability", "numeric", 0 },
};

const ActionParam create_breakdowns_params[] =
{
    { "Breakdowns", "list", 0 },
};

const ActionParam create_knowledge_group_params[] =
{
    { "Camp", "identifier", 0 },
    { "Parent", "identifier", 0 },
    { "Type", "string", 0 },
};

const ActionParam create_object_params[] =
{
    { "Type", "string", 0 },
    { "Location", "location", 0 },
    { "Name", "string", 0 },
    { "Camp", "army", 0 },
    { "Attributes", "list", 0 },
};

const ActionParam create_wounds_params[] =
{
    { "Wounds", "list", 0 },
};

const ActionParam crowd_change_affinities_params[] =
{
    { "Affinities", "list", 0 },
};

const ActionParam crowd_change_armed_individuals_params[] =
{
    { "ArmedIndividuals", "quantity", 0 },
};

const ActionParam crowd_change_attitude_params[] =
{
    { "Attitude", "enumeration", &attitude_enum },
};

const ActionParam crowd_change_health_state_params[] =
{
    { "Healthy", "quantity", 0 },
    { "Wounded", "quantity", 0 },
    { "Contaminated", "quantity", 0 },
    { "Dead", "quantity", 0 },
};

const ActionParam crowd_creation_params[] =
{
    { "PopulationType", "string", 0 },
    { "Location", "point", 0 },
    { "Number healthy", "quantity", 0 },
    { "Number wounded", "quantity", 0 },
    { "Number dead", "quantity", 0 },
    { "Name", "string", 0 },
};

const ActionParam fire_order_params[] =
{
    { "UnitTarget", "identifier", 0 },
    { "Ammo", "resourcetype", 0 },
    { "Iterations", "numeric", 0 },
};

const ActionParam fire_order_on_location_params[] =
{
    { "LocationTarget", "location", 0 },
    { "Ammo", "resourcetype", 0 },
    { "Iterations", "numeric", 0 },
};

const ActionParam formation_creation_params[] =
{
    { "Level", "numeric", 0 },
    { "Name", "string", 0 },
    { "LogLevel", "string", 0 },
};

const ActionParam global_weather_params[] =
{
    { "Temperature", "numeric", 0 },
    { "WindSpeed", "numeric", 0 },
    { "WindDirection", "heading", 0 },
    { "CloudFloor", "numeric", 0 },
    { "CloudCeiling", "numeric", 0 },
    { "CloudDensity", "numeric", 0 },
    { "Precipitation", "enumeration", &weather_enum },
};

const ActionParam inhabitant_change_affinities_params[] =
{
    { "Affinities", "list", 0 },
};

const ActionParam inhabitant_change_alerted_state_params[] =
{
    { "Alerted", "boolean", 0 },
};

const ActionParam inhabitant_change_confined_state_params[] =
{
    { "Confined", "boolean", 0 },
};

const ActionParam inhabitant_change_health_state_params[] =
{
    { "Healthy", "quantity", 0 },
    { "Wounded", "quantity", 0 },
    { "Dead", "quantity", 0 },
};

const ActionParam knowledge_group_add_knowledge_params[] =
{
    { "Entity", "identifier", 0 },
    { "Perception", "enumeration", &identification_enum },
};

const ActionParam knowledge_group_enable_params[] =
{
    { "Enabled", "boolean", 0 },
};

const ActionParam knowledge_group_update_side_params[] =
{
    { "Camp", "army", 0 },
};

const ActionParam knowledge_group_update_side_parent_params[] =
{
    { "Camp", "army", 0 },
    { "Parent", "knowledgegroup", 0 },
};

const ActionParam knowledge_group_update_type_params[] =
{
    { "Type", "string", 0 },
};

const ActionParam local_weather_params[] =
{
    { "Temperature", "numeric", 0 },
    { "WindSpeed", "numeric", 0 },
    { "WindDirection", "heading", 0 },
    { "CloudFloor", "numeric", 0 },
    { "CloudCeiling", "numeric", 0 },
    { "CloudDensity", "numeric", 0 },
    { "Precipitation", "enumeration", &weather_enum },
    { "StartTime", "datetime", 0 },
    { "EndTime", "datetime", 0 },
    { "Location", "location", 0 },
    { "ID", "identifier", 0 },
};

const ActionParam local_weather_destruction_params[] =
{
    { "WeatherID", "identifier", 0 },
};

const ActionParam log_supply_change_quotas_params[] =
{
    { "Receiver", "identifier", 0 },
    { "Dotations", "list", 0 },
};

const ActionParam log_supply_pull_flow_params[] =
{
    { "Parameters", "pullflowparameters", 0 },
};

const ActionParam log_supply_push_flow_params[] =
{
    { "Parameters", "pushflowparameters", 0 },
};

const ActionParam log_supply_set_manual_params[] =
{
    { "Manual", "boolean", 0 },
};

const ActionParam log_maintenance_set_manual_params[] =
{
    { "Manual", "boolean", 0 },
};

const ActionParam select_repair_team_params[] =
{
    { "Request", "identifier", 0 },
    { "EquipmentType", "identifier", 0 },
};

const ActionParam select_maintenance_transporter_params[] =
{
    { "Request", "identifier", 0 },
    { "EquipmentType", "identifier", 0 },
};

const ActionParam select_diagnosis_team_params[] =
{
    { "Request", "identifier", 0 },
    { "EquipmentType", "identifier", 0 },
};

const ActionParam select_new_logistic_state_params[] =
{
    { "Request", "identifier", 0 },
};

const ActionParam transfer_to_logistic_superior_params[] =
{
    { "Request", "identifier", 0 },
};

const ActionParam partial_recovery_params[] =
{
    { "Equipments", "list", 0 },
    { "Humans", "list", 0 },
    { "Dotations", "list", 0 },
    { "Ammo", "list", 0 },
    { "Stocks", "list", 0 },
};

const ActionParam reload_brain_params[] =
{
    { "Model", "string", 0 },
};

const ActionParam surrender_params[] =
{
    { "Camp", "army", 0 },
};

const ActionParam teleport_params[] =
{
    { "Location", "point", 0 },
};

const ActionParam transfer_equipment_params[] =
{
    { "Receiver", "identifier", 0 },
    { "Equipments", "list", 0 },
};

const ActionParam unit_change_affinities_params[] =
{
    { "Affinities", "list", 0 },
};

const ActionParam unit_change_superior_params[] =
{
    { "Superior", "automat", 0 },
};

const ActionParam unit_creation_params[] =
{
    { "UnitType", "identifier", 0 },
    { "Location", "point", 0 },
};

const ActionParam update_object_params[] =
{
    { "Attributes", "list", 0 },
};

const ActionParam update_urban_params[] =
{
    { "Target", "identifier", 0 },
    { "StructuralState", "quantity", 0 },
};

}  // namespace
#define ACTION( name ) { #name, name##_params, COUNT_OF( name##_params ) }

const Action actions[] =
{
    ACTION( automat_and_units_creation ),
    ACTION( automat_creation ),
    ACTION( change_automat_superior ),
    ACTION( change_brain_debug ),
    ACTION( change_critical_intelligence ),
    ACTION( change_diplomacy ),
    ACTION( change_dotation ),
    ACTION( change_equipment_state ),
    ACTION( change_extension ),
    ACTION( change_formation_superior ),
    ACTION( change_human_factors ),
    ACTION( change_human_state ),
    ACTION( change_knowledge_group ),
    ACTION( change_logistic_links ),
    ACTION( change_resource_links ),
    ACTION( change_trafficability ),
    ACTION( create_breakdowns ),
    ACTION( create_knowledge_group ),
    ACTION( create_object ),
    ACTION( create_wounds ),
    ACTION( crowd_change_affinities ),
    ACTION( crowd_change_armed_individuals ),
    ACTION( crowd_change_attitude ),
    ACTION( crowd_change_health_state ),
    ACTION( crowd_creation ),
    ACTION( fire_order ),
    ACTION( fire_order_on_location ),
    ACTION( formation_creation ),
    ACTION( global_weather ),
    ACTION( inhabitant_change_affinities ),
    ACTION( inhabitant_change_alerted_state ),
    ACTION( inhabitant_change_confined_state ),
    ACTION( inhabitant_change_health_state ),
    ACTION( knowledge_group_add_knowledge ),
    ACTION( knowledge_group_enable ),
    ACTION( knowledge_group_update_side ),
    ACTION( knowledge_group_update_side_parent ),
    ACTION( knowledge_group_update_type ),
    ACTION( local_weather ),
    ACTION( local_weather_destruction ),
    ACTION( log_maintenance_set_manual ),
    ACTION( log_supply_change_quotas ),
    ACTION( log_supply_pull_flow ),
    ACTION( log_supply_push_flow ),
    ACTION( log_supply_set_manual ),
    ACTION( partial_recovery ),
    ACTION( reload_brain ),
    ACTION( select_diagnosis_team ),
    ACTION( select_maintenance_transporter ),
    ACTION( select_new_logistic_state ),
    ACTION( select_repair_team ),
    ACTION( surrender ),
    ACTION( teleport ),
    ACTION( transfer_equipment ),
    ACTION( transfer_to_logistic_superior ),
    ACTION( unit_change_affinities ),
    ACTION( unit_change_superior ),
    ACTION( unit_creation ),
    ACTION( update_object ),
    ACTION( update_urban ),
};
const size_t actionsCount = COUNT_OF( actions );

}  // namespace mapping
}  // namespace protocol
