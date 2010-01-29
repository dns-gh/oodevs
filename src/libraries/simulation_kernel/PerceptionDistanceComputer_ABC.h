// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __PerceptionDistanceComputer_ABC_h_
#define __PerceptionDistanceComputer_ABC_h_


#include "simulation_kernel/OnComponentComputer_ABC.h"
#include "simulation_kernel/Entities/Agents/Units/Composantes/PHY_Composante_ABC.h"

class PHY_ComposantePion;

namespace detection
{

// =============================================================================
/** @class  PerceptionDistanceComputer_ABC
    @brief  PerceptionDistanceComputer_ABC
*/
// Created: MGD 2009-09-15
// =============================================================================
class PerceptionDistanceComputer_ABC
{

public:
    //! @name Operations
    //@{
    PerceptionDistanceComputer_ABC() {}
    virtual ~PerceptionDistanceComputer_ABC() {}
    virtual void AddModifier( double modifier ) = 0;
    virtual void AddElongationFactor( double modifier ) = 0;
    virtual double GetElongationFactor() const = 0;
    virtual double GetFactor() const = 0;
    //@}
};

} // namespace detection

#endif // __PerceptionDistanceComputer_ABC_h_
