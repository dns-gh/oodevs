// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LogisticHelpers.h"
#include "LogisticBase.h"
#include "LogisticHierarchiesBase.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentComposition.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/CommandPostAttributes_ABC.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/DotationCapacityType.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/StaticModel.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
namespace
{
    const kernel::Positions* GetPCPosition( const kernel::Automat_ABC& automat )
    {
        auto hierarchy = automat.Retrieve< kernel::TacticalHierarchies >();
        if( hierarchy )
        {
            auto it = hierarchy->CreateSubordinateIterator();
            while( it.HasMoreElements() )
            {
                const kernel::Entity_ABC& child = it.NextElement();
                const kernel::CommandPostAttributes_ABC* pAttributes = child.Retrieve< kernel::CommandPostAttributes_ABC >();
                if( pAttributes && pAttributes->IsCommandPost() )
                    return child.Retrieve< kernel::Positions >();
            }
        }
        return 0;
    }

    void AggregateLogisticAutomataPositions( const kernel::Entity_ABC& entity, geometry::Point2f& aggregatedPos, int& count, bool onlySupply )
    {
        if( const kernel::Automat_ABC* pRecientAutomat = dynamic_cast< const kernel::Automat_ABC* >( &entity ) )
        {
            if( ( onlySupply && pRecientAutomat->GetType().IsLogisticSupply() ) 
                || ( !onlySupply && pRecientAutomat->GetType().HasLogistics() ) )
            {
                const kernel::Positions* pPos = GetPCPosition( *pRecientAutomat );
                if( pPos )
                {
                    aggregatedPos += geometry::Vector2f( pPos->GetPosition().X(), pPos->GetPosition().Y() );
                    ++count;
                }
            }
        }
        else if( dynamic_cast< const kernel::Formation_ABC* >( &entity ) )
        {
            auto hierarchy = entity.Retrieve< kernel::TacticalHierarchies >();
            if( hierarchy )
            {
                auto it = hierarchy->CreateSubordinateIterator();
                while( it.HasMoreElements() )
                    AggregateLogisticAutomataPositions( it.NextElement(), aggregatedPos, count, onlySupply );
            }
        }
    }

    void ComputeRequirements( const kernel::Agent_ABC& agent, const kernel::LogisticSupplyClass& logType, logistic_helpers::T_Requirements& requirements, const kernel::StaticModel& staticModel )
    {
        kernel::AgentType& agentType = staticModel.types_.tools::Resolver< kernel::AgentType >::Get( agent.GetType().GetId() );
        auto agentCompositionIterator = agentType.CreateIterator();
        while( agentCompositionIterator.HasMoreElements() )
        {
            const kernel::AgentComposition& agentComposition = agentCompositionIterator.NextElement();
            const kernel::EquipmentType& equipmentType = staticModel.objectTypes_.Resolver2< kernel::EquipmentType >::Get( agentComposition.GetType().GetId() );
            auto resourcesIterator = equipmentType.CreateResourcesIterator();
            while( resourcesIterator.HasMoreElements() )
            {
                const kernel::DotationCapacityType& dotationCapacity = resourcesIterator.NextElement();
                const kernel::DotationType& category = staticModel.objectTypes_.Resolver2< kernel::DotationType >::Get( dotationCapacity.GetName() );
                if( &category.GetLogisticSupplyClass() == &logType )
                    requirements[ &category ] += static_cast< unsigned int >( agentComposition.GetCount() * dotationCapacity.GetNormalizedConsumption() + 0.5 );
            }
        }
    }

    void FillSupplyRequirements( const kernel::Entity_ABC& entity, const kernel::LogisticSupplyClass& logType, logistic_helpers::T_Requirements& requirements, const kernel::StaticModel& staticModel )
    {
        const kernel::Agent_ABC* pAgent = dynamic_cast< const kernel::Agent_ABC* >( &entity );
        if( pAgent )
            ComputeRequirements( *pAgent, logType, requirements, staticModel );
        const kernel::TacticalHierarchies* pTacticalHierarchies = entity.Retrieve< kernel::TacticalHierarchies >();
        if( pTacticalHierarchies )
        {
            auto children = pTacticalHierarchies->CreateSubordinateIterator();
            while( children.HasMoreElements() )
            {
                const kernel::Entity_ABC& childrenEntity = children.NextElement();
                FillSupplyRequirements( childrenEntity, logType, requirements, staticModel );
            }
        }
    }
}

namespace logistic_helpers
{
    // -----------------------------------------------------------------------------
    // Name: GetLogisticPosition
    // Created: MMC 2013-01-29
    // -----------------------------------------------------------------------------
    geometry::Point2f GetLogisticPosition( const kernel::Entity_ABC& entity, bool onlySupply /*= false*/ )
    {
        const kernel::Automat_ABC* pAutomat = dynamic_cast< const kernel::Automat_ABC* >( &entity );
        if( pAutomat )
        {
            const kernel::Positions* pPos = GetPCPosition( *pAutomat );
            if( pPos )
                return pPos->GetPosition();
        }
        else if( const kernel::Formation_ABC* pFormation = dynamic_cast< const kernel::Formation_ABC* >( &entity ) )
        {
            int count = 0;
            geometry::Point2f aggregatedPos;
            AggregateLogisticAutomataPositions( entity, aggregatedPos, count, onlySupply );
            if( count > 0 )
                return geometry::Point2f( aggregatedPos.X() / count, aggregatedPos.Y() / count );
        }
        if( const kernel::Positions* pPositions = entity.Retrieve< kernel::Positions >() )
            return pPositions->GetPosition();
        return geometry::Point2f();
    }

    // -----------------------------------------------------------------------------
    // Name: Tools::IsLogisticBase
    // Created: ABR 2014-01-20
    // -----------------------------------------------------------------------------
    bool IsLogisticBase( const kernel::Entity_ABC& entity )
    {
        if( const gui::LogisticBase* extension = entity.Retrieve< gui::LogisticBase >() )
            return extension->IsBase();
        return false;
    }

    void VisitAgentsWithLogisticSupply( const kernel::Entity_ABC& entity, const std::function< void( const kernel::Agent_ABC& ) >& func )
    {
        if( entity.GetTypeName() == kernel::Agent_ABC::typeName_ )
        {
            const kernel::Agent_ABC& agent = static_cast< const kernel::Agent_ABC& >( entity );
            if( agent.GetType().IsLogisticSupply() )
                func( agent );
            return;
        }
        const kernel::TacticalHierarchies* pTacticalHierarchies = entity.Retrieve< kernel::TacticalHierarchies >();
        if( !pTacticalHierarchies )
            return;
        auto children = pTacticalHierarchies->CreateSubordinateIterator();
        while( children.HasMoreElements() )
        {
            const kernel::Entity_ABC& child = children.NextElement();
            if( !IsLogisticBase( child ) )
                VisitAgentsWithLogisticSupply( child, func );
        }
    }

    void ComputeLogisticConsumptions( const kernel::StaticModel& staticModel, const kernel::Entity_ABC& logBase, const kernel::LogisticSupplyClass& logType, T_Requirements& requirements, bool forceLogisticBase )
    {
        auto hierarchiesBase = logBase.Retrieve< gui::LogisticHierarchiesBase >();
        if( !hierarchiesBase )
            return;
        auto logChildren = hierarchiesBase->CreateSubordinateIterator();
        while( logChildren.HasMoreElements() )
        {
            const kernel::Entity_ABC& entity = logChildren.NextElement();
            if( logistic_helpers::IsLogisticBase( entity ) )
            {
                if( logBase.GetId() != entity.GetId() )
                    ComputeLogisticConsumptions( staticModel, entity, logType, requirements, forceLogisticBase );
            }
            else
                FillSupplyRequirements( entity, logType, requirements, staticModel );
        }
        if( logBase.GetTypeName() == kernel::Automat_ABC::typeName_ && ( !forceLogisticBase || logistic_helpers::IsLogisticBase( logBase ) ) )
            FillSupplyRequirements( logBase, logType, requirements, staticModel );
    }

    kernel::Entity_ABC* FindLogisticEntity( const sword::ParentEntity& message,
                                            const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver,
                                            const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver )
    {
        if( message.has_automat() )
            return automatResolver.Find( message.automat().id() );
        else if( message.has_formation() )
            return formationResolver.Find( message.formation().id() );
        return 0;
    }
}
