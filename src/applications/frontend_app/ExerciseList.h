// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ExerciseList_h_
#define __ExerciseList_h_

#include <qlistview.h>

// =============================================================================
/** @class  ExerciseList
    @brief  ExerciseList
*/
// Created: SBO 2007-01-26
// =============================================================================
class ExerciseList : public QListView
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ExerciseList( QWidget* parent );
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
