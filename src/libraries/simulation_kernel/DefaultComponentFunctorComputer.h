// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef DEFAULTCOMPONENTFUNCTORCOMPUTER_H
#define DEFAULTCOMPONENTFUNCTORCOMPUTER_H

#include "simulation_kernel/OnComponentFunctorComputer_ABC.h"

// =============================================================================
/** @class  DefaultDotationcomputer
    @brief  Default dotation computer
*/
// Created: MGD 2009-09-24
// =============================================================================
class DefaultComponentFunctorComputer : public OnComponentFunctorComputer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DefaultComponentFunctorComputer( OnComponentFunctor_ABC& componentOperator );
    virtual ~DefaultComponentFunctorComputer();
    //@}

    //! @name Operations
    //@{
    virtual void ApplyOnComponent( PHY_ComposantePion& component );
    //@}

private:
    //! @name Member data
    //@{
    OnComponentFunctor_ABC& componentOperator_;
    //@}
};

#endif // DEFAULTCOMPONENTFUNCTORCOMPUTER_H
