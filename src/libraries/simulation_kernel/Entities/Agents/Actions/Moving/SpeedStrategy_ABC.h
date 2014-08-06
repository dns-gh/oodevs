// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __SpeedStrategy_ABC_h_
#define __SpeedStrategy_ABC_h_

class DEC_Knowledge_PopulationCollision;
class MIL_Agent_ABC;
class PHY_ComposantePion;

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
             SpeedStrategy_ABC() {}
    virtual ~SpeedStrategy_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual double ApplyOnComponent( const PHY_ComposantePion& ) const = 0;
    virtual double ApplyOnReinforcement( const MIL_Agent_ABC& ) const = 0;
    virtual double ApplyOnPopulation( const DEC_Knowledge_PopulationCollision& ) const = 0;
    virtual double AddModifier( double ratio, bool isMax = true ) const = 0;
    virtual bool IsTheoric() const = 0;
    //@}
};

}

#endif // __SpeedStrategy_ABC_h_
