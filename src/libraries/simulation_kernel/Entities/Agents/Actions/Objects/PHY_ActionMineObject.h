// *****************************************************************************
//
// $Mined: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionMineObject.h $
// $Author: Nld $
// $Modtime: 26/10/04 17:05 $
// $Revision: 2 $
// $Workfile: PHY_ActionMineObject.h $
//
// *****************************************************************************

#ifndef __PHY_ActionMineObject_h_
#define __PHY_ActionMineObject_h_

#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"

class PHY_RoleAction_Objects;
class MIL_AgentPion;
class DEC_Knowledge_Object;

// =============================================================================
// @class  PHY_ActionMineObject
// Mined: JVT 2004-08-03
// =============================================================================
class PHY_ActionMineObject : public PHY_DecisionCallbackAction_ABC
{

public:
    typedef MIL_AgentPion ActorType;

public:
             PHY_ActionMineObject( MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    virtual ~PHY_ActionMineObject();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    PHY_RoleAction_Objects& role_;
    boost::shared_ptr< DEC_Knowledge_Object > pKnowledge_;    
};

#endif // __PHY_ActionMineObject_h_
