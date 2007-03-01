// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ExerciseInfo_h_
#define __ExerciseInfo_h_

namespace kernel
{
    class Controllers;
}

// =============================================================================
/** @class  ExerciseInfo
    @brief  ExerciseInfo
*/
// Created: SBO 2007-02-01
// =============================================================================
class ExerciseInfo : public QVBox
{

public:
    //! @name Constructors/Destructor
    //@{
             ExerciseInfo( QWidget* parent, kernel::Controllers& controllers );
    virtual ~ExerciseInfo();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ExerciseInfo( const ExerciseInfo& );            //!< Copy constructor
    ExerciseInfo& operator=( const ExerciseInfo& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __ExerciseInfo_h_
