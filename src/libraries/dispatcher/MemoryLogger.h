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

#include "MemoryLogger_ABC.h"
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
class MemoryLogger : public MemoryLogger_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MemoryLogger();
    virtual ~MemoryLogger();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::ControlEndTick& message );
    virtual void Update();
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< ProcessMonitor > monitor_;
    int next_;
    unsigned int nCurrentTick_;
    unsigned int lastTick_;
    //@}
};
}

#endif // __dispatcher_MemoryLogger_h_
