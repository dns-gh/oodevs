// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ConsoleLogger_h_
#define __ConsoleLogger_h_

#include "Logger.h"

namespace mission_tester
{
// =============================================================================
/** @class  ConsoleLogger
    @brief  Console logger
*/
// Created: PHC 2011-04-07
// =============================================================================
class ConsoleLogger : public Logger
{
public:
    //! @name Constructors/Destructor
    //@{
             ConsoleLogger();
    virtual ~ConsoleLogger();
    //@}
};

}

#endif // __ConsoleLogger_h_
