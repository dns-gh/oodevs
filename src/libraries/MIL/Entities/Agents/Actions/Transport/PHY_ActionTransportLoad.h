// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Transport/PHY_ActionTransportLoad.h $
// $Author: Nld $
// $Modtime: 19/11/04 11:56 $
// $Revision: 1 $
// $Workfile: PHY_ActionTransportLoad.h $
//
// *****************************************************************************

#ifndef __PHY_ActionTransportLoad_h_
#define __PHY_ActionTransportLoad_h_

#include "MIL.h"

#include "Entities/Agents/Actions/PHY_Action_ABC.h"

class PHY_RoleAction_Transport;

// =============================================================================
// @class  PHY_ActionTransportLoad
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionTransportLoad : public PHY_Action_ABC
{
    MT_COPYNOTALLOWED( PHY_ActionTransportLoad )

public:
    PHY_ActionTransportLoad( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionTransportLoad();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    PHY_RoleAction_Transport& role_;
    DIA_Variable_ABC&         diaReturnCode_;
};

#include "PHY_ActionTransportLoad.inl"

#endif // __PHY_ActionTransportLoad_h_
