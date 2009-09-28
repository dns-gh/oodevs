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

#include "simulation_kernel/OnComponentFunctorComputer_ABC.h"

// =============================================================================
/** @class  DefaultDotationcomputer
    @brief  DefaultDotationcomputer
*/
// Created: MGD 2009-09-24
// =============================================================================
class DefaultComponentFunctorComputer : public OnComponentFunctorComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DefaultComponentFunctorComputer();
    virtual ~DefaultComponentFunctorComputer();
    //@}

    //! @name Operations
    //@{
    virtual void Reset( OnComponentFunctor_ABC& componentOperator );
    virtual void ApplyOnComponent( PHY_ComposantePion& component );
    //@}

private:
    //! @name Member data
    //@{
    OnComponentFunctor_ABC* pConsumptionOperator_;
    //@}
};

#endif // __DefaultDotationComputer_h_
