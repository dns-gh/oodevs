// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/ConvoySupply/PHY_ActionConvoySupply.h $
// $Author: Nld $
// $Modtime: 10/02/05 10:50 $
// $Revision: 1 $
// $Workfile: PHY_ActionConvoySupply.h $
//
// *****************************************************************************

#ifndef __PHY_ActionConvoySupply_h_
#define __PHY_ActionConvoySupply_h_

#include "MIL.h"

#include "Entities/Actions/PHY_Action_ABC.h"

class PHY_RolePion_Supply;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_ActionConvoySupply
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionConvoySupply : public PHY_Action_ABC
{
    MT_COPYNOTALLOWED( PHY_ActionConvoySupply )

public:
    typedef MIL_AgentPion ActorType;        

public:
    PHY_ActionConvoySupply( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionConvoySupply();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    PHY_RolePion_Supply& role_;
    DIA_Variable_ABC&    diaReturnCode_;
};

#include "PHY_ActionConvoySupply.inl"

#endif // __PHY_ActionConvoySupply_h_
