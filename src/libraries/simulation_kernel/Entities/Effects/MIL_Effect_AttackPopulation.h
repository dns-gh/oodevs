// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __MIL_Effect_AttackPopulation_h_
#define __MIL_Effect_AttackPopulation_h_

#include "MIL_Effect_ABC.h"

class MIL_PopulationElement_ABC;

// =============================================================================
/** @class  MIL_Effect_AttackPopulation
    @brief  MIL effect attack population
*/
// Created: LGY 2012-02-10
// =============================================================================
class MIL_Effect_AttackPopulation : public MIL_Effect_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_Effect_AttackPopulation( MIL_PopulationElement_ABC& attacker, MIL_PopulationElement_ABC& defender );
    virtual ~MIL_Effect_AttackPopulation();
    //@}

    //! @name Operations
    //@{
    virtual bool Execute();
    //@}

private:
    //! @name Member Data
    //@{
    MIL_PopulationElement_ABC& attacker_;
    MIL_PopulationElement_ABC& defender_;
    //@}
};

#endif // __MIL_Effect_AttackPopulation_h_
