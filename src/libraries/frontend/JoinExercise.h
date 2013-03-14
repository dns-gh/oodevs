// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
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
             JoinExercise( const tools::GeneralConfig& config, const tools::Path& exercise, const tools::Path& session, bool attach = false );
             JoinExercise( const tools::GeneralConfig& config, const tools::Path& exercise, const tools::Path& session, const QString& profile, bool attach = false );
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
