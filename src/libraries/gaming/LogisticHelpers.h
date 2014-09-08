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

namespace kernel
{
    class Automat_ABC;
    class Availability;
    class DotationType;
    class Entity_ABC;
    class Positions;
    class StaticModel;
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
    void VisitBaseStocksDotations( const kernel::Entity_ABC& entity,
                                   const std::function< void( const Dotation& ) >& func );
    void VisitPartialBaseStocksDotations( const kernel::Entity_ABC& entity,
                                          const std::function< void( const Dotation& ) >& func,
                                          const std::function< bool( const kernel::Entity_ABC& ) >& selector );
    void VisitEntityAndSubordinatesUpToBaseLog( const kernel::Entity_ABC& entity,
                                                const std::function< void( const kernel::Entity_ABC& ) >& func );
    bool CheckEntityAndSubordinatesUpToBaseLog( const kernel::Entity_ABC& entity,
                                                const std::function< bool( const kernel::Entity_ABC& ) >& func );

    template< typename Extension >
    bool HasRetrieveEntityAndSubordinatesUpToBaseLog( const kernel::Entity_ABC& entity, const Extension* pExtension = 0 )
    {
        return CheckEntityAndSubordinatesUpToBaseLog( entity, [&]( const kernel::Entity_ABC& entity ) -> bool
        {
            auto ptr = entity.Retrieve< Extension >();
            return ptr && ( !pExtension || pExtension == ptr );
        } );
    }

    float ComputeNormalizedQuantity( const kernel::StaticModel& staticModel, const kernel::Entity_ABC& logBase, const kernel::DotationType& dotation, int quantity );

} // namespace logistic_helpers

#endif // __LogisticTools_h_
