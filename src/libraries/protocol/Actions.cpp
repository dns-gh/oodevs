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

// Defines an enum <name>_enum and it's values <name>_values
#define ENUM( name, ... )                                                               \
    const int name##_values[] = { __VA_ARGS__ };                                        \
    const ActionEnum name##_enum = { #name, name##_values, COUNT_OF( name##_values ) };

// Defines a list of parameters:
// <name>_list_params should be declared before the call to LIST
#define LIST( name ) \
    const Action name##_list = { #name, name##_list_params, COUNT_OF( name##_list_params ) };

// Defines a structure of parameters:
// <name>_structure_params should be declared before the call to STRUCTURE
#define STRUCTURE( name ) \
const Action name##_structure = { #name, name##_structure_params, COUNT_OF( name##_structure_params ) };

// Defines a union of parameters:
// <name>_types should be declared before the call to UNION
#define UNION( name ) \
    const ActionUnion name##_union = { name##_types, COUNT_OF( name##_types ) };

// Defines a list of structure of parameters:
// <name>_structure_params should be declared before the call to LIST_OF_STRUCTURES
#define LIST_OF_STRUCTURES( name, structureName )                                            \
STRUCTURE( name )                                                                            \
const ActionParam name##_list_params[] = { { structureName, "list", 0, 0, &name##_structure, 0 } }; \
LIST( name )

// Defines a list of union of parameters:
// <name>_types should be declared before the call to LIST_OF_UNIONS
#define LIST_OF_UNIONS( name, unionName )                                            \
UNION( name )                                                                        \
const ActionParam name##_list_params[] = { { unionName, "list", 0, 0, 0, &name##_union } }; \
LIST( name )

// Defines an ActionParam table
#define PARAMS_PAIR( name, first_name, first_type, second_name, second_type )   \
const ActionParam name##_params[] =                                             \
{                                                                               \
    { first_name, first_type, 0, 0, 0, 0 },                                     \
    { second_name, second_type, 0, 0, 0, 0 },                                   \
};
#define PARAMS_PAIR_ID_BOOL( name, identifier, boolean )            \
PARAMS_PAIR( name, identifier, "identifier", boolean, "boolean" )
#define PARAMS_PAIR_ID_NUMERIC( name, identifier, numeric )         \
PARAMS_PAIR( name, identifier, "identifier", numeric, "numeric" )
#define PARAMS_PAIR_ID_QUANTITY( name, identifier, quantity )       \
PARAMS_PAIR( name, identifier, "identifier", quantity, "quantity" )

// Defines an action
// <name>_params should be declared before the call to ACTION
#define ACTION( name ) { #name, name##_params, COUNT_OF( name##_params ) }

// Defines an object attribute
// <name>_attribute_params should be declared before the call to OBJECT_ATTRIBUTE
#define OBJECT_ATTRIBUTE( attribute, name ) \
{ sword::ObjectMagicAction::##attribute, name, attribute##_attribute_params, COUNT_OF( attribute##_attribute_params ) }

namespace protocol
{
namespace mapping
{
namespace
{

// -----------------------------------------------------------------------------
// Enumerations
// -----------------------------------------------------------------------------

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

ENUM( injury,
    eInjuriesSeriousness_U1,
    eInjuriesSeriousness_U2,
    eInjuriesSeriousness_U3,
    eInjuriesSeriousness_UE,
);

ENUM( human_rank,
    eHumanRank_Officier,
    eHumanRank_SousOfficer,
    eHumanRank_Mdr,
);

ENUM( human_state,
    eHumanState_Healthy,
    eHumanState_Injured,
    eHumanState_Dead,
);

}  // namespace

const ActionEnum* enums[] =
{
    &attitude_enum,
    &diplomacy_enum,
    &experience_enum,
    &human_rank_enum,
    &human_state_enum,
    &identification_enum,
    &injury_enum,
    &moral_enum,
    &stress_enum,
    &tiredness_enum,
    &weather_enum,
};
const size_t enumsCount = COUNT_OF( enums );

namespace
{

// -----------------------------------------------------------------------------
// Object attributes
// -----------------------------------------------------------------------------

PARAMS_PAIR_ID_BOOL( alerted_attribute, "AttributeId", "Alerted" )
PARAMS_PAIR_ID_BOOL( confined_attribute, "AttributeId", "Confined" )
PARAMS_PAIR_ID_BOOL( evacuated_attribute, "AttributeId", "Evacuated" )
PARAMS_PAIR_ID_BOOL( underground_attribute, "AttributeId", "Underground" )

PARAMS_PAIR_ID_QUANTITY( altitude_modifier_attribute, "AttributeId", "Height" )
PARAMS_PAIR_ID_QUANTITY( bypass_attribute, "AttributeId", "Percentage" )
PARAMS_PAIR_ID_QUANTITY( effect_delay_attribute, "AttributeId", "Delay" )
PARAMS_PAIR_ID_QUANTITY( interaction_height_attribute, "AttributeId", "Height" )
PARAMS_PAIR_ID_QUANTITY( lodging_attribute, "AttributeId", "Capacity" )
PARAMS_PAIR_ID_QUANTITY( mine_creation_attribute, "AttributeId", "Density" )
PARAMS_PAIR_ID_QUANTITY( time_limit_attribute, "AttributeId", "Time" )

PARAMS_PAIR_ID_NUMERIC( density_attribute, "AttributeId", "Density" )
PARAMS_PAIR_ID_NUMERIC( infrastructure_attribute, "AttributeId", "Threshold" )
PARAMS_PAIR_ID_NUMERIC( structural_state_attribute, "AttributeId", "Value" )
PARAMS_PAIR_ID_NUMERIC( trafficability_attribute, "AttributeId", "Trafficability" )

PARAMS_PAIR( logistic_attribute, "AttributeId", "identifier", "TC2", "identifier" )

const ActionParam* nbc_agent_attribute_params = 0; // deprecated
const ActionParam* medical_treatment_attribute_params = 0; // deprecated
const ActionParam* toxic_cloud_attribute_params = 0; // deprecated
const ActionParam* usages_attribute_params = 0; // deprecated

const ActionParam construction_attribute_params[] =
{
    { "AttributeId", "identifier", 0, 0, 0, 0 },
    { "Type", "identifier", 0, 0, 0, 0 },
    { "Number", "quantity", 0, 0, 0, 0 },
    { "Density", "numeric", 0, 0, 0, 0 },
    { "Percentage", "quantity", 0, 0, 0, 0 },
};

const ActionParam crossing_site_attribute_params[] =
{
    { "AttributeId", "identifier", 0, 0, 0, 0 },
    { "Width", "quantity", 0, 0, 0, 0 },
    { "Depth", "quantity", 0, 0, 0, 0 },
    { "FlowRate", "quantity", 0, 0, 0, 0 },
    { "BanksRequireFitting", "boolean", 0, 0, 0, 0 },
};

const ActionParam disaster_attribute_params[] =
{
    { "AttributeId", "identifier", 0, 0, 0, 0 },
    { "Model", "string", 0, 0, 0, 0 },
    { "Date", "datetime", 0, 0, 0, 0 },
};

const ActionParam fire_attribute_params[] =
{
    { "AttributeId", "identifier", 0, 0, 0, 0 },
    { "FireClass", "string", 0, 0, 0, 0 },
    { "MaxCombustionEnergy", "quantity", 0, 0, 0, 0 },
};

const ActionParam flood_attribute_params[] =
{
    { "AttributeId", "identifier", 0, 0, 0, 0 },
    { "Depth", "quantity", 0, 0, 0, 0 },
    { "RefDist", "quantity", 0, 0, 0, 0 },
};

const ActionParam mine_update_attribute_params[] =
{
    { "AttributeId", "identifier", 0, 0, 0, 0 },
    { "Type", "identifier", 0, 0, 0, 0 },
    { "Number", "quantity", 0, 0, 0, 0 },
    { "Density", "numeric", 0, 0, 0, 0 },
    { "Percentage", "quantity", 0, 0, 0, 0 },
};

const ActionParam nbc_agents_list_params[] =
{
    { "ID", "identifier", 0, 0, 0, 0 },
};
LIST( nbc_agents )

const ActionParam nbc_attribute_params[] =
{
    { "AttributeId", "identifier", 0, 0, 0, 0 },
    { "DangerLevel", "quantity", 0, 0, 0, 0 },
    { "NBCAgents", "list", 0, &nbc_agents_list, 0, 0 },
    { "State", "quantity", 0, 0, 0, 0 },
};

const ActionParam obstacle_attribute_params[] =
{
    { "AttributeId", "identifier", 0, 0, 0, 0 },
    { "TargetType", "identifier", 0, 0, 0, 0 },
    { "Activation", "boolean", 0, 0, 0, 0 },
    { "ActivationTime", "quantity", 0, 0, 0, 0 },
    { "ActivityTime", "quantity", 0, 0, 0, 0 },
};

const ActionParam resource_network_attribute_params[] =
{
    { "AttributeId", "identifier", 0, 0, 0, 0 },
};

const ActionParam stock_attribute_dotations_structure_params[] =
{
    { "DotationId", "identifier", 0, 0, 0, 0 },
    { "Stock", "quantity", 0, 0, 0, 0 },
    { "MaxStock", "quantity", 0, 0, 0, 0 },
};
LIST_OF_STRUCTURES( stock_attribute_dotations, "Dotation" );

const ActionParam stock_attribute_params[] =
{
    { "AttributeId", "identifier", 0, 0, 0, 0 },
    { "Dotations", "list", 0, &stock_attribute_dotations_list, 0, 0 },
};

const ActionParam supply_route_attribute_params[] =
{
    { "AttributeId", "identifier", 0, 0, 0, 0 },
    { "Equipped", "boolean", 0, 0, 0, 0 },
    { "MaxWeight", "quantity", 0, 0, 0, 0 },
    { "Width", "quantity", 0, 0, 0, 0 },
    { "Length", "quantity", 0, 0, 0, 0 },
    { "FlowRate", "quantity", 0, 0, 0, 0 },
};

const ActionUnionType object_create_attributes_types[] =
{
    OBJECT_ATTRIBUTE( alerted, "Alerted" ),
    OBJECT_ATTRIBUTE( altitude_modifier, "Altitude modifier" ),
    OBJECT_ATTRIBUTE( bypass, "Bypass" ),
    OBJECT_ATTRIBUTE( confined, "Confined" ),
    OBJECT_ATTRIBUTE( construction, "Construction" ),
    OBJECT_ATTRIBUTE( crossing_site, "Crossing site" ),
    OBJECT_ATTRIBUTE( density, "Density" ),
    OBJECT_ATTRIBUTE( disaster, "Disaster" ),
    OBJECT_ATTRIBUTE( effect_delay, "Effect delay" ),
    OBJECT_ATTRIBUTE( evacuated, "Evacuated" ),
    OBJECT_ATTRIBUTE( fire, "Fire" ),
    OBJECT_ATTRIBUTE( flood, "Flood" ),
    OBJECT_ATTRIBUTE( infrastructure, "Infrastructure" ),
    OBJECT_ATTRIBUTE( interaction_height, "Interaction height" ),
    OBJECT_ATTRIBUTE( lodging, "Lodging" ),
    OBJECT_ATTRIBUTE( logistic, "Logistic" ),
    OBJECT_ATTRIBUTE( medical_treatment, "Medical treatment" ),
    { sword::ObjectMagicAction::mine, "Mine", mine_creation_attribute_params, COUNT_OF( mine_creation_attribute_params ) },
    OBJECT_ATTRIBUTE( nbc, "Nbc" ),
    OBJECT_ATTRIBUTE( nbc_agent, "Nbc agent" ),
    OBJECT_ATTRIBUTE( obstacle, "Obstacle" ),
    OBJECT_ATTRIBUTE( resource_network, "Resource network" ),
    OBJECT_ATTRIBUTE( stock, "Stock" ),
    OBJECT_ATTRIBUTE( structural_state, "Structural state" ),
    OBJECT_ATTRIBUTE( supply_route, "Supply route" ),
    OBJECT_ATTRIBUTE( time_limit, "Time limit" ),
    OBJECT_ATTRIBUTE( toxic_cloud, "Toxic cloud" ),
    OBJECT_ATTRIBUTE( trafficability, "Trafficability" ),
    OBJECT_ATTRIBUTE( underground, "Underground" ),
    OBJECT_ATTRIBUTE( usages, "Usages" ),
};
LIST_OF_UNIONS( object_create_attributes, "Attribute" )

const ActionUnionType object_update_attributes_types[] =
{
    OBJECT_ATTRIBUTE( alerted, "Alerted" ),
    OBJECT_ATTRIBUTE( altitude_modifier, "Altitude modifier" ),
    OBJECT_ATTRIBUTE( bypass, "Bypass" ),
    OBJECT_ATTRIBUTE( confined, "Confined" ),
    OBJECT_ATTRIBUTE( construction, "Construction" ),
    OBJECT_ATTRIBUTE( crossing_site, "Crossing site" ),
    OBJECT_ATTRIBUTE( density, "Density" ),
    OBJECT_ATTRIBUTE( disaster, "Disaster" ),
    OBJECT_ATTRIBUTE( effect_delay, "Effect delay" ),
    OBJECT_ATTRIBUTE( evacuated, "Evacuated" ),
    OBJECT_ATTRIBUTE( fire, "Fire" ),
    OBJECT_ATTRIBUTE( flood, "Flood" ),
    OBJECT_ATTRIBUTE( infrastructure, "Infrastructure" ),
    OBJECT_ATTRIBUTE( interaction_height, "Interaction height" ),
    OBJECT_ATTRIBUTE( lodging, "Lodging" ),
    OBJECT_ATTRIBUTE( logistic, "Logistic" ),
    OBJECT_ATTRIBUTE( medical_treatment, "Medical treatment" ),
    { sword::ObjectMagicAction::mine, "Mine", mine_update_attribute_params, COUNT_OF( mine_update_attribute_params ) },
    OBJECT_ATTRIBUTE( nbc, "Nbc" ),
    OBJECT_ATTRIBUTE( nbc_agent, "Nbc agent" ),
    OBJECT_ATTRIBUTE( obstacle, "Obstacle" ),
    OBJECT_ATTRIBUTE( resource_network, "Resource network" ),
    OBJECT_ATTRIBUTE( stock, "Stock" ),
    OBJECT_ATTRIBUTE( structural_state, "Structural state" ),
    OBJECT_ATTRIBUTE( supply_route, "Supply route" ),
    OBJECT_ATTRIBUTE( time_limit, "Time limit" ),
    OBJECT_ATTRIBUTE( toxic_cloud, "Toxic cloud" ),
    OBJECT_ATTRIBUTE( trafficability, "Trafficability" ),
    OBJECT_ATTRIBUTE( underground, "Underground" ),
    OBJECT_ATTRIBUTE( usages, "Usages" ),
};
LIST_OF_UNIONS( object_update_attributes, "Attribute" )


// -----------------------------------------------------------------------------
// Actions
// -----------------------------------------------------------------------------

const ActionParam automat_and_units_creation_params[] =
{
    { "AutomatType", "identifier", 0, 0, 0, 0 },
    { "Location", "point", 0, 0, 0, 0 },
    { "KnowledgeGroup", "identifier", 0, 0, 0, 0 },
};

const ActionParam automat_creation_params[] =
{
    { "AutomatType", "identifier", 0, 0, 0, 0 },
    { "KnowledgeGroup", "identifier", 0, 0, 0, 0 },
};

const ActionParam change_automat_superior_params[] =
{
    { "Automat", "automat", 0, 0, 0, 0 },
};

const ActionParam change_brain_debug_params[] =
{
    { "Enable", "boolean", 0, 0, 0, 0 },
};

const ActionParam change_critical_intelligence_params[] =
{
    { "Name", "string", 0, 0, 0, 0 },
};

const ActionParam change_diplomacy_params[] =
{
    { "Camp1", "identifier", 0, 0, 0, 0 },
    { "Camp2", "identifier", 0, 0, 0, 0 },
    { "Diplomacy", "enumeration", &diplomacy_enum, 0, 0, 0 },
};

const ActionParam dotations_structure_params[] =
{
    { "ID", "identifier", 0, 0, 0, 0 },
    { "Quantity", "quantity", 0, 0, 0, 0 },
    { "Threshold", "numeric", 0, 0, 0, 0 },
};
LIST_OF_STRUCTURES( dotations, "Dotation" )

const ActionParam change_dotation_params[] =
{
    { "Dotations", "list", 0, &dotations_list, 0, 0 },
};

const ActionParam breakdowns_list_params[] =
{
    { "Breakdown", "identifier", 0, 0, 0, 0 },
};
LIST( breakdowns )

const ActionParam equipments_states_structure_params[] =
{
    { "ID", "identifier", 0, 0, 0, 0 },
    { "NbAvailable", "quantity", 0, 0, 0, 0 },
    { "NbDestroyed", "quantity", 0, 0, 0, 0 },
    { "NbRepairableWithEvac", "quantity", 0, 0, 0, 0 },
    { "NbOnSiteFixable", "quantity", 0, 0, 0, 0 },
    { "NbInMaintenance", "quantity", 0, 0, 0, 0 },
    { "NbPrisoner", "quantity", 0, 0, 0, 0 },
    { "Breakdowns", "list", 0, &breakdowns_list, 0, 0 },
};
LIST_OF_STRUCTURES( equipments_states, "Equipment" );

const ActionParam change_equipment_state_params[] =
{
    { "EquipmentStates", "list", 0, &equipments_states_list, 0, 0 },
};

const ActionParam change_extension_params[] =
{
    { "Extensions", "extensionlist", 0, 0, 0, 0 },
};

const ActionParam change_formation_superior_params[] =
{
    { "Formation", "formation", 0, 0, 0, 0 },
};

const ActionParam change_human_factors_params[] =
{
    { "Tiredness", "enumeration", &tiredness_enum, 0, 0, 0 },
    { "Morale", "enumeration", &moral_enum, 0, 0, 0 },
    { "Experience", "enumeration", &experience_enum, 0, 0, 0 },
    { "Stress", "enumeration", &stress_enum, 0, 0, 0 },
};

const ActionParam injuries_structure_params[] =
{
    { "ID", "identifier", 0, 0, 0, 0 },
    { "InjurySeriousness", "enumeration", &injury_enum, 0, 0, 0 },
};
LIST_OF_STRUCTURES( injuries, "Injury" )

const ActionParam human_states_structure_params[] =
{
    { "Quantity", "quantity", 0, 0, 0, 0 },
    { "HumanRank", "enumeration", &human_rank_enum, 0, 0, 0 },
    { "HumanState", "enumeration", &human_state_enum, 0, 0, 0 },
    { "Injuries", "list", 0, &injuries_list, 0, 0 },
    { "MentallyWounded", "boolean", 0, 0, 0, 0 },
    { "Contaminated", "boolean", 0, 0, 0, 0 },
};
LIST_OF_STRUCTURES( human_states, "Human" )

const ActionParam change_human_state_params[] =
{
    { "HumanStates", "list", 0, &human_states_list, 0, 0 },
};

const ActionParam change_knowledge_group_params[] =
{
    { "Group", "knowledgegroup", 0, 0, 0, 0 },
    { "Camp", "army", 0, 0, 0, 0 },
};

const ActionParam change_logistic_links_params[] =
{
    { "Nominal", "identifier", 0, 0, 0, 0 },
    { "Current", "identifier", 0, 0, 0, 0 },
};

PARAMS_PAIR_ID_QUANTITY( resource_links_structure, "Link", "Capacity" )
LIST_OF_STRUCTURES( resource_links, "Link" )

const ActionParam resource_nodes_structure_params[] =
{
    { "Resource", "string", 0, 0, 0, 0 },
    { "Consumption", "quantity", 0, 0, 0, 0 },
    { "Critical", "boolean", 0, 0, 0, 0 },
    { "Enabled", "boolean", 0, 0, 0, 0 },
    { "Production", "quantity", 0, 0, 0, 0 },
    { "MaxStock", "quantity", 0, 0, 0, 0 },
    { "Links", "list", 0, &resource_links_list, 0, 0 },
};
LIST_OF_STRUCTURES( resource_nodes, "Node" )

const ActionParam change_resource_links_params[] =
{
    { "Target", "identifier", 0, 0, 0, 0 },
    { "Urban", "boolean", 0, 0, 0, 0 },
    { "Nodes", "list", 0, &resource_nodes_list, 0, 0 },
};

const ActionParam change_trafficability_params[] =
{
    { "Trafficability", "numeric", 0, 0, 0, 0 },
};

const ActionParam create_breakdowns_params[] =
{
    { "Breakdowns", "list", 0, 0, 0, 0 }, // Deprecated
};

const ActionParam create_knowledge_group_params[] =
{
    { "Camp", "identifier", 0, 0, 0, 0 },
    { "Parent", "identifier", 0, 0, 0, 0 },
    { "Type", "string", 0, 0, 0, 0 },
};

const ActionParam create_object_params[] =
{
    { "Type", "string", 0, 0, 0, 0 },
    { "Location", "location", 0, 0, 0, 0 },
    { "Name", "string", 0, 0, 0, 0 },
    { "Camp", "army", 0, 0, 0, 0 },
    { "Attributes", "list", 0, &object_create_attributes_list, 0, 0 },
};

const ActionParam create_wounds_params[] =
{
    { "Wounds", "list", 0, 0, 0, 0 }, // Deprecated
};

const ActionParam affinities_structure_params[] =
{
    { "Id", "identifier", 0, 0, 0, 0 },
    { "Value", "numeric", 0, 0, 0, 0 },
};
LIST_OF_STRUCTURES( affinities, "Affinity" )

const ActionParam crowd_change_affinities_params[] =
{
    { "Affinities", "list", 0, &affinities_list, 0, 0 },
};

const ActionParam crowd_change_armed_individuals_params[] =
{
    { "ArmedIndividuals", "quantity", 0, 0, 0, 0 },
};

const ActionParam crowd_change_attitude_params[] =
{
    { "Attitude", "enumeration", &attitude_enum, 0, 0, 0 },
};

const ActionParam crowd_change_health_state_params[] =
{
    { "Healthy", "quantity", 0, 0, 0, 0 },
    { "Wounded", "quantity", 0, 0, 0, 0 },
    { "Contaminated", "quantity", 0, 0, 0, 0 },
    { "Dead", "quantity", 0, 0, 0, 0 },
};

const ActionParam crowd_creation_params[] =
{
    { "PopulationType", "string", 0, 0, 0, 0 },
    { "Location", "point", 0, 0, 0, 0 },
    { "Number healthy", "quantity", 0, 0, 0, 0 },
    { "Number wounded", "quantity", 0, 0, 0, 0 },
    { "Number dead", "quantity", 0, 0, 0, 0 },
    { "Name", "string", 0, 0, 0, 0 },
};

const ActionParam fire_order_params[] =
{
    { "UnitTarget", "identifier", 0, 0, 0, 0 },
    { "Ammo", "resourcetype", 0, 0, 0, 0 },
    { "Iterations", "numeric", 0, 0, 0, 0 },
};

const ActionParam fire_order_on_location_params[] =
{
    { "LocationTarget", "location", 0, 0, 0, 0 },
    { "Ammo", "resourcetype", 0, 0, 0, 0 },
    { "Iterations", "numeric", 0, 0, 0, 0 },
};

const ActionParam formation_creation_params[] =
{
    { "Level", "numeric", 0, 0, 0, 0 },
    { "Name", "string", 0, 0, 0, 0 },
    { "LogLevel", "string", 0, 0, 0, 0 },
};

const ActionParam global_weather_params[] =
{
    { "Temperature", "numeric", 0, 0, 0, 0 },
    { "WindSpeed", "numeric", 0, 0, 0, 0 },
    { "WindDirection", "heading", 0, 0, 0, 0 },
    { "CloudFloor", "numeric", 0, 0, 0, 0 },
    { "CloudCeiling", "numeric", 0, 0, 0, 0 },
    { "CloudDensity", "numeric", 0, 0, 0, 0 },
    { "Precipitation", "enumeration", &weather_enum, 0, 0, 0 },
};

const ActionParam inhabitant_change_affinities_params[] =
{
    { "Affinities", "list", 0, &affinities_list, 0, 0 },
};

const ActionParam inhabitant_change_alerted_state_params[] =
{
    { "Alerted", "boolean", 0, 0, 0, 0 },
};

const ActionParam inhabitant_change_confined_state_params[] =
{
    { "Confined", "boolean", 0, 0, 0, 0 },
};

const ActionParam inhabitant_change_health_state_params[] =
{
    { "Healthy", "quantity", 0, 0, 0, 0 },
    { "Wounded", "quantity", 0, 0, 0, 0 },
    { "Dead", "quantity", 0, 0, 0, 0 },
};

const ActionParam knowledge_group_add_knowledge_params[] =
{
    { "Entity", "identifier", 0, 0, 0, 0 },
    { "Perception", "enumeration", &identification_enum, 0, 0, 0 },
};

const ActionParam knowledge_group_enable_params[] =
{
    { "Enabled", "boolean", 0, 0, 0, 0 },
};

const ActionParam knowledge_group_update_side_params[] =
{
    { "Camp", "army", 0, 0, 0, 0 },
};

const ActionParam knowledge_group_update_side_parent_params[] =
{
    { "Camp", "army", 0, 0, 0, 0 },
    { "Parent", "knowledgegroup", 0, 0, 0, 0 },
};

const ActionParam knowledge_group_update_type_params[] =
{
    { "Type", "string", 0, 0, 0, 0 },
};

const ActionParam local_weather_params[] =
{
    { "Temperature", "numeric", 0, 0, 0, 0 },
    { "WindSpeed", "numeric", 0, 0, 0, 0 },
    { "WindDirection", "heading", 0, 0, 0, 0 },
    { "CloudFloor", "numeric", 0, 0, 0, 0 },
    { "CloudCeiling", "numeric", 0, 0, 0, 0 },
    { "CloudDensity", "numeric", 0, 0, 0, 0 },
    { "Precipitation", "enumeration", &weather_enum, 0, 0, 0 },
    { "StartTime", "datetime", 0, 0, 0, 0 },
    { "EndTime", "datetime", 0, 0, 0, 0 },
    { "Location", "location", 0, 0, 0, 0 },
    { "ID", "identifier", 0, 0, 0, 0 },
};

const ActionParam local_weather_destruction_params[] =
{
    { "WeatherID", "identifier", 0, 0, 0, 0 },
};

PARAMS_PAIR_ID_QUANTITY( quota_dotations_structure, "Type", "Number" )
LIST_OF_STRUCTURES( quota_dotations, "Dotation" )

const ActionParam log_supply_change_quotas_params[] =
{
    { "Receiver", "identifier", 0, 0, 0, 0 },
    { "Dotations", "list", 0, &quota_dotations_list, 0, 0 },
};

const ActionParam log_supply_pull_flow_params[] =
{
    { "Parameters", "pullflowparameters", 0, 0, 0, 0 },
};

const ActionParam log_supply_push_flow_params[] =
{
    { "Parameters", "pushflowparameters", 0, 0, 0, 0 },
};

const ActionParam log_supply_set_manual_params[] =
{
    { "Manual", "boolean", 0, 0, 0, 0 },
};

const ActionParam log_maintenance_set_manual_params[] =
{
    { "Manual", "boolean", 0, 0, 0, 0 },
};

const ActionParam select_repair_team_params[] =
{
    { "Request", "identifier", 0, 0, 0, 0 },
    { "EquipmentType", "identifier", 0, 0, 0, 0 },
};

const ActionParam select_maintenance_transporter_params[] =
{
    { "Request", "identifier", 0, 0, 0, 0 },
    { "EquipmentType", "identifier", 0, 0, 0, 0 },
    { "Destination", "agent", 0 },
};

const ActionParam select_diagnosis_team_params[] =
{
    { "Request", "identifier", 0, 0, 0, 0 },
    { "EquipmentType", "identifier", 0, 0, 0, 0 },
};

const ActionParam select_new_logistic_state_params[] =
{
    { "Request", "identifier", 0, 0, 0, 0 },
};

const ActionParam transfer_to_logistic_superior_params[] =
{
    { "Request", "identifier", 0, 0, 0, 0 },
};

PARAMS_PAIR_ID_QUANTITY( partial_recovery_equipments_structure, "Equipment", "Quantity" )
LIST_OF_STRUCTURES( partial_recovery_equipments, "Equipment" )
PARAMS_PAIR_ID_QUANTITY( partial_recovery_humans_structure, "Rank", "Quantity" )
LIST_OF_STRUCTURES( partial_recovery_humans, "Human" )
PARAMS_PAIR_ID_QUANTITY( partial_recovery_dotations_structure, "Dotation", "Quantity" )
LIST_OF_STRUCTURES( partial_recovery_dotations, "Dotation" )
PARAMS_PAIR_ID_QUANTITY( partial_recovery_ammos_structure, "Ammo", "Quantity" )
LIST_OF_STRUCTURES( partial_recovery_ammos, "Ammo" )
PARAMS_PAIR_ID_QUANTITY( partial_recovery_stocks_structure, "Stock", "Quantity" )
LIST_OF_STRUCTURES( partial_recovery_stocks, "Stock" )

const ActionParam partial_recovery_params[] =
{
    { "Equipments", "list", 0, &partial_recovery_equipments_list, 0, 0 },
    { "Humans", "list", 0, &partial_recovery_humans_list, 0, 0 },
    { "Dotations", "list", 0, &partial_recovery_dotations_list, 0, 0 },
    { "Ammo", "list", 0, &partial_recovery_ammos_list, 0, 0 },
    { "Stocks", "list", 0, &partial_recovery_stocks_list, 0, 0 },
};

const ActionParam reload_brain_params[] =
{
    { "Model", "string", 0, 0, 0, 0 },
};

const ActionParam surrender_params[] =
{
    { "Camp", "army", 0, 0, 0, 0 },
};

const ActionParam teleport_params[] =
{
    { "Location", "point", 0, 0, 0, 0 },
};

PARAMS_PAIR_ID_QUANTITY( transfert_equipments_structure, "Equipment", "Quantity" )
LIST_OF_STRUCTURES( transfert_equipments, "Dotation" )

const ActionParam transfer_equipment_params[] =
{
    { "Receiver", "identifier", 0, 0, 0, 0 },
    { "Equipments", "list", 0, &transfert_equipments_list, 0, 0 },
};

const ActionParam unit_change_affinities_params[] =
{
    { "Affinities", "list", 0, &affinities_list, 0, 0 },
};

const ActionParam unit_change_superior_params[] =
{
    { "Superior", "automat", 0, 0, 0, 0 },
};

const ActionParam unit_creation_params[] =
{
    { "UnitType", "identifier", 0, 0, 0, 0 },
    { "Location", "point", 0, 0, 0, 0 },
};

const ActionParam update_object_params[] =
{
    { "Attributes", "list", 0, &object_update_attributes_list, 0, 0 },
};

const ActionParam update_urban_params[] =
{
    { "Target", "identifier", 0, 0, 0, 0 },
    { "StructuralState", "quantity", 0, 0, 0, 0 },
};

}  // namespace

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
