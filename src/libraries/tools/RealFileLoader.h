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
    class FileMigration_ABC;
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
    virtual std::auto_ptr< xml::xistream > LoadFile( const std::string& file, RealFileLoaderObserver_ABC& observer ) const;
    //@}

private:
    //! @name Operations
    //@{
    void ReadDefaultSchemaAssignment( xml::xistream& xis );
    void ReadMigration              ( xml::xistream& xis );

    const std::string&             AssignDefaultSchema ( xml::xistream& xis, const std::string& currentSchema ) const;
    std::auto_ptr< xml::xistream > UpgradeToLastVersion( const std::string& inputFile, std::auto_ptr< xml::xistream > xis, const std::string& initialSchema, const std::string& initialVersion, RealFileLoaderObserver_ABC& observer ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::list< boost::shared_ptr< FileMigration_ABC > > T_Migrations;
    typedef std::map< std::string, std::string >                T_DefaultSchemasAssignment;
    //@}

private:
    const SchemaVersionExtractor_ABC& versionExtractor_;
    T_Migrations migrations_;
    T_DefaultSchemasAssignment defaultSchemasAssignment_;
};

} // end namespace tools

#endif // RealFileLoader_h
