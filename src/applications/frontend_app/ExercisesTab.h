// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ExercisesTab_h_
#define __ExercisesTab_h_

namespace kernel
{
    class Controllers;
}

// =============================================================================
/** @class  ExercisesTab
    @brief  ExercisesTab
*/
// Created: SBO 2007-02-01
// =============================================================================
class ExercisesTab : public QHBox
{

public:
    //! @name Constructors/Destructor
    //@{
             ExercisesTab( QWidget* parent, kernel::Controllers& controllers );
    virtual ~ExercisesTab();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ExercisesTab( const ExercisesTab& );            //!< Copy constructor
    ExercisesTab& operator=( const ExercisesTab& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __ExercisesTab_h_
