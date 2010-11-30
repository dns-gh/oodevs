// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ExerciseFileLoader_h_
#define __ExerciseFileLoader_h_

#include "FileLoader.h"

namespace kernel
{
// =============================================================================
/** @class  ExerciseFileLoader
    @brief  ExerciseFileLoader
*/
// Created: LDC 2010-11-29
// =============================================================================
class ExerciseFileLoader : public FileLoader
{

public:
    //! @name Constructors/Destructor
    //@{
             ExerciseFileLoader( const tools::ExerciseConfig& config );
    virtual ~ExerciseFileLoader();
    //@}

    //! @name Operations
    //@{
    virtual FileLoader& LoadAndUpdate( const std::string& rootTag, T_Loader loader, const std::string& xslTransform );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ExerciseFileLoader( const ExerciseFileLoader& );            //!< Copy constructor
    ExerciseFileLoader& operator=( const ExerciseFileLoader& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};
}

#endif // __ExerciseFileLoader_h_
