// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DefaultWeaponAvailabilityComputer_h_
#define __DefaultWeaponAvailabilityComputer_h_

#include "simulation_kernel/WeaponAvailabilityComputer_ABC.h"

namespace firing
{
// =============================================================================
/** @class  DefaultWeaponAvailabilitycomputer
    @brief  DefaultWeaponAvailabilitycomputer
*/
// Created: MGD 2009-09-15
// =============================================================================
class DefaultWeaponAvailabilityComputer : public WeaponAvailabilityComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DefaultWeaponAvailabilityComputer();
    virtual ~DefaultWeaponAvailabilityComputer();
    //@}

    //! @name Operations
    //@{
    virtual void Reset( FireData_ABC& firerWeapons );
    virtual void ApplyOnWeapon( const PHY_ComposantePion& firer, PHY_Weapon& weapon );
    //@}

private:
    //! @name Member data
    //@{
    FireData_ABC* pFirerWeapons_; //TODO MGD Fusionner WeaponAvailabilityComputer et PHY_DirectFireData , ajouter la stratégie à ce moment
    //@}
};

} // namespace firing

#endif // __DefaultWeaponAvailabilityComputer_h_
