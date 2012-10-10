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
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/EntityHelpers.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "gaming/Dotation.h"
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
            tools::Iterator< const Dotation& > it = entity.Get< SupplyStates >().CreateIterator();
            while( it.HasMoreElements() )
                func( it.NextElement() );
        }
        else if( entity.Retrieve< TacticalHierarchies >() )
        {
            tools::Iterator< const Entity_ABC& > it = entity.Get< TacticalHierarchies >().CreateSubordinateIterator();
            while( it.HasMoreElements() )
            {
                const Entity_ABC& child = it.NextElement();
                if( !IsLogisticBase( child ) )
                    VisitBaseStocksDotations( child, func );
            }
        }
    }

}