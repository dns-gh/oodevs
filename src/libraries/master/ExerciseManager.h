// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ExerciseManager_h_
#define __ExerciseManager_h_

// =============================================================================
/** @class  ExerciseManager
    @brief  ExerciseManager
*/
// Created: NLD 2007-01-29
// =============================================================================
class ExerciseManager
{
public:
    //! @name Constructors/Destructor
    //@{
             ExerciseManager();
    virtual ~ExerciseManager();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ExerciseManager( const ExerciseManager& );            //!< Copy constructor
    ExerciseManager& operator=( const ExerciseManager& ); //!< Assignment operator
    //@}

private:
};

#endif // __ExerciseManager_h_
