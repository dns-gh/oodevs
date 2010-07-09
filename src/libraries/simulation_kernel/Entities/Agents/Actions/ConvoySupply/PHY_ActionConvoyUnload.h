// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/ConvoySupply/PHY_ActionConvoyUnload.h $
// $Author: Nld $
// $Modtime: 10/02/05 10:50 $
// $Revision: 1 $
// $Workfile: PHY_ActionConvoyUnload.h $
//
// *****************************************************************************

#ifndef __PHY_ActionConvoyUnload_h_
#define __PHY_ActionConvoyUnload_h_

#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"

class MIL_Agent_ABC;
class PHY_RoleInterface_Supply;

// =============================================================================
// @class  PHY_ActionConvoyUnload
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionConvoyUnload : public PHY_DecisionCallbackAction_ABC
{
public:
    typedef MIL_Agent_ABC ActorType;

public:
    explicit PHY_ActionConvoyUnload( MIL_Agent_ABC& pion );
    virtual ~PHY_ActionConvoyUnload();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    virtual void StopAction();
    //@}

private:
    //! @name Member data
    //@{
    PHY_RoleInterface_Supply& role_;
    //@}
};

#endif // __PHY_ActionConvoyUnload_h_
