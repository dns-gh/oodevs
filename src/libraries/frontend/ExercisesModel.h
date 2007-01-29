// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ExercisesModel_h_
#define __ExercisesModel_h_

#include "Messages.h"
#include "clients_kernel/Resolver.h"

namespace frontend
{
    class Exercise;
    class ExerciseFactory_ABC;

// =============================================================================
/** @class  ExercisesModel
    @brief  ExercisesModel
*/
// Created: SBO 2007-01-29
// =============================================================================
class ExercisesModel : public kernel::StringResolver< Exercise >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ExercisesModel( ExerciseFactory_ABC& factory );
    virtual ~ExercisesModel();
    //@}

    //! @name Operations
    //@{
    void CreateExercise( const ASN1T_MsgExerciseCreation& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ExercisesModel( const ExercisesModel& );            //!< Copy constructor
    ExercisesModel& operator=( const ExercisesModel& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    ExerciseFactory_ABC& factory_;
    //@}
};

}

#endif // __ExercisesModel_h_
