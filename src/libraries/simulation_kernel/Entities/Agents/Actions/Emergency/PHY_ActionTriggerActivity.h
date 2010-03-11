// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PHY_ActionTriggerActivity_h_
#define __PHY_ActionTriggerActivity_h_

#include "MIL.h"
#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"

class MIL_AgentPion;
class PHY_RoleAction_FolkInfluence;

namespace population
{
    class Object_ABC;
}

// =============================================================================
/** @class  PHY_ActionTriggerActivity
    @brief  PHY_ActionTriggerActivity
*/
// Created: JCR 2007-09-12
// =============================================================================
class PHY_ActionTriggerActivity : public PHY_DecisionCallbackAction_ABC
{

public:
    //! @name Types
    //@{
    typedef MIL_AgentPion ActorType;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_ActionTriggerActivity( MIL_AgentPion& pion, const std::string& activity, double influence );
    virtual ~PHY_ActionTriggerActivity();
    //@}

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    virtual void StopAction();
    //@}

private:
    //! @name Member data
    //@{
    const MIL_AgentPion&            pion_;
    PHY_RoleAction_FolkInfluence&   role_;
    std::string                     activity_;
    double                          influence_;
    population::Object_ABC*         pObject_;
    //@}
};

#endif // __PHY_ActionTriggerActivity_h_
