// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ConsumptionComputerFactory_ABC_h_
#define __ConsumptionComputerFactory_ABC_h_

namespace dotation
{
    class ConsumptionComputer_ABC;
// =============================================================================
/** @class  ConsumptionComputerFactory_ABC
    @brief  ConsumptionComputerFactory_ABC
*/
// Created: MGD 2009-08-13
// =============================================================================
class ConsumptionComputerFactory_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             ConsumptionComputerFactory_ABC();
    virtual ~ConsumptionComputerFactory_ABC();
    //@}

    //! @name Operations
    //@{
    virtual ConsumptionComputer_ABC& Create() const = 0;
    //@}
};

}

#endif // __ConsumptionComputerFactory_ABC_h_
