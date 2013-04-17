// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
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
    class Profile;
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
                   int index,
                   const Profile& profile );
    virtual ~StartTimeline();
    //@}

protected:
    virtual bool IsHidden() const;
};

}

#endif // __StartExercise_h_
