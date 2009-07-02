// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Transport/PHY_ActionTransportUnload.h $
// $Author: Nld $
// $Modtime: 19/11/04 12:00 $
// $Revision: 1 $
// $Workfile: PHY_ActionTransportUnload.h $
//
// *****************************************************************************

#ifndef __PHY_ActionTransportUnload_h_
#define __PHY_ActionTransportUnload_h_

#include "Entities/Actions/PHY_Action_ABC.h"

class PHY_RoleAction_Transport;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_ActionTransportUnload
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionTransportUnload : public PHY_Action_ABC
{

public:
    typedef MIL_AgentPion ActorType;        

public:
             PHY_ActionTransportUnload( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionTransportUnload();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    PHY_RoleAction_Transport& role_;
    DIA_Variable_ABC&         diaReturnCode_;
};

#endif // __PHY_ActionTransportUnload_h_
