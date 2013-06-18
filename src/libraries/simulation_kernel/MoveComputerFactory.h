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
    virtual std::auto_ptr< MoveComputer_ABC > CreateMoveComputer() const;
    virtual std::auto_ptr< MoveComputer_ABC > CreateMagicMoveComputer() const;
    virtual std::auto_ptr< SpeedComputer_ABC > CreateSpeedComputer( const SpeedStrategy_ABC& strategy ) const;
    virtual std::auto_ptr< MaxSlopeComputer_ABC > CreateMaxSlopeComputer() const;
    //@}
};

}

#endif // __MoveComputerFactory_h_
