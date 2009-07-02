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

class DEC_PathWalker;

// =============================================================================
// @class  MIL_Effect_Move
// Created: JVT 2004-08-03
// =============================================================================
class MIL_Effect_Move : public MIL_Effect_ABC
{

public:
             MIL_Effect_Move( DEC_PathWalker& roleMoving );
    virtual ~MIL_Effect_Move();

    //! @name Operations
    //@{
    virtual bool Execute();
    //@}

private:
    DEC_PathWalker& roleMoving_;
};

#include "MIL_Effect_Move.inl"

#endif // __MIL_Effect_Move_h_
