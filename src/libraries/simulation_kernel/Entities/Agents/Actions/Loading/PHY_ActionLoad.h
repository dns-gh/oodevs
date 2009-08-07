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

#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"

class MIL_AgentPion;
class PHY_RoleAction_Loading;

// =============================================================================
// @class  PHY_ActionLoad
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionLoad : public PHY_DecisionCallbackAction_ABC
{
        
public:
    typedef MIL_AgentPion ActorType;

public:
    explicit PHY_ActionLoad( MIL_AgentPion& pion );
    virtual ~PHY_ActionLoad();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    //! @name Member data
    //@{
    PHY_RoleAction_Loading& role_;
    //@}
};

#endif // __PHY_ActionLoad_h_
