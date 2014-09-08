// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gui_LogisticHelpers_h_
#define __gui_LogisticHelpers_h_

#include <tools/Resolver_ABC.h>

namespace kernel
{
    class Agent_ABC;
    class Automat_ABC;
    class DotationType;
    class Entity_ABC;
    class Formation_ABC;
    class LogisticSupplyClass;
    class StaticModel;
}

namespace sword
{
    class ParentEntity;
}

// =============================================================================
/** @namespace logistic_helpers
    @brief  logistic helpers...
*/
// Created: MMC 2012-10-10
// =============================================================================
namespace logistic_helpers
{
    geometry::Point2f GetLogisticPosition( const kernel::Entity_ABC& entity, bool onlySupply = false );
    bool IsLogisticBase( const kernel::Entity_ABC& entity );
    void VisitAgentsWithLogisticSupply( const kernel::Entity_ABC& entity, const std::function< void( const kernel::Agent_ABC& ) >& func );

    typedef std::map< const kernel::DotationType*, unsigned int > T_Requirements;
    void ComputeLogisticConsumptions( const kernel::StaticModel& staticModel, const kernel::Entity_ABC& logBase,
                                      const kernel::LogisticSupplyClass& logType, T_Requirements& requirements, bool forceLogisticBase );

    kernel::Entity_ABC* FindParentEntity( const sword::ParentEntity& message,
                                          const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver,
                                          const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver );

    const kernel::Entity_ABC* GetLogisticBase( const kernel::Entity_ABC* entity );
}

#endif // __gui_LogisticHelpers_h_
