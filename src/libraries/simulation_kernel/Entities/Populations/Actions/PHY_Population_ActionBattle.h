// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __PHY_Population_ActionBattle_h_
#define __PHY_Population_ActionBattle_h_

#include "Entities/Actions/PHY_Action_ABC.h"

class MIL_Population;

// =============================================================================
/** @class  PHY_Population_ActionBattle
    @brief  PHY population action battle
*/
// Created: LGY 2012-02-10
// =============================================================================
class PHY_Population_ActionBattle : public PHY_Action_ABC
{
public:
    typedef MIL_Population ActorType;

public:
    //! @name Constructors/Destructor
    //@{
             PHY_Population_ActionBattle( MIL_Population& attacker, unsigned int nID );
    virtual ~PHY_Population_ActionBattle();
    //@}

    //! @name Operations
    //@{
    virtual void Execute();
    virtual void ExecuteSuspended();
    virtual void Stop();
    //@}

private:
    //! @name Member Data
    //@{
    MIL_Population& attacker_;
    MIL_Population* defender_;
    //@}
};

#endif // __PHY_Population_ActionBattle_h_
