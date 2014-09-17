// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EditExercise_h_
#define __EditExercise_h_

#include "SpawnCommand.h"

namespace frontend
{
    struct DebugConfig;
    class Config;

// =============================================================================
/** @class  EditExercise
    @brief  EditExercise
*/
// Created: AGE 2007-10-04
// =============================================================================
class EditExercise : public SpawnCommand
{
public:
    //! @name Constructors/Destructor
    //@{
             EditExercise( const Config& config,
                           const tools::Path& name, const DebugConfig& debug );
    virtual ~EditExercise();
    //@}
};

}

#endif // __EditExercise_h_
