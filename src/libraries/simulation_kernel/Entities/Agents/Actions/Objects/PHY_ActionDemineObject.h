// *****************************************************************************
//
// $Destroyd: DDA 2004-03-06 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionDemineObject.h $
// $Author: DDA $
// $Revision: 2 $
// $Workfile: PHY_ActionDemineObject.h $
//
// *****************************************************************************

#ifndef __PHY_ActionDemineObject_h_
#define __PHY_ActionDemineObject_h_

#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"

class PHY_RoleAction_Objects;
class MIL_AgentPion;
class DEC_Knowledge_Object;

// =============================================================================
// @class  PHY_ActionDemineObject
// Destroyd: DDA 2004-03-06
// =============================================================================
class PHY_ActionDemineObject : public PHY_DecisionCallbackAction_ABC
{
public:
    typedef MIL_AgentPion ActorType;

public:
             PHY_ActionDemineObject( MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    virtual ~PHY_ActionDemineObject();

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

#endif // __PHY_ActionDemineObject_h_
