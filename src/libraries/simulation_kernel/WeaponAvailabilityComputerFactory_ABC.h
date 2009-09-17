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

class MIL_AgentPion;
class MIL_AgentTypePion;
class MIL_Automate;

class PHY_DirectFireData;

namespace xml
{
    class xistream;
}

namespace firing
{
    class WeaponAvailabilityComputer_ABC;
// =============================================================================
/** @class  WeaponAvailabilityComputerFactory_ABC
    @brief  WeaponAvailabilityComputerFactory_ABC
*/
// Created: MGD 2009-08-13
// =============================================================================
class WeaponAvailabilityComputerFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             WeaponAvailabilityComputerFactory_ABC();
    virtual ~WeaponAvailabilityComputerFactory_ABC();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< WeaponAvailabilityComputer_ABC > Create( PHY_DirectFireData& firerWeapons ) const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    WeaponAvailabilityComputerFactory_ABC( const WeaponAvailabilityComputerFactory_ABC& );            //!< Copy constructor
    WeaponAvailabilityComputerFactory_ABC& operator=( const WeaponAvailabilityComputerFactory_ABC& ); //!< Assignment operator
    //@}
};

}

#endif // __WeaponAvailabilityComputerFactory_ABC_h_
