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
#include "xeumeuleu/xml.h"
#include "PathTools.h"

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

    // $$$$ AGE 2006-05-03: clean all
    xml::xifstream xis( scipioXml );
    xis >> start( "Scipio" )    
            >> start( "Donnees" );

    std::string components, decisional, agents, automats, sensors, populations;
    xis >> content( "Capteurs", sensors )
        >> content( "Composantes", components )
        >> content( "Decisionnel", decisional )
        >> content( "Pions", agents )
        >> content( "Automates", automats )
        >> content( "Populations", populations );

    ReadComponents( path_tools::BuildChildPath( scipioXml, components ) );
    ReadDecisional( path_tools::BuildChildPath( scipioXml, decisional ) );
    ReadSensors( path_tools::BuildChildPath( scipioXml, sensors ) );
    ReadAgents( path_tools::BuildChildPath( scipioXml, agents ) );
    ReadAutomats( path_tools::BuildChildPath( scipioXml, automats ) );
    ReadPopulations( path_tools::BuildChildPath( scipioXml, populations ) );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::Purge
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void AgentTypes::Purge()
{
    Resolver< PopulationType >::DeleteAll();
    Resolver< PopulationType, std::string >::Clear();
    Resolver< AgentType >::DeleteAll();
    Resolver< AgentType, std::string >::Clear();
    Resolver< AutomatType >::DeleteAll();
    Resolver< ComponentType >::DeleteAll();
    Resolver< ComponentType, std::string >::Clear();
    Resolver< DecisionalModel, std::string >::DeleteAll();
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
    Resolver< ComponentType, std::string >::Register( component->GetName(), *component );
    Resolver< ComponentType >::Register( component->GetId(), *component );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadDecisional
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadDecisional( const std::string& decisional )
{
    xifstream xis( decisional );
    std::string models;
    xis >> start( "Decisionnel" )
            >> start( "DirectIA" )
                >> content( "Modeles", models );
    ReadModels( path_tools::BuildChildPath( decisional, models ) );
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
                >> list( "Modele", *this, &AgentTypes::ReadModel, unitResolver )
            >> end()
            >> start( "Automates" )
                >> list( "Modele", *this, &AgentTypes::ReadModel, automatResolver )
            >> end()
            >> start( "Populations" )
                >> list( "Modele", *this, &AgentTypes::ReadModel, populationResolver )
            >> end();
}
    
// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadModel
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadModel( xml::xistream& xis, const T_Resolver& missionResolver )
{
    MissionFactory factory;
    DecisionalModel* model = new DecisionalModel( xis, factory, missionResolver );
    Resolver< DecisionalModel, std::string >::Register( model->GetName(), *model );
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
    SensorType* type = new SensorType( sensor, xis );
    Resolver< SensorType, std::string >::Register( sensor, *type );
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
    AgentType* type = new AgentType( xis, *this, *this, *symbolFactory_ );
    Resolver< AgentType >             ::Register( type->GetId(), *type );
    Resolver< AgentType, std::string >::Register( type->GetName(), *type );
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
    AutomatType* type = new AutomatType( xis, *this, *this, *symbolFactory_ );
    Resolver< AutomatType >::Register( type->GetId(), *type );
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
    PopulationType* type = new PopulationType( xis, *this );
    Resolver< PopulationType >::Register( type->GetId(), *type );
    Resolver< PopulationType, std::string >::Register( type->GetName(), *type );
}

