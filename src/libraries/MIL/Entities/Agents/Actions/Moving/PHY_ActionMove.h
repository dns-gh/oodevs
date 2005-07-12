// *****************************************************************************
//
// $Bypassd: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Moving/PHY_ActionMove.h $
// $Author: Nld $
// $Modtime: 30/11/04 15:40 $
// $Revision: 2 $
// $Workfile: PHY_ActionMove.h $
//
// *****************************************************************************

#ifndef __PHY_ActionMove_h_
#define __PHY_ActionMove_h_

#include "MIL.h"

#include "Entities/Agents/Actions/PHY_Action_ABC.h"

class PHY_RoleAction_Moving;
class DEC_Path;

// =============================================================================
// @class  PHY_ActionMove
// Bypassd: JVT 2004-08-03
// =============================================================================
class PHY_ActionMove : public PHY_Action_ABC
{
    MT_COPYNOTALLOWED( PHY_ActionMove )

public:
    PHY_ActionMove( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionMove();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    //! @name Tools
    //@{
    void CreateJoiningPath ();
    void DestroyJoiningPath();
    void AvoidObstacles    ();
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< uint > T_ObjectKnowledgeSet; // Dia ids
    //@}

private:
    PHY_RoleAction_Moving& role_;
    DIA_Variable_ABC&      diaReturnCode_;
    DEC_Path*              pMainPath_;
    DEC_Path*              pJoiningPath_;
    T_ObjectKnowledgeSet   objectAvoidAttempts_;
};

#include "PHY_ActionMove.inl"

#endif // __PHY_ActionMove_h_
