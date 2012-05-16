// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __UrbanPositions_h_
#define __UrbanPositions_h_

#include "clients_kernel/UrbanTypes.h"
#include "clients_kernel/UrbanPositions.h"

namespace xml
{
    class xistream;
}

namespace kernel
{
    class CoordinateConverter_ABC;
    class UrbanObject_ABC;
}

// =============================================================================
/** @class  UrbanPositions
    @brief  Urban positions
*/
// Created: JSR 2010-09-06
// =============================================================================
class UrbanPositions : public kernel::UrbanPositions
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanPositions( xml::xistream& xis, EUrbanLevel level, const kernel::UrbanObject_ABC& object, const kernel::CoordinateConverter_ABC& converter );
    virtual ~UrbanPositions();
    //@}
};

#endif // __UrbanPositions_h_
