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
    virtual void                           CheckFile        ( const Path& file ) const;
    virtual void                           CheckOptionalFile( const Path& file ) const;
    virtual void                           LoadFile         ( const Path& fileName, T_Loader loader ) const;
    virtual bool                           LoadOptionalFile ( const Path& fileName, T_Loader loader ) const;
    virtual std::unique_ptr< xml::xistream > LoadFile         ( const Path& fileName ) const;
    virtual const RealFileLoaderObserver_ABC& GetObserver() const;
    //@}

protected:
    //! @name Member data
    //@{
    RealFileLoaderObserver_ABC& observer_;
    std::unique_ptr< SchemaVersionExtractor_ABC > schemaVersionExtractor_;
    std::unique_ptr< RealFileLoader_ABC > fileLoader_;
    //@}
};

}

#endif // tools_DefaultLoader_h
