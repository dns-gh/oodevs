// *****************************************************************************
//
// $Demined: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionExtinguishObject.h $
// $Author: Nld $
// $Modtime: 26/10/04 17:05 $
// $Revision: 2 $
// $Workfile: PHY_ActionExtinguishObject.h $
//
// *****************************************************************************

#ifndef __PHY_ActionExtinguishObject_h_
#define __PHY_ActionExtinguishObject_h_

#include "Entities/Actions/PHY_Action_ABC.h"

class PHY_RoleAction_Objects;
class MIL_RealObject_ABC;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_ActionExtinguishObject
// Demined: JVT 2004-08-03
// =============================================================================
class PHY_ActionExtinguishObject : public PHY_Action_ABC
{

public:
    typedef MIL_AgentPion ActorType;

public:
             PHY_ActionExtinguishObject( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionExtinguishObject();

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

#endif // __PHY_ActionExtinguishObject_h_
