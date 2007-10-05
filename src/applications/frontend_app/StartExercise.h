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
             StartExercise( QObject* parent, const tools::GeneralConfig& config, const QString& exercise );
             StartExercise( QObject* parent, const tools::GeneralConfig& config, const QString& exercise, const QString& checkpoint );
    virtual ~StartExercise();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    StartExercise( const StartExercise& );            //!< Copy constructor
    StartExercise& operator=( const StartExercise& ); //!< Assignment operator
    //@}
};

#endif // __StartExercise_h_
