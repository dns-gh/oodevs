// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __WeaponReloadingComputerFactory_ABC_h_
#define __WeaponReloadingComputerFactory_ABC_h_

namespace firing
{
    class WeaponReloadingComputer_ABC;
// =============================================================================
/** @class  WeaponReloadingComputerFactory_ABC
    @brief  WeaponReloadingComputerFactory_ABC
*/
// Created: MGD 2009-08-13
// =============================================================================
class WeaponReloadingComputerFactory_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             WeaponReloadingComputerFactory_ABC();
    virtual ~WeaponReloadingComputerFactory_ABC();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< WeaponReloadingComputer_ABC > Create( double duration ) const = 0;
    //@}
};

}

#endif // __WeaponReloadingComputerFactory_ABC_h_
