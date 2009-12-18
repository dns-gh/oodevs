// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __OnComponentFunctorComputerFactory_ABC_h_
#define __OnComponentFunctorComputerFactory_ABC_h_

class OnComponentFunctor_ABC;
class OnComponentFunctorComputer_ABC;
// =============================================================================
/** @class  DotationComputerFactory_ABC
    @brief  DotationComputerFactory_ABC
*/
// Created: MGD 2009-08-13
// =============================================================================
class OnComponentFunctorComputerFactory_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             OnComponentFunctorComputerFactory_ABC();
    virtual ~OnComponentFunctorComputerFactory_ABC();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< OnComponentFunctorComputer_ABC > Create( OnComponentFunctor_ABC& componentOperator ) const = 0;
    //@}
};

#endif // __OnComponentFunctorComputerFactory_ABC_h_
