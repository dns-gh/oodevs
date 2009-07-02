// *****************************************************************************
//
// $Demined: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Objects/PHY_ActionAnimateObject.h $
// $Author: Nld $
// $Modtime: 23/03/05 10:50 $
// $Revision: 1 $
// $Workfile: PHY_ActionAnimateObject.h $
//
// *****************************************************************************

#ifndef __PHY_ActionAnimateObject_h_
#define __PHY_ActionAnimateObject_h_

#include "Entities/Actions/PHY_Action_ABC.h"

class PHY_RoleAction_Objects;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_ActionAnimateObject
// Demined: JVT 2004-08-03
// =============================================================================
class PHY_ActionAnimateObject : public PHY_Action_ABC
{

public:
    typedef MIL_AgentPion ActorType;

public:
             PHY_ActionAnimateObject( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionAnimateObject();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    PHY_RoleAction_Objects& role_;
    uint                    nKnowledgeID_;    
};

#endif // __PHY_ActionAnimateObject_h_
