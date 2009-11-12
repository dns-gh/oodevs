// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __LocationActionNotificationHandler_ABC_H__
#define __LocationActionNotificationHandler_ABC_H__

class MT_Vector2D;
class MIL_Agent_ABC;

namespace location {

// =============================================================================
/** @class  LocationActionNotificationHandler_ABC
@brief  LocationActionNotificationHandler_ABC
*/
// Created: MGD 2009-10-17
// =============================================================================
class LocationActionNotificationHandler_ABC {
public:
             LocationActionNotificationHandler_ABC() {}
    virtual ~LocationActionNotificationHandler_ABC() {}

    virtual void Hide     () = 0;
    virtual void Show     ( const MT_Vector2D& vNewPosition ) = 0;
    virtual void Follow   ( const MIL_Agent_ABC& agent ) = 0;
};

} // namespace location

#endif /* __LocationActionNotificationHandler_ABC_H__ */
