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

namespace kernel
{
    class Agent_ABC;
    class Entity_ABC;
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
}

#endif // __gui_LogisticHelpers_h_
