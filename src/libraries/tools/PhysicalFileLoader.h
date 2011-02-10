// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __PhysicalFileLoader_h_
#define __PhysicalFileLoader_h_

#include "FileLoader.h"

namespace tools
{
// =============================================================================
/** @class  PhysicalFileLoader
    @brief  PhysicalFileLoader
*/
// Created: LDC 2010-11-29
// =============================================================================
class PhysicalFileLoader : public FileLoader
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit PhysicalFileLoader( const tools::ExerciseConfig& config );
             PhysicalFileLoader( const tools::ExerciseConfig& config, std::string& invalidSignatureFiles, std::string& missingSignatureFiles );
    virtual ~PhysicalFileLoader();
    //@}

    //! @name Operations
    //@{
    virtual FileLoader& LoadAndUpdate( const std::string& rootTag, T_Loader loader, const std::string& xslTransform );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PhysicalFileLoader( const PhysicalFileLoader& );            //!< Copy constructor
    PhysicalFileLoader& operator=( const PhysicalFileLoader& ); //!< Assignment operator
    //@}
};
}

#endif // __PhysicalFileLoader_h_
