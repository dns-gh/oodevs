// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __EditExercise_h_
#define __EditExercise_h_

#include "SpawnCommand.h"

namespace frontend
{

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
             EditExercise( const tools::GeneralConfig& config, const tools::Path& name, bool attach = false );
    virtual ~EditExercise();
    //@}
};

}

#endif // __EditExercise_h_
