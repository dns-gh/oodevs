// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef tools_FileLoader_h
#define tools_FileLoader_h

#include "DefaultLoader.h"
#include <memory>

namespace tools
{
    class RealFileLoader_ABC;
    class SchemaVersionExtractor_ABC;
    class RealFileLoaderObserver_ABC;
    class ExerciseConfig;

// =============================================================================
/** @class  Loader
    @brief  Loader
*/
// Created: NLD 2011-02-14
// =============================================================================
class Loader : public DefaultLoader
{
public:
    //! @name Constructors/Destructor
    //@{
             Loader( const Path& physicalFile, const ExerciseConfig& config,
                     RealFileLoaderObserver_ABC& observer );
    virtual ~Loader();
    //@}

    //! @name Operations
    //@{
    virtual Path LoadPhysicalFile        ( const std::string& rootTag, T_Loader loader ) const; // Return the file path/name loaded
    virtual Path LoadOptionalPhysicalFile( const std::string& rootTag, T_Loader loader ) const; // Return the file path/name loaded
    virtual Path GetPhysicalChildFile( const std::string& rootTag ) const;
    virtual Path GetPhysicalChildPath( const std::string& rootTag ) const;
    //@}

private:
    Path LoadPhysicalFile( const std::string& rootTag, T_Loader loader, bool optional ) const;

private:
    //! @name Member data
    //@{
    const ExerciseConfig& config_;
    std::map< std::string, std::string > allowedFiles_;
    std::map< std::string, std::string > allowedPaths_;
    //@}
};

}

#endif // tools_FileLoader_h
