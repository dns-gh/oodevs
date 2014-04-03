// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __HumansActionsNotificationHandler_ABC_H__
#define __HumansActionsNotificationHandler_ABC_H__

class Human_ABC;
class MIL_AutomateLOG;

namespace human
{
// =============================================================================
/** @class  HumansActionsNotificationHandler_ABC
    @brief  HumansActionsNotificationHandler_ABC
*/
// Created: MGD 2009-10-01
// =============================================================================
class HumansActionsNotificationHandler_ABC : boost::noncopyable
{
public:
             HumansActionsNotificationHandler_ABC() {}
    virtual ~HumansActionsNotificationHandler_ABC() {}

    virtual void NotifyHumanEvacuatedByThirdParty( Human_ABC& human, MIL_AutomateLOG& destinationTC2 ) = 0;
    virtual void NotifyHumanWaitingForMedical    ( Human_ABC& human ) = 0;
    virtual void NotifyHumanAdded  ( Human_ABC& human ) = 0;
    virtual void NotifyHumanRemoved( Human_ABC& human ) = 0;
    virtual void NotifyHumanChanged( Human_ABC& human, const Human_ABC& copyOfOldHumanState ) = 0;
};

}

#endif /* __HumansActionsNotificationHandler_ABC_H__ */
