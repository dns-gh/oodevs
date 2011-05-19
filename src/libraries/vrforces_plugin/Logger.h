// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __vrforces_Logger_h_
#define __vrforces_Logger_h_

#include "dispatcher/Logger_ABC.h"

namespace plugins
{
namespace vrforces
{
// =============================================================================
/** @class  Logger
    @brief  Logger
*/
// Created: SBO 2011-05-19
// =============================================================================
class Logger : public dispatcher::Logger_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Logger( dispatcher::Logger_ABC& logger );
    virtual ~Logger();
    //@}

    //! @name Operations
    //@{
    virtual void LogInfo( const std::string& message );
    virtual void LogError( const std::string& message );
    virtual void LogWarning( const std::string& message );
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Logger_ABC& logger_;
    //@}
};
}
}

#endif // __vrforces_Logger_h_
