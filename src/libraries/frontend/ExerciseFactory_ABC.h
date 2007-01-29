// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ExerciseFactory_ABC_h_
#define __ExerciseFactory_ABC_h_

#include "Messages.h"

namespace frontend
{
    class Exercise;

// =============================================================================
/** @class  ExerciseFactory_ABC
    @brief  ExerciseFactory_ABC
*/
// Created: SBO 2007-01-29
// =============================================================================
class ExerciseFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ExerciseFactory_ABC() {}
    virtual ~ExerciseFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual Exercise* Create( const ASN1T_MsgExerciseCreation& message ) = 0;
    //@}
};

}

#endif // __ExerciseFactory_ABC_h_
