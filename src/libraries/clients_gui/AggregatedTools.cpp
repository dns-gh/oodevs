// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "AggregatedTools.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/TacticalHierarchies.h"
#include <boost/bind.hpp>

bool IsAggregated( const kernel::Entity_ABC& entity )
{
    if( const kernel::Positions* positions = entity.Retrieve< kernel::Positions >() )
        return positions->IsAggregated();
    return false;
}

bool HasAggregatedSubordinate( const kernel::Entity_ABC& entity, boost::function< bool( const kernel::Entity_ABC& ) > fun )
{
    auto it = entity.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
    while( it.HasMoreElements() )
        return fun( it.NextElement() );
    return false;
}

bool HasAggregatedSubordinate( const kernel::Entity_ABC& entity )
{
    return HasAggregatedSubordinate( entity, boost::bind( &IsAggregated, _1 ) );
}
