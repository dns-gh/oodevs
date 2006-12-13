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
#include "PathTools.h"
#include "Mission.h"
#include "FragOrder.h"
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

// -----------------------------------------------------------------------------
// Name: AgentTypes::Load
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void AgentTypes::Load( const std::string& scipioXml )
{
    Purge();

    xml::xifstream xisSymbols( path_tools::BuildWorkingDirectoryPath( "symbols.xml" ) );
    symbolFactory_ = new SymbolFactory( xisSymbols );

    xml::xifstream xis( scipioXml );
    xis >> start( "Scipio" )    
            >> start( "Donnees" );

    std::string components, decisional, agents, automats, sensors, populations, groups;
    xis >> content( "Capteurs", sensors )
        >> content( "Composantes", components )
        >> content( "Decisionnel", decisional )
        >> content( "Pions", agents )
        >> content( "Automates", automats )
        >> content( "Populations", populations )
        >> content( "GroupesConnaissance", groups );

    ReadComponents( path_tools::BuildChildPath( scipioXml, components ) );
    ReadDecisional( path_tools::BuildChildPath( scipioXml, decisional ) );
    ReadSensors( path_tools::BuildChildPath( scipioXml, sensors ) );
    ReadAgents( path_tools::BuildChildPath( scipioXml, agents ) );
    ReadAutomats( path_tools::BuildChildPath( scipioXml, automats ) );
    ReadPopulations( path_tools::BuildChildPath( scipioXml, populations ) );
    ReadKnowledgeGroups( path_tools::BuildChildPath( scipioXml, groups ) );
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
    Resolver< Mission >::DeleteAll();
    Resolver< FragOrder >::DeleteAll();
    Resolver< FragOrder, QString >::Clear();
    Resolver< KnowledgeGroupType, QString >::DeleteAll();
    Resolver< PopulationType >::DeleteAll();
    Resolver< PopulationType, QString >::Clear();
    Resolver< AgentType >::DeleteAll();
    Resolver< AgentType, QString >::Clear();
    Resolver< AutomatType >::DeleteAll();
    Resolver< AutomatType, QString >::Clear();
    Resolver< ComponentType >::DeleteAll();
    Resolver< ComponentType, QString >::Clear();
    Resolver< SensorType, QString >::DeleteAll();
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
    xis >> start( "Composantes" )
            >> list( "Composante", *this, &AgentTypes::ReadComponent );
}
    
// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadComponent
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadComponent( xml::xistream& xis )
{
    ComponentType* component = new ComponentType( xis );
    Resolver< ComponentType, QString >::Register( component->GetName(), *component );
    Resolver< ComponentType >::Register( component->GetId(), *component );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadDecisional
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadDecisional( const std::string& decisional )
{
    xifstream xis( decisional );
    std::string missions, models;
    xis >> start( "Decisionnel" )
            >> start( "DirectIA" )
                >> content( "Missions", missions )
                >> content( "Modeles", models );
    ReadMissions( path_tools::BuildChildPath( decisional, missions ) );
    ReadModels( path_tools::BuildChildPath( decisional, models ) );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadMissions
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void AgentTypes::ReadMissions( const std::string& missions )
{
    xifstream xis( missions );
    xis >> start( "missions" )
            >> start( "units" )
                >> list( "mission", *this, &AgentTypes::ReadMission, unitMissions_ )
            >> end()
            >> start( "automats" )
                >> list( "mission", *this, &AgentTypes::ReadMission, automatMissions_ )
            >> end()
            >> start( "populations" )
                >> list( "mission", *this, &AgentTypes::ReadMission, populationMissions_ )
            >> end()
            >> start( "fragorders" )
                >> list( "fragorder", *this, &AgentTypes::ReadFragOrder )
            >> end();
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadMission
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void AgentTypes::ReadMission( xml::xistream& xis, T_MissionResolver& missions )
{
    Mission* mission = new Mission( xis );
    Resolver< Mission >::Register( mission->GetId(), *mission );
    missions.Register( mission->GetName(), *mission );
}
    
// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadFragOrder
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void AgentTypes::ReadFragOrder( xml::xistream& xis )
{
    FragOrder* order = new FragOrder( xis );
    Resolver< FragOrder >::Register( order->GetId(), *order );
    Resolver< FragOrder, QString >::Register( order->GetName(), *order );
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

    xis >> start( "Modeles" )
            >> start( "Pions" )
                >> list( "Modele", *this, &AgentTypes::ReadModel, unitResolver, unitModels_ )
            >> end()
            >> start( "Automates" )
                >> list( "Modele", *this, &AgentTypes::ReadModel, automatResolver, automatModels_ )
            >> end()
            >> start( "Populations" )
                >> list( "Modele", *this, &AgentTypes::ReadModel, populationResolver, populationModels_ )
            >> end();
}
    
// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadModel
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadModel( xml::xistream& xis, const T_Resolver& missionResolver, Resolver< DecisionalModel, QString >& models )
{
    MissionFactory factory( unitMissions_, automatMissions_, populationMissions_, *this );
    DecisionalModel* model = new DecisionalModel( xis, factory, missionResolver, (Resolver< FragOrder >&)*this );
    models.Register( model->GetName(), *model );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadSensors
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadSensors( const std::string& sensors )
{
    xifstream xis( sensors );
    xis >> start( "Capteurs" )
            >> start( "Senseurs" )
                >> list( "Senseur", *this, &AgentTypes::ReadSensor );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadSensor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadSensor( xml::xistream& xis )
{
    std::string name;
    xis >> attribute( "nom", name )
        >> list( "DetectionAgents", *this, &AgentTypes::ReallyReadSensor, name );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReallyReadSensor
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
void AgentTypes::ReallyReadSensor( xml::xistream& xis, const std::string& sensor )
{
    SensorType* type = new SensorType( sensor.c_str(), xis );
    Resolver< SensorType, QString >::Register( type->GetName(), *type );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadAgents
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadAgents( const std::string& agents )
{
    xifstream xis( agents );
    xis >> start( "Pions" )
            >> list( "Unite", *this, &AgentTypes::ReadAgentType );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadAgentType
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadAgentType( xml::xistream& xis )
{
    AgentType* type = new AgentType( xis, *this, unitModels_, *symbolFactory_ );
    Resolver< AgentType >         ::Register( type->GetId(), *type );
    Resolver< AgentType, QString >::Register( type->GetName(), *type );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadAutomats
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadAutomats( const std::string& automats )
{
    xifstream xis( automats );
    xis >> start( "Automates" )
            >> list( "Unite", *this, &AgentTypes::ReadAutomatType );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadAutomatType
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadAutomatType( xml::xistream& xis )
{
    AutomatType* type = new AutomatType( xis, *this, automatModels_ );
    Resolver< AutomatType >         ::Register( type->GetId(), *type );
    Resolver< AutomatType, QString >::Register( type->GetName(), *type );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadPopulations
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
void AgentTypes::ReadPopulations( const std::string& populations )
{
    xifstream xis( populations );
    xis >> start( "Populations" )
            >> list( "Population", *this, &AgentTypes::ReadPopulationType );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadPopulationType
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
void AgentTypes::ReadPopulationType( xml::xistream& xis )
{
    PopulationType* type = new PopulationType( xis, populationModels_ );
    Resolver< PopulationType >::Register( type->GetId(), *type );
    Resolver< PopulationType, QString >::Register( type->GetName(), *type );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadKnowledgeGroups
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
void AgentTypes::ReadKnowledgeGroups( const std::string& groups )
{
    xifstream xis( groups );
    xis >> start( "GroupesConnaissance" )
            >> list( "GroupeConnaissance", *this, &AgentTypes::ReadKnowledgeGroupType );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadKnowledgeGroupType
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
void AgentTypes::ReadKnowledgeGroupType( xml::xistream& xis )
{
    KnowledgeGroupType* type = new KnowledgeGroupType( xis );
    Resolver< KnowledgeGroupType, QString >::Register( type->GetName(), *type );
}
