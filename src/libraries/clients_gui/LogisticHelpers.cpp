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

#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/CommandPostAttributes_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/TacticalHierarchies.h"

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
}
