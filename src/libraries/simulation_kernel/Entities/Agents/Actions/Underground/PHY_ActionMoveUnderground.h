// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PHY_ActionMoveUnderground_h_
#define __PHY_ActionMoveUnderground_h_

#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"

class PHY_RoleAction_MovingUnderground;

// =============================================================================
/** @class  PHY_ActionMoveUnderground
    @brief  PHY_ActionMoveUnderground
*/
// Created: JSR 2011-06-08
// =============================================================================
class PHY_ActionMoveUnderground : public PHY_DecisionCallbackAction_ABC
{
public:
    //! @name Types
    //@{
    typedef MIL_AgentPion ActorType;

    enum E_ReturnCode
    {
        eRunning    = 0,
        eFinished   = 1,
        eNotAllowed = 2
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_ActionMoveUnderground( MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Object > pDestination );
    virtual ~PHY_ActionMoveUnderground();
    //@}

    //! @name Operations
    //@{
    virtual void Execute();
    virtual void ExecuteSuspended();
    virtual void StopAction();
    //@}

private:
    //! @name Member data
    //@{
    PHY_RoleAction_MovingUnderground& role_;
    //@}
};

#endif // __PHY_ActionMoveUnderground_h_
