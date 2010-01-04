// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __moving_MaxSlopeComputer_ABC_H__
#define __moving_MaxSlopeComputer_ABC_H__

#include "simulation_kernel/OnComponentComputer_ABC.h"

namespace moving
{

// =============================================================================
/** @class  MaxSlopeComputer_ABC
    @brief  MaxSlopeComputer_ABC
*/
// Created: AHC 2009-10-01
// =============================================================================
class MaxSlopeComputer_ABC : public OnComponentComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MaxSlopeComputer_ABC() {}
    virtual ~MaxSlopeComputer_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual double GetMaxSlope() const = 0;
    //@}
};

}

#endif /* __moving_MaxSlopeComputer_ABC_H__ */
