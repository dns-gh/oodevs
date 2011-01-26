// *****************************************************************************
//
// $Mined: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionExtinguishObject.h $
// $Author: Nld $
// $Modtime: 26/10/04 17:05 $
// $Revision: 2 $
// $Workfile: PHY_ActionExtinguishObject.h $
//
// *****************************************************************************

#ifndef __PHY_ActionExtinguishObject_h_
#define __PHY_ActionExtinguishObject_h_

#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"

class PHY_RoleAction_Objects;
class MIL_AgentPion;
class DEC_Knowledge_Object;

// =============================================================================
// @class  PHY_ActionExtinguishObject
// Created: BCI 2011-01-25
// =============================================================================
class PHY_ActionExtinguishObject : public PHY_DecisionCallbackAction_ABC
{
public:
    typedef MIL_AgentPion ActorType;

public:
             PHY_ActionExtinguishObject( MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    virtual ~PHY_ActionExtinguishObject();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    virtual void StopAction();
    //@}

private:
    PHY_RoleAction_Objects& role_;
    boost::shared_ptr< DEC_Knowledge_Object > pKnowledge_;
};

#endif // __PHY_ActionExtinguishObject_h_
