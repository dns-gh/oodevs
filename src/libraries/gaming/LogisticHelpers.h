// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticTools_h_
#define __LogisticTools_h_

#include <boost/function.hpp>
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/EntityHelpers.h"

namespace kernel
{
    class Entity_ABC;
}

class Dotation;

// =============================================================================
/** @namespace logistic_helpers
    @brief  logistic helpers...
*/
// Created: MMC 2012-10-10
// =============================================================================
namespace logistic_helpers
{
    void VisitBaseStocksDotations( const kernel::Entity_ABC& logisticBase, boost::function< void( const Dotation& ) > func );    
    bool CheckEntityAndSubordinatesUpToBaseLog( const kernel::Entity_ABC& entity, boost::function< bool( const kernel::Entity_ABC& ) > func );

    template< typename UnaryFunction >
    void VisitEntityAndSubordinatesUpToBaseLog( const kernel::Entity_ABC& entity, UnaryFunction& func )
    {
        func( entity );
        if( entity.Retrieve< kernel::TacticalHierarchies >() )
        {
            tools::Iterator< const kernel::Entity_ABC& > it = entity.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
            while( it.HasMoreElements() )
            {
                const kernel::Entity_ABC& child = it.NextElement();
                if( !kernel::EntityHelpers::IsLogisticBase( child ) )
                    VisitEntityAndSubordinatesUpToBaseLog( child, func );
            }
        }
    }

    template< typename Extension >
    bool HasRetrieveEntityAndSubordinatesUpToBaseLog( const kernel::Entity_ABC& entity, const Extension& extension )
    {
        if( entity.Retrieve< Extension >() == &extension )
            return true;
        if( entity.Retrieve< kernel::TacticalHierarchies >() )
        {
            tools::Iterator< const kernel::Entity_ABC& > it = entity.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
            while( it.HasMoreElements() )
            {
                const kernel::Entity_ABC& child = it.NextElement();
                if( !kernel::EntityHelpers::IsLogisticBase( child ) )
                    if( HasRetrieveEntityAndSubordinatesUpToBaseLog( child, extension ) )
                        return true;
            }
        }
        return false;
    }

    template< typename Extension >
    bool HasRetrieveEntityAndSubordinatesUpToBaseLog( const kernel::Entity_ABC& entity )
    {
        if( entity.Retrieve< Extension >() )
            return true;
        if( entity.Retrieve< kernel::TacticalHierarchies >() )
        {
            tools::Iterator< const kernel::Entity_ABC& > it = entity.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
            while( it.HasMoreElements() )
            {
                const kernel::Entity_ABC& child = it.NextElement();
                if( !kernel::EntityHelpers::IsLogisticBase( child ) )
                    if( HasRetrieveEntityAndSubordinatesUpToBaseLog< Extension >( child ) )
                        return true;
            }
        }
        return false;
    }

} // namespace

#endif // __LogisticTools_h_
