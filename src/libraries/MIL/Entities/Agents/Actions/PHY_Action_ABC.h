// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/PHY_Action_ABC.h $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_Action_ABC.h $
//
// *****************************************************************************

#ifndef __PHY_Action_ABC_h_
#define __PHY_Action_ABC_h_

#include "MIL.h"

class MIL_AgentPion;

// =============================================================================
// @class  PHY_Action_ABC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Action_ABC
{
    MT_COPYNOTALLOWED( PHY_Action_ABC )

public:
    PHY_Action_ABC( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    PHY_Action_ABC( MIL_AgentPion& pion );
    virtual ~PHY_Action_ABC();

    //! @name Operations
    //@{
    void Suspend();
    void Resume ();
    void Update ();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void Execute         () = 0;
    virtual void ExecuteSuspended() = 0;
    //@}

protected:
    MIL_AgentPion& pion_;

private:
    bool            bSuspended_;
};

#include "PHY_Action_ABC.inl"

#endif // __PHY_Action_ABC_h_
