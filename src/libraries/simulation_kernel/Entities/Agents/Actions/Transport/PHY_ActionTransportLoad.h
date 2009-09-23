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

#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"

namespace transport
{
	class PHY_RoleAction_Transport;
}
class MIL_AgentPion;

// =============================================================================
// @class  PHY_ActionTransportLoad
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionTransportLoad : public PHY_DecisionCallbackAction_ABC
{

public:
    typedef MIL_AgentPion ActorType;        

public:
    explicit PHY_ActionTransportLoad( MIL_AgentPion& pion );
    virtual ~PHY_ActionTransportLoad();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    transport::PHY_RoleAction_Transport& role_;
};

#endif // __PHY_ActionTransportLoad_h_
