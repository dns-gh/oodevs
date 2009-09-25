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
    virtual void Reset();
    virtual void ApplyOnComponent( PHY_ComposantePion& component );
    virtual void Heal( const PHY_HumanRank& rank, unsigned int nNbrToChange ) const;
    virtual void Wound( const PHY_HumanRank& rank, unsigned int nNbrToChange ) const;
    virtual void HealAll() const;
    virtual void EvacuateWoundedHumans( MIL_AutomateLOG& destinationTC2 ) const;
    virtual bool HasWoundedHumansToEvacuate() const;
    //@}

private:
    //! @name Attribute
    //@{
    PHY_ComposantePion::T_ComposantePionVector components_;
    //@}

};

} // namespace human

#endif // __DefaultHealableComputer_h_
