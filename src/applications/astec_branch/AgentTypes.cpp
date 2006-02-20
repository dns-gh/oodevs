// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "AgentTypes.h"
#include "AgentType.h"
#include "ComponentType.h"
#include "DecisionalModel.h"
#include "SensorType.h"
#include "AutomatType.h"
#include "PopulationType.h"
#include "ent/ENT_Tr.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: AgentTypes constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentTypes::AgentTypes( const std::string& scipioXml )
{
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

    ReadComponents( components );
    ReadDecisional( decisional );
    ReadSensors( sensors );
    ReadAgents( agents );
    ReadAutomats( automats );
    ReadPopulations( populations );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentTypes::~AgentTypes()
{
    Resolver< PopulationType >::DeleteAll();
    Resolver< AgentType >::DeleteAll();
    Resolver< AutomatType >::DeleteAll();
    Resolver< ComponentType, std::string >::DeleteAll();
    Resolver< DecisionalModel, std::string >::DeleteAll();
    Resolver< SensorType, std::string >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadComponents
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadComponents( const std::string& components )
{
    xifstream xis( components );
    xis >> start( "Composantes" )
            >> list( "Composante", *this, ReadComponent );
}
    
// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadComponent
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadComponent( xml::xistream& xis )
{
    ComponentType* component = new ComponentType( xis );
    Resolver< ComponentType, std::string >::Register( component->GetName(), *component );
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
    const std::string current = MT_GetCurrentDir();
    std::string modelDir;
    MT_ExtractFilePath( decisional, modelDir );
    MT_ChangeDir( modelDir );
    ReadModels( models );
    MT_ChangeDir( current );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadModels
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadModels( const std::string& models )
{
    xifstream xis( models );
    T_Resolver unitResolver       = T_Resolver( ENT_Tr::ConvertToUnitMission );
    T_Resolver automatResolver    = T_Resolver( ENT_Tr::ConvertToAutomataMission );
    T_Resolver populationResolver = T_Resolver( ENT_Tr::ConvertToPopulationMission );

    xis >> start( "Modeles" )
            >> start( "Pions" )
                >> list( "Modele", *this, ReadModel, unitResolver )
            >> end()
            >> start( "Automates" )
                >> list( "Modele", *this, ReadModel, automatResolver )
            >> end()
            >> start( "Populations" )
                >> list( "Modele", *this, ReadModel, populationResolver )
            >> end();
}
    
// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadModel
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadModel( xml::xistream& xis, const T_Resolver& missionResolver )
{
    T_Resolver fragResolver = T_Resolver( ENT_Tr::ConvertToFragOrder );
    DecisionalModel* model = new DecisionalModel( xis, missionResolver, fragResolver );
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
                >> list( "Senseur", *this, ReadSensor );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadSensor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadSensor( xml::xistream& xis )
{
    SensorType* type = new SensorType( xis );
    Resolver< SensorType, std::string >::Register( type->GetName(), *type );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadAgents
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadAgents( const std::string& agents )
{
    xifstream xis( agents );
    xis >> start( "Pions" )
            >> list( "Unite", *this, ReadAgentType );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadAgentType
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadAgentType( xml::xistream& xis )
{
    AgentType* type = new AgentType( xis, *this, *this );
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
            >> list( "Unite", *this, ReadAutomatType );
}

// -----------------------------------------------------------------------------
// Name: AgentTypes::ReadAutomatType
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentTypes::ReadAutomatType( xml::xistream& xis )
{
    AutomatType* type = new AutomatType( xis, *this, *this );
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
            >> list( "Population", *this, ReadPopulationType );
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

