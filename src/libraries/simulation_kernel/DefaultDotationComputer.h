// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DefaultDotationComputer_h_
#define __DefaultDotationComputer_h_

#include "simulation_kernel/DotationComputer_ABC.h"

namespace dotation
{

// =============================================================================
/** @class  DefaultDotationcomputer
    @brief  DefaultDotationcomputer
*/
// Created: MGD 2009-09-24
// =============================================================================
class DefaultDotationComputer : public DotationComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DefaultDotationComputer();
    virtual ~DefaultDotationComputer();
    //@}

    //! @name Operations
    //@{
    virtual void Reset( ConsumptionOperator_ABC& consumptionOperator );
    virtual void ApplyOnComponent( PHY_ComposantePion& component );
    //@}

private:
    //! @name Member data
    //@{
    ConsumptionOperator_ABC* pConsumptionOperator_; //TODO MGD Fusionner DotationComputer et ConsumptionOperator
    //@}
};

} // namespace firing

#endif // __DefaultDotationComputer_h_
