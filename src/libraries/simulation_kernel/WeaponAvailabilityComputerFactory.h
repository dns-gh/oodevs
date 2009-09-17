// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __WeaponAvailabilityComputerFactory_h_
#define __WeaponAvailabilityComputerFactory_h_

#include "simulation_kernel/WeaponAvailabilityComputerFactory_ABC.h"

class MIL_AgentPion;
class MIL_AgentTypePion;
class MIL_Automate;

namespace xml
{
    class xistream;
}

namespace firing
{

// =============================================================================
/** @class  WeaponAvailabilityComputerFactory
    @brief  WeaponAvailabilityComputerFactory
*/
// Created: MGD 2009-08-13
// =============================================================================
class WeaponAvailabilityComputerFactory : public WeaponAvailabilityComputerFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             WeaponAvailabilityComputerFactory();
    virtual ~WeaponAvailabilityComputerFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< WeaponAvailabilityComputer_ABC > Create( PHY_DirectFireData& firerWeapons ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    WeaponAvailabilityComputerFactory( const WeaponAvailabilityComputerFactory& );            //!< Copy constructor
    WeaponAvailabilityComputerFactory& operator=( const WeaponAvailabilityComputerFactory& ); //!< Assignment operator
    //@}
};

}

#endif // __WeaponAvailabilityComputerFactory_h_
