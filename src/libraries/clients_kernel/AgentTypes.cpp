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

#include "protocol/protocol.h"
#include "AgentType.h"
#include "AutomatType.h"
#include "ComponentType.h"
#include "DecisionalModel.h"
#include "DotationType.h"
#include "FileLoader.h"
#include "FragOrderType.h"
#include "KnowledgeGroupType.h"
#include "MagicActionType.h"
#include "MissionFactory.h"
#include "MissionType.h"
#include "OrderContext.h"
#include "PopulationType.h"
#include "SensorType.h"
#include "SymbolFactory.h"
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
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void AgentTypes::Load( const tools::ExerciseConfig& config )
{
    Purge();
    symbolFactory_ = new SymbolFactory();
    FileLoader( config )
        .Load( "components", boost::bind( &AgentTypes::ReadComponents, this, _1 ) )
        .Load( "missions", boost::bind( &AgentTypes::ReadOrderTypes, this, _1 ) )
        //.Load( "magic-orders", boost::bind( &AgentTypes::ReadMagicOrderTypes, this, _1 ) )
        .Load( "models", boost::bind( &AgentTypes::ReadModels, this, _1 ) )
        .Load( "sensors", boost::bind( &AgentTypes::ReadSensors, this, _1 ) )
        .Load( "units", boost::bind( &AgentTypes::ReadAgents, this, _1 ) )
        .Load( "automats", boost::bind( &AgentTypes::ReadAutomats, this, _1 ) )
        .Load( "populations", boost::bind( &AgentTypes::ReadPopulations, this, _1 ) )
        .Load( "knowledge-groups", boost::bind( &AgentTypes::ReadKnowledgeGroups, this, _1 ) );

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
    xis >> xml::start( "elements" )
            >> xml::list( "element", *this, &AgentTypes::ReadComponent );
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
    OrderContext context( xis );
    xis     >> xml::list( "mission", *this, &AgentTypes::ReadMissionType, missions, context )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadMissionType
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void AgentTypes::ReadMissionType( xml::xistream& xis, T_MissionResolver& missions, const OrderContext& context )
{
    MissionType* mission = new MissionType( xis, context );
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
            >> xml::start( "populations" )
                >> xml::list( "population", *this, &AgentTypes::ReadModel, populationResolver, populationModels_ )
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
    AgentType* type = new AgentType( xis, *this, unitModels_, *symbolFactory_ );
    tools::Resolver< AgentType >         ::Register( type->GetId(), *type );
    tools::Resolver< AgentType, std::string >::Register( type->GetName(), *type );
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
    AutomatType* type = new AutomatType( xis, *this, automatModels_ );
    tools::Resolver< AutomatType >         ::Register( type->GetId(), *type );
    tools::Resolver< AutomatType, std::string >::Register( type->GetName(), *type );
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
    RegisterActionType( *new MagicActionType( "teleport", MsgsClientToSim::MsgUnitMagicAction::move_to ) );
    RegisterActionType( *new MagicActionType( "surrender", MsgsClientToSim::MsgUnitMagicAction::surrender_to ) );
    RegisterActionType( *new MagicActionType( "cancel_surrender", MsgsClientToSim::MsgUnitMagicAction::cancel_surrender ) );
    RegisterActionType( *new MagicActionType( "recover_transporters", MsgsClientToSim::MsgUnitMagicAction::recover_transporters) );
    RegisterActionType( *new MagicActionType( "destroy_component", MsgsClientToSim::MsgUnitMagicAction::destroy_component ) );
    RegisterActionType( *new MagicActionType( "recover_all", MsgsClientToSim::MsgUnitMagicAction::recover_all ) );
    RegisterActionType( *new MagicActionType( "recover_troops", MsgsClientToSim::MsgUnitMagicAction::recover_troops ) );
    RegisterActionType( *new MagicActionType( "recover_equipments", MsgsClientToSim::MsgUnitMagicAction::recover_equipments ) );
    RegisterActionType( *new MagicActionType( "recover_resources", MsgsClientToSim::MsgUnitMagicAction::recover_resources ) );
    RegisterActionType( *new MagicActionType( "destroy_all", MsgsClientToSim::MsgUnitMagicAction::destroy_all ) );
    RegisterActionType( *new MagicActionType( "change_human_factors", MsgsClientToSim::MsgUnitMagicAction::change_human_factors ) );
    RegisterActionType( *new MagicActionType( "partial_recovery", MsgsClientToSim::MsgUnitMagicAction::partial_recovery ) );
    RegisterActionType( *new MagicActionType( "unit_creation", MsgsClientToSim::MsgUnitMagicAction::unit_creation ) );
    RegisterActionType( *new MagicActionType( "automat_creation", MsgsClientToSim::MsgUnitMagicAction::automat_creation ) );
    RegisterActionType( *new MagicActionType( "fire_order", MsgsClientToSim::MsgUnitMagicAction::create_fire_order ) );
    RegisterActionType( *new MagicActionType( "population_total_destruction", MsgsClientToSim::MsgUnitMagicAction::crowd_total_destruction ) );
    RegisterActionType( *new MagicActionType( "population_kill", MsgsClientToSim::MsgUnitMagicAction::crowd_kill ) );
    RegisterActionType( *new MagicActionType( "population_resurrect", MsgsClientToSim::MsgUnitMagicAction::crowd_resurrect ) );
    RegisterActionType( *new MagicActionType( "population_change_attitude", MsgsClientToSim::MsgUnitMagicAction::crowd_change_attitude ) );
    RegisterActionType( *new MagicActionType( "change_knowledge_group", MsgsClientToSim::MsgUnitMagicAction::change_knowledge_group ) );
    RegisterActionType( *new MagicActionType( "unit_change_superior", MsgsClientToSim::MsgUnitMagicAction::unit_change_superior ) );
    RegisterActionType( *new MagicActionType( "change_automat_superior", MsgsClientToSim::MsgUnitMagicAction::change_automat_superior ) );
    RegisterActionType( *new MagicActionType( "change_formation_superior", MsgsClientToSim::MsgUnitMagicAction::change_formation_superior ) );
    RegisterActionType( *new MagicActionType( "change_logistic_links", MsgsClientToSim::MsgUnitMagicAction::change_logistic_links ) );
    RegisterActionType( *new MagicActionType( "log_supply_push_flow", MsgsClientToSim::MsgUnitMagicAction::log_supply_push_flow ) );
    RegisterActionType( *new MagicActionType( "log_supply_change_quotas", MsgsClientToSim::MsgUnitMagicAction::log_supply_change_quotas ) );

    // Knowledge Magic Actions
    RegisterActionType( *new MagicActionType( "knowledge_group_enable", MsgsClientToSim::MsgKnowledgeMagicAction::enable ) );
    RegisterActionType( *new MagicActionType( "knowledge_group_update_side", MsgsClientToSim::MsgKnowledgeMagicAction::update_party ) );
    RegisterActionType( *new MagicActionType( "knowledge_group_update_side_parent", MsgsClientToSim::MsgKnowledgeMagicAction::update_party_parent ) );
    RegisterActionType( *new MagicActionType( "knowledge_group_update_type", MsgsClientToSim::MsgKnowledgeMagicAction::update_type ) );

    // Object Magic Actions
    RegisterActionType( *new MagicActionType( "create_object", MsgsClientToSim::MsgObjectMagicAction::create ) );
    RegisterActionType( *new MagicActionType( "update_object", MsgsClientToSim::MsgObjectMagicAction::update ) );
    RegisterActionType( *new MagicActionType( "destroy_object", MsgsClientToSim::MsgObjectMagicAction::destroy ) );

    // Other Magic Actions
    RegisterActionType( *new MagicActionType( "global_weather", MsgsClientToSim::MsgMagicAction::global_weather ) );
    RegisterActionType( *new MagicActionType( "local_weather", MsgsClientToSim::MsgMagicAction::local_weather ) );
    RegisterActionType( *new MagicActionType( "change_diplomacy", MsgsClientToSim::MsgMagicAction::change_diplomacy ) );
    RegisterActionType( *new MagicActionType( "create_knowledge_group", MsgsClientToSim::MsgMagicAction::create_knowledge_group ) );
    RegisterActionType( *new MagicActionType( "change_resource_links", MsgsClientToSim::MsgMagicAction::change_resource_network_properties ) );
}
