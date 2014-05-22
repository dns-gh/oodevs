// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Agent.h"
#include "AgentFactory_ABC.h"
#include "AgentsModel.h"
#include "Automat.h"
#include "CircleFormation.h"
#include "GhostModel.h"
#include "InitialState.h"
#include "LogisticBaseStates.h"
#include "LimitsModel.h"
#include "Model.h"
#include "StaticModel.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/AutomatComposition.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "tools/ExerciseConfig.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentsModel constructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
AgentsModel::AgentsModel( Controllers& controllers, AgentFactory_ABC& agentFactory, const ::StaticModel& staticModel )
    : controllers_ ( controllers )
    , agentFactory_( agentFactory )
    , staticModel_ ( staticModel )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel destructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
AgentsModel::~AgentsModel()
{
    Purge();
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::Purge
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void AgentsModel::Purge()
{
    tools::Resolver< Agent_ABC >::DeleteAll();
    tools::Resolver< Automat_ABC >::DeleteAll();
    tools::Resolver< Population_ABC >::DeleteAll();
    tools::Resolver< Inhabitant_ABC >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateAutomat
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
Automat_ABC& AgentsModel::CreateAutomat( Entity_ABC& parent, const AutomatType& type, const QString& name )
{
    Automat_ABC* agent = agentFactory_.Create( parent, type, name );
    tools::Resolver< Automat_ABC >::Register( agent->GetId(), *agent );
    return *agent;
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateAutomat
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void AgentsModel::CreateAutomat( Entity_ABC& parent, const AutomatType& type, const geometry::Point2f& position )
{
    Automat_ABC& automat = CreateAutomat( parent, type );
    CreateAutomatChilds( automat, type, position );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateAutomatInsteadOf
// Created: ABR 2012-06-28
// -----------------------------------------------------------------------------
kernel::Automat_ABC* AgentsModel::CreateAutomatInsteadOf( Entity_ABC& original, const kernel::AutomatType& type, const geometry::Point2f& position )
{
    Automat_ABC* result = 0;
    if( Ghost_ABC* ghost = dynamic_cast< Ghost_ABC* >( &original ) )
    {
        result = agentFactory_.Create( *ghost, type );
        if( result )
        {
            tools::Resolver< Automat_ABC >::Register( result->GetId(), *result );
            ReplaceAutomatChildrenByAGhost( *ghost, *result, type, position );
        }
    }
    else
        if( Automat_ABC* automat = dynamic_cast< Automat_ABC* >( &original ) )
            if( const TacticalHierarchies* pHierarchy = original.Retrieve< TacticalHierarchies >() )
                if( Entity_ABC* parent = const_cast< Entity_ABC* >( pHierarchy->GetSuperior() ) )
                    if( result = agentFactory_.Create( *parent, type, original.GetName() ) )
                    {
                        tools::Resolver< Automat_ABC >::Register( result->GetId(), *result );
                        ReplaceAutomatChildrenByAnAutomat( *automat, *result, type, position );
                    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateAutomatChilds
// Created: ABR 2011-10-25
// -----------------------------------------------------------------------------
void AgentsModel::CreateAutomatChilds( Automat_ABC& automat, const AutomatType& type, const geometry::Point2f& position )
{
    CircleFormation formation( position, type.NumberOfAgents() );
    bool pcSet = false;
    const auto& compositions = type.GetCompositions();
    for( auto composition = compositions.begin(); composition != compositions.end(); ++composition )
        for( unsigned toAdd = composition->GetSensibleNumber(); toAdd > 0; --toAdd )
            InternalCreateAgent( automat, type, *composition, formation, pcSet );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::ReplaceAutomatChildrenByAGhost
// Created: ABR 2012-07-03
// -----------------------------------------------------------------------------
void AgentsModel::ReplaceAutomatChildrenByAGhost( const kernel::Ghost_ABC& original, kernel::Automat_ABC& automat, const kernel::AutomatType& type, const geometry::Point2f& position )
{
    const kernel::Ghost_ABC::T_Children& originalChildren = original.GetChildren();
    kernel::Ghost_ABC::CIT_Children originalIterator = originalChildren.begin();
    CircleFormation formation( position, type.NumberOfAgents() );
    bool pcSet = false;

    const auto& compositions = type.GetCompositions();
    auto it = compositions.begin();
    while( originalIterator != originalChildren.end() && it != compositions.end() )
    {
        unsigned toAdd = it->GetSensibleNumber();
        for( ; toAdd > 0 && originalIterator != originalChildren.end(); --toAdd )
        {
            kernel::Ghost_ABC::T_Child child = *originalIterator++;
            const geometry::Point2f entityPosition = child.second;
            const bool isPc = !pcSet && &it->GetType() == type.GetTypePC();
            CreateAgent( automat, it->GetType(), entityPosition, isPc, child.first.c_str() );
            if( isPc )
                pcSet = true;
        }
        for( ; toAdd > 0; --toAdd ) // Type bigger than original
            InternalCreateAgent( automat, type, *it, formation, pcSet );
    }
    for( ; it != compositions.end(); ++it ) // Type bigger than original
        for( unsigned toAdd = it->GetSensibleNumber(); toAdd > 0; --toAdd )
            InternalCreateAgent( automat, type, *it, formation, pcSet );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::ReplaceAutomatChildrenByAnAutomat
// Created: ABR 2012-06-28
// -----------------------------------------------------------------------------
void AgentsModel::ReplaceAutomatChildrenByAnAutomat( const Automat_ABC& original, Automat_ABC& automat, const AutomatType& type, const geometry::Point2f& position )
{
    const TacticalHierarchies& pHierarchy = original.Get< TacticalHierarchies >();
    tools::Iterator< const Entity_ABC& > originalIterator = pHierarchy.CreateSubordinateIterator();
    CircleFormation formation( position, type.NumberOfAgents() );
    bool pcSet = false;

    const auto& compositions = type.GetCompositions();
    auto it = compositions.begin();
    while( originalIterator.HasMoreElements() && it != compositions.end() )
    {
        unsigned toAdd = it->GetSensibleNumber();
        for( ; toAdd > 0 && originalIterator.HasMoreElements(); --toAdd )
        {
            const Entity_ABC& entity = originalIterator.NextElement();
            const geometry::Point2f entityPosition = entity.Get< Positions >().GetPosition();
            const bool isPc = !pcSet && &it->GetType() == type.GetTypePC();
            CreateAgent( automat, it->GetType(), entityPosition, isPc, entity.GetName() );
            if( isPc )
                pcSet = true;
        }
        for( ; toAdd > 0; --toAdd ) // Type bigger than original
            InternalCreateAgent( automat, type, *it, formation, pcSet );
    }
    for( ; it != compositions.end(); ++it ) // Type bigger than original
        for( unsigned toAdd = it->GetSensibleNumber(); toAdd > 0; --toAdd )
            InternalCreateAgent( automat, type, *it, formation, pcSet );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::InternalCreateAgent
// Created: ABR 2012-06-28
// -----------------------------------------------------------------------------
void AgentsModel::InternalCreateAgent( Automat_ABC& automat, const AutomatType& type, const AutomatComposition& composition, CircleFormation& formation, bool& pcSet )
{
    const bool isPc = !pcSet && &composition.GetType() == type.GetTypePC();
    CreateAgent( automat, composition.GetType(), formation.NextPosition( isPc ), isPc );
    if( isPc )
        pcSet = true;
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateAutomat
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void AgentsModel::CreateAutomat( xml::xistream& xis, Entity_ABC& parent, Model& model )
{
    try
    {
        if( Automat_ABC* automat = agentFactory_.Create( xis, parent ) )
        {
            tools::Resolver< Automat_ABC >::Register( automat->GetId(), *automat );
            xis >> xml::list( "automat", *this        , &AgentsModel::CreateAutomat, *automat, model )
                >> xml::list( "unit"   , *this        , &AgentsModel::CreateAgent  , *automat, model )
                >> xml::list( "phantom", *model.ghosts_, &GhostModel::Create        , *automat )
                >> xml::list( "lima"   , *model.limits_, &LimitsModel::CreateLima   , *automat )
                >> xml::list( "limit"  , *model.limits_, &LimitsModel::CreateLimit  , *automat );
        }
        else
            model.ghosts_->Create( xis, parent, eGhostType_Automat );
    }
    catch( const std::exception& e )
    {
        model.AppendLoadingError( eOthers, tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::GetAutomat
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
Automat_ABC& AgentsModel::GetAutomat( unsigned long id )
{
    return tools::Resolver< Automat_ABC >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::FindAutomat
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
Automat_ABC* AgentsModel::FindAutomat( unsigned long id )
{
    return tools::Resolver< Automat_ABC >::Find( id );
}

namespace
{
    geometry::Point2f Clip( const geometry::Point2f& point, float width, float height )
    {
        return geometry::Point2f( std::min( std::max( point.X(), 0.f ), width )
                                , std::min( std::max( point.Y(), 0.f ), height ) );
    }

}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateAgent
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
Agent_ABC& AgentsModel::CreateAgent( Automat_ABC& parent, const AgentType& type, const geometry::Point2f& position, bool commandPost, const QString& name )
{
    Agent_ABC* agent = agentFactory_.Create( parent, type, Clip( position, width_, height_ ), commandPost, name );
    tools::Resolver< Agent_ABC >::Register( agent->GetId(), *agent );
    return *agent;
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateAgent
// Created: ABR 2011-10-25
// -----------------------------------------------------------------------------
Agent_ABC* AgentsModel::CreateAgent( Ghost_ABC& ghost, const AgentType& type, const geometry::Point2f& position )
{
    Agent_ABC* agent = agentFactory_.Create( ghost, type, Clip( position, width_, height_ ) );
    tools::Resolver< Agent_ABC >::Register( agent->GetId(), *agent );
    return agent;
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateAgent
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
void AgentsModel::CreateAgent( xml::xistream& xis, Automat_ABC& parent, Model& model )
{
    try
    {
        Agent_ABC* agent = agentFactory_.Create( xis, parent );
        if( agent )
        {
            auto unknownResources = agent->Get< InitialState >().GetUnknownResources();
            for( auto it = unknownResources.begin(); it != unknownResources.end(); ++it )
                model.AppendLoadingError( eUnknownResource, *it, agent );
            tools::Resolver< Agent_ABC >::Register( agent->GetId(), *agent );
            xis >> xml::list( "lima"   , *model.limits_, &LimitsModel::CreateLima   , *agent )
                >> xml::list( "limit"  , *model.limits_, &LimitsModel::CreateLimit  , *agent );
        }
        else
            model.ghosts_->Create( xis, parent, eGhostType_Agent );
    }
    catch( const std::exception& e )
    {
        model.AppendLoadingError( eOthers, tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::GetAgent
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
Agent_ABC& AgentsModel::GetAgent( unsigned long id ) const
{
    return tools::Resolver< Agent_ABC >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::FindAgent
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Agent_ABC* AgentsModel::FindAgent( unsigned long id ) const
{
    return tools::Resolver< Agent_ABC >::Find( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::FindAllAgent
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Entity_ABC* AgentsModel::FindAllAgent( unsigned long id ) const
{
    Entity_ABC* agent = tools::Resolver< Automat_ABC >::Find( id );
    if( agent )
        return agent;
    agent = tools::Resolver< Agent_ABC >::Find( id );
    if( agent )
        return agent;
    return tools::Resolver< Population_ABC >::Find( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreatePopulation
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void AgentsModel::CreatePopulation( Entity_ABC& parent, const PopulationType& type, int number, const geometry::Point2f& position )
{
    Population_ABC* popu = agentFactory_.Create( parent, type, number, Clip( position, width_, height_ ) );
    tools::Resolver< Population_ABC >::Register( popu->GetId(), *popu );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreatePopulation
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void AgentsModel::CreatePopulation( xml::xistream& xis, Team_ABC& parent, Model& model )
{
    try
    {
        const std::string typeName = xis.attribute< std::string >( "type" );
        const PopulationType* type = staticModel_.types_.tools::StringResolver< PopulationType >::Find( typeName );
        if( type )
        {
            Population_ABC* population = agentFactory_.Create( xis, parent, *type );
            tools::Resolver< Population_ABC >::Register( population->GetId(), *population );
        }
        else
            model.AppendLoadingError( eUnknownCrowdTypes, typeName );
    }
    catch( const std::exception& e )
    {
        model.AppendLoadingError( eOthers, tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::GetPopulation
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Population_ABC& AgentsModel::GetPopulation( unsigned long id )
{
    return tools::Resolver< Population_ABC >::Get( id );
}
//
// -----------------------------------------------------------------------------
// Name: AgentsModel::FindPopulation
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Population_ABC* AgentsModel::FindPopulation( unsigned long id )
{
    return tools::Resolver< Population_ABC>::Find( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateInhabitant
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void AgentsModel::CreateInhabitant( Entity_ABC& parent, const InhabitantType& type, int number, const QString& name, const Location_ABC& location )
{
    Inhabitant_ABC* inhab = agentFactory_.Create( parent, type, number, name, location );
    if( inhab )
        tools::Resolver< Inhabitant_ABC >::Register( inhab->GetId(), *inhab );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateInhabitant
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void AgentsModel::CreateInhabitant( xml::xistream& xis, Team_ABC& parent, Model& model )
{
    try
    {
        const std::string typeName = xis.attribute< std::string >( "type" );
        const InhabitantType* type = staticModel_.types_.tools::StringResolver< InhabitantType >::Find( typeName );
        if( type )
        {
            Inhabitant_ABC* inhabitant = agentFactory_.Create( xis, parent, *type );
            tools::Resolver< Inhabitant_ABC >::Register( inhabitant->GetId(), *inhabitant );
        }
        else
            model.AppendLoadingError( eUnknownPopulationTypes, typeName );
    }
    catch( const std::exception& e )
    {
        model.AppendLoadingError( eDeletedPopulationUrbanBlocks, tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::GetInhabitant
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
Inhabitant_ABC& AgentsModel::GetInhabitant( unsigned long id )
{
    return tools::Resolver< Inhabitant_ABC >::Get( id );
}
//
// -----------------------------------------------------------------------------
// Name: AgentsModel::FindInhabitant
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
Inhabitant_ABC* AgentsModel::FindInhabitant( unsigned long id )
{
    return tools::Resolver< Inhabitant_ABC>::Find( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::NotifyDeleted
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void AgentsModel::NotifyDeleted( const Agent_ABC& agent )
{
    tools::Resolver< Agent_ABC >::Remove( agent.GetId() );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::NotifyDeleted
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void AgentsModel::NotifyDeleted( const Automat_ABC& agent )
{
    tools::Resolver< Automat_ABC >::Remove( agent.GetId() );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::NotifyDeleted
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void AgentsModel::NotifyDeleted( const Population_ABC& agent )
{
    tools::Resolver< Population_ABC >::Remove( agent.GetId() );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::NotifyDeleted
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void AgentsModel::NotifyDeleted( const Inhabitant_ABC& agent )
{
    tools::Resolver< Inhabitant_ABC >::Remove( agent.GetId() );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::NotifyUpdated
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void AgentsModel::NotifyUpdated( const ModelLoaded& model )
{
    width_ = model.config_.GetTerrainWidth();
    height_ = model.config_.GetTerrainHeight();
}
