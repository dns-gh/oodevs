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

#include "Entities/Actions/PHY_Action_ABC.h"

class MIL_AgentPion;
class PHY_RolePion_Supply;

// =============================================================================
// @class  PHY_ActionConvoyLoad
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionConvoyLoad : public PHY_Action_ABC
{
    MT_COPYNOTALLOWED( PHY_ActionConvoyLoad )

public:
    typedef MIL_AgentPion ActorType;

public:
             PHY_ActionConvoyLoad( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionConvoyLoad();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    //! @name Member data
    //@{
    PHY_RolePion_Supply& role_;
    DIA_Variable_ABC&    diaReturnCode_;
    //@}
};

#endif // __PHY_ActionConvoyLoad_h_
