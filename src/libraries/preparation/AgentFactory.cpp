// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AgentFactory.h"
#include "Agent.h"
#include "AgentCommunications.h"
#include "AgentFactory.h"
#include "AgentHierarchies.h"
#include "AgentPositions.h"
#include "AgentsModel.h"
#include "Automat.h"
#include "AutomatCommunications.h"
#include "AutomatDecisions.h"
#include "AutomatHierarchies.h"
#include "AutomatPositions.h"
#include "CommandPostAttributes.h"
#include "Dotations.h"
#include "KnowledgeGroupsModel.h"
#include "MaintenanceStates.h"
#include "MedicalStates.h"
#include "Model.h"
#include "Population.h"
#include "PopulationHierarchies.h"
#include "PopulationPositions.h"
#include "Populations.h"
#include "StaticModel.h"
#include "Stocks.h"
#include "SupplyStates.h"
#include "TacticalLines.h"
#include "Tc2States.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Team_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentFactory constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
AgentFactory::AgentFactory( Controllers& controllers, Model& model, const StaticModel& staticModel, IdManager& idManager, kernel::KnowledgeGroupFactory_ABC& knowledgeGroupFactory )
    : controllers_( controllers )
    , model_( model )
    , static_( staticModel )
    , idManager_( idManager )
    , knowledgeGroupFactory_( knowledgeGroupFactory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentFactory destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
AgentFactory::~AgentFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Agent_ABC* AgentFactory::Create( Automat_ABC& parent, const AgentType& type, const geometry::Point2f& position, bool commandPost, const QString& name )
{
    Agent* result = new Agent( type, controllers_.controller_, idManager_, commandPost );
    if( !name.isEmpty() )
        result->Rename( name );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< Positions >( *new AgentPositions( *result, static_.coordinateConverter_, controllers_.controller_, position, dico ) );
    result->Attach< kernel::TacticalHierarchies >( *new AgentHierarchies( controllers_.controller_, *result, &parent ) );
    result->Attach< CommunicationHierarchies >( *new AgentCommunications( controllers_.controller_, *result, &parent ) );
    result->Attach( *new Dotations( controllers_.controller_, *result, dico ) );
    if( commandPost )
        result->Attach( *new CommandPostAttributes( *result ) );
    if( type.IsLogisticSupply() )
        result->Attach( *new Stocks( controllers_.controller_, *result, dico ) );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
kernel::Automat_ABC* AgentFactory::Create( Entity_ABC& parent, const AutomatType& type, const QString& name )
{
    Automat* result = new Automat( type, controllers_.controller_, idManager_ );
    if( !name.isEmpty() )
        result->Rename( name );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< Positions >( *new AutomatPositions( *result ) );
    result->Attach< kernel::TacticalHierarchies >( *new AutomatHierarchies( controllers_.controller_, *result, &parent ) );
    result->Attach( *new AutomatDecisions( controllers_.controller_, *result ) );
    Entity_ABC* kg = FindorCreateKnowledgeGroup( parent );
    result->Attach< CommunicationHierarchies >( *new AutomatCommunications( controllers_.controller_, *result, kg ) );
    result->Attach< TC2Hierarchies >( *new Tc2States( controllers_.controller_, *result, dico ) );
    if( type.IsLogisticMaintenance() )
        result->Attach< MaintenanceHierarchies >( *new MaintenanceStates( controllers_.controller_, *result, dico ) );
    if( type.IsLogisticMedical() )
        result->Attach< MedicalHierarchies >( *new MedicalStates( controllers_.controller_, *result, dico ) );
    if( type.IsLogisticSupply() )
        result->Attach< SupplyHierarchies >( *new SupplyStates( controllers_.controller_, *result, static_.objectTypes_ , dico ) );
    result->Attach( *new TacticalLines() );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
kernel::Population_ABC* AgentFactory::Create( kernel::Entity_ABC& parent, const kernel::PopulationType& type, const geometry::Point2f& position )
{
    Entity_ABC* top = 0;
    if( const kernel::TacticalHierarchies* hierarchies = parent.Retrieve< kernel::TacticalHierarchies >() )
        top = const_cast< kernel::Entity_ABC* >( &hierarchies->GetTop() );
    else
        top = const_cast< kernel::Entity_ABC* >( &parent.Get< kernel::CommunicationHierarchies >().GetTop() );
    Population* result = new Population( type, controllers_.controller_, idManager_ );
    result->Attach< Positions >( *new PopulationPositions( *result, static_.coordinateConverter_, position ) );
    result->Attach< kernel::TacticalHierarchies >( *new PopulationHierarchies( *result, top ) );
    if( Populations* popus = top->Retrieve< Populations >() )
        popus->AddPopulation( *result );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::FindKnowledgeGroup
// Created: AGE 2006-10-10
// -----------------------------------------------------------------------------
Entity_ABC* AgentFactory::FindorCreateKnowledgeGroup( const kernel::Entity_ABC& parent )
{
    const Entity_ABC& team = parent.Get< kernel::TacticalHierarchies >().GetTop();
    const CommunicationHierarchies& teamHierarchy = team.Get< CommunicationHierarchies >();
    tools::Iterator< const Entity_ABC& > it = teamHierarchy.CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        const Entity_ABC* entity = &it.NextElement();
        if( dynamic_cast< const KnowledgeGroup_ABC* >( entity ) )
            return const_cast< Entity_ABC* >( entity );
    }
    // LTO begin
    Team_ABC* teamtop = dynamic_cast< Team_ABC* >( const_cast< Entity_ABC* >(&team) );
    if( teamtop )
        return knowledgeGroupFactory_.Create( *teamtop );
    // LTO end
    return const_cast< Entity_ABC* >( &team );
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
kernel::Agent_ABC* AgentFactory::Create( xml::xistream& xis, kernel::Automat_ABC& parent )
{
    Agent* result = new Agent( xis, controllers_.controller_, idManager_, static_.types_ );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< Positions >( *new AgentPositions( xis, *result, static_.coordinateConverter_, controllers_.controller_, dico ) );
    result->Attach< kernel::TacticalHierarchies >( *new AgentHierarchies( controllers_.controller_, *result, &parent ) );
    result->Attach< CommunicationHierarchies >( *new AgentCommunications( controllers_.controller_, *result, &parent ) );
    result->Attach( *new Dotations( xis, controllers_.controller_, *result, static_.objectTypes_, dico ) );
    if( result->IsCommandPost() )
        result->Attach( *new CommandPostAttributes( *result ) );
    if( result->GetType().IsLogisticSupply() )
        result->Attach( *new Stocks( xis, controllers_.controller_, *result, static_.objectTypes_, dico ) );
    if( xis.has_child( "extensions" ) )
    {
        xis.start( "extensions" );
        result->Attach( *new DictionaryExtensions( xis ) );
        xis.end();
    }
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
kernel::Automat_ABC* AgentFactory::Create( xml::xistream& xis, kernel::Entity_ABC& parent )
{
    Automat* result = new Automat( xis, controllers_.controller_, idManager_, static_.types_ );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< Positions >( *new AutomatPositions( *result ) );
    result->Attach( *new AutomatDecisions( xis, controllers_.controller_, *result ) );
    result->Attach< kernel::TacticalHierarchies >( *new AutomatHierarchies( controllers_.controller_, *result, &parent ) );
    result->Attach< CommunicationHierarchies >( *new AutomatCommunications( xis, controllers_.controller_, *result, model_.knowledgeGroups_ ) );
    result->Attach< TC2Hierarchies >        ( *new Tc2States( controllers_.controller_, *result, dico ) );
    if( result->GetType().IsLogisticMaintenance() )
        result->Attach< MaintenanceHierarchies >( *new MaintenanceStates( controllers_.controller_, *result, dico ) );
    if( result->GetType().IsLogisticMedical() )
        result->Attach< MedicalHierarchies >( *new MedicalStates( controllers_.controller_, *result, dico ) );
    if( result->GetType().IsLogisticSupply() )
        result->Attach< SupplyHierarchies >( *new SupplyStates( controllers_.controller_, *result, static_.objectTypes_ , dico ) );
    result->Attach( *new TacticalLines() );
    if( xis.has_child( "extensions" ) )
    {
        xis.start( "extensions" );
        result->Attach( *new DictionaryExtensions( xis ) );
        xis.end();
    }
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
kernel::Population_ABC* AgentFactory::Create( xml::xistream& xis, kernel::Team_ABC& parent )
{
    Population* result = new Population( xis, controllers_.controller_, idManager_, static_.types_ );
    result->Attach< Positions >( *new PopulationPositions( xis, *result, static_.coordinateConverter_ ) );
    result->Attach< kernel::TacticalHierarchies >( *new PopulationHierarchies( *result, &parent ) );
    if( xis.has_child( "extensions" ) )
    {
        xis.start( "extensions" );
        result->Attach( *new DictionaryExtensions( xis ) );
        xis.end();
    }
    if( Populations* popus = parent.Retrieve< Populations >() )
        popus->AddPopulation( *result );
    result->Polish();
    return result;
}
