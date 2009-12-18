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
    explicit DefaultComponentLendedFunctorComputer( OnComponentFunctor_ABC& componentOperator );
    virtual ~DefaultComponentLendedFunctorComputer();
    //@}

    //! @name Operations
    //@{
    virtual void ApplyOnLendedComponent( PHY_ComposantePion& component );
    //@}

private:
    //! @name Member data
    //@{
    OnComponentFunctor_ABC& componentOperator_;
    //@}
};

#endif // __DefaultComponentLendedFunctorComputer_h_
