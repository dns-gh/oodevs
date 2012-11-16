// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef RealFileLoaderObserver_ABC_h
#define RealFileLoaderObserver_ABC_h

#include <string>
#include <vector>
#include <tools/XmlCrc32Signature.h>

namespace xml
{
    class exception;
}

namespace tools
{
// =============================================================================
/** @class  RealFileLoader_ABC
    @brief  RealFileLoader_ABC
*/
// Created: NLD 2011-02-14
// =============================================================================
class RealFileLoaderObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             RealFileLoaderObserver_ABC() {}
    virtual ~RealFileLoaderObserver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool NotifySignatureError      ( const std::string& file, const EXmlCrc32SignatureError& error ) = 0;
    virtual bool NotifyInvalidXml          ( const std::string& file, const xml::exception& e ) = 0;
    virtual void NotifyNoXmlSchemaSpecified( const std::string& file ) = 0;
    virtual void NotifyFileMigrated        ( const std::string& file , const std::string& fromVersion, const std::string& toVersion ) = 0;
    virtual void GetSignatureErrors( std::vector< std::string >& /* errors */ ) const { /* NOTHING */ }
    virtual void GetXsdErrors( std::vector< std::string >& /* errors */ ) const { /* NOTHING */ }
    virtual void Purge() { /* NOTHING */ };
    //@}
};

}

#endif // RealFileLoaderObserver_ABC_h
