// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __OnComponentFunctorComputerFactory_h_
#define __OnComponentFunctorComputerFactory_h_

#include "simulation_kernel/OnComponentFunctorComputerFactory_ABC.h"

class OnComponentFunctor_ABC;
class OnComponentFunctorComputer_ABC;

// =============================================================================
/** @class  DotationComputerFactory
    @brief  DotationComputerFactory
*/
// Created: MGD 2009-08-13
// =============================================================================
class OnComponentFunctorComputerFactory : public OnComponentFunctorComputerFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             OnComponentFunctorComputerFactory();
    virtual ~OnComponentFunctorComputerFactory();
    //@}

    //! @name Operations
    //@{
    virtual OnComponentComputer_ABC& Create( OnComponentFunctor_ABC& componentOperator ) const;
    //@}

private:
    std::auto_ptr< OnComponentFunctorComputer_ABC > pDotationComputer_;
};

#endif // __OnComponentFunctorComputerFactory_h_
