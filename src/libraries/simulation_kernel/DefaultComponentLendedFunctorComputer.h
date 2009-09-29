// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DefaultComponentLendedFunctorComputer_h_
#define __DefaultComponentLendedFunctorComputer_h_

#include "simulation_kernel/OnComponentLendedFunctorComputer_ABC.h"

// =============================================================================
/** @class  DefaultDotationcomputer
    @brief  DefaultDotationcomputer
*/
// Created: MGD 2009-09-24
// =============================================================================
class DefaultComponentLendedFunctorComputer : public OnComponentLendedFunctorComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DefaultComponentLendedFunctorComputer();
    virtual ~DefaultComponentLendedFunctorComputer();
    //@}

    //! @name Operations
    //@{
    virtual void Reset( OnComponentFunctor_ABC& componentOperator );
    virtual void ApplyOnLendedComponent( PHY_ComposantePion& component );
    //@}

private:
    //! @name Member data
    //@{
    OnComponentFunctor_ABC* pComponentOperator;
    //@}
};

#endif // __DefaultComponentLendedFunctorComputer_h_
