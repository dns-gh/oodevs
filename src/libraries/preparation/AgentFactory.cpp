// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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
#include "LogisticBaseStates.h"
#include "Model.h"
#include "Inhabitant.h"
#include "EntityAffinities.h"
#include "InhabitantHierarchies.h"
#include "InhabitantPositions.h"
#include "Inhabitants.h"
#include "Population.h"
#include "PopulationHierarchies.h"
#include "PopulationPositions.h"
#include "Populations.h"
#include "StaticModel.h"
#include "Stocks.h"
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
AgentFactory::AgentFactory( Controllers& controllers, Model& model, const StaticModel& staticModel, IdManager& idManager, KnowledgeGroupFactory_ABC& knowledgeGroupFactory )
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
Automat_ABC* AgentFactory::Create( Entity_ABC& parent, const AutomatType& type, const QString& name )
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
    if( type.IsTC2() )
        result->Attach< LogisticBaseHierarchies >( *new LogisticBaseStates( controllers_.controller_, *result, static_.objectTypes_, dico ) );
    result->Attach( *new TacticalLines() );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
Population_ABC* AgentFactory::Create( Entity_ABC& parent, const PopulationType& type, int number, const geometry::Point2f& position )
{
    Entity_ABC* top = 0;
    if( const kernel::TacticalHierarchies* hierarchies = parent.Retrieve< kernel::TacticalHierarchies >() )
        top = const_cast< Entity_ABC* >( &hierarchies->GetTop() );
    else
        top = const_cast< Entity_ABC* >( &parent.Get< CommunicationHierarchies >().GetTop() );
    Population* result = new Population( type, number, controllers_.controller_, idManager_ );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< Positions >( *new PopulationPositions( *result, static_.coordinateConverter_, position ) );
    result->Attach< kernel::TacticalHierarchies >( *new PopulationHierarchies( *result, top ) );
    result->Attach( *new EntityAffinities( controllers_, model_, *result, dico ) );
    if( Populations* popus = top->Retrieve< Populations >() )
        popus->AddPopulation( *result );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2010-11-23
// -----------------------------------------------------------------------------
Inhabitant_ABC* AgentFactory::Create( Entity_ABC& parent, const InhabitantType& type, int number, const QString& name, const Location_ABC& location )
{
    Entity_ABC* top = 0;
    if( const kernel::TacticalHierarchies* hierarchies = parent.Retrieve< kernel::TacticalHierarchies >() )
        top = const_cast< Entity_ABC* >( &hierarchies->GetTop() );
    else
        top = const_cast< Entity_ABC* >( &parent.Get< CommunicationHierarchies >().GetTop() );

    Inhabitant* result = new Inhabitant( type, number, name, controllers_.controller_, idManager_ );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();

    Positions& positions = *new InhabitantPositions( static_.coordinateConverter_, location, model_.urban_, *result, dico );
    if( positions.GetPosition() == geometry::Point2f( 0, 0 ) )
    {
        delete &positions;
        delete result;
        return 0;
    }

    result->Attach< Positions >( positions );
    result->Attach< kernel::TacticalHierarchies >( *new InhabitantHierarchies( *result, top ) );
    result->Attach( *new EntityAffinities( controllers_, model_, *result, dico ) );
    if( Inhabitants* inhabs = top->Retrieve< Inhabitants >() )
        inhabs->AddInhabitant( *result );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::FindKnowledgeGroup
// Created: AGE 2006-10-10
// -----------------------------------------------------------------------------
Entity_ABC* AgentFactory::FindorCreateKnowledgeGroup( const Entity_ABC& parent )
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
Agent_ABC* AgentFactory::Create( xml::xistream& xis, Automat_ABC& parent )
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
        result->Attach( *new DictionaryExtensions( "orbat-attributes", xis, static_.extensions_ ) );
        xis.end();
    }
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
Automat_ABC* AgentFactory::Create( xml::xistream& xis, Entity_ABC& parent )
{
    Automat* result = new Automat( xis, controllers_.controller_, idManager_, static_.types_ );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< Positions >( *new AutomatPositions( *result ) );
    result->Attach( *new AutomatDecisions( xis, controllers_.controller_, *result ) );
    result->Attach< kernel::TacticalHierarchies >( *new AutomatHierarchies( controllers_.controller_, *result, &parent ) );
    result->Attach< CommunicationHierarchies >( *new AutomatCommunications( xis, controllers_.controller_, *result, model_.knowledgeGroups_ ) );
    result->Attach< TC2Hierarchies >        ( *new Tc2States( controllers_.controller_, *result, dico ) );
    if( result->GetType().IsTC2() )
        result->Attach< LogisticBaseHierarchies >( *new LogisticBaseStates( controllers_.controller_, *result, static_.objectTypes_, dico ) );
    result->Attach( *new TacticalLines() );
    if( xis.has_child( "extensions" ) )
    {
        xis.start( "extensions" );
        result->Attach( *new DictionaryExtensions( "orbat-attributes", xis, static_.extensions_ ) );
        xis.end();
    }
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
Population_ABC* AgentFactory::CreatePop( xml::xistream& xis, Team_ABC& parent )
{
    Population* result = new Population( xis, controllers_.controller_, idManager_, static_.types_ );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< Positions >( *new PopulationPositions( xis, *result, static_.coordinateConverter_ ) );
    result->Attach< kernel::TacticalHierarchies >( *new PopulationHierarchies( *result, &parent ) );
    result->Attach( *new EntityAffinities( xis, controllers_, model_, *result, dico ) );
    if( xis.has_child( "extensions" ) )
    {
        xis.start( "extensions" );
        result->Attach( *new DictionaryExtensions( "orbat-attributes", xis, static_.extensions_ ) );
        xis.end();
    }
    if( Populations* popus = parent.Retrieve< Populations >() )
        popus->AddPopulation( *result );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
Inhabitant_ABC* AgentFactory::CreateInhab( xml::xistream& xis, Team_ABC& parent )
{
    Inhabitant* result = new Inhabitant( xis, controllers_.controller_, idManager_, static_.types_ );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();

    result->Attach< Positions >( *new InhabitantPositions( xis, static_.coordinateConverter_, model_.urban_, *result, dico ) );
    result->Attach< kernel::TacticalHierarchies >( *new InhabitantHierarchies( *result, &parent ) );
    result->Attach( *new EntityAffinities( xis, controllers_, model_, *result, dico ) );
    if( xis.has_child( "extensions" ) )
    {
        xis.start( "extensions" );
        result->Attach( *new DictionaryExtensions( "orbat-attributes", xis, static_.extensions_ ) );
        xis.end();
    }
    if( Inhabitants* popus = parent.Retrieve< Inhabitants >() )
        popus->AddInhabitant( *result );
    result->Polish();
    return result;
}
