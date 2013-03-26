// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "LogisticHelpers.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/EntityHelpers.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "gaming/Attributes.h"
#include "gaming/Dotation.h"
#include "gaming/LogisticLinks.h"
#include "gaming/SupplyStates.h"
#include <boost/bind.hpp>

using namespace kernel;
using namespace EntityHelpers;

namespace logistic_helpers
{
    // -----------------------------------------------------------------------------
    namespace
    {
        struct VisitDotationsFunc
        {
            VisitDotationsFunc( boost::function< void( const Dotation& ) > func ) : func_( func ) {}
            void Call( const kernel::Entity_ABC& entity )
            {
                if( entity.Retrieve< SupplyStates >() )
                {
                    auto it = entity.Get< SupplyStates >().CreateIterator();
                    while( it.HasMoreElements() )
                        func_( it.NextElement() );
                }
            }

            boost::function< void( const Dotation& ) > func_;
        };
    }

    // -----------------------------------------------------------------------------
    // Name: VisitBaseStocksDotations
    // Created: MMC 2012-10-10
    // -----------------------------------------------------------------------------
    void VisitBaseStocksDotations( const Entity_ABC& entity, boost::function< void( const Dotation& ) > func )
    {
        VisitDotationsFunc visitFunc( func );
        VisitEntityAndSubordinatesUpToBaseLog( entity, boost::bind( &VisitDotationsFunc::Call, &visitFunc, _1 ) );
    }

    // -----------------------------------------------------------------------------
    namespace
    {
        struct ProxyReturnFalseFunc
        {
            ProxyReturnFalseFunc( boost::function< void( const kernel::Entity_ABC& ) > func ) : func_( func ) {}
            bool Call( const kernel::Entity_ABC& entity ) { func_( entity ); return false; }

            boost::function< void( const kernel::Entity_ABC& ) > func_;
        };
    }

    // -----------------------------------------------------------------------------
    // Name: VisitEntityAndSubordinatesUpToBaseLog
    // Created: MMC 2012-01-23
    // -----------------------------------------------------------------------------
    void VisitEntityAndSubordinatesUpToBaseLog( const kernel::Entity_ABC& entity, boost::function< void( const kernel::Entity_ABC& ) > func )
    {
        logistic_helpers::ProxyReturnFalseFunc checkFunc( func );
        CheckEntityAndSubordinatesUpToBaseLog( entity, boost::bind( &ProxyReturnFalseFunc::Call, &checkFunc, _1 ) );
    }

    // -----------------------------------------------------------------------------
    // Name: CheckEntityAndSubordinatesUpToBaseLog
    // Created: MMC 2012-01-23
    // -----------------------------------------------------------------------------
    bool CheckEntityAndSubordinatesUpToBaseLog( const kernel::Entity_ABC& entity, boost::function< bool( const kernel::Entity_ABC& ) > func )
    {
        if( func( entity ) )
            return true;
        if( auto tactical = entity.Retrieve< TacticalHierarchies >() )
        {
            auto it = tactical->CreateSubordinateIterator();
            while( it.HasMoreElements() )
            {
                const Entity_ABC& child = it.NextElement();
                if( !IsLogisticBase( child ) )
                    if( CheckEntityAndSubordinatesUpToBaseLog( child, func ) )
                        return true;
            }
        }
        return false;
    }

    // -----------------------------------------------------------------------------
    namespace
    {
        const kernel::Positions* GetPCPosition( const Automat_ABC& automat )
        {
            auto hierarchy = automat.Retrieve< TacticalHierarchies >();
            if( hierarchy )
            {
                auto it = hierarchy->CreateSubordinateIterator();
                while( it.HasMoreElements() )
                {
                    const Entity_ABC& child = it.NextElement();
                    auto pAttributes = child.Retrieve< Attributes >();
                    if( pAttributes && pAttributes->isPC_ )
                        return child.Retrieve< kernel::Positions >();
                }
            }
            return 0;
        }

        void AggregateLogisticAutomataPositions( const Entity_ABC& entity, geometry::Point2f& aggregatedPos, int& count, bool onlySupply )
        {
            if( const kernel::Automat_ABC* pRecientAutomat = dynamic_cast< const Automat_ABC* >( &entity ) )
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
                auto hierarchy = entity.Retrieve< TacticalHierarchies >();
                if( hierarchy )
                {
                    auto it = hierarchy->CreateSubordinateIterator();
                    while( it.HasMoreElements() )
                        AggregateLogisticAutomataPositions( it.NextElement(), aggregatedPos, count, onlySupply );
                }
            }
        }
    }

    // -----------------------------------------------------------------------------
    // Name: GetLogisticPosition
    // Created: MMC 2013-01-29
    // -----------------------------------------------------------------------------
    geometry::Point2f GetLogisticPosition( const Entity_ABC& entity, bool onlySupply /*= false*/ )
    {
        const kernel::Automat_ABC* pAutomat = dynamic_cast< const Automat_ABC* >( &entity );
        if( pAutomat && pAutomat->GetLogisticLevel() == LogisticLevel::logistic_base_ )
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
}