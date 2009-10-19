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
    DefaultSpeedComputer();
    virtual ~DefaultSpeedComputer();

    //! @name Operations
    //@{
    virtual void Reset( const SpeedStrategy_ABC* strategy );
    virtual void ApplyOnComponent( const PHY_ComposantePion& );
    virtual void ApplyOnReinforcement( MIL_AgentPion& );
    virtual void ApplyOnPopulation( const DEC_Knowledge_PopulationCollision& );
    virtual void AddModifier( double ratio, bool isMax=true );
    virtual double GetSpeed() const;
    //@}
private:
    //! @name Attributes
    //@{
    const SpeedStrategy_ABC* strategy_;
    double speed_;
    bool hasUsableComponent_;
    double speedRatio_;
    //@}
};

}

#endif /* DEFAULTSPEEDCOMPUTER_H_ */
