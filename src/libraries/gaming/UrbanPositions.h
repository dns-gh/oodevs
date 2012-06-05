// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __gaming_UrbanPositions_h_
#define __gaming_UrbanPositions_h_

#include "clients_kernel/UrbanPositions.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class UrbanObject_ABC;
}

namespace sword
{
    class Location;
}

// =============================================================================
/** @class  UrbanPositions
    @brief  Urban positions
*/
// Created: LGY 2011-04-15
// =============================================================================
class UrbanPositions : public kernel::UrbanPositions
{
public:
    //! @name Constructors/Destructor
    //@{s
             UrbanPositions( const sword::Location& location, const kernel::CoordinateConverter_ABC& converter, const kernel::UrbanObject_ABC& object );
    virtual ~UrbanPositions();
    //@}
};

#endif // __gaming_UrbanPositions_h_
