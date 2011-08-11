// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PHY_ActionLoadCrowd_h_
#define __PHY_ActionLoadCrowd_h_

#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"

class MIL_Population;

namespace crowdtransport
{
class PHY_RoleAction_CrowdTransport;

// =============================================================================
/** @class  PHY_ActionLoadCrowd
    @brief  PHY_ActionLoadCrowd
*/
// Created: JSR 2011-08-09
// =============================================================================
class PHY_ActionLoadCrowd : public PHY_DecisionCallbackAction_ABC
{
public:
    //! @name Types
    //@{
    typedef MIL_AgentPion ActorType;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_ActionLoadCrowd( MIL_AgentPion& pion, int knowledgeId, unsigned int concentrationId );
    virtual ~PHY_ActionLoadCrowd();
    //@}

    //! @name Operations
    //@{
    virtual void Execute();
    virtual void ExecuteSuspended();
    virtual void StopAction();
    //@}

private:
    //! @name Member data
    //@{
    PHY_RoleAction_CrowdTransport& role_;
    MIL_Population* crowd_;
    unsigned int concentrationId_;
    //@}
};

} // namespace crowdtransport

#endif // __PHY_ActionLoadCrowd_h_
