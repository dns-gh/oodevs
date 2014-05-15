// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __DisasterImpactComputer_h_
#define __DisasterImpactComputer_h_

#include "OnComponentComputer_ABC.h"

// =============================================================================
/** @class  DisasterImpactComputer
    @brief  DisasterImpactComputer
*/
// Created: JSR 2014-05-15
// =============================================================================
class DisasterImpactComputer : public OnComponentComputer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DisasterImpactComputer( double value );
    virtual ~DisasterImpactComputer();
    //@}

public:
    //! @name Operations
    //@{
    virtual void ApplyOnComponent( PHY_ComposantePion& composante );
    double GetModifier() const;
    //@}

private:
    //! @name Member data
    //@{
    double modifier_;
    double value_;
    //@}
};

#endif // __DisasterImpactComputer_h_
