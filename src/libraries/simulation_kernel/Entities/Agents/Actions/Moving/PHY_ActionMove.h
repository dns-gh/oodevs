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

class DisasterAttribute;
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
             PHY_ActionMove( MIL_AgentPion& pion, boost::shared_ptr< DEC_Path_ABC > pPath, bool suspended = false );
    virtual ~PHY_ActionMove();
    //@}

    //! @name Operations
    //@{
    virtual void Execute();
    virtual void ExecuteSuspended();
    virtual E_State GetState() const;
    virtual void StopAction();
    //@}

private:
    //! @name Tools
    //@{
    bool AvoidObstacles();
    bool UpdateObjectsToAvoid();
    bool IsDisasterToAvoid( const DisasterAttribute& disaster );
    void CreateNewPath();
    //@}

private:
    MIL_AgentPion& pion_;
    moving::PHY_RoleAction_Moving& role_;
    boost::shared_ptr< DEC_Agent_Path > pMainPath_;
    T_KnowledgeObjectVector objectsToAvoid_;
    std::vector< TER_Localisation > geometrySignatures_;
    bool executionSuspended_;
    bool isBlockedByObject_;
    unsigned int blockedTickCounter_;
    int obstacleId_;
    bool blockedByDisaster_;
    double oldDisasterImpact_;
};

#endif // __PHY_ActionMove_h_
