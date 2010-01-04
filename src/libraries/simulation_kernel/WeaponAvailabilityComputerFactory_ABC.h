// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __WeaponAvailabilityComputerFactory_ABC_h_
#define __WeaponAvailabilityComputerFactory_ABC_h_

namespace firing
{
    class FireData_ABC;
    class WeaponAvailabilityComputer_ABC;

// =============================================================================
/** @class  WeaponAvailabilityComputerFactory_ABC
    @brief  WeaponAvailabilityComputerFactory_ABC
*/
// Created: MGD 2009-08-13
// =============================================================================
class WeaponAvailabilityComputerFactory_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             WeaponAvailabilityComputerFactory_ABC() {}
    virtual ~WeaponAvailabilityComputerFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< WeaponAvailabilityComputer_ABC > Create( FireData_ABC& firerWeapons ) const = 0;
    //@}
};

}

#endif // __WeaponAvailabilityComputerFactory_ABC_h_
