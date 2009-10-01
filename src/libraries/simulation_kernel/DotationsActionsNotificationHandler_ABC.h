// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DotationsActionsNotificationHandler_ABC_H__
#define __DotationsActionsNotificationHandler_ABC_H__

class PHY_DotationCapacities;

namespace dotation {

// =============================================================================
/** @class  DotationsActionsNotificationHandler_ABC
    @brief  DotationsActionsNotificationHandler_ABC
*/
// Created: MGD 2009-10-01
// =============================================================================
class DotationsActionsNotificationHandler_ABC {
public:
	DotationsActionsNotificationHandler_ABC();
	virtual ~DotationsActionsNotificationHandler_ABC();

    virtual void RegisterDotationsCapacities  ( const PHY_DotationCapacities& capacities ) = 0;
    virtual void UnregisterDotationsCapacities( const PHY_DotationCapacities& capacities ) = 0;
};

} // namespace dotation

#endif /* __DotationsActionsNotificationHandler_ABC_H__ */
