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

#include "MIL.h"

#include "Entities/Actions/PHY_Action_ABC.h"

class PHY_RoleAction_Objects;
class MIL_RealObject_ABC;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_ActionResumeWorkObject
// Mined: JVT 2004-08-03
// =============================================================================
class PHY_ActionResumeWorkObject : public PHY_Action_ABC
{
    MT_COPYNOTALLOWED( PHY_ActionResumeWorkObject )

public:
    typedef MIL_AgentPion ActorType;

public:
    PHY_ActionResumeWorkObject( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionResumeWorkObject();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    PHY_RoleAction_Objects& role_;
    DIA_Variable_ABC&       diaReturnCode_;
    uint                    nKnowledgeID_;    
};

#include "PHY_ActionResumeWorkObject.inl"

#endif // __PHY_ActionResumeWorkObject_h_
