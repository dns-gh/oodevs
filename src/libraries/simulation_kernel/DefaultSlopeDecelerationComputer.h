// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __moving_DefaultSlopeDecelerationComputer_H__
#define __moving_DefaultSlopeDecelerationComputer_H__

#include "simulation_kernel/SlopeDecelerationComputer_ABC.h"

namespace moving
{

// =============================================================================
/** @class  DefaultSlopeDecelerationComputer
    @brief  DefaultSlopeDecelerationComputer
*/
// Created: JSR 2013-11-08

// =============================================================================
class DefaultSlopeDecelerationComputer : public SlopeDecelerationComputer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DefaultSlopeDecelerationComputer();
    virtual ~DefaultSlopeDecelerationComputer();
    //@}

    //! @name Operations
    //@{
    virtual void ApplyOnComponent( PHY_ComposantePion& component );
    virtual double GetSlopeDeceleration() const;
    //@}
private:
    //! @name Attributes
    //@{
    double slopeDeceleration_;
    //@}
};

}

#endif /* __moving_DefaultSlopeDecelerationComputer_H__ */
