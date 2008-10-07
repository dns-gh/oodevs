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
    class ExerciseListener;

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
             StartExercise( const tools::GeneralConfig& config, const QString& exercise, const QString& session, bool attach = false );
             StartExercise( const tools::GeneralConfig& config, const QString& exercise, const QString& session, const QString& checkpoint, bool attach = false );
    virtual ~StartExercise();
    //@}

    //! @name Operations
    //@{
    virtual bool Wait();
    virtual unsigned int GetPercentage() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    StartExercise( const StartExercise& );            //!< Copy constructor
    StartExercise& operator=( const StartExercise& ); //!< Assignment operator
    //@}

private:
    //! @name Member data 
    //@{
    std::string exercise_;
    std::string session_ ;
    std::auto_ptr<ExerciseListener> listener_;
    //@}
};

}

#endif // __StartExercise_h_
