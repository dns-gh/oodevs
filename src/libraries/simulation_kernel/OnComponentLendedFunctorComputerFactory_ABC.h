// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __OnComponentLendedFunctorComputerFactory_ABC_h_
#define __OnComponentLendedFunctorComputerFactory_ABC_h_

class OnComponentFunctor_ABC;
class OnComponentLendedFunctorComputer_ABC;
// =============================================================================
/** @class  DotationComputerFactory_ABC
    @brief  DotationComputerFactory_ABC
*/
// Created: MGD 2009-08-13
// =============================================================================
class OnComponentLendedFunctorComputerFactory_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             OnComponentLendedFunctorComputerFactory_ABC();
    virtual ~OnComponentLendedFunctorComputerFactory_ABC();
    //@}

    //! @name Operations
    //@{
    virtual OnComponentLendedFunctorComputer_ABC& Create( OnComponentFunctor_ABC& componentOperator ) const = 0;
    //@}
};

#endif // __OnComponentLendedFunctorComputerFactory_ABC_h_
