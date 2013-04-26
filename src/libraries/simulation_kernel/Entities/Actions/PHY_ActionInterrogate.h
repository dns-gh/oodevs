// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PHY_ActionInterrogate_h_
#define __PHY_ActionInterrogate_h_

#include <boost/noncopyable.hpp>
#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"

class MIL_AgentPion;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Population;

// =============================================================================
/** @class  PHY_ActionInterrogate
    @brief  PHY_ActionInterrogate
*/
// Created: ABR 2011-03-25
// =============================================================================
class PHY_ActionInterrogate : public PHY_DecisionCallbackAction_ABC
{
public:
    typedef MIL_AgentPion ActorType;

    enum E_ReturnCode
    {
        eRunning = 0,
        eFinished = 1,
        eFailed = 2
    };

public:
    //! @name Constructors/Destructor
    //@{
             PHY_ActionInterrogate( MIL_AgentPion& caller, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge ); // unit
             PHY_ActionInterrogate( MIL_AgentPion& caller, int knowledgeId ); // crowd
    virtual ~PHY_ActionInterrogate();
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
    void ComputeTimeToWait( float affinity );
    //@}

private:
    //! @name Member data
    //@{
    unsigned int rBaseTime_;
    unsigned int rTimeToWait_;
    boost::shared_ptr< DEC_Knowledge_Agent > pAgentKnowledge_;
    boost::shared_ptr< DEC_Knowledge_Population > pPopulationKnowledge_;
    MIL_AgentPion& caller_;
    //@}
};

#endif // __PHY_ActionInterrogate_h_
