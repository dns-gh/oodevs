// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef ADN_FileLoaderObserver_h
#define ADN_FileLoaderObserver_h

#include "tools/RealFileLoaderObserver_ABC.h"
#include <string>

// =============================================================================
/** @class  ADN_FileLoaderObserver
    @brief  ADN_FileLoaderObserver
*/
// Created: NLD 2011-02-14
// =============================================================================
class ADN_FileLoaderObserver : public tools::RealFileLoaderObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_FileLoaderObserver();
    virtual ~ADN_FileLoaderObserver();
    //@}

    //! @name Operations
    //@{
    virtual bool NotifyInvalidXml          ( const std::string& file, const xml::exception& e );
    virtual void NotifyNoXmlSchemaSpecified( const std::string& file );
    virtual void NotifyFileMigrated        ( const std::string& file , const std::string& fromVersion, const std::string& toVersion );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetInvalidSignedFiles() const;
    //@}

private:
    std::string invalidSignedFiles_;
};

#endif // ADN_FileLoaderObserver_h
