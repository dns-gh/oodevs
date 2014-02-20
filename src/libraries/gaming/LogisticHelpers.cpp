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
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_gui/LogisticBase.h"
#include "clients_gui/LogisticHelpers.h"
#include "gaming/Attributes.h"
#include "gaming/SupplyStates.h"

#include <boost/bind.hpp>

using namespace kernel;

namespace logistic_helpers
{
    // -----------------------------------------------------------------------------
    void VisitBaseStocksDotations( const Entity_ABC& entity, const std::function< void( const Dotation& ) >& func )
    {
        VisitEntityAndSubordinatesUpToBaseLog( entity, [&]( const kernel::Entity_ABC& entity )
        {
            if( entity.Retrieve< SupplyStates >() )
            {
                auto it = entity.Get< SupplyStates >().CreateIterator();
                while( it.HasMoreElements() )
                    func( it.NextElement() );
            }
        } );
    }
    // -----------------------------------------------------------------------------
    void VisitEntityAndSubordinatesUpToBaseLog( const kernel::Entity_ABC& entity, const std::function< void( const kernel::Entity_ABC& ) >& func )
    {
        CheckEntityAndSubordinatesUpToBaseLog( entity, [&]( const kernel::Entity_ABC& entity ) -> bool
        {
            func( entity );
            return false;
        } );
    }
    // -----------------------------------------------------------------------------
    bool CheckEntityAndSubordinatesUpToBaseLog( const kernel::Entity_ABC& entity, const std::function< bool( const kernel::Entity_ABC& ) >& func )
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
    const kernel::Entity_ABC* GetLogisticBase( const kernel::Entity_ABC* entity )
    {
        if( !entity )
            return 0;
        if( auto base = entity->Retrieve< gui::LogisticBase >() )
            if( base->IsBase() )
                return entity;
        return GetLogisticBase( entity->Get< kernel::TacticalHierarchies >().GetSuperior() );
    }
}
