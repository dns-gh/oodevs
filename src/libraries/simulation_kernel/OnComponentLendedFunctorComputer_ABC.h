// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __OnComponentLendFunctorComputer_ABC_h_
#define __OnComponentLendFunctorComputer_ABC_h_

class OnComponentFunctor_ABC;
class PHY_ComposantePion;
// =============================================================================
/** @class  Dotationcomputer_ABC
@brief  Dotationcomputer_ABC
*/
// Created: MGD 2009-09-15
// =============================================================================
class OnComponentLendedFunctorComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             OnComponentLendedFunctorComputer_ABC() {}
    virtual ~OnComponentLendedFunctorComputer_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void ApplyOnLendedComponent( PHY_ComposantePion& component ) = 0;
    //@}
};

#endif // __DotationComputer_ABC_h_
