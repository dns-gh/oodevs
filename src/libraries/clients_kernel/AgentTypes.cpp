// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "AgentTypes.h"

#include "protocol/Protocol.h"
#include "AgentType.h"
#include "AutomatType.h"
#include "ComponentType.h"
#include "DecisionalModel.h"
#include "DotationType.h"
#include "FragOrderType.h"
#include "KnowledgeGroupType.h"
#include "MagicActionType.h"
#include "MissionFactory.h"
#include "MissionType.h"
#include "PopulationType.h"
#include "InhabitantType.h"
#include "SensorType.h"
#include "SymbolFactory.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/ExerciseConfig.h"
#include "tools/Loader_ABC.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentTypes constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentTypes::AgentTypes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentTypes constructor
// Created: AGE 2008-03-14
// -----------------------------------------------------------------------------
AgentTypes::AgentTypes( const tools::ExerciseConfig& config )
{
    Load( config );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::Load
// Created: JSR 2010-11-25
// -----------------------------------------------------------------------------
void AgentTypes::Load( const tools::ExerciseConfig& config )
{
    Purge();
    symbolFactory_ = new SymbolFactory();
    symbolFactory_->Load( config );
    const tools::Loader_ABC& loader = config.GetLoader();
    loader.LoadPhysicalFile( "components", boost::bind( &AgentTypes::ReadComponents, this, _1 ) );
    loader.LoadPhysicalFile( "missions", boost::bind( &AgentTypes::ReadOrderTypes, this, _1 ) );
    //loader.LoadPhysicalFile( "magic-orders", boost::bind( &AgentTypes::ReadMagicOrderTypes, this, _1 ) );
    loader.LoadPhysicalFile( "models", boost::bind( &AgentTypes::ReadModels, this, _1 ) );
    loader.LoadPhysicalFile( "sensors", boost::bind( &AgentTypes::ReadSensors, this, _1 ) );
    loader.LoadPhysicalFile( "units", boost::bind( &AgentTypes::ReadAgents, this, _1 ) );
    loader.LoadPhysicalFile( "automats", boost::bind( &AgentTypes::ReadAutomats, this, _1 ) );
    loader.LoadPhysicalFile( "populations", boost::bind( &AgentTypes::ReadPopulations, this, _1 ) );
    loader.LoadPhysicalFile( "inhabitants", boost::bind( &AgentTypes::ReadInhabitants, this, _1 ) );
    loader.LoadPhysicalFile( "knowledge-groups", boost::bind( &AgentTypes::ReadKnowledgeGroups, this, _1 ) );;
    CreateMagicActionTypes();
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::Purge
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void AgentTypes::Purge()
{
    unitModels_.DeleteAll();
    automatModels_.DeleteAll();
    populationModels_.DeleteAll();
    unitMissions_.DeleteAll();
    automatMissions_.DeleteAll();
    populationMissions_.DeleteAll();
    tools::Resolver< MissionType >::Clear();
    tools::Resolver< FragOrderType >::DeleteAll();
    tools::Resolver< FragOrderType, std::string >::Clear();
    tools::Resolver< KnowledgeGroupType, std::string >::DeleteAll();
    tools::Resolver< PopulationType >::DeleteAll();
    tools::Resolver< PopulationType, std::string >::Clear();
    tools::Resolver< InhabitantType >::DeleteAll();
    tools::Resolver< InhabitantType, std::string >::Clear();
    tools::Resolver< AgentType >::DeleteAll();
    tools::Resolver< AgentType, std::string >::Clear();
    tools::Resolver< AutomatType >::DeleteAll();
    tools::Resolver< AutomatType, std::string >::Clear();
    tools::Resolver< ComponentType >::DeleteAll();
    tools::Resolver< ComponentType, std::string >::Clear();
    tools::Resolver< SensorType, std::string >::DeleteAll();
    tools::Resolver< MagicActionType, std::string >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: AgentTypes destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentTypes::~AgentTypes()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadComponents
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadComponents( xml::xistream& xis )
{
    xis >> xml::start( "equipments" )
            >> xml::list( "equipment", *this, &AgentTypes::ReadComponent );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadComponent
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadComponent( xml::xistream& xis )
{
    ComponentType* component = new ComponentType( xis );
    tools::Resolver< ComponentType, std::string >::Register( component->GetName(), *component );
    tools::Resolver< ComponentType >::Register( component->GetId(), *component );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadOrderTypes
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void AgentTypes::ReadOrderTypes( xml::xistream& xis )
{
    xis >> xml::start( "missions" );
    ReadMissions( xis, "units"      , unitMissions_ );
    ReadMissions( xis, "automats"   , automatMissions_ );
    ReadMissions( xis, "populations", populationMissions_ );
    xis     >> xml::start( "fragorders" )
                >> xml::list( "fragorder", *this, &AgentTypes::ReadFragOrderType )
            >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadMissions
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
void AgentTypes::ReadMissions( xml::xistream& xis, const std::string& name, T_MissionResolver& missions )
{
    xis >> xml::start( name );
    xis     >> xml::list( "mission", *this, &AgentTypes::ReadMissionType, missions )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadMissionType
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void AgentTypes::ReadMissionType( xml::xistream& xis, T_MissionResolver& missions )
{
    MissionType* mission = new MissionType( xis );
    tools::Resolver< MissionType >::Register( mission->GetId(), *mission );
    missions.Register( mission->GetName(), *mission );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadFragOrderType
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void AgentTypes::ReadFragOrderType( xml::xistream& xis )
{
    FragOrderType* order = new FragOrderType( xis );
    tools::Resolver< FragOrderType >::Register( order->GetId(), *order );
    tools::Resolver< FragOrderType, std::string >::Register( order->GetName(), *order );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadModels
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadModels( xml::xistream& xis )
{
    T_Resolver unitResolver       = &MissionFactory::CreateAgentMission;
    T_Resolver automatResolver    = &MissionFactory::CreateAutomatMission;
    T_Resolver populationResolver = &MissionFactory::CreatePopulationMission;

    xis >> xml::start( "models" )
            >> xml::start( "units" )
                >> xml::list( "unit", *this, &AgentTypes::ReadModel, unitResolver, unitModels_ )
            >> xml::end
            >> xml::start( "automats" )
                >> xml::list( "automat", *this, &AgentTypes::ReadModel, automatResolver, automatModels_ )
            >> xml::end
            >> xml::start( "crowd" )
                >> xml::list( "crowd", *this, &AgentTypes::ReadModel, populationResolver, populationModels_ )
            >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadModel
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadModel( xml::xistream& xis, const T_Resolver& missionResolver, tools::Resolver< DecisionalModel, std::string >& models )
{
    MissionFactory factory( unitMissions_, automatMissions_, populationMissions_, *this );
    DecisionalModel* model = new DecisionalModel( xis, factory, missionResolver, (Resolver< FragOrderType >&)*this );
    models.Register( model->GetName(), *model );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadSensors
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadSensors( xml::xistream& xis )
{
    xis >> xml::start( "sensors" )
            >> xml::start( "sensors" )
                >> xml::list( "sensor", *this, &AgentTypes::ReadSensor );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadSensor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadSensor( xml::xistream& xis )
{
    std::string name;
    xis >> xml::attribute( "name", name )
        >> xml::list( "unit-detection", *this, &AgentTypes::ReallyReadSensor, name );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReallyReadSensor
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
void AgentTypes::ReallyReadSensor( xml::xistream& xis, const std::string& sensor )
{
    SensorType* type = new SensorType( sensor, xis );
    tools::Resolver< SensorType, std::string >::Register( type->GetName(), *type );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadAgents
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadAgents( xml::xistream& xis )
{
    xis >> xml::start( "units" )
            >> xml::list( "unit", *this, &AgentTypes::ReadAgentType );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadAgentType
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadAgentType( xml::xistream& xis )
{
    try
    {
        AgentType* type = new AgentType( xis, *this, unitModels_, *symbolFactory_ );
        tools::Resolver< AgentType >         ::Register( type->GetId(), *type );
        tools::Resolver< AgentType, std::string >::Register( type->GetName(), *type );
    }
    catch( std::runtime_error& e )
    {
        MT_LOG_ERROR_MSG( "exception caught: " << e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadAutomats
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadAutomats( xml::xistream& xis )
{
    xis >> xml::start( "automats" )
            >> xml::list( "automat", *this, &AgentTypes::ReadAutomatType );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadAutomatType
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadAutomatType( xml::xistream& xis )
{
    try
    {
        AutomatType* type = new AutomatType( xis, *this, automatModels_ );
        tools::Resolver< AutomatType >         ::Register( type->GetId(), *type );
        tools::Resolver< AutomatType, std::string >::Register( type->GetName(), *type );
    }
    catch( std::runtime_error& e )
    {
        MT_LOG_ERROR_MSG( e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadPopulations
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
void AgentTypes::ReadPopulations( xml::xistream& xis )
{
    xis >> xml::start( "populations" )
            >> xml::list( "population", *this, &AgentTypes::ReadPopulationType );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadPopulationType
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
void AgentTypes::ReadPopulationType( xml::xistream& xis )
{
    PopulationType* type = new PopulationType( xis, populationModels_ );
    tools::Resolver< PopulationType >::Register( type->GetId(), *type );
    tools::Resolver< PopulationType, std::string >::Register( type->GetName(), *type );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadInhabitants
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
void AgentTypes::ReadInhabitants( xml::xistream& xis )
{
    xis >> xml::start( "populations" )
        >> xml::list( "population", *this, &AgentTypes::ReadInhabitantType );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadInhabitantType
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
void AgentTypes::ReadInhabitantType( xml::xistream& xis )
{
    InhabitantType* type = new InhabitantType( xis, *this );
    tools::Resolver< InhabitantType >::Register( type->GetId(), *type );
    tools::Resolver< InhabitantType, std::string >::Register( type->GetName(), *type );
}
// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadKnowledgeGroups
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
void AgentTypes::ReadKnowledgeGroups( xml::xistream& xis )
{
    xis >> xml::start( "knowledge-groups" )
            >> xml::list( "knowledge-group", *this, &AgentTypes::ReadKnowledgeGroupType );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadKnowledgeGroupType
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
void AgentTypes::ReadKnowledgeGroupType( xml::xistream& xis )
{
    KnowledgeGroupType* type = new KnowledgeGroupType( xis );
    tools::Resolver< KnowledgeGroupType, std::string >::Register( type->GetName(), *type );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::RegisterActionType
// Created: JSR 2010-04-13
// -----------------------------------------------------------------------------
void AgentTypes::RegisterActionType( MagicActionType& actionType )
{
    tools::Resolver< MagicActionType, std::string >::Register( actionType.GetName(), actionType );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::CreateMagicActionTypes
// Created: JSR 2010-04-08
// -----------------------------------------------------------------------------
void AgentTypes::CreateMagicActionTypes()
{
    // Unit Magic Actions
    RegisterActionType( *new MagicActionType( "teleport", sword::UnitMagicAction::move_to ) );
    RegisterActionType( *new MagicActionType( "surrender", sword::UnitMagicAction::surrender_to ) );
    RegisterActionType( *new MagicActionType( "cancel_surrender", sword::UnitMagicAction::cancel_surrender ) );
    RegisterActionType( *new MagicActionType( "recover_transporters", sword::UnitMagicAction::recover_transporters) );
    RegisterActionType( *new MagicActionType( "destroy_component", sword::UnitMagicAction::destroy_component ) );
    RegisterActionType( *new MagicActionType( "recover_all", sword::UnitMagicAction::recover_all ) );
    RegisterActionType( *new MagicActionType( "recover_troops", sword::UnitMagicAction::recover_troops ) );
    RegisterActionType( *new MagicActionType( "recover_equipments", sword::UnitMagicAction::recover_equipments ) );
    RegisterActionType( *new MagicActionType( "recover_resources", sword::UnitMagicAction::recover_resources ) );
    RegisterActionType( *new MagicActionType( "destroy_all", sword::UnitMagicAction::destroy_all ) );
    RegisterActionType( *new MagicActionType( "change_human_factors", sword::UnitMagicAction::change_human_factors ) );
    RegisterActionType( *new MagicActionType( "partial_recovery", sword::UnitMagicAction::partial_recovery ) );
    RegisterActionType( *new MagicActionType( "automat_creation", sword::UnitMagicAction::automat_creation ) );
    RegisterActionType( *new MagicActionType( "crowd_creation", sword::UnitMagicAction::crowd_creation ) );
    RegisterActionType( *new MagicActionType( "formation_creation", sword::UnitMagicAction::formation_creation ) );
    RegisterActionType( *new MagicActionType( "unit_creation", sword::UnitMagicAction::unit_creation ) );
    RegisterActionType( *new MagicActionType( "fire_order", sword::UnitMagicAction::create_fire_order ) );
    RegisterActionType( *new MagicActionType( "change_knowledge_group", sword::UnitMagicAction::change_knowledge_group ) );
    RegisterActionType( *new MagicActionType( "unit_change_superior", sword::UnitMagicAction::unit_change_superior ) );
    RegisterActionType( *new MagicActionType( "change_automat_superior", sword::UnitMagicAction::change_automat_superior ) );
    RegisterActionType( *new MagicActionType( "change_formation_superior", sword::UnitMagicAction::change_formation_superior ) );
    RegisterActionType( *new MagicActionType( "change_logistic_links", sword::UnitMagicAction::change_logistic_links ) );
    RegisterActionType( *new MagicActionType( "automat_log_supply_push_flow", sword::UnitMagicAction::log_supply_push_flow ) );
    RegisterActionType( *new MagicActionType( "automat_log_supply_pull_flow", sword::UnitMagicAction::log_supply_pull_flow ) );
    RegisterActionType( *new MagicActionType( "automat_log_supply_change_quotas", sword::UnitMagicAction::log_supply_change_quotas ) );
    RegisterActionType( *new MagicActionType( "formation_log_supply_push_flow", sword::UnitMagicAction::log_supply_push_flow ) );
    RegisterActionType( *new MagicActionType( "formation_log_supply_pull_flow", sword::UnitMagicAction::log_supply_pull_flow ) );
    RegisterActionType( *new MagicActionType( "formation_log_supply_change_quotas", sword::UnitMagicAction::log_supply_change_quotas ) );
    RegisterActionType( *new MagicActionType( "crowd_total_destruction", sword::UnitMagicAction::crowd_total_destruction ) );
    RegisterActionType( *new MagicActionType( "crowd_change_attitude", sword::UnitMagicAction::crowd_change_attitude ) );
    RegisterActionType( *new MagicActionType( "crowd_change_health_state", sword::UnitMagicAction::crowd_change_health_state ) );
    RegisterActionType( *new MagicActionType( "crowd_change_affinities", sword::UnitMagicAction::crowd_change_affinities ) );
    RegisterActionType( *new MagicActionType( "crowd_change_armed_individuals", sword::UnitMagicAction::crowd_change_armed_individuals ) );
    RegisterActionType( *new MagicActionType( "inhabitant_change_health_state", sword::UnitMagicAction::inhabitant_change_health_state ) );
    RegisterActionType( *new MagicActionType( "inhabitant_change_affinities", sword::UnitMagicAction::inhabitant_change_affinities ) );
    RegisterActionType( *new MagicActionType( "inhabitant_change_alerted_state", sword::UnitMagicAction::inhabitant_change_alerted_state ) );
    RegisterActionType( *new MagicActionType( "inhabitant_change_confined_state", sword::UnitMagicAction::inhabitant_change_confined_state ) );
    RegisterActionType( *new MagicActionType( "unit_change_affinities", sword::UnitMagicAction::unit_change_affinities ) );
    RegisterActionType( *new MagicActionType( "change_extension", sword::UnitMagicAction::change_extension ) );
    RegisterActionType( *new MagicActionType( "change_critical_intelligence", sword::UnitMagicAction::change_critical_intelligence ) );
    RegisterActionType( *new MagicActionType( "transfer_equipment", sword::UnitMagicAction::transfer_equipment ) );
    RegisterActionType( *new MagicActionType( "reload_brain", sword::UnitMagicAction::reload_brain ) );
    RegisterActionType( *new MagicActionType( "change_equipment_state", sword::UnitMagicAction::change_equipment_state ) );
    RegisterActionType( *new MagicActionType( "change_human_state", sword::UnitMagicAction::change_human_state ) );
    RegisterActionType( *new MagicActionType( "change_dotation", sword::UnitMagicAction::change_dotation ) );

    // Knowledge Magic Actions
    RegisterActionType( *new MagicActionType( "knowledge_group_enable", sword::KnowledgeMagicAction::enable ) );
    RegisterActionType( *new MagicActionType( "knowledge_group_update_side", sword::KnowledgeMagicAction::update_party ) );
    RegisterActionType( *new MagicActionType( "knowledge_group_update_side_parent", sword::KnowledgeMagicAction::update_party_parent ) );
    RegisterActionType( *new MagicActionType( "knowledge_group_update_type", sword::KnowledgeMagicAction::update_type ) );
    RegisterActionType( *new MagicActionType( "knowledge_group_add_knowledge", sword::KnowledgeMagicAction::add_knowledge ) );

    // Object Magic Actions
    RegisterActionType( *new MagicActionType( "create_object", sword::ObjectMagicAction::create ) );
    RegisterActionType( *new MagicActionType( "update_object", sword::ObjectMagicAction::update ) );
    RegisterActionType( *new MagicActionType( "destroy_object", sword::ObjectMagicAction::destroy ) );

    // Other Magic Actions
    RegisterActionType( *new MagicActionType( "global_weather", sword::MagicAction::global_weather ) );
    RegisterActionType( *new MagicActionType( "local_weather", sword::MagicAction::local_weather ) );
    RegisterActionType( *new MagicActionType( "change_diplomacy", sword::MagicAction::change_diplomacy ) );
    RegisterActionType( *new MagicActionType( "create_knowledge_group", sword::MagicAction::create_knowledge_group ) );
    RegisterActionType( *new MagicActionType( "change_resource_links", sword::MagicAction::change_resource_network_properties ) );
    RegisterActionType( *new MagicActionType( "fire_order_on_location", sword::MagicAction::create_fire_order_on_location ) );

    RegisterActionType( *new MagicActionType( "change_mode" ) );

}
