// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef RealFileLoader_h
#define RealFileLoader_h

#include "RealFileLoader_ABC.h"
#include <boost/shared_ptr.hpp>
#include <list>
#include <map>
#include <memory>

namespace xml
{
    class xistream;
}

namespace tools
{
    class FileMatcher_ABC;
    class FileMatcherFactory_ABC;
    class FileMigration_ABC;
    class Path;
    class SchemaVersionExtractor_ABC;

// =============================================================================
/** @class  RealFileLoader_ABC
    @brief  RealFileLoader_ABC
*/
// Created: NLD 2011-02-14
// =============================================================================
class RealFileLoader : public RealFileLoader_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit RealFileLoader( xml::xistream& xis, const SchemaVersionExtractor_ABC& versionExtractor );
    virtual ~RealFileLoader();
    //@}

    //! @name Operations
    //@{
    virtual std::unique_ptr< xml::xistream > LoadFile( const Path& file, RealFileLoaderObserver_ABC& observer ) const;
    //@}

private:
    //! @name Operations
    //@{
    void ReadFileMatcher( xml::xistream& xis );
    void ReadAddedFile  ( xml::xistream& xis );
    void ReadMigration  ( xml::xistream& xis );

    bool                           AssignDefaultSchema ( const Path& inputFileName, xml::xistream& xis, Path& newSchema ) const;
    std::unique_ptr< xml::xistream > UpgradeToLastVersion( const Path& inputFileName, std::unique_ptr< xml::xistream > xis, const Path& initialSchema, const Path& initialVersion, RealFileLoaderObserver_ABC& observer ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< Path, Path >                             T_AddedFile; // filename/default file
    typedef std::list< T_AddedFile >                            T_AddedFiles;
    typedef std::list< boost::shared_ptr< FileMigration_ABC > > T_Migrations;
    typedef std::list< boost::shared_ptr< FileMatcher_ABC > >   T_DefaultSchemasAssignment;
    //@}

private:
    const std::unique_ptr< FileMatcherFactory_ABC > fileMatcherFactory_;
    const SchemaVersionExtractor_ABC& versionExtractor_;
    T_Migrations migrations_;
    T_DefaultSchemasAssignment defaultSchemasAssignment_;
    T_AddedFiles addedFiles_;
};

} // end namespace tools

#endif // RealFileLoader_h
