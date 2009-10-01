// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DotationComputerFactory_h_
#define __DotationComputerFactory_h_

#include "simulation_kernel/DotationComputerFactory_ABC.h"
#include "simulation_kernel/PostureComputer_ABC.h"

namespace dotation
{
// =============================================================================
/** @class  DotationComputerFactory
    @brief  DotationComputerFactory
*/
// Created: MGD 2009-08-13
// =============================================================================
class DotationComputerFactory : public DotationComputerFactory_ABC
{

public:
    //! @name ConstructorsDestructor
    //@{
             DotationComputerFactory();
    virtual ~DotationComputerFactory();
    //@}

    //! @name Operations
    //@{
    virtual DotationComputer_ABC& Create() const;
    //@}

private:
    //! @name Operations
    //@{
    std::auto_ptr< DotationComputer_ABC > pDotationComputer_;
    //@}
};

}

#endif // __DotationComputerFactory_h_
