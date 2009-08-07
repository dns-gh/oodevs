// *****************************************************************************
//
// $Destroyd: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionDestroyObject.h $
// $Author: Nld $
// $Modtime: 26/10/04 17:05 $
// $Revision: 2 $
// $Workfile: PHY_ActionDestroyObject.h $
//
// *****************************************************************************

#ifndef __PHY_ActionDestroyObject_h_
#define __PHY_ActionDestroyObject_h_

#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"

class PHY_RoleAction_Objects;
class MIL_Object_ABC;
class MIL_AgentPion;
class DEC_Decision_ABC;

// =============================================================================
// @class  PHY_ActionDestroyObject
// Destroyd: JVT 2004-08-03
// =============================================================================
class PHY_ActionDestroyObject : public PHY_DecisionCallbackAction_ABC
{

public:
    typedef MIL_AgentPion ActorType;

public:
             PHY_ActionDestroyObject( MIL_AgentPion& pion, unsigned int nKnowledgeID );
    virtual ~PHY_ActionDestroyObject();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    PHY_RoleAction_Objects& role_;
    unsigned int            nKnowledgeID_;    
};

#endif // __PHY_ActionDestroyObject_h_
