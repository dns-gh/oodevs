// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __StartExercise_h_
#define __StartExercise_h_

#include "SpawnCommand.h"

namespace frontend
{
    class ConfigurationManipulator;
    struct DebugConfig;

// =============================================================================
/** @class  StartExercise
    @brief  StartExercise
*/
// Created: AGE 2007-10-04
// =============================================================================
class StartExercise : public SpawnCommand
{
public:
    //! @name Constructors/Destructor
    //@{
    StartExercise(  const tools::GeneralConfig& config, const tools::Path& exercise, const tools::Path& session, bool launchDispatchedIfNotEmbedded, const std::string& name, const tools::Path& checkpoint, const frontend::DebugConfig& debug );
    virtual ~StartExercise();
    //@}

    //! @name Operations
    //@{
    virtual void         Start();
    virtual void         Stop();
    virtual unsigned int GetPercentage() const;
    virtual tools::Path  GetStartedExercise() const;
    virtual tools::Path  GetExercise() const;
    virtual tools::Path  GetSession() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual QString GetStatus() const;
    //@}

private:
    //! @name Member data
    //@{
    tools::Path exercise_;
    tools::Path session_ ;
    std::unique_ptr< SpawnCommand > dispatcher_;
    std::unique_ptr< ConfigurationManipulator > configManipulator_;
    unsigned int percentage_;
    //@}
};

}

#endif // __StartExercise_h_
