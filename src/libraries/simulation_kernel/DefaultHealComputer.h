// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DefaultHealableComputer_h_
#define __DefaultHealableComputer_h_

#include "simulation_kernel/HealComputer_ABC.h"
#include "simulation_kernel/Entities/Agents/Units/Composantes/PHY_ComposantePion.h"

namespace human
{

// =============================================================================
/** @class  DefaultHealableComputer
    @brief  DefaultHealableComputer
*/
// Created: MGD 2009-09-15
// =============================================================================
class DefaultHealComputer : public HealComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DefaultHealComputer();
    virtual ~DefaultHealComputer();
    //@}

    //! @name Operations
    //@{
    virtual void Reset( const PHY_HumanRank& rank, unsigned int nNbrToChange );
    virtual void ApplyOnComposante( PHY_ComposantePion& component );
    virtual void Heal() const;
    //@}

private:
    //! @name Attribute
    //@{
    PHY_ComposantePion::T_ComposantePionVector healablesComponents_;
    const PHY_HumanRank* pRank_;
    unsigned int nNbrToChange_;
    //@}

};

} // namespace human

#endif // __DefaultHealableComputer_h_
