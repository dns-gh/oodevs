// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef STARTDISPATCHER_H_
#define STARTDISPATCHER_H_

#include "SpawnCommand.h"

namespace frontend
{
// =============================================================================
/** @class  StartDispatcher
    @brief  StartDispatcher
*/
// Created: AHC 2011-05-16
// =============================================================================
class StartDispatcher : public SpawnCommand
{
public:
    //! @name Constructors/Destructor
    //@{
    StartDispatcher( const tools::GeneralConfig& config, bool attach,
                     const tools::Path& exercise, const tools::Path& session,
                     const tools::Path& checkpoint = "", const tools::Path& dispatcher_path = "",
                     std::string commanderEndpoint = "", std::string processJobName = "" );
    virtual ~StartDispatcher();
    //@}

    //! @name Operations
    //@{
    virtual void Stop();
    //@}
};

}

#endif // STARTDISPATCHER_H_
