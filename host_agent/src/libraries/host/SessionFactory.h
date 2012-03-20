// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SESSION_FACTORY_H
#define SESSION_FACTORY_H

#include "SessionFactory_ABC.h"

namespace runtime
{
    class Runtime_ABC;
}

namespace host
{
    class UuidFactory_ABC;
    class FileSystem_ABC;

// =============================================================================
/** @class  SessionFactory
    @brief  SessionFactory interface
*/
// Created: BAX 2012-03-19
// =============================================================================
class SessionFactory : public SessionFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SessionFactory( const runtime::Runtime_ABC& runtime, const UuidFactory_ABC& uuids, const FileSystem_ABC& system,
                             const boost::filesystem::wpath& data, const boost::filesystem::wpath& applications );
    virtual ~SessionFactory();
    //@}

    //! @name Methods
    //@{
    virtual boost::shared_ptr< Session_ABC > Create( const std::string& exercise, const std::string& name, int port ) const;
    //@}

private:
    //! @name Private members
    //@{
    const runtime::Runtime_ABC& runtime_;
    const UuidFactory_ABC& uuids_;
    const FileSystem_ABC& system_;
    const boost::filesystem::wpath data_;
    const boost::filesystem::wpath applications_;
    const boost::filesystem::wpath output_;
    //@}
};

}

#endif // SESSION_FACTORY_H
