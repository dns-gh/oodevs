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

#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"
#include "Knowledge/DEC_Knowledge_Def.h"
#include "simulation_terrain/TER_Localisation.h"
#include <boost/shared_ptr.hpp>

class MIL_AgentPion;
namespace moving
{
    class PHY_RoleAction_Moving;
}
class DEC_Path_ABC;
class DEC_Agent_Path;

// =============================================================================
// @class  PHY_ActionMove
// Bypassd: JVT 2004-08-03
// =============================================================================
class PHY_ActionMove : public PHY_DecisionCallbackAction_ABC
{
public:
    //! @name Types
    //@{
    typedef MIL_AgentPion ActorType;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_ActionMove( MIL_AgentPion& pion, boost::shared_ptr< DEC_Path_ABC > pPath );
    virtual ~PHY_ActionMove();
    //@}

    //! @name Operations
    //@{
    virtual void Execute();
    virtual void ExecuteSuspended();
    virtual void StopAction();
    //@}

private:
    //! @name Tools
    //@{
    bool CreateJoiningPath( bool forceNextPoint );
    void DestroyJoiningPath();
    void AvoidObstacles();
    bool UpdateObjectsToAvoid();
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< unsigned int > T_ObjectKnowledgeSet; // Dia ids
    //@}

private:
    MIL_AgentPion& pion_;
    moving::PHY_RoleAction_Moving& role_;
    boost::shared_ptr< DEC_Agent_Path > pMainPath_;
    boost::shared_ptr< DEC_Agent_Path > pJoiningPath_;
    T_ObjectKnowledgeSet objectAvoidAttempts_;
    T_KnowledgeObjectVector objectsToAvoid_;
    std::vector< TER_Localisation > geometrySignatures_;
    bool forceNextPoint_;
};

#endif // __PHY_ActionMove_h_
