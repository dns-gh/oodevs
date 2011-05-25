// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef tools_DefaultLoader_h
#define tools_DefaultLoader_h

#include "Loader_ABC.h"

namespace tools
{
    class RealFileLoader_ABC;
    class SchemaVersionExtractor_ABC;
    class RealFileLoaderObserver_ABC;

// =============================================================================
/** @class  Loader
    @brief  Loader
*/
// Created: NLD 2011-02-14
// =============================================================================
class DefaultLoader : public Loader_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DefaultLoader( RealFileLoaderObserver_ABC& observer );
    virtual ~DefaultLoader();
    //@}

    //! @name Operations
    //@{
    virtual void                           CheckFile        ( const std::string& file ) const;
    virtual void                           CheckOptionalFile( const std::string& file ) const;
    virtual void                           LoadFile         ( const std::string& fileName, T_Loader loader ) const;
    virtual std::auto_ptr< xml::xistream > LoadFile         ( const std::string& fileName ) const;
    
    virtual std::string                    LoadPhysicalFile        ( const std::string& rootTag, T_Loader loader ) const; // Invalid for this class
    virtual std::string                    LoadOptionalPhysicalFile( const std::string& rootTag, T_Loader loader ) const; // Invalid for this class
    //@}

protected:
    //! @name Member data
    //@{
    RealFileLoaderObserver_ABC& observer_;
    std::auto_ptr< SchemaVersionExtractor_ABC > schemaVersionExtractor_;
    std::auto_ptr< RealFileLoader_ABC > fileLoader_;
    //@}
};

}

#endif // tools_DefaultLoader_h
