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
    // Name: LogisticHelpers::GetLogisticBase
    // Created: ABR 2014-02-07
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
