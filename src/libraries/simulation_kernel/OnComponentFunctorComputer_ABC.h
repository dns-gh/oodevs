// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __OnComponentFunctorComputer_ABC_h_
#define __OnComponentFunctorComputer_ABC_h_

#include "simulation_kernel/OnComponentComputer_ABC.h"

class OnComponentFunctor_ABC;
// =============================================================================
/** @class  Dotationcomputer_ABC
@brief  Dotationcomputer_ABC
*/
// Created: MGD 2009-09-15
// =============================================================================
class OnComponentFunctorComputer_ABC : public OnComponentComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    OnComponentFunctorComputer_ABC();
    virtual ~OnComponentFunctorComputer_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Reset( OnComponentFunctor_ABC& componentOperator ) = 0;
    virtual void ApplyOnComponent( PHY_ComposantePion& component ) = 0;
    //@}
};

#endif // __OnComponentFunctorComputer_ABC_h_
