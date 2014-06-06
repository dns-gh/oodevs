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

#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"

namespace transport
{
    class PHY_RoleAction_Transport;
}

class MIL_AgentPion;

// =============================================================================
// @class  PHY_ActionTransportUnload
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionTransportUnload : public PHY_DecisionCallbackAction_ABC
{
public:
    typedef MIL_AgentPion ActorType;

public:
             PHY_ActionTransportUnload( MIL_AgentPion& pion, MT_Vector2D* position );
    virtual ~PHY_ActionTransportUnload();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    virtual void StopAction();
    //@}

private:
    transport::PHY_RoleAction_Transport& role_;
    std::unique_ptr< MT_Vector2D > position_;
};

#endif // __PHY_ActionTransportUnload_h_
