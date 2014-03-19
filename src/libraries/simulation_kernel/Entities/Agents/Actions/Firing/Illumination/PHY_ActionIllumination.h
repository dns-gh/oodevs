// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PHY_ActionIllumination_h_
#define __PHY_ActionIllumination_h_

#include "MIL.h"
#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_RoleAction_DirectFiring.h"

class PHY_AmmoDotationClass;
class MIL_AgentPion;
class MIL_KnowledgeGroup;

// =============================================================================
// @class  PHY_ActionIllumination
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionIllumination : public PHY_DecisionCallbackAction_ABC
{
public:
    typedef MIL_AgentPion ActorType;

public:
             PHY_ActionIllumination( MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Agent > pEnemy, DEC_Decision_ABC* allie );
    virtual ~PHY_ActionIllumination();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    virtual void StopAction();
    //@}

private:
    firing::PHY_RoleAction_DirectFiring& role_;
    boost::shared_ptr< DEC_Knowledge_Agent > pEnemy_;
    boost::shared_ptr< MIL_KnowledgeGroup > knowledgeGroup_;
};

#endif // __PHY_ActionIllumination_h_
