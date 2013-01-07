// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef Preparation_FileLoaderObserver_h
#define Preparation_FileLoaderObserver_h

#include "tools/RealFileLoaderObserver_ABC.h"

// =============================================================================
/** @class  FileLoader
    @brief  FileLoader
*/
// Created: NLD 2011-02-14
// =============================================================================
class FileLoaderObserver : public tools::RealFileLoaderObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FileLoaderObserver();
    virtual ~FileLoaderObserver();
    //@}

    //! @name Operations
    //@{
    virtual bool NotifyInvalidXml          ( const std::string& file, const xml::exception& e );
    virtual void NotifyNoXmlSchemaSpecified( const std::string& file );
    virtual void NotifyFileMigrated        ( const std::string& file , const std::string& fromVersion, const std::string& toVersion );

    void DisplayErrors() const;
    virtual void GetSignatureErrors( std::vector< std::string >& errors ) const;
    virtual void GetXsdErrors( std::vector< std::string >& errors ) const;
    virtual void Purge();
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< std::string > ::const_iterator CIT_Error;
    //@}

    std::vector< std::string > invalidSignatureFiles_;
    std::vector< std::string > missingSignatureFiles_;
    std::vector< std::string > malformedFiles_;
    std::vector< std::string > missingSchemaFiles_;
};

#endif // Preparation_FileLoaderObserver_h
