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

#include "AgentType.h"
#include "AutomatType.h"
#include "ComponentType.h"
#include "DecisionalModel.h"
#include "DotationType.h"
#include "FileLoader.h"
#include "FragOrderType.h"
#include "KnowledgeGroupType.h"
#include "MissionFactory.h"
#include "MissionType.h"
#include "OrderContext.h"
#include "PopulationType.h"
#include "SensorType.h"
#include "SymbolFactory.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.h>

using namespace kernel;
using namespace xml;

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
    xis >> start( "elements" )
            >> list( "element", *this, &AgentTypes::ReadComponent );
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
    xis >> start( "missions" );
    ReadMissions( xis, "units"      , unitMissions_ );
    ReadMissions( xis, "automats"   , automatMissions_ );
    ReadMissions( xis, "populations", populationMissions_ );
    xis     >> start( "fragorders" )
                >> list( "fragorder", *this, &AgentTypes::ReadFragOrderType )
            >> end();
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadMissions
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
void AgentTypes::ReadMissions( xml::xistream& xis, const std::string& name, T_MissionResolver& missions )
{
    xis >> start( name );
    OrderContext context( xis );
    xis     >> list( "mission", *this, &AgentTypes::ReadMissionType, missions, context )
        >> end();
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

    xis >> start( "models" )
            >> start( "units" )
                >> list( "unit", *this, &AgentTypes::ReadModel, unitResolver, unitModels_ )
            >> end()
            >> start( "automats" )
                >> list( "automat", *this, &AgentTypes::ReadModel, automatResolver, automatModels_ )
            >> end()
            >> start( "populations" )
                >> list( "population", *this, &AgentTypes::ReadModel, populationResolver, populationModels_ )
            >> end();
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
    xis >> start( "sensors" )
            >> start( "sensors" )
                >> list( "sensor", *this, &AgentTypes::ReadSensor );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadSensor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadSensor( xml::xistream& xis )
{
    std::string name;
    xis >> attribute( "name", name )
        >> list( "unit-detection", *this, &AgentTypes::ReallyReadSensor, name );
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
    xis >> start( "units" )
            >> list( "unit", *this, &AgentTypes::ReadAgentType );
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
    xis >> start( "automats" )
            >> list( "automat", *this, &AgentTypes::ReadAutomatType );
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
    xis >> start( "populations" )
            >> list( "population", *this, &AgentTypes::ReadPopulationType );
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
    xis >> start( "knowledge-groups" )
            >> list( "knowledge-group", *this, &AgentTypes::ReadKnowledgeGroupType );
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
