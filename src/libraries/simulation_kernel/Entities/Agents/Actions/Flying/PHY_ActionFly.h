// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Flying/PHY_ActionFly.h $
// $Author: Nld $
// $Modtime: 7/03/05 10:25 $
// $Revision: 2 $
// $Workfile: PHY_ActionFly.h $
//
// *****************************************************************************

#ifndef __PHY_ActionFly_h_
#define __PHY_ActionFly_h_

#include "Entities/Actions/PHY_Action_ABC.h"

class MIL_AgentPion;
class PHY_RoleAction_InterfaceFlying;

// =============================================================================
// @class  PHY_ActionFly
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionFly : public PHY_Action_ABC
{

public:
    typedef MIL_AgentPion ActorType;        

public:
    explicit PHY_ActionFly( MIL_AgentPion& pion );
    virtual ~PHY_ActionFly();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    //! @name Member data
    //@{
    PHY_RoleAction_InterfaceFlying& role_;
    //@}
};


#endif // __PHY_ActionFly_h_
