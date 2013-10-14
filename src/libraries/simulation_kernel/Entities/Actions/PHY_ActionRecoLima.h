// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __PHY_ActionRecoLima_h_
#define __PHY_ActionRecoLima_h_

#include <boost/noncopyable.hpp>
#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"

class MIL_Agent_ABC;
class MIL_AgentPion;
class MIL_Automate;
class DEC_Knowledge_Agent;
class MIL_LimaFunction;
class MIL_LimaOrder;

// =============================================================================
/** @class  PHY_ActionRecoLima
    @brief  PHY_ActionRecoLima
*/
// Created: MMC 2013-07-08
// =============================================================================
class PHY_ActionRecoLima : public PHY_DecisionCallbackAction_ABC
{
public:
    typedef MIL_AgentPion ActorType;

    enum E_ReturnCode
    {
        eRunning = 0,
        eFailedBadLima = 1,
    };

    //! @name Constructors/Destructor
    //@{
    explicit PHY_ActionRecoLima( MIL_AgentPion& caller, int limaType );
    virtual ~PHY_ActionRecoLima();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void Execute();
    virtual void ExecuteSuspended();
    virtual void StopAction();
    //@}

private:
    //! @name Helpers
    //@{
    bool ComputeLimas();
    void CheckAgentKnowledgesNearLimas( const DEC_Knowledge_Agent& agent );
    void CheckAgentsNearLimas( const std::vector< MIL_Automate* >& automats );
    void CheckAgentsNearLimas( const MIL_Agent_ABC& agent );
    bool IsFirstInAutomatWithMission();
    //@}

private:
    //! @name Member data
    //@{
    unsigned int simTime_;
    MIL_AgentPion& caller_;
    const MIL_LimaFunction* limaType_;
    std::vector< const MIL_LimaOrder* > limas_;
    std::map< const MIL_Agent_ABC*, MT_Vector2D > previousAgentsPos_;
    std::map< const MIL_Agent_ABC*, MT_Vector2D > agentsPos_;
    std::set< const MIL_Agent_ABC* > lineCrossedAgents_;
    //@}
};

#endif // __PHY_ActionRecoLima_h_
