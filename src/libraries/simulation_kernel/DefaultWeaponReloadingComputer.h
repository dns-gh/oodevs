// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DefaultWeaponReloadingComputer_h_
#define __DefaultWeaponReloadingComputer_h_

#include "simulation_kernel/WeaponReloadingComputer_ABC.h"

namespace firing
{

// =============================================================================
/** @class  DefaultWeaponReloadingComputer
    @brief  DefaultWeaponReloadingComputer
*/
// Created: MGD 2009-10-05
// =============================================================================
class DefaultWeaponReloadingComputer : public WeaponReloadingComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit DefaultWeaponReloadingComputer( double initialDuration );
    virtual ~DefaultWeaponReloadingComputer();
    //@}

    //! @name Operations
    //@{
    virtual void AddModifier( double modifier );
    virtual double GetDuration() const;
    //@}

private:
    //! @name Member data
    //@{
    std::vector< double > modifiers_;
    double initialDuration_;
    //@}
};

} // namespace firing

#endif // __DefaultWeaponReloadingComputer_h_
