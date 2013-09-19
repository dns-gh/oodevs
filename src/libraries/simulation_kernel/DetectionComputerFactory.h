// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DetectionComputerFactory_h_
#define __DetectionComputerFactory_h_

#include "simulation_kernel/DetectionComputerFactory_ABC.h"

namespace detection
{
// =============================================================================
/** @class  DetectionComputerFactory
    @brief  DetectionComputerFactory
*/
// Created: MGD 2009-08-13
// =============================================================================
class DetectionComputerFactory : public DetectionComputerFactory_ABC
{
public:
    //! @name ConstructorsDestructor
    //@{
             DetectionComputerFactory();
    virtual ~DetectionComputerFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< DetectionComputer_ABC > Create( MIL_Agent_ABC& target ) const;
    //@}
};

}

#endif // __DetectionComputerFactory_h_
