// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __VisionConeNotificationHandler_ABC_H__
#define __VisionConeNotificationHandler_ABC_H__

namespace network {

// =============================================================================
/** @class  VisionConeNotificationHandler_ABC
@brief  VisionConeNotificationHandler_ABC
*/
// Created: MGD 2009-09-24
// =============================================================================
class VisionConeNotificationHandler_ABC {
public:
    VisionConeNotificationHandler_ABC();
    virtual ~VisionConeNotificationHandler_ABC();

    virtual void NotifyVisionConeDataHasChanged() = 0;
};

} // namespace network

#endif /* __VisionConeNotificationHandler_ABC_H__ */
