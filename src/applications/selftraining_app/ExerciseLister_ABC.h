// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ExerciseLister_ABC_h_
#define __ExerciseLister_ABC_h_

// =============================================================================
/** @class  ExerciseLister_ABC
    @brief  ExerciseLister_ABC
*/
// Created: LDC 2008-10-24
// =============================================================================
class ExerciseLister_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ExerciseLister_ABC() {}
    virtual ~ExerciseLister_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void ListExercises( QStringList& list ) const = 0;
    virtual unsigned short GetPort( const QString& exercise ) const = 0;
    //@}
};

#endif // __ExerciseLister_ABC_h_
