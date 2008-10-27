// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __DirectoryExerciseLister_h_
#define __DirectoryExerciseLister_h_

#include "ExerciseLister_ABC.h"

namespace tools
{
    class GeneralConfig;
}

// =============================================================================
/** @class  DirectoryExerciseLister
    @brief  DirectoryExerciseLister
*/
// Created: LDC 2008-10-24
// =============================================================================
class DirectoryExerciseLister : public ExerciseLister_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DirectoryExerciseLister( const tools::GeneralConfig& config, const std::string& subDir ="" );
    virtual ~DirectoryExerciseLister();
    //@}

    //! @name Operations
    //@{
    virtual void ListExercises( QStringList& list ) const;
    virtual unsigned short GetPort( const QString& exercise ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DirectoryExerciseLister( const DirectoryExerciseLister& );            //!< Copy constructor
    DirectoryExerciseLister& operator=( const DirectoryExerciseLister& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
     std::string subDir_;
     const tools::GeneralConfig& config_;
    //@}
};

#endif // __DirectoryExerciseLister_h_
