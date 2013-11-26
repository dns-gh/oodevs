// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __moving_SlopeDecelerationComputer_H__
#define __moving_SlopeDecelerationComputer_H__

#include "OnComponentComputer_ABC.h"

class PHY_ComposantePion;

namespace moving
{

// =============================================================================
/** @class  SlopeDecelerationComputer
    @brief  SlopeDecelerationComputer
*/
// Created: JSR 2013-11-08

// =============================================================================
class SlopeDecelerationComputer : public OnComponentComputer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SlopeDecelerationComputer();
    virtual ~SlopeDecelerationComputer();
    //@}

    //! @name Operations
    //@{
    virtual void ApplyOnComponent( PHY_ComposantePion& component );
    virtual double GetSlopeDeceleration() const;
    //@}
private:
    //! @name Attributes
    //@{
    double deceleration_;
    //@}
};

}

#endif /* __moving_SlopeDecelerationComputer_H__ */
