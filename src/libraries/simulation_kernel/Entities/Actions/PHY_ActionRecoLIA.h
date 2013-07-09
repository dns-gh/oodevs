// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __PHY_ActionRecoLIA_h_
#define __PHY_ActionRecoLIA_h_

#include <boost/noncopyable.hpp>
#include "Entities/Actions/PHY_Action_ABC.h"

class MIL_AgentPion;
class MIL_Automate;
class DEC_Knowledge_Agent;

// =============================================================================
/** @class  PHY_ActionRecoLIA
    @brief  PHY_ActionRecoLIA
*/
// Created: MMC 2013-07-08
// =============================================================================
class PHY_ActionRecoLIA : public PHY_Action_ABC
{
public:
    typedef MIL_AgentPion ActorType;

    //! @name Constructors/Destructor
    //@{
    explicit PHY_ActionRecoLIA( MIL_AgentPion& caller );
    virtual ~PHY_ActionRecoLIA();
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
    void CheckAgentKnowledgesNearLimasLIA( const DEC_Knowledge_Agent& agent );
    void CheckAgentsNearLimasLIA( const std::vector< MIL_Automate* >& automats );
    void CheckAgentsNearLimasLIA( const MIL_AgentPion& agent );
    bool IsFirstInAutomatWithMission();
    //@}

private:
    //! @name Member data
    //@{
    unsigned int simTime_;
    MIL_AgentPion& caller_;
    std::map< const MIL_AgentPion*, MT_Vector2D > previousAgentsPos_;
    std::map< const MIL_AgentPion*, MT_Vector2D > agentsPos_;
    std::set< const MIL_AgentPion* > lineCrossedAgents_;
    //@}
};

#endif // __PHY_ActionRecoLIA_h_
