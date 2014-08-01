// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __StartTimeline_h_
#define __StartTimeline_h_

#include "SpawnCommand.h"

namespace tools
{
    class GeneralConfig;
}

namespace frontend
{

    struct DebugConfig;

// =============================================================================
/** @class  StartTimeline
    @brief  StartTimeline
*/
// Created: BAX 2014-04-16
// =============================================================================
class StartTimeline : public SpawnCommand
{
public:
    //! @name Constructors/Destructor
    //@{
    StartTimeline( const tools::GeneralConfig& config,
                   const tools::Path& exercise,
                   const tools::Path& session,
                   const frontend::DebugConfig& debug );
    virtual ~StartTimeline();
    //@}
};

}

#endif // __StartExercise_h_
