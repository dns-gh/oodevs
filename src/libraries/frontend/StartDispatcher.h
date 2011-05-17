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
    StartDispatcher(const tools::GeneralConfig& config, bool attach, const QString& dispatcher_path,
            const QString& exercise, const QString& session, const QString& checkpoint = "" );
    StartDispatcher(const tools::GeneralConfig& config,
            const QString& exercise, const QString& session, bool attach, const QString& checkpoint = "" );
    //@}
};

}

#endif // STARTDISPATCHER_H_
