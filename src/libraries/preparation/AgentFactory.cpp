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
#include "InitialState.h"
#include "KnowledgeGroupsModel.h"
#include "LogisticBaseStates.h"
#include "Model.h"
#include "LogisticLevelAttritube.h"
#include "Inhabitant.h"
#include "Affinities.h"
#include "PeopleAffinities.h"
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
#include "Color.h"
#include "Symbol.h"
#include "AgentAffinities.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/SymbolHierarchy_ABC.h"
#include "clients_kernel/Color_ABC.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: AgentFactory constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
AgentFactory::AgentFactory( kernel::Controllers& controllers, Model& model, const ::StaticModel& staticModel, IdManager& idManager, kernel::KnowledgeGroupFactory_ABC& knowledgeGroupFactory, kernel::SymbolFactory& symbolsFactory )
    : controllers_( controllers )
    , model_( model )
    , static_( staticModel )
    , idManager_( idManager )
    , knowledgeGroupFactory_( knowledgeGroupFactory )
    , symbolsFactory_( symbolsFactory )
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
kernel::Agent_ABC* AgentFactory::Create( kernel::Automat_ABC& parent, const kernel::AgentType& type, const geometry::Point2f& position, bool commandPost, const QString& name )
{
    Agent* result = new Agent( type, controllers_.controller_, idManager_ );
    if( !name.isEmpty() )
        result->Rename( name );
    kernel::PropertiesDictionary& dico = result->Get< kernel::PropertiesDictionary >();
    result->Attach< kernel::Positions >( *new AgentPositions( *result, static_.coordinateConverter_, controllers_.controller_, position, dico ) );
    result->Attach< kernel::TacticalHierarchies >( *new AgentHierarchies( controllers_.controller_, *result, result->GetType().GetLevelSymbol(), result->GetType().GetSymbol(), &parent ) );
    result->Attach< kernel::CommunicationHierarchies >( *new AgentCommunications( controllers_.controller_, *result, &parent ) );
    result->Attach( *new InitialState( static_, result->GetType().GetId() ) );
    result->Attach< Affinities >( *new AgentAffinities( *result, controllers_, model_, dico, tools::translate( "Affinities", "Affinities" ) ) );
    if( result->GetType().IsLogisticSupply() )
        result->Attach( *new Stocks( controllers_.controller_, *result, dico ) );
    result->Attach( *new kernel::DictionaryExtensions( controllers_, "orbat-attributes", static_.extensions_ ) );
    result->Attach( *new CommandPostAttributes( *result, type, dico, commandPost ) );
    result->Attach< kernel::Color_ABC >( *new Color( parent ) );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
kernel::Automat_ABC* AgentFactory::Create( kernel::Entity_ABC& parent, const kernel::AutomatType& type, const QString& name )
{
    Automat* result = new Automat( type, controllers_.controller_, idManager_, name );
    kernel::PropertiesDictionary& dico = result->Get< kernel::PropertiesDictionary >();
    result->Attach< kernel::Positions >( *new AutomatPositions( *result ) );
    const kernel::Karma& karma = parent.Get< kernel::TacticalHierarchies >().GetTop().Get< kernel::Diplomacies_ABC >().GetKarma();
    result->Attach< kernel::SymbolHierarchy_ABC >( *new Symbol( symbolsFactory_.GetSymbolBase( karma ) ) );
    result->Attach< kernel::TacticalHierarchies >( *new AutomatHierarchies( controllers_.controller_, *result, &parent ) );
    result->Attach( *new AutomatDecisions( controllers_.controller_, *result ) );
    kernel::Entity_ABC* kg = FindorCreateKnowledgeGroup( parent, knowledgeGroupFactory_ );
    result->Attach< kernel::CommunicationHierarchies >( *new AutomatCommunications( controllers_.controller_, *result, kg ) );

    bool isTC2 = result->GetType().IsTC2(); //$$ NAZE
    result->Attach( *new LogisticLevelAttritube( controllers_.controller_, *result, isTC2, dico ) );
    result->Attach< LogisticHierarchiesBase >( *new LogisticBaseStates( controllers_.controller_, *result, static_.objectTypes_, dico, isTC2 ) );

    result->Attach( *new TacticalLines() );
    result->Attach< kernel::Color_ABC >( *new Color( parent ) );
    result->Attach( *new kernel::DictionaryExtensions( controllers_, "orbat-attributes", static_.extensions_ ) );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
kernel::Population_ABC* AgentFactory::Create( kernel::Entity_ABC& parent, const kernel::PopulationType& type, int number, const geometry::Point2f& position )
{
    kernel::Entity_ABC* top = 0;
    if( const kernel::TacticalHierarchies* hierarchies = parent.Retrieve< kernel::TacticalHierarchies >() )
        top = const_cast< kernel::Entity_ABC* >( &hierarchies->GetTop() );
    else
        top = const_cast< kernel::Entity_ABC* >( &parent.Get< kernel::CommunicationHierarchies >().GetTop() );
    Population* result = new Population( type, number, controllers_.controller_, idManager_ );
    result->Attach< kernel::Positions >( *new PopulationPositions( *result, controllers_.controller_, static_.coordinateConverter_, position ) );
    result->Attach< kernel::TacticalHierarchies >( *new PopulationHierarchies( *result, top ) );
    kernel::PropertiesDictionary& dictionary = result->Get< kernel::PropertiesDictionary >();
    result->Attach< Affinities >( *new PeopleAffinities( controllers_, model_, dictionary ) );
    result->Attach( *new kernel::DictionaryExtensions( controllers_, "orbat-attributes", static_.extensions_ ) );
    if( Populations* popus = top->Retrieve< Populations >() )
        popus->AddPopulation( *result );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2010-11-23
// -----------------------------------------------------------------------------
kernel::Inhabitant_ABC* AgentFactory::Create( kernel::Entity_ABC& parent, const kernel::InhabitantType& type, int number, const QString& name, const kernel::Location_ABC& location )
{
    kernel::Entity_ABC* top = 0;
    if( const kernel::TacticalHierarchies* hierarchies = parent.Retrieve< kernel::TacticalHierarchies >() )
        top = const_cast< kernel::Entity_ABC* >( &hierarchies->GetTop() );
    else
        top = const_cast< kernel::Entity_ABC* >( &parent.Get< kernel::CommunicationHierarchies >().GetTop() );

    Inhabitant* result = new Inhabitant( type, number, name, controllers_.controller_, idManager_ );
    kernel::PropertiesDictionary& dico = result->Get< kernel::PropertiesDictionary >();

    kernel::Positions& positions = *new InhabitantPositions( controllers_.controller_, static_.coordinateConverter_, location, model_.urban_, *result, dico );
    if( positions.GetPosition() == geometry::Point2f( 0, 0 ) )
    {
        delete &positions;
        delete result;
        return 0;
    }
    result->Attach< kernel::Positions >( positions );
    result->Attach< kernel::TacticalHierarchies >( *new InhabitantHierarchies( *result, top ) );
    result->Attach< Affinities >( *new PeopleAffinities( controllers_, model_, dico ) );
    result->Attach( *new kernel::DictionaryExtensions( controllers_, "orbat-attributes", static_.extensions_ ) );
    if( Inhabitants* inhabs = top->Retrieve< Inhabitants >() )
        inhabs->AddInhabitant( *result );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::FindKnowledgeGroup
// Created: AGE 2006-10-10
// -----------------------------------------------------------------------------
kernel::Entity_ABC* AgentFactory::FindorCreateKnowledgeGroup( const kernel::Entity_ABC& parent, kernel::KnowledgeGroupFactory_ABC& knowledgeFactory )
{
    const kernel::Entity_ABC& team = parent.Get< kernel::TacticalHierarchies >().GetTop();
    const kernel::CommunicationHierarchies& teamHierarchy = team.Get< kernel::CommunicationHierarchies >();
    tools::Iterator< const kernel::Entity_ABC& > it = teamHierarchy.CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::Entity_ABC* entity = &it.NextElement();
        if( dynamic_cast< const kernel::KnowledgeGroup_ABC* >( entity ) )
            return const_cast< kernel::Entity_ABC* >( entity );
    }
    // LTO begin
    kernel::Team_ABC* teamtop = dynamic_cast< kernel::Team_ABC* >( const_cast< kernel::Entity_ABC* >(&team) );
    if( teamtop )
        return knowledgeFactory.Create( *teamtop );
    // LTO end
    return const_cast< kernel::Entity_ABC* >( &team );
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
kernel::Agent_ABC* AgentFactory::Create( xml::xistream& xis, kernel::Automat_ABC& parent )
{
    const kernel::AgentType* type = static_.types_.Resolver< kernel::AgentType, std::string >::Find( xis.attribute< std::string >( "type" ) );
    if( !type )
        return 0;
    Agent* result = new Agent( xis, controllers_.controller_, idManager_, *type );
    kernel::PropertiesDictionary& dico = result->Get< kernel::PropertiesDictionary >();
    result->Attach< kernel::Positions >( *new AgentPositions( xis, *result, static_.coordinateConverter_, controllers_.controller_, dico ) );
    result->Attach< kernel::TacticalHierarchies >( *new AgentHierarchies( controllers_.controller_, *result, result->GetType().GetLevelSymbol(), result->GetType().GetSymbol(), &parent ) );
    result->Attach< kernel::CommunicationHierarchies >( *new AgentCommunications( controllers_.controller_, *result, &parent ) );
    result->Attach( *new InitialState( xis, static_, result->GetType().GetId() ) );
    result->Attach< Affinities >( *new AgentAffinities( xis, *result, controllers_, model_, dico, tools::translate( "Affinities", "Affinities" ) ) );
    result->Attach( *new CommandPostAttributes( xis, *result, *type, dico ) );
    if( result->GetType().IsLogisticSupply() )
        result->Attach( *new Stocks( xis, controllers_.controller_, *result, static_.objectTypes_, dico ) );
    result->Attach( *new kernel::DictionaryExtensions( controllers_, "orbat-attributes", xis, static_.extensions_ ) );
    result->Attach< kernel::Color_ABC >( *new Color( xis ) );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
kernel::Automat_ABC* AgentFactory::Create( xml::xistream& xis, kernel::Entity_ABC& parent )
{
    const kernel::AutomatType* type = static_.types_.Resolver< kernel::AutomatType, std::string >::Find( xis.attribute< std::string >( "type" ) );
    if( !type )
        return 0;
    Automat* result = new Automat( xis, controllers_.controller_, idManager_, *type );
    kernel::PropertiesDictionary& dico = result->Get< kernel::PropertiesDictionary >();
    result->Attach< kernel::Positions >( *new AutomatPositions( *result ) );
    result->Attach< kernel::SymbolHierarchy_ABC >( *new Symbol( xis ) );
    result->Attach( *new AutomatDecisions( xis, controllers_.controller_, *result ) );
    result->Attach< kernel::TacticalHierarchies >( *new AutomatHierarchies( controllers_.controller_, *result, &parent ) );
    result->Attach< kernel::CommunicationHierarchies >( *new AutomatCommunications( xis, controllers_.controller_, *result, model_.knowledgeGroups_ ) );

    bool isTC2 = result->GetType().IsTC2(); //$$ NAZE
    result->Attach( *new LogisticLevelAttritube( controllers_.controller_, xis, *result, isTC2, dico ) );
    result->Attach< LogisticHierarchiesBase >( *new LogisticBaseStates( controllers_.controller_, *result, static_.objectTypes_, dico, isTC2 ) );

    result->Attach( *new TacticalLines() );
    result->Attach( *new kernel::DictionaryExtensions( controllers_, "orbat-attributes", xis, static_.extensions_ ) );
    result->Attach< kernel::Color_ABC >( *new Color( xis ) );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
kernel::Population_ABC* AgentFactory::Create( xml::xistream& xis, kernel::Team_ABC& parent, const kernel::PopulationType& type )
{
    Population* result = new Population( xis, type, controllers_.controller_, idManager_ );
    result->Attach< kernel::Positions >( *new PopulationPositions( xis, *result, controllers_.controller_, static_.coordinateConverter_ ) );
    result->Attach< kernel::TacticalHierarchies >( *new PopulationHierarchies( *result, &parent ) );
    kernel::PropertiesDictionary& dictionary = result->Get< kernel::PropertiesDictionary >();
    result->Attach< Affinities >( *new PeopleAffinities( xis, controllers_, model_, dictionary ) );
    result->Attach( *new kernel::DictionaryExtensions( controllers_, "orbat-attributes", xis, static_.extensions_ ) );
    if( Populations* popus = parent.Retrieve< Populations >() )
        popus->AddPopulation( *result );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
kernel::Inhabitant_ABC* AgentFactory::Create( xml::xistream& xis, kernel::Team_ABC& parent, const kernel::InhabitantType& type )
{
    Inhabitant* result = new Inhabitant( xis, type, controllers_.controller_, idManager_ );
    kernel::PropertiesDictionary& dico = result->Get< kernel::PropertiesDictionary >();
    result->Attach< kernel::Positions >( *new InhabitantPositions( xis, controllers_.controller_, static_.coordinateConverter_, model_.urban_, *result, dico ) );
    result->Attach< kernel::TacticalHierarchies >( *new InhabitantHierarchies( *result, &parent ) );
    result->Attach< Affinities >( *new PeopleAffinities( xis, controllers_, model_, dico ) );
    result->Attach( *new kernel::DictionaryExtensions( controllers_, "orbat-attributes", xis, static_.extensions_ ) );
    if( Inhabitants* popus = parent.Retrieve< Inhabitants >() )
        popus->AddInhabitant( *result );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: ABR 2011-10-25
// -----------------------------------------------------------------------------
kernel::Agent_ABC* AgentFactory::Create( kernel::Ghost_ABC& ghost, const kernel::AgentType& type, const geometry::Point2f position )
{
    assert( ghost.GetGhostType() == eGhostType_Agent );
    Agent* result = new Agent( type, controllers_.controller_, idManager_ );
    result->Rename( ghost.GetName() );
    kernel::PropertiesDictionary& dico = result->Get< kernel::PropertiesDictionary >();
    result->Attach< kernel::Positions >( *new AgentPositions( *result, static_.coordinateConverter_, controllers_.controller_, position, dico ) );
    // Hierarchies
    {
        const kernel::TacticalHierarchies* ghostHierarchy = ghost.Retrieve< kernel::TacticalHierarchies >();
        assert( ghostHierarchy );
        kernel::Entity_ABC* tactSuperior = const_cast< kernel::Entity_ABC* >( ghostHierarchy->GetSuperior() );
        assert( tactSuperior );

        result->Attach< kernel::TacticalHierarchies >( *new AgentHierarchies( controllers_.controller_, *result, result->GetType().GetLevelSymbol(), result->GetType().GetSymbol(), tactSuperior ) );
        result->Attach< kernel::CommunicationHierarchies >( *new AgentCommunications( controllers_.controller_, *result, tactSuperior ) );
    }
    result->Attach( *new InitialState( static_, result->GetType().GetId() ) );
    result->Attach< Affinities >( *new AgentAffinities( *result, controllers_, model_, dico, tools::translate( "Affinities", "Affinities" ) ) );
    if( result->GetType().IsLogisticSupply() )
        result->Attach( *new Stocks( controllers_.controller_, *result, dico ) );
    result->Attach( *new kernel::DictionaryExtensions( controllers_, "orbat-attributes", static_.extensions_ ) );
    result->Attach( *new CommandPostAttributes( *result, type, dico ) );
    result->Attach< kernel::Color_ABC >( *new Color( ghost ) );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: ABR 2011-10-25
// -----------------------------------------------------------------------------
kernel::Automat_ABC* AgentFactory::Create( kernel::Ghost_ABC& ghost, const kernel::AutomatType& type )
{
    assert( ghost.GetGhostType() == eGhostType_Automat );

    Automat* result = new Automat( type, controllers_.controller_, idManager_, ghost.GetName() );
    kernel::PropertiesDictionary& dico = result->Get< kernel::PropertiesDictionary >();
    result->Attach< kernel::Positions >( *new AutomatPositions( *result ) );
    const kernel::Karma& karma = ghost.Get< kernel::TacticalHierarchies >().GetTop().Get< kernel::Diplomacies_ABC >().GetKarma();
    result->Attach< kernel::SymbolHierarchy_ABC >( *new Symbol( symbolsFactory_.GetSymbolBase( karma ) ) );
    result->Attach( *new AutomatDecisions( controllers_.controller_, *result ) );
    // Tactical Hierarchies
    {
        const kernel::TacticalHierarchies& ghostHierarchy = ghost.Get< kernel::TacticalHierarchies >();
        kernel::Entity_ABC* tactSuperior = const_cast< kernel::Entity_ABC* >( ghostHierarchy.GetSuperior() );
        assert( tactSuperior );
        result->Attach< kernel::TacticalHierarchies >( *new AutomatHierarchies( controllers_.controller_, *result, tactSuperior ) );
    }
    // Communication Hierarchies
    {
        const kernel::CommunicationHierarchies& ghostHierarchy = ghost.Get< kernel::CommunicationHierarchies >();
        kernel::Entity_ABC* comSuperior = const_cast< kernel::Entity_ABC* >( ghostHierarchy.GetSuperior() );
        assert( comSuperior );
        result->Attach< kernel::CommunicationHierarchies >( *new AutomatCommunications( controllers_.controller_, *result, comSuperior ) );
    }

    bool isTC2 = result->GetType().IsTC2(); //$$ NAZE
    result->Attach( *new LogisticLevelAttritube( controllers_.controller_, *result, isTC2, dico ) );
    result->Attach< LogisticHierarchiesBase >( *new LogisticBaseStates( controllers_.controller_, *result, static_.objectTypes_, dico, isTC2 ) );

    result->Attach( *new TacticalLines() );
    result->Attach< kernel::Color_ABC >( *new Color( ghost ) );
    result->Attach( *new kernel::DictionaryExtensions( controllers_, "orbat-attributes", static_.extensions_ ) );
    result->Polish();
    return result;
}
