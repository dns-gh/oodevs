// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Loading/PHY_ActionUnload.h $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_ActionUnload.h $
//
// *****************************************************************************

#ifndef __PHY_ActionUnload_h_
#define __PHY_ActionUnload_h_

#include "Entities/Actions/PHY_Action_ABC.h"

class MIL_AgentPion;
class PHY_RoleAction_Loading;

// =============================================================================
// @class  PHY_ActionUnload
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionUnload : public PHY_Action_ABC
{
    MT_COPYNOTALLOWED( PHY_ActionUnload )

public:
    typedef MIL_AgentPion ActorType;

public:
             PHY_ActionUnload( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionUnload();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    //! @name Member data
    //@{
    PHY_RoleAction_Loading& role_;
    DIA_Variable_ABC&        diaReturnCode_;
    //@}
};

#endif // __PHY_ActionUnload_h_
