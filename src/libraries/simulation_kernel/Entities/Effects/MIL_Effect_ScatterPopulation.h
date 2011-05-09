// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MIL_Effect_ScatterPopulation_h_
#define __MIL_Effect_ScatterPopulation_h_

#include "MIL_Effect_ABC.h"

class MIL_PopulationElement_ABC;
class PHY_FireResults_ABC;

// =============================================================================
/** @class  MIL_Effect_ScatterPopulation
    @brief  MIL_Effect_ScatterPopulation
*/
// Created: LDC 2011-05-09
// =============================================================================
class MIL_Effect_ScatterPopulation : public MIL_Effect_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MIL_Effect_ScatterPopulation( MIL_PopulationElement_ABC& target, unsigned int nNbrAmmoFired, PHY_FireResults_ABC& fireResult );
    virtual ~MIL_Effect_ScatterPopulation();
    //@}

    //! @name Operations
    //@{
    virtual bool Execute();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_Effect_ScatterPopulation( const MIL_Effect_ScatterPopulation& );            //!< Copy constructor
    MIL_Effect_ScatterPopulation& operator=( const MIL_Effect_ScatterPopulation& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    MIL_PopulationElement_ABC& target_;
    const unsigned int nNbrAmmoFired_;
    PHY_FireResults_ABC& fireResult_;
    //@}
};

#endif // __MIL_Effect_ScatterPopulation_h_
