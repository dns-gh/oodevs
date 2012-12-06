// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __PHY_ActionDeteriorateUrbanBlock_h_
#define __PHY_ActionDeteriorateUrbanBlock_h_

#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"

class PHY_RoleAction_Objects;
class MIL_AgentPion;
class MIL_UrbanObject_ABC;

// =============================================================================
/** @class  PHY_ActionDeteriorateUrbanBlock
    @brief  PHY_ActionDeteriorateUrbanBlock
*/
// Created: JSR 2012-04-19
// =============================================================================
class PHY_ActionDeteriorateUrbanBlock : public PHY_DecisionCallbackAction_ABC
{
public:
    //! @name Types
    //@{
    typedef MIL_AgentPion ActorType;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_ActionDeteriorateUrbanBlock( MIL_AgentPion& pion, MIL_UrbanObject_ABC* pUrbanBlock, double percentage );
    virtual ~PHY_ActionDeteriorateUrbanBlock();
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
    PHY_RoleAction_Objects& role_;
    MIL_UrbanObject_ABC* pUrbanBlock_;
    double percentage_;
    //@}
};

#endif // __PHY_ActionDeteriorateUrbanBlock_h_
