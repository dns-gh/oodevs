// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MovementHandler_ABC_H__
#define __MovementHandler_ABC_H__

class MT_Vector2D;

namespace location
{
// =============================================================================
/** @class  MovementHandler_ABC
    @brief  MovementHandler_ABC
*/
// Created: MGD 2010-04-21
// =============================================================================
class MovementHandler_ABC
{
public:
             MovementHandler_ABC() {}
    virtual ~MovementHandler_ABC() {}

    virtual void NotifyHasMove( const MT_Vector2D& newPos ) = 0;
};

}

#endif /* __MovementHandler_ABC_H__ */
