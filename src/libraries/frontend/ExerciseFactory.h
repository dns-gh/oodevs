// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ExerciseFactory_h_
#define __ExerciseFactory_h_

#include "ExerciseFactory_ABC.h"

namespace kernel
{
    class Controllers;
}

namespace frontend
{
    class Model;

// =============================================================================
/** @class  ExerciseFactory
    @brief  ExerciseFactory
*/
// Created: SBO 2007-01-29
// =============================================================================
class ExerciseFactory : public ExerciseFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ExerciseFactory( kernel::Controllers& controllers, const Model& model );
    virtual ~ExerciseFactory();
    //@}

    //! @name Operations
    //@{
    virtual Exercise* Create( const ASN1T_MsgExerciseCreation& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ExerciseFactory( const ExerciseFactory& );            //!< Copy constructor
    ExerciseFactory& operator=( const ExerciseFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const Model&         model_;
    //@}
};

}

#endif // __ExerciseFactory_h_
