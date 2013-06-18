// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef DEFAULTSPEEDCOMPUTER_H_
#define DEFAULTSPEEDCOMPUTER_H_

#include "SpeedComputer_ABC.h"

namespace moving
{

class SpeedStrategy_ABC;

// =============================================================================
/** @class  DefaultSpeedComputer
    @brief  DefaultSpeedComputer
*/
// Created: AHC 2009-10-01
// =============================================================================
class BaseSpeedComputer : public SpeedComputer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             BaseSpeedComputer( const SpeedStrategy_ABC& strategy );
    virtual ~BaseSpeedComputer();
    //@}

    //! @name Operations
    //@{
    virtual void ApplyOnReinforcement( const MIL_Agent_ABC& );
    virtual void ApplyOnPopulation( const DEC_Knowledge_PopulationCollision& );
    virtual void AddModifier( double ratio, bool isMax=true );
    virtual double GetSpeed() const;
    void LoadedSpeedComputer( const PHY_ComposantePion& component );
    void UnloadedSpeedComputer( const PHY_ComposantePion& component );
    //@}
protected:
    //! @name Attributes
    //@{
    const SpeedStrategy_ABC& strategy_;
    double speedRatio_;
    double speed_;
    bool hasUsableComponent_;
    //@}
};

class DefaultSpeedComputer : public BaseSpeedComputer
{
public:
             DefaultSpeedComputer( const SpeedStrategy_ABC& strategy );
    virtual ~DefaultSpeedComputer();

    virtual void ApplyOnComponent( const PHY_ComposantePion& );
};

class LoadedSpeedComputer : public BaseSpeedComputer
{
public:
             LoadedSpeedComputer( const SpeedStrategy_ABC& strategy );
    virtual ~LoadedSpeedComputer();

    virtual void ApplyOnComponent( const PHY_ComposantePion& );
};

class UnloadedSpeedComputer : public BaseSpeedComputer
{
public:
             UnloadedSpeedComputer( const SpeedStrategy_ABC& strategy );
    virtual ~UnloadedSpeedComputer();

    virtual void ApplyOnComponent( const PHY_ComposantePion& );
};

}

#endif /* DEFAULTSPEEDCOMPUTER_H_ */
