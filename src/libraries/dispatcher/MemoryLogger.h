// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __dispatcher_MemoryLogger_h_
#define __dispatcher_MemoryLogger_h_

#include <boost/noncopyable.hpp>
#include <memory>

class ProcessMonitor;

namespace dispatcher
{
// =============================================================================
/** @class  MemoryLogger
    @brief  Memory logger
*/
// Created: LDC 2009-09-09
// =============================================================================
class MemoryLogger : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MemoryLogger();
    virtual ~MemoryLogger();
    //@}

    //! @name Operations
    //@{
    void Update();
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ProcessMonitor > monitor_;
    int next_;
    //@}
};
}

#endif // __dispatcher_MemoryLogger_h_
