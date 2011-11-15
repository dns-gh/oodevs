// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __WeaponReloadingComputer_ABC_h_
#define __WeaponReloadingComputer_ABC_h_

#include "simulation_kernel/OnComponentComputer_ABC.h"
#include "simulation_kernel/Entities/Agents/Units/Composantes/PHY_Composante_ABC.h"

class PHY_ComposantePion;

namespace firing
{

// =============================================================================
/** @class  WeaponReloadingComputer_ABC
    @brief  WeaponReloadingComputer_ABC
*/
// Created: MGD 2009-09-15
// =============================================================================
class WeaponReloadingComputer_ABC
{
public:
    virtual ~WeaponReloadingComputer_ABC() {}
    //! @name Operations
    //@{
    virtual void AddModifier( double modifier ) = 0;
    virtual double GetDuration() const = 0;
    //@}
};

} // namespace firing

#endif // __WeaponReloadingComputer_ABC_h_
