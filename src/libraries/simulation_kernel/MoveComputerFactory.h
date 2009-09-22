// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MoveComputerFactory_h_
#define __MoveComputerFactory_h_

#include "simulation_kernel/MoveComputerFactory_ABC.h"
#include "simulation_kernel/PostureComputer_ABC.h"

namespace moving
{
// =============================================================================
/** @class  MoveComputerFactory
    @brief  MoveComputerFactory
*/
// Created: MGD 2009-08-13
// =============================================================================
class MoveComputerFactory : public MoveComputerFactory_ABC
{

public:
    //! @name ConstructorsDestructor
    //@{
             MoveComputerFactory();
    virtual ~MoveComputerFactory();
    //@}

    //! @name Operations
    //@{
    virtual MoveComputer_ABC& Create() const;
    //@}

private:
    //! @name Operations
    //@{
    std::auto_ptr< MoveComputer_ABC > pMoveComputer_;
    //@}
};

}

#endif // __MoveComputerFactory_h_
