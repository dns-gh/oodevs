// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __moving_DefaultMaxSlopeComputer_H__
#define __moving_DefaultMaxSlopeComputer_H__

#include "simulation_kernel/MaxSlopeComputer_ABC.h"

namespace moving
{

// =============================================================================
/** @class  DefaultMaxSlopeComputer
    @brief  DefaultMaxSlopeComputer
*/
// Created: AHC 2009-10-01
// =============================================================================
class DefaultMaxSlopeComputer : public MaxSlopeComputer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    DefaultMaxSlopeComputer();
    virtual ~DefaultMaxSlopeComputer();
    //@}

    //! @name Operations
    //@{
    void ApplyOnComponent( PHY_ComposantePion& component );
    void Reset();
    double GetMaxSlope() const;
    //@}
private:
    //! @name Attributes
    //@{
    double maxSlope_;
    bool hasUsableComponent_;
    //@}
};

}

#endif /* __moving_DefaultMaxSlopeComputer_H__ */
