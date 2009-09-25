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

namespace dotation
{
    class ConsumptionOperator_ABC;
// =============================================================================
/** @class  DotationComputerFactory
    @brief  DotationComputerFactory
*/
// Created: MGD 2009-08-13
// =============================================================================
class DotationComputerFactory : public DotationComputerFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DotationComputerFactory();
    virtual ~DotationComputerFactory();
    //@}

    //! @name Operations
    //@{
    virtual DotationComputer_ABC& Create( ConsumptionOperator_ABC& consumptionOperator ) const;
    //@}

private:
    std::auto_ptr< DotationComputer_ABC > pDotationComputer_;
};

} // namespace dotation

#endif // __DotationComputerFactory_h_
