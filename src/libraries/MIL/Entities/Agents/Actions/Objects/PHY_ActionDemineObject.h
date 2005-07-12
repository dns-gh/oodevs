// *****************************************************************************
//
// $Demined: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionDemineObject.h $
// $Author: Nld $
// $Modtime: 26/10/04 17:05 $
// $Revision: 2 $
// $Workfile: PHY_ActionDemineObject.h $
//
// *****************************************************************************

#ifndef __PHY_ActionDemineObject_h_
#define __PHY_ActionDemineObject_h_

#include "MIL.h"

#include "Entities/Agents/Actions/PHY_Action_ABC.h"

class PHY_RoleAction_Objects;
class MIL_RealObject_ABC;

// =============================================================================
// @class  PHY_ActionDemineObject
// Demined: JVT 2004-08-03
// =============================================================================
class PHY_ActionDemineObject : public PHY_Action_ABC
{
    MT_COPYNOTALLOWED( PHY_ActionDemineObject )

public:
    PHY_ActionDemineObject( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionDemineObject();

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

#include "PHY_ActionDemineObject.inl"

#endif // __PHY_ActionDemineObject_h_
