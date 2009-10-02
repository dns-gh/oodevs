// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __RefugeeActionsNotificationHandler_ABC_H__
#define __RefugeeActionsNotificationHandler_ABC_H__

class MIL_AgentPion;
class MIL_Object_ABC;

namespace refugee {

// =============================================================================
/** @class  RefugeeActionsNotificationHandler_ABC
    @brief  RefugeeActionsNotificationHandler_ABC
*/
// Created: MGD 2009-10-02
// =============================================================================
class RefugeeActionsNotificationHandler_ABC {
public:
	RefugeeActionsNotificationHandler_ABC();
	virtual ~RefugeeActionsNotificationHandler_ABC();

    virtual void Orientate( MIL_AgentPion& pionManaging ) = 0;
    virtual void Release  ( MIL_AgentPion& callerAgent ) = 0;
    virtual void ReleaseCamp( MIL_AgentPion& callerAgent, const MIL_Object_ABC& camp ) = 0;
};

} // namespace refugee

#endif /* __RefugeeActionsNotificationHandler_ABC_H__ */
