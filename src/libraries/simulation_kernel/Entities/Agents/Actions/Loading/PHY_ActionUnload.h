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

#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"

class MIL_AgentPion;
namespace transport
{
    class PHY_RoleAction_Loading;

// =============================================================================
// @class  PHY_ActionUnload
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionUnload : public PHY_DecisionCallbackAction_ABC
{
public:
    typedef MIL_AgentPion ActorType;

public:
    explicit PHY_ActionUnload( MIL_AgentPion& pion );
    virtual ~PHY_ActionUnload();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    virtual void StopAction();
    //@}

private:
    //! @name Member data
    //@{
    PHY_RoleAction_Loading& role_;
    //@}
};

} // namespace transport

#endif // __PHY_ActionUnload_h_
