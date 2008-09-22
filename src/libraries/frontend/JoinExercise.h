// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __JoinExercise_h_
#define __JoinExercise_h_

#include "SpawnCommand.h"

namespace frontend
{

// =============================================================================
/** @class  JoinExercise
    @brief  JoinExercise
*/
// Created: AGE 2007-10-05
// =============================================================================
class JoinExercise : public SpawnCommand
{

public:
    //! @name Constructors/Destructor
    //@{
             JoinExercise( const tools::GeneralConfig& config, const QString& exercise, const QString& session, bool attach = false );
             JoinExercise( const tools::GeneralConfig& config, const QString& exercise, const QString& session, const QString& profile, bool attach = false );
    virtual ~JoinExercise();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    JoinExercise( const JoinExercise& );            //!< Copy constructor
    JoinExercise& operator=( const JoinExercise& ); //!< Assignment operator
    //@}

};

}

#endif // __JoinExercise_h_
