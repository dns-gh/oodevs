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

#include "Knowledge/DEC_Knowledge_Def.h"
#include "Entities/Actions/PHY_Action_ABC.h"

class MIL_AgentPion;
class PHY_RoleAction_Moving;
class DEC_Agent_Path;

// =============================================================================
// @class  PHY_ActionMove
// Bypassd: JVT 2004-08-03
// =============================================================================
class PHY_ActionMove : public PHY_Action_ABC
{

public:
    typedef MIL_AgentPion ActorType;

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
    void CreateJoiningPath   ();
    void DestroyJoiningPath  ();
    void AvoidObstacles      ();
    bool UpdateObjectsToAvoid();
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< uint > T_ObjectKnowledgeSet; // Dia ids
    //@}

private:
    MIL_AgentPion&          pion_;   
    PHY_RoleAction_Moving&  role_;
    DIA_Variable_ABC&       diaReturnCode_;
    DEC_Agent_Path*         pMainPath_;
    DEC_Agent_Path*         pJoiningPath_;
    T_ObjectKnowledgeSet    objectAvoidAttempts_;
    T_KnowledgeObjectVector objectsToAvoid_;
};

#endif // __PHY_ActionMove_h_
