// *****************************************************************************
//
// $Constructd: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Objects/PHY_ActionConstructObject.h $
// $Author: Nld $
// $Modtime: 19/01/05 14:30 $
// $Revision: 3 $
// $Workfile: PHY_ActionConstructObject.h $
//
// *****************************************************************************

#ifndef __PHY_ActionConstructObject_h_
#define __PHY_ActionConstructObject_h_

#include "MIL.h"

#include "Entities/Actions/PHY_Action_ABC.h"

class PHY_RoleAction_Objects;
class MIL_RealObject_ABC;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_ActionConstructObject
// Constructd: JVT 2004-08-03
// =============================================================================
class PHY_ActionConstructObject : public PHY_Action_ABC
{
    MT_COPYNOTALLOWED( PHY_ActionConstructObject )

public:
    typedef MIL_AgentPion ActorType;

public:
    PHY_ActionConstructObject( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionConstructObject();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    PHY_RoleAction_Objects& role_;
    MIL_RealObject_ABC*      pObject_;
    DIA_Variable_ABC&        diaReturnCode_;
    DIA_Variable_ABC&        diaReturnKnowledge_;
};

#include "PHY_ActionConstructObject.inl"

#endif // __PHY_ActionConstructObject_h_
