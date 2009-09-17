// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __Logger_h_
#define __Logger_h_

#include "tools/win32/ProcessMonitor.h"

// =============================================================================
/** @class  Logger
    @brief  Logger
*/
// Created: LDC 2009-09-09
// =============================================================================
namespace dispatcher
{
class Logger
{

public:
    //! @name Constructors/Destructor
    //@{
             Logger();
    virtual ~Logger();
    //@}

    //! @name Operations
    //@{
    void Update();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Logger( const Logger& );            //!< Copy constructor
    Logger& operator=( const Logger& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    ProcessMonitor monitor_;
    int nextMemoryDump_;
    //@}
};
}

#endif // __Logger_h_
