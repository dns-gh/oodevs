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
#include "Decision/DEC_PathResult.h"
#include <boost/shared_ptr.hpp>

class MIL_AgentPion;
namespace moving
{
    class PHY_RoleAction_Moving;
}
class DEC_Path_ABC;
class DEC_Agent_Path;
class MIL_Object_ABC;

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
    bool CreateJoiningPath( const MT_Vector2D& lastJoiningPoint, bool forceNextPoint, double minDistance );
    void DestroyJoiningPath();
    MT_Vector2D GetLastPointAndDestroyJoiningPath();
    void AvoidObstacles();
    bool UpdateObjectsToAvoid();
    void CreateFinalPath();
    int CreatePathAfterObjectCollision( boost::shared_ptr< DEC_PathResult > pCurrentPath, MIL_Object_ABC* obstacle );
    int CreateAdaptedPath( boost::shared_ptr< DEC_PathResult > pCurrentPath, const MT_Vector2D& lastJoiningPoint, bool forceNextPoint );
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
    bool isTreatingJoining_;
    std::pair< std::pair< MT_Vector2D, MT_Vector2D >, unsigned int > lastBlockedPoint_;
    MIL_Object_ABC* obstacle_;
};

#endif // __PHY_ActionMove_h_
