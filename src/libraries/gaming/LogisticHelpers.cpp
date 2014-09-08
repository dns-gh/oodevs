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
#include "Dotation.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Entity_ABC.h"
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
        VisitPartialBaseStocksDotations( entity, func, []( const kernel::Entity_ABC& ) { return true; } );
    }

    // -----------------------------------------------------------------------------
    void VisitPartialBaseStocksDotations( const kernel::Entity_ABC& entity,
                                          const std::function< void( const Dotation& ) >& func,
                                          const std::function< bool( const kernel::Entity_ABC& ) >& selector )
    {
        VisitEntityAndSubordinatesUpToBaseLog( entity, [&]( const kernel::Entity_ABC& entity )
        {
            if( selector( entity ) && entity.Retrieve< SupplyStates >() )
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
    float ComputeNormalizedQuantity( const kernel::StaticModel& staticModel, const kernel::Entity_ABC& logBase, const kernel::DotationType& dotationType, int quantity )
    {
        logistic_helpers::T_Requirements requirements;
        logistic_helpers::ComputeLogisticConsumptions( staticModel, logBase, dotationType.GetLogisticSupplyClass(), requirements, false );
        const float requirement = static_cast< float >( requirements[ &dotationType ] );
        return requirement > 0 ? quantity / requirement : 0;
    }
}
