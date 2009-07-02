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

#include "Entities/Actions/PHY_Action_ABC.h"

class PHY_RoleAction_Objects;
class MIL_Object_ABC;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_ActionMineObject
// Mined: JVT 2004-08-03
// =============================================================================
class PHY_ActionMineObject : public PHY_Action_ABC
{

public:
    typedef MIL_AgentPion ActorType;

public:
             PHY_ActionMineObject( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionMineObject();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    PHY_RoleAction_Objects& role_;
    DIA_Variable_ABC&        diaReturnCode_;
    uint                     nKnowledgeID_;    
};

#endif // __PHY_ActionMineObject_h_
