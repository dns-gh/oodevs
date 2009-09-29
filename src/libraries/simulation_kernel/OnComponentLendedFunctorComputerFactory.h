// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __OnComponentLendedFunctorComputerFactory_h_
#define __OnComponentLendedFunctorComputerFactory_h_

#include "simulation_kernel/OnComponentLendedFunctorComputerFactory_ABC.h"

class OnComponentFunctor_ABC;

// =============================================================================
/** @class  DotationComputerFactory
    @brief  DotationComputerFactory
*/
// Created: MGD 2009-08-13
// =============================================================================
class OnComponentLendedFunctorComputerFactory : public OnComponentLendedFunctorComputerFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             OnComponentLendedFunctorComputerFactory();
    virtual ~OnComponentLendedFunctorComputerFactory();
    //@}

    //! @name Operations
    //@{
    virtual OnComponentLendedFunctorComputer_ABC& Create( OnComponentFunctor_ABC& componentOperator ) const;
    //@}

private:
    std::auto_ptr< OnComponentLendedFunctorComputer_ABC > pDotationComputer_;
};

#endif // __DotationComputerFactory_h_
