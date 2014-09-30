// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __terrain_ObjectCollisionNotificationHandler_ABC_H__
#define __terrain_ObjectCollisionNotificationHandler_ABC_H__

class MIL_Object_ABC;
class MT_Vector2D;

namespace terrain
{

// =============================================================================
/** @class  ObjectCollisionNotificationHandler_ABC
    @brief  ObjectCollisionNotificationHandler_ABC
*/
// Created: AHC 2009-10-06
// =============================================================================
class ObjectCollisionNotificationHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectCollisionNotificationHandler_ABC() {}
    virtual ~ObjectCollisionNotificationHandler_ABC() {}
    //@}

    //! @name Constructors/Destructor
    //@{
    virtual void NotifyMovingInsideObject( MIL_Object_ABC& object, const MT_Vector2D& startPos, const MT_Vector2D& endPos  ) = 0;
    virtual void NotifyMovingOutsideObject( MIL_Object_ABC& object ) = 0;
    virtual void NotifyPutInsideObject( MIL_Object_ABC& object ) = 0;
    virtual void NotifyPutOutsideObject( MIL_Object_ABC& object ) = 0;
    //@}
};

}

#endif /* __terrain_ObjectCollisionNotificationHandler_ABC_H__ */
