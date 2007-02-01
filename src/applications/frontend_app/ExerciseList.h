// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ExerciseList_h_
#define __ExerciseList_h_

#include "frontend/Exercise.h"
#include "ElementListView.h"

namespace kernel
{
    class Controllers;
}

// =============================================================================
/** @class  ExerciseList
    @brief  ExerciseList
*/
// Created: SBO 2007-01-26
// =============================================================================
class ExerciseList : public ElementListView< frontend::Exercise >
{

public:
    //! @name Constructors/Destructor
    //@{
             ExerciseList( QWidget* parent, kernel::Controllers& controllers );
    virtual ~ExerciseList();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ExerciseList( const ExerciseList& );            //!< Copy constructor
    ExerciseList& operator=( const ExerciseList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __ExerciseList_h_
