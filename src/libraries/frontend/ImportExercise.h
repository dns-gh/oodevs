// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ImportExercise_h_
#define __ImportExercise_h_

#include "SpawnCommand.h"

namespace frontend
{
// =============================================================================
/** @class  ImportExercise
    @brief  Import exercise
*/
// Created: LDC 2010-07-06
// =============================================================================
class ImportExercise : public SpawnCommand
{
public:
    //! @name Constructors/Destructor
    //@{
             ImportExercise( const tools::GeneralConfig& config, const tools::Path& inputScenario, const tools::Path& outputScenario );
    virtual ~ImportExercise();
    //@}
};

}

#endif // __ImportExercise_h_
