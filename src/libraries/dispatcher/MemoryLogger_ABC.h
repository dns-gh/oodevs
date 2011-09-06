// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef dispatcher_MemoryLogger_ABC_h
#define dispatcher_MemoryLogger_ABC_h

#include <boost/noncopyable.hpp>

namespace sword
{
    class ControlEndTick;
}

namespace dispatcher
{
// =============================================================================
/** @class  MemoryLogger_ABC
    @brief  Memory logger declaration
*/
// Created: LGY 2011-09-06
// =============================================================================
class MemoryLogger_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MemoryLogger_ABC() {}
    virtual ~MemoryLogger_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::ControlEndTick& message ) = 0;
    virtual void Update() = 0;
    //@}
};

}

#endif // dispatcher_MemoryLogger_ABC_h
