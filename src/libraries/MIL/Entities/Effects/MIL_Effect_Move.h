// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Effects/MIL_Effect_Move.h $
// $Author: Age $
// $Modtime: 29/11/04 14:56 $
// $Revision: 2 $
// $Workfile: MIL_Effect_Move.h $
//
// *****************************************************************************

#ifndef __MIL_Effect_Move_h_
#define __MIL_Effect_Move_h_

#include "MIL.h"

#include "MIL_Effect_ABC.h"

class PHY_RoleAction_Moving;

// =============================================================================
// @class  MIL_Effect_Move
// Created: JVT 2004-08-03
// =============================================================================
class MIL_Effect_Move : public MIL_Effect_ABC
{
    MT_COPYNOTALLOWED( MIL_Effect_Move )

public:
             MIL_Effect_Move( PHY_RoleAction_Moving& roleMoving );
    virtual ~MIL_Effect_Move();

    //! @name Operations
    //@{
    virtual bool Execute();
    //@}

private:
    PHY_RoleAction_Moving& roleMoving_;
};

#include "MIL_Effect_Move.inl"

#endif // __MIL_Effect_Move_h_
