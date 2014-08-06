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
class DefaultSpeedComputer : public SpeedComputer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DefaultSpeedComputer( const SpeedStrategy_ABC& strategy );
    virtual ~DefaultSpeedComputer();
    //@}

    //! @name Operations
    //@{
    virtual void ApplyOnComponent( const PHY_ComposantePion& );
    virtual void ApplyOnReinforcement( const MIL_Agent_ABC& );
    virtual void ApplyOnPopulation( const DEC_Knowledge_PopulationCollision& );
    virtual void AddModifier( double ratio, bool isMax = true );
    virtual double GetSpeed() const;
    virtual bool IsTheoric() const;
    //@}
protected:
    //! @name Attributes
    //@{
    const SpeedStrategy_ABC& strategy_;
    double speedRatio_;
    double speed_;
    bool hasComponent_;
    //@}
};

}

#endif /* DEFAULTSPEEDCOMPUTER_H_ */
