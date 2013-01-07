// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef FileLoaderObserver_h
#define FileLoaderObserver_h

#include "tools/RealFileLoaderObserver_ABC.h"

// =============================================================================
/** @class  FileLoaderObserver
    @brief  FileLoaderObserver
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
    //@}
};

#endif // FileLoaderObserver_h
