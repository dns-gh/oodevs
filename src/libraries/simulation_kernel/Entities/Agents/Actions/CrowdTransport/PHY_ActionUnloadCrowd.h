// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PHY_ActionUnloadCrowd_h_
#define __PHY_ActionUnloadCrowd_h_

#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"

class MIL_Population;

namespace crowdtransport
{
class PHY_RoleAction_CrowdTransport;

// =============================================================================
/** @class  PHY_ActionUnloadCrowd
    @brief  PHY_ActionUnloadCrowd
*/
// Created: JSR 2011-08-09
// =============================================================================
class PHY_ActionUnloadCrowd : public PHY_DecisionCallbackAction_ABC
{
public:
    //! @name Types
    //@{
    typedef MIL_AgentPion ActorType;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_ActionUnloadCrowd( MIL_AgentPion& pion, int knowledgeId, MT_Vector2D* position );
    virtual ~PHY_ActionUnloadCrowd();
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
    MT_Vector2D* position_;
    //@}
};

} // namespace crowdtransport

#endif // __PHY_ActionUnloadCrowd_h_
