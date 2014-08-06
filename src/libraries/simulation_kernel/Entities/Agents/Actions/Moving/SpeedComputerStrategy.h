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
    class PHY_RoleAction_InterfaceMoving;

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
    explicit SpeedComputerStrategy( bool isMaxSpeed, bool isTheoric = false );
             SpeedComputerStrategy( bool isMaxSpeed, const TerrainData& env, bool isTheoric = false );
             SpeedComputerStrategy( bool isMaxSpeed, const MIL_Object_ABC& obj );
    virtual ~SpeedComputerStrategy();
    //@}

    //! @name Operations
    //@{
    virtual double ApplyOnComponent( const PHY_ComposantePion& comp ) const;
    virtual double ApplyOnReinforcement( const MIL_Agent_ABC& pion ) const;
    virtual double ApplyOnPopulation( const DEC_Knowledge_PopulationCollision& population ) const;
    virtual double AddModifier( double ratio, bool isMax = true ) const;
    virtual bool IsTheoric() const;
    //@}

private:
    //! @name Member data
    //@{
    bool isMax_;
    bool isTheoric_;
    boost::function< double( const PHY_ComposantePion& ) > compFunctor_;
    boost::function< double( const PHY_RoleAction_InterfaceMoving& ) > pionFunctor_;
    //@}
};

}

#endif // __SpeedComputerStrategy_h_
