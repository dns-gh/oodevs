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

class PHY_Human;
class MIL_AutomateLOG;
class PHY_MedicalHumanState;

namespace human {

// =============================================================================
/** @class  HumansActionsNotificationHandler_ABC
    @brief  HumansActionsNotificationHandler_ABC
*/
// Created: MGD 2009-10-01
// =============================================================================
class HumansActionsNotificationHandler_ABC {
public:
	         HumansActionsNotificationHandler_ABC() {}
	virtual ~HumansActionsNotificationHandler_ABC() {}

    virtual void NotifyHumanEvacuatedByThirdParty( PHY_Human& human, MIL_AutomateLOG& destinationTC2 ) = 0;
    virtual void NotifyHumanWaitingForMedical    ( PHY_Human& human ) = 0;
    virtual void NotifyHumanBackFromMedical      ( PHY_MedicalHumanState& humanState ) = 0;
    virtual void NotifyHumanAdded  ( PHY_Human& human ) = 0;
    virtual void NotifyHumanRemoved( PHY_Human& human ) = 0;
    virtual void NotifyHumanChanged( PHY_Human& human, const PHY_Human& copyOfOldHumanState ) = 0;
};

} // namespace human

#endif /* __HumansActionsNotificationHandler_ABC_H__ */
