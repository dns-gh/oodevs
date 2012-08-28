// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PHY_ActionResumeWorkUrbanBlock_h_
#define __PHY_ActionResumeWorkUrbanBlock_h_

#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"

class PHY_RoleAction_Objects;
class MIL_AgentPion;
class MIL_UrbanObject_ABC;

// =============================================================================
/** @class  PHY_ActionResumeWorkUrbanBlock
    @brief  PHY_ActionResumeWorkUrbanBlock
*/
// Created: LGY 2011-03-23
// =============================================================================
class PHY_ActionResumeWorkUrbanBlock : public PHY_DecisionCallbackAction_ABC
{
public:
    //! @name Types
    //@{
    typedef MIL_AgentPion ActorType;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_ActionResumeWorkUrbanBlock( MIL_AgentPion& pion, MIL_UrbanObject_ABC* pUrbanBlock );
    virtual ~PHY_ActionResumeWorkUrbanBlock();
    //@}

    //! @name Operations
    //@{
    virtual void Execute();
    virtual void ExecuteSuspended();
    virtual void StopAction();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PHY_ActionResumeWorkUrbanBlock( const PHY_ActionResumeWorkUrbanBlock& );            //!< Copy constructor
    PHY_ActionResumeWorkUrbanBlock& operator=( const PHY_ActionResumeWorkUrbanBlock& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    PHY_RoleAction_Objects& role_;
    MIL_UrbanObject_ABC* pUrbanBlock_;
    //@}
};

#endif // __PHY_ActionResumeWorkUrbanBlock_h_
