// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_CORE_LOGGER_H
#define SWORD_CORE_LOGGER_H

#include <core/Logger_ABC.h>

namespace sword
{
// =============================================================================
/** @class  CoreLogger
    @brief  Core logger
*/
// Created: MCO 2012-06-15
// =============================================================================
class CoreLogger : public core::Logger_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             CoreLogger();
    virtual ~CoreLogger();
    //@}

    //! @name Operations
    //@{
    virtual void Log( core::Level level, const std::string& message ) const;
    //@}
};

}

#endif // SWORD_CORE_LOGGER_H
