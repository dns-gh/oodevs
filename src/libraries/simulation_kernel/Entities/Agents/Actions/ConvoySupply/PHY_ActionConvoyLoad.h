// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/ConvoySupply/PHY_ActionConvoyLoad.h $
// $Author: Nld $
// $Modtime: 10/02/05 10:50 $
// $Revision: 1 $
// $Workfile: PHY_ActionConvoyLoad.h $
//
// *****************************************************************************

#ifndef __PHY_ActionConvoyLoad_h_
#define __PHY_ActionConvoyLoad_h_

#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"

class MIL_AgentPion;
class PHY_RoleInterface_Supply;

// =============================================================================
// @class  PHY_ActionConvoyLoad
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionConvoyLoad : public PHY_DecisionCallbackAction_ABC
{

public:
    typedef MIL_AgentPion ActorType;

public:
    explicit PHY_ActionConvoyLoad( MIL_AgentPion& pion );
    virtual ~PHY_ActionConvoyLoad();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    //! @name Member data
    //@{
    PHY_RoleInterface_Supply& role_;
    //@}
};

#endif // __PHY_ActionConvoyLoad_h_
