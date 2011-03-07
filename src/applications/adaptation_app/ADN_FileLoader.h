// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_FileLoader_h_
#define __ADN_FileLoader_h_

#include "tools/Loader_ABC.h"
#include <memory>

namespace tools 
{
    class GeneralConfig;
    class RealFileLoader_ABC;
    class SchemaVersionExtractor_ABC;
    class RealFileLoaderObserver_ABC;
}

// =============================================================================
/** @class  ADN_FileLoader
*/
// Created: NLD 2011-03-07
// =============================================================================
class ADN_FileLoader : public tools::Loader_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    ADN_FileLoader( tools::GeneralConfig& config, tools::RealFileLoaderObserver_ABC& observer );
    virtual ~ADN_FileLoader();
    //@}

        //! @name Operations
    //@{
    virtual void        CheckFile       ( const std::string& file ) const;
    virtual void        LoadFile        ( const std::string& fileName, T_Loader loader ) const;
    virtual std::string LoadPhysicalFile( const std::string& rootTag, T_Loader loader ) const; // Return the file path/name loaded
    //@}

private:
    tools::GeneralConfig& config_;
    tools::RealFileLoaderObserver_ABC& observer_;
    std::auto_ptr< tools::RealFileLoader_ABC > fileLoader_;    
    std::auto_ptr< tools::SchemaVersionExtractor_ABC > schemaVersionExtractor_; 
};

#endif // __ADN_FileLoader_h_
