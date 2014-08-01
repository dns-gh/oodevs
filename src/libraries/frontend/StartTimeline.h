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
#include <boost/optional.hpp>

namespace tools
{
    class GeneralConfig;
}

namespace frontend
{
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
                   boost::optional< tools::Path > wwwDir );
    virtual ~StartTimeline();
    //@}
};

}

#endif // __StartExercise_h_
