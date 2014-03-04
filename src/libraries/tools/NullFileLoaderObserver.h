// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef Tools_NullFileLoaderObserver_h
#define Tools_NullFileLoaderObserver_h

#include "RealFileLoaderObserver_ABC.h"
#include <boost/shared_ptr.hpp>

namespace tools
{

// =============================================================================
/** @class  NullFileLoaderObserver
    @brief  NullFileLoaderObserver
*/
// Created: NLD 2011-02-14
// =============================================================================
class NullFileLoaderObserver : public RealFileLoaderObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             NullFileLoaderObserver();
    virtual ~NullFileLoaderObserver();
    //@}

    //! @name Operations
    //@{
    virtual bool NotifyInvalidXml( const Path& file, const xml::exception& e );
    virtual void NotifyNoXmlSchemaSpecified( const Path& file );
    virtual void NotifyFileMigrated( const Path& file , const std::string& fromVersion, const std::string& toVersion );
    //@}
};

boost::shared_ptr< NullFileLoaderObserver > CreateNullFileLoaderObserver();

}

#endif // Tools_NullFileLoaderObserver_h
