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

#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace kernel
{
    class Automat_ABC;
    class Availability;
    class Entity_ABC;
    class Positions;
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
    void VisitEntityAndSubordinatesUpToBaseLog( const kernel::Entity_ABC& entity, boost::function< void( const kernel::Entity_ABC& ) > func );
    bool CheckEntityAndSubordinatesUpToBaseLog( const kernel::Entity_ABC& entity, boost::function< bool( const kernel::Entity_ABC& ) > func );
    
    template< typename Extension >
    struct RetrieveFunc
    {
        RetrieveFunc( const Extension* pExtension = 0 ) : pExtension_( pExtension ) {}
        bool Check( const kernel::Entity_ABC& entity )
        { 
            if( pExtension_ && entity.Retrieve< Extension >() == pExtension_ )
                return true;
            if( !pExtension_ && entity.Retrieve< Extension >() )
                return true;
            return false;
        }
        const Extension* pExtension_;
    };

    template< typename Extension >
    bool HasRetrieveEntityAndSubordinatesUpToBaseLog( const kernel::Entity_ABC& entity, const Extension* pExtension = 0 )
    {
        RetrieveFunc< Extension > retrieve( pExtension );
        return CheckEntityAndSubordinatesUpToBaseLog( entity, boost::bind( &RetrieveFunc< Extension >::Check, &retrieve, _1 ) );
    }

    const kernel::Entity_ABC* GetLogisticBase( const kernel::Entity_ABC* entity );
} // namespace

#endif // __LogisticTools_h_
