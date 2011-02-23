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

namespace tools
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
             ExerciseFileLoader( const tools::ExerciseConfig& config, std::string& invalidSignatureFiles, std::string& missingSignatureFiles );
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
};
}

#endif // __ExerciseFileLoader_h_
