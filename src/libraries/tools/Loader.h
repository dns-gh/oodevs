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

#include "Loader_ABC.h"
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
class Loader : public Loader_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    Loader( const ExerciseConfig& config, RealFileLoaderObserver_ABC& observer );
    virtual ~Loader();
    //@}

    //! @name Operations
    //@{
    virtual void                           CheckFile       ( const std::string& file ) const;
    virtual void                           LoadFile        ( const std::string& fileName, T_Loader loader ) const;
    virtual std::auto_ptr< xml::xistream > LoadFile        ( const std::string& fileName ) const;
    virtual std::string                    LoadPhysicalFile( const std::string& rootTag, T_Loader loader ) const; // Return the file path/name loaded
    //@}

private:
    const ExerciseConfig& config_;
    RealFileLoaderObserver_ABC& observer_;
    std::auto_ptr< SchemaVersionExtractor_ABC > schemaVersionExtractor_;
    std::auto_ptr< RealFileLoader_ABC > fileLoader_;
};

}

#endif // tools_FileLoader_h
