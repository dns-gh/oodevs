// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __moving_SlopeDecelerationComputer_ABC_H__
#define __moving_SlopeDecelerationComputer_ABC_H__

#include "simulation_kernel/OnComponentComputer_ABC.h"

namespace moving
{

// =============================================================================
/** @class  SlopeDecelerationComputer_ABC
    @brief  SlopeDecelerationComputer_ABC
*/
// Created: JSR 2013-11-08
// =============================================================================
class SlopeDecelerationComputer_ABC : public OnComponentComputer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SlopeDecelerationComputer_ABC() {}
    virtual ~SlopeDecelerationComputer_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual double GetSlopeDeceleration() const = 0;
    //@}
};

}

#endif /* __moving_SlopeDecelerationComputer_ABC_H__ */
