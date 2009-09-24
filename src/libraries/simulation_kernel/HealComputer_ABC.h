// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __HealableComputer_ABC_h_
#define __HealableComputer_ABC_h_

#include "simulation_kernel/Entities/Agents/Units/Humans/PHY_Human.h"

class PHY_ComposantePion;

namespace human
{

// =============================================================================
/** @class  HealableComputer_ABC
    @brief  HealableComputer_ABC
*/
// Created: MGD 2009-09-24
// =============================================================================
class HealComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             HealComputer_ABC();
    virtual ~HealComputer_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Reset( const PHY_HumanRank& rank, unsigned int nNbrToChange ) = 0;
    virtual void ApplyOnComposante( PHY_ComposantePion& component ) = 0;
    virtual void Heal() const = 0;
    //@}
};

} // namespace human

#endif // __HealableComputer_ABC_h_
