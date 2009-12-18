// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __WeaponReloadingComputerFactory_h_
#define __WeaponReloadingComputerFactory_h_

#include "simulation_kernel/WeaponReloadingComputerFactory_ABC.h"

namespace firing
{

// =============================================================================
/** @class  WeaponReloadingComputerFactory
    @brief  WeaponReloadingComputerFactory
*/
// Created: MGD 2009-08-13
// =============================================================================
class WeaponReloadingComputerFactory : public WeaponReloadingComputerFactory_ABC
{

public:
    //! @name ConstructorsDestructor
    //@{
             WeaponReloadingComputerFactory();
    virtual ~WeaponReloadingComputerFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< WeaponReloadingComputer_ABC > Create( double duration ) const;
    //@}
};

}

#endif // __WeaponReloadingComputerFactory_h_
