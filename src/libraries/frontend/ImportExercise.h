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
    @brief  ImportExercise
*/
// Created: LDC 2010-07-06
// =============================================================================
class ImportExercise : public SpawnCommand
{

public:
    //! @name Constructors/Destructor
    //@{
             ImportExercise( const tools::GeneralConfig& config, const QString& inputScenario, const QString& outputScenario, bool attach = false );
    virtual ~ImportExercise();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ImportExercise( const ImportExercise& );            //!< Copy constructor
    ImportExercise& operator=( const ImportExercise& ); //!< Assignment operator
    //@}
};

}

#endif // __ImportExercise_h_
