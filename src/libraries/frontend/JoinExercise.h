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
             // Pass a null profile to propose login dialog at gaming startup, an
             // empty string to log anonymously.
             JoinExercise( const tools::GeneralConfig& config, const tools::Path& exercise,
                 const tools::Path& session, const QString* profile,
                 const QString& devFeatures );
    virtual ~JoinExercise();
    //@}

private:
    //! @name Helpers
    //@{
    virtual QString GetStatus() const;
    //@}
};

}

#endif // __JoinExercise_h_
