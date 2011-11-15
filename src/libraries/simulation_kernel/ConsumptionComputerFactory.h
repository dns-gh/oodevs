// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ConsumptionComputerFactory_h_
#define __ConsumptionComputerFactory_h_

#include "simulation_kernel/ConsumptionComputerFactory_ABC.h"
#include "simulation_kernel/PostureComputer_ABC.h"

namespace dotation
{
// =============================================================================
/** @class  ConsumptionComputerFactory
    @brief  ConsumptionComputerFactory
*/
// Created: MGD 2009-08-13
// =============================================================================
class ConsumptionComputerFactory : public ConsumptionComputerFactory_ABC
{
public:
    //! @name ConstructorsDestructor
    //@{
             ConsumptionComputerFactory();
    virtual ~ConsumptionComputerFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< ConsumptionComputer_ABC > CreateConsumptionComputer() const;
    virtual std::auto_ptr< ConsumptionModeChangeRequest_ABC > CreateConsumptionModeChangeRequest( const PHY_ConsumptionType& ) const;
    //@}
};

}

#endif // __ConsumptionComputerFactory_h_
