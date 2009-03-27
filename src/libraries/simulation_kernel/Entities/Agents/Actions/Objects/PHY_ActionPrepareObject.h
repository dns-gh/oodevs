// *****************************************************************************
//
// $Prepared: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionPrepareObject.h $
// $Author: Nld $
// $Modtime: 26/10/04 17:05 $
// $Revision: 2 $
// $Workfile: PHY_ActionPrepareObject.h $
//
// *****************************************************************************

#ifndef __PHY_ActionPrepareObject_h_
#define __PHY_ActionPrepareObject_h_

#include "MIL.h"

#include "Entities/Actions/PHY_Action_ABC.h"

class PHY_RoleAction_Objects;
class MIL_AgentPion;
class MIL_Object_ABC;

// =============================================================================
// @class  PHY_ActionPrepareObject
// Prepared: JVT 2004-08-03
// =============================================================================
class PHY_ActionPrepareObject : public PHY_Action_ABC
{
    MT_COPYNOTALLOWED( PHY_ActionPrepareObject )

public:
    typedef MIL_AgentPion ActorType;

public:
            PHY_ActionPrepareObject( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionPrepareObject();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    PHY_RoleAction_Objects& role_;
    MIL_Object_ABC*         pObject_;
    DIA_Variable_ABC&       diaReturnCode_;
    DIA_Variable_ABC&       diaReturnKnowledge_;
};

#include "PHY_ActionPrepareObject.inl"

#endif // __PHY_ActionPrepareObject_h_
