// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __LocalExerciseFilter_h_
#define __LocalExerciseFilter_h_

#include "ExerciseFilter_ABC.h"
#include <boost/noncopyable.hpp>

namespace frontend
{

// =============================================================================
/** @class  LocalExerciseFilter
    @brief  LocalExerciseFilter
*/
// Created: SBO 2010-10-26
// =============================================================================
class LocalExerciseFilter : private boost::noncopyable
                          , public ExerciseFilter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LocalExerciseFilter();
    virtual ~LocalExerciseFilter();
    //@}

    //! @name Operations
    //@{
    virtual bool Allows( const frontend::Exercise_ABC& exercise ) const;
    //@}
};

}

#endif // __LocalExerciseFilter_h_
