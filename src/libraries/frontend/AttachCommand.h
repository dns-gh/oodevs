// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __AttachCommand_h_
#define __AttachCommand_h_

#include "SpawnCommand.h"

namespace tools
{
    class GeneralConfig;
}

namespace frontend
{

// =============================================================================
/** @class  AttachCommand
    @brief  AttachCommand
*/
// Created: JSR 2011-12-14
// =============================================================================
class AttachCommand : public SpawnCommand
{

public:
    //! @name Constructors/Destructor
    //@{
             AttachCommand( const tools::GeneralConfig& config, unsigned long processId, bool attach );
    virtual ~AttachCommand();
    //@}
};

}

#endif // __AttachCommand_h_