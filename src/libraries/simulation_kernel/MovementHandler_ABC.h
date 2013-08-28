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

#include <boost/noncopyable.hpp>

class MT_Vector2D;

namespace location
{
// =============================================================================
/** @class  MovementHandler_ABC
    @brief  MovementHandler_ABC
*/
// Created: MGD 2010-04-21
// =============================================================================
class MovementHandler_ABC : boost::noncopyable
{
public:
             MovementHandler_ABC() {}
    virtual ~MovementHandler_ABC() {}

    virtual void NotifyHasMoved( const MT_Vector2D& newPos ) = 0;
};

}

#endif /* __MovementHandler_ABC_H__ */
