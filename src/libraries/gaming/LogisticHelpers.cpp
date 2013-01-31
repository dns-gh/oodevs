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
#include "gaming/Dotation.h"
#include "gaming/LogisticLinks.h"
#include "gaming/SupplyStates.h"

using namespace kernel;
using namespace EntityHelpers;

namespace logistic_helpers
{
    // -----------------------------------------------------------------------------
    // Name: VisitBaseStocksDotations
    // Created: MMC 2012-10-10
    // -----------------------------------------------------------------------------
    void VisitBaseStocksDotations( const Entity_ABC& entity, boost::function< void( const Dotation& ) > func )
    {
        if( entity.Retrieve< SupplyStates >() )
        {
            auto it = entity.Get< SupplyStates >().CreateIterator();
            while( it.HasMoreElements() )
                func( it.NextElement() );
        }
        else if( auto tactical = entity.Retrieve< TacticalHierarchies >() )
        {
            auto it = tactical->CreateSubordinateIterator();
            while( it.HasMoreElements() )
            {
                const Entity_ABC& child = it.NextElement();
                if( !IsLogisticBase( child ) )
                    VisitBaseStocksDotations( child, func );
            }
        }
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
}