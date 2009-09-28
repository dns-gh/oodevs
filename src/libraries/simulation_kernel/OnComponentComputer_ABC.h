// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ComponentFunctorComputer_ABC_h_
#define __ComponentFunctorComputer_ABC_h_

class OnComponentFunctor_ABC;
class PHY_ComposantePion;

// =============================================================================
/** @class  ComponentFunctorcomputer_ABC
    @brief  ComponentFunctorcomputer_ABC
*/
// Created: MGD 2009-09-15
// =============================================================================
class OnComponentComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             OnComponentComputer_ABC();
    virtual ~OnComponentComputer_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void ApplyOnComponent( PHY_ComposantePion& component ) = 0;
    //@}
};

#endif // __ComponentFunctorComputer_ABC_h_
