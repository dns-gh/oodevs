// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Effects/MIL_Effect_Fly.h $
// $Author: Nld $
// $Modtime: 7/03/05 10:24 $
// $Revision: 4 $
// $Workfile: MIL_Effect_Fly.h $
//
// *****************************************************************************

#ifndef __MIL_Effect_Fly_h_
#define __MIL_Effect_Fly_h_

#include "MIL.h"

#include "MIL_Effect_ABC.h"

class PHY_RoleAction_InterfaceFlying;

// =============================================================================
// @class  MIL_Effect_Fly
// Created: JVT 2004-08-03
// =============================================================================
class MIL_Effect_Fly : public MIL_Effect_ABC
{
    MT_COPYNOTALLOWED( MIL_Effect_Fly )

public:
    MIL_Effect_Fly( PHY_RoleAction_InterfaceFlying& roleFlying );
    virtual ~MIL_Effect_Fly();

    //! @name Operations
    //@{
    virtual bool Execute();
    
    void SetFlyingHeight( MT_Float rHeight );
    //@}

private:
    PHY_RoleAction_InterfaceFlying& roleFlying_;
    MT_Float                        rHeight_;
};

#include "MIL_Effect_Fly.inl"

#endif // __MIL_Effect_Fly_h_
