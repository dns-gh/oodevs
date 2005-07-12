// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Loading/PHY_ActionRecoverHumanTransporters.h $
// $Author: Jvt $
// $Modtime: 4/05/05 14:45 $
// $Revision: 2 $
// $Workfile: PHY_ActionRecoverHumanTransporters.h $
//
// *****************************************************************************

#ifndef __PHY_ActionRecoverHumanTransporters_h_
#define __PHY_ActionRecoverHumanTransporters_h_

#include "MIL.h"

#include "Entities/Agents/Actions/PHY_Action_ABC.h"

class PHY_RoleAction_Loading;

// =============================================================================
// @class  PHY_ActionRecoverHumanTransporters
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionRecoverHumanTransporters: public PHY_Action_ABC
{
    MT_COPYNOTALLOWED( PHY_ActionRecoverHumanTransporters)

public:
             PHY_ActionRecoverHumanTransporters( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionRecoverHumanTransporters();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    PHY_RoleAction_Loading& role_;
    DIA_Variable_ABC&       diaReturnCode_;
    MT_Vector2D             vRecoveringPosition_;
};

#include "PHY_ActionRecoverHumanTransporters.inl"

#endif // __PHY_ActionRecoverHumanTransporters_h_
