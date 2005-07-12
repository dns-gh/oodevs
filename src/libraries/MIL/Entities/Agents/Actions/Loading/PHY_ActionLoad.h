// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Loading/PHY_ActionLoad.h $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_ActionLoad.h $
//
// *****************************************************************************

#ifndef __PHY_ActionLoad_h_
#define __PHY_ActionLoad_h_

#include "MIL.h"

#include "Entities/Agents/Actions/PHY_Action_ABC.h"

class PHY_RoleAction_Loading;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_ActionLoad
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionLoad : public PHY_Action_ABC
{
    MT_COPYNOTALLOWED( PHY_ActionLoad )

public:
    PHY_ActionLoad( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionLoad();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    PHY_RoleAction_Loading& role_;
    DIA_Variable_ABC&        diaReturnCode_;
};

#include "PHY_ActionLoad.inl"

#endif // __PHY_ActionLoad_h_
