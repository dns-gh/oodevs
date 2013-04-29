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

#include "simulation_kernel/OnComponentComputer_ABC.h"

class PHY_ComposantePion;
class PHY_HumanRank;
class MIL_AutomateLOG;
namespace human
{

// =============================================================================
/** @class  HealableComputer_ABC
    @brief  HealableComputer_ABC
*/
// Created: MGD 2009-09-24
// =============================================================================
class HealComputer_ABC : public OnComponentComputer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             HealComputer_ABC() {}
    virtual ~HealComputer_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void ApplyOnComponent( PHY_ComposantePion& component ) = 0;
    virtual void Heal( const PHY_HumanRank& rank, unsigned int nNbrToChange ) const = 0;
    virtual void Wound( const PHY_HumanRank& rank, unsigned int nNbrToChange ) const = 0;
    virtual void HealAll( bool withLog ) const = 0;
    virtual void EvacuateWoundedHumans( MIL_AutomateLOG& destinationTC2 ) const = 0;
    virtual bool HasWoundedHumansToEvacuate() const = 0;
    //@}
};

} // namespace human

#endif // __HealableComputer_ABC_h_
