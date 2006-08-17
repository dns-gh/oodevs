// *****************************************************************************
//
// $Bypassd: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionBypassObject.h $
// $Author: Nld $
// $Modtime: 26/10/04 17:05 $
// $Revision: 2 $
// $Workfile: PHY_ActionBypassObject.h $
//
// *****************************************************************************

#ifndef __PHY_ActionBypassObject_h_
#define __PHY_ActionBypassObject_h_

#include "MIL.h"

#include "Entities/Actions/PHY_Action_ABC.h"

class PHY_RoleAction_Objects;
class MIL_RealObject_ABC;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_ActionBypassObject
// Bypassd: JVT 2004-08-03
// =============================================================================
class PHY_ActionBypassObject : public PHY_Action_ABC
{
    MT_COPYNOTALLOWED( PHY_ActionBypassObject )

public:
    typedef MIL_AgentPion ActorType;

public:
    PHY_ActionBypassObject( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionBypassObject();

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

#include "PHY_ActionBypassObject.inl"

#endif // __PHY_ActionBypassObject_h_
