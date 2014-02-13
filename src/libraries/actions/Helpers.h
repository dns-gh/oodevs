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
}

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace actions
{
    namespace parameters
    {
        void FillFromPointList( T_PointVector& vector,
                                const sword::PointList& list,
                                const kernel::CoordinateConverter_ABC& converter );
    }
}

#endif // __actions_Helpers_h_
