// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __actions_Helpers_h_
#define __actions_Helpers_h_

#include "clients_kernel/Types.h"

#include <string>

namespace sword
{
    class PointList;
    class PathfindRequest;
}

namespace kernel
{
    class CoordinateConverter_ABC;
    class Entity_ABC;
}

namespace actions
{
    namespace parameters
    {
        void FillFromPointList( T_PointVector& vector,
                                const sword::PointList& list,
                                const kernel::CoordinateConverter_ABC& converter );
        void FillPathfindRequest( sword::PathfindRequest& dst,
                                  uint32_t id,
                                  const kernel::CoordinateConverter_ABC& converter,
                                  const kernel::Entity_ABC& entity,
                                  const std::vector< geometry::Point2f >& points );
    }
}

#endif // __actions_Helpers_h_
