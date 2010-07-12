// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __SpeedComputerStrategy_h_
#define __SpeedComputerStrategy_h_

#include "SpeedStrategy_ABC.h"
#include <boost/function.hpp>

class MIL_Object_ABC;
class TerrainData;

namespace moving
{

class PHY_RoleAction_Moving;

// =============================================================================
/** @class  SpeedComputerStrategy
    @brief  SpeedComputerStrategy
*/
// Created: LDC 2009-12-16
// =============================================================================
class SpeedComputerStrategy : public SpeedStrategy_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SpeedComputerStrategy( bool isMaxSpeed, bool withReinforcement, const MIL_Object_ABC& obj, const TerrainData* env=0 );
             SpeedComputerStrategy( bool isMaxSpeed, bool withReinforcement, const TerrainData* env=0 );
    virtual ~SpeedComputerStrategy();
    //@}

    //! @name Operations
    //@{
    virtual double ApplyOnComponent( const PHY_ComposantePion& comp ) const;
    virtual double ApplyOnReinforcement( MIL_Agent_ABC& pion ) const;
    virtual double ApplyOnPopulation( const DEC_Knowledge_PopulationCollision& population ) const;
    virtual double AddModifier( double ratio, bool isMax=true ) const;
    //@}

private:
    //! @name Member data
    //@{
    bool withReinforcement_;
    bool isMax_;
    boost::function< double( const PHY_ComposantePion& ) > compFunctor_;
    boost::function< double( PHY_RoleAction_Moving& ) > pionFunctor_;
    //@}
};

}

#endif // __SpeedComputerStrategy_h_
