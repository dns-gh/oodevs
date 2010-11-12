// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ExerciseFilter_ABC_h_
#define __ExerciseFilter_ABC_h_

namespace frontend
{
    class Exercise_ABC;

// =============================================================================
/** @class  ExerciseFilter_ABC
    @brief  ExerciseFilter_ABC
*/
// Created: SBO 2010-10-21
// =============================================================================
class ExerciseFilter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ExerciseFilter_ABC() {}
    virtual ~ExerciseFilter_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool Allows( const Exercise_ABC& exercise ) const = 0;
    //@}
};

}

#endif // __ExerciseFilter_ABC_h_
