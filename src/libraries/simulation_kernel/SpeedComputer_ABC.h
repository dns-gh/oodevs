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

class MIL_Agent_ABC;
class PHY_ComposantePion;
class DEC_Knowledge_PopulationCollision;

namespace moving
{
// =============================================================================
/** @class  SpeedComputer_ABC
    @brief  SpeedComputer_ABC
*/
// Created: AHC 2009-10-01
// =============================================================================
class SpeedComputer_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SpeedComputer_ABC() {}
    virtual ~SpeedComputer_ABC() {}
    //@}

    //! @name Constructors/Destructor
    //@{
    virtual void ApplyOnComponent( const PHY_ComposantePion& ) = 0;
    virtual void ApplyOnReinforcement( const MIL_Agent_ABC& ) = 0;
    virtual void AddModifier( double ratio, bool isMax = true ) = 0;
    virtual void ApplyOnPopulation( const DEC_Knowledge_PopulationCollision& ) = 0;
    virtual double GetSpeed() const = 0;
    virtual bool IsTheoric() const = 0;
    //@}
};

}

#endif /* __moving_SpeedComputer_ABC_H__ */
