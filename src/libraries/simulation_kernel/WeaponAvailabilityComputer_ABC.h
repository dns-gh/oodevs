// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __WeaponAvailabilitycomputer_ABC_h_
#define __WeaponAvailabilitycomputer_ABC_h_

class PHY_ComposantePion;
class PHY_Weapon;

namespace firing
{

// =============================================================================
/** @class  WeaponAvailabilitycomputer_ABC
    @brief  WeaponAvailabilitycomputer_ABC
*/
// Created: MGD 2009-09-15
// =============================================================================
class WeaponAvailabilityComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             WeaponAvailabilityComputer_ABC();
    virtual ~WeaponAvailabilityComputer_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void ApplyOnWeapon( const PHY_ComposantePion& firer, PHY_Weapon& weapon ) = 0;
    //@}
};

} // namespace firing

#endif // __WeaponAvailabilitycomputer_ABC_h_
