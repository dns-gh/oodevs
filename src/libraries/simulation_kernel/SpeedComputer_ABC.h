// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __moving_SpeedComputer_ABC_H__
#define __moving_SpeedComputer_ABC_H__

//#include "simulation_kernel/OnComponentComputer_ABC.h"

class PHY_ComposantePion;
class MIL_AgentPion;
class DEC_Knowledge_PopulationCollision;

namespace moving
{

// =============================================================================
/** @class  SpeedStrategy_ABC
    @brief  SpeedStrategy_ABC
*/
// Created: AHC 2009-10-01
// =============================================================================
class SpeedStrategy_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    SpeedStrategy_ABC();
    virtual ~SpeedStrategy_ABC();
    //@}

    //! @name Constructors/Destructor
    //@{
    virtual double ApplyOnComponent(const PHY_ComposantePion&) const =0;
    virtual double ApplyOnReinforcement( MIL_AgentPion&) const =0;
    virtual double ApplyOnPopulation( const DEC_Knowledge_PopulationCollision&) const =0;
    virtual double AddModifier( double ratio, bool isMax=true) const =0;
    //@}
};

// =============================================================================
/** @class  SpeedComputer_ABC
    @brief  SpeedComputer_ABC
*/
// Created: AHC 2009-10-01
// =============================================================================
class SpeedComputer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    SpeedComputer_ABC();
    virtual ~SpeedComputer_ABC();
    //@}

    //! @name Constructors/Destructor
    //@{
    virtual void Reset(const SpeedStrategy_ABC* strategy)= 0;
    virtual void ApplyOnComponent(const PHY_ComposantePion&) =0;
    virtual void ApplyOnReinforcement( MIL_AgentPion&) =0;
    virtual void AddModifier(double ratio, bool isMax=true) =0;
    virtual void ApplyOnPopulation( const DEC_Knowledge_PopulationCollision&) =0;
    virtual double GetSpeed() const =0;
    //@}
};

}

#endif /* __moving_SpeedComputer_ABC_H__ */
