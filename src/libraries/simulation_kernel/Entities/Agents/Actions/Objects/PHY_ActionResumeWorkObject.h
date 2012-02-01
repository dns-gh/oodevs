// *****************************************************************************
//
// $Mined: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionResumeWorkObject.h $
// $Author: Nld $
// $Modtime: 19/01/05 14:57 $
// $Revision: 1 $
// $Workfile: PHY_ActionResumeWorkObject.h $
//
// *****************************************************************************

#ifndef __PHY_ActionResumeWorkObject_h_
#define __PHY_ActionResumeWorkObject_h_

#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"

class PHY_RoleAction_Objects;
class MIL_AgentPion;
class DEC_Knowledge_Object;

// =============================================================================
// @class  PHY_ActionResumeWorkObject
// Mined: JVT 2004-08-03
// =============================================================================
class PHY_ActionResumeWorkObject : public PHY_DecisionCallbackAction_ABC
{
public:
    typedef MIL_AgentPion ActorType;

public:
             PHY_ActionResumeWorkObject( MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, bool valorizeIt );
    virtual ~PHY_ActionResumeWorkObject();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    virtual void StopAction();
    //@}

private:
    PHY_RoleAction_Objects& role_;
    boost::shared_ptr< DEC_Knowledge_Object > pKnowledge_;
    bool valorizeIt_;
};

#endif // __PHY_ActionResumeWorkObject_h_
