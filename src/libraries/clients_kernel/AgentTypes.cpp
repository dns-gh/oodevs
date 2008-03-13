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
#include "ComponentType.h"
#include "DecisionalModel.h"
#include "SensorType.h"
#include "AutomatType.h"
#include "PopulationType.h"
#include "DotationType.h"
#include "MissionFactory.h"
#include "SymbolFactory.h"
#include "KnowledgeGroupType.h"
#include "MissionType.h"
#include "FragOrderType.h"
#include "OrderContext.h"
#include "tools/ExerciseConfig.h"
#include "xeumeuleu/xml.h"

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

namespace
{
    struct FileReader
    {
        FileReader( xml::xistream& xis ) : xis_( &xis ) {}
        const FileReader& Read( const std::string& tag, std::string& file ) const
        {
            *xis_ >> start( tag ) >> attribute( "file", file ) >> end();
            return *this;
        }
        xml::xistream* xis_;
    };
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::Load
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void AgentTypes::Load( const tools::ExerciseConfig& config )
{
    Purge();

    symbolFactory_ = new SymbolFactory();

    xml::xifstream xis( config.GetPhysicalFile() );
    xis >> start( "physical" );

    std::string components, missions, models, agents, automats, sensors, populations, groups;
    FileReader( xis )
        .Read( "sensors", sensors )
        .Read( "components", components )
        .Read( "missions", missions )
        .Read( "models", models )
        .Read( "units", agents )
        .Read( "automats", automats )
        .Read( "populations", populations )
        .Read( "knowledge-groups", groups );

    ReadComponents( config.BuildPhysicalChildFile( components ) );
    ReadOrderTypes( config.BuildPhysicalChildFile( missions ) );
    ReadModels( config.BuildPhysicalChildFile( models ) );
    ReadSensors( config.BuildPhysicalChildFile( sensors ) );
    ReadAgents( config.BuildPhysicalChildFile( agents ) );
    ReadAutomats( config.BuildPhysicalChildFile( automats ) );
    ReadPopulations( config.BuildPhysicalChildFile( populations ) );
    ReadKnowledgeGroups( config.BuildPhysicalChildFile( groups ) );
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
    Resolver< MissionType >::Clear();
    Resolver< FragOrderType >::DeleteAll();
    Resolver< FragOrderType, std::string >::Clear();
    Resolver< KnowledgeGroupType, std::string >::DeleteAll();
    Resolver< PopulationType >::DeleteAll();
    Resolver< PopulationType, std::string >::Clear();
    Resolver< AgentType >::DeleteAll();
    Resolver< AgentType, std::string >::Clear();
    Resolver< AutomatType >::DeleteAll();
    Resolver< AutomatType, std::string >::Clear();
    Resolver< ComponentType >::DeleteAll();
    Resolver< ComponentType, std::string >::Clear();
    Resolver< SensorType, std::string >::DeleteAll();
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
void AgentTypes::ReadComponents( const std::string& components )
{
    xifstream xis( components );
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
    Resolver< ComponentType, std::string >::Register( component->GetName(), *component );
    Resolver< ComponentType >::Register( component->GetId(), *component );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadOrderTypes
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void AgentTypes::ReadOrderTypes( const std::string& missions )
{
    xifstream xis( missions );
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
    Resolver< MissionType >::Register( mission->GetId(), *mission );
    missions.Register( mission->GetName(), *mission );
}
    
// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadFragOrderType
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void AgentTypes::ReadFragOrderType( xml::xistream& xis )
{
    FragOrderType* order = new FragOrderType( xis );
    Resolver< FragOrderType >::Register( order->GetId(), *order );
    Resolver< FragOrderType, std::string >::Register( order->GetName(), *order );
}


// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadModels
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadModels( const std::string& models )
{
    xifstream xis( models );
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
void AgentTypes::ReadModel( xml::xistream& xis, const T_Resolver& missionResolver, Resolver< DecisionalModel, std::string >& models )
{
    MissionFactory factory( unitMissions_, automatMissions_, populationMissions_, *this );
    DecisionalModel* model = new DecisionalModel( xis, factory, missionResolver, (Resolver< FragOrderType >&)*this );
    models.Register( model->GetName(), *model );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadSensors
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadSensors( const std::string& sensors )
{
    xifstream xis( sensors );
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
    Resolver< SensorType, std::string >::Register( type->GetName(), *type );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadAgents
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadAgents( const std::string& agents )
{
    xifstream xis( agents );
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
    Resolver< AgentType >         ::Register( type->GetId(), *type );
    Resolver< AgentType, std::string >::Register( type->GetName(), *type );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadAutomats
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadAutomats( const std::string& automats )
{
    xifstream xis( automats );
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
    Resolver< AutomatType >         ::Register( type->GetId(), *type );
    Resolver< AutomatType, std::string >::Register( type->GetName(), *type );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadPopulations
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
void AgentTypes::ReadPopulations( const std::string& populations )
{
    xifstream xis( populations );
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
    Resolver< PopulationType >::Register( type->GetId(), *type );
    Resolver< PopulationType, std::string >::Register( type->GetName(), *type );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadKnowledgeGroups
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
void AgentTypes::ReadKnowledgeGroups( const std::string& groups )
{
    xifstream xis( groups );
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
    Resolver< KnowledgeGroupType, std::string >::Register( type->GetName(), *type );
}
