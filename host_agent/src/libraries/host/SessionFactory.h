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

namespace cpplog
{
    class BaseLogger;
}

namespace runtime
{
    class Runtime_ABC;
}

namespace host
{
    class FileSystem_ABC;
    class Pool_ABC;
    class PortFactory_ABC;
    class UuidFactory_ABC;

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
             SessionFactory( cpplog::BaseLogger& log, const runtime::Runtime_ABC& runtime, const UuidFactory_ABC& uuids,
                             const FileSystem_ABC& system, PortFactory_ABC& ports, const boost::filesystem::path& data,
                             const boost::filesystem::path& applications, Pool_ABC& pool );
    virtual ~SessionFactory();
    //@}

    //! @name Methods
    //@{
    virtual boost::shared_ptr< Session_ABC > Create( const boost::uuids::uuid& node, const std::string& exercise, const std::string& name ) const;
    virtual T_Sessions Reload() const;
    virtual std::vector< std::string > GetExercises() const;
    //@}

private:
    //! @name Private members
    //@{
    mutable cpplog::BaseLogger& log_;
    const runtime::Runtime_ABC& runtime_;
    const UuidFactory_ABC& uuids_;
    const FileSystem_ABC& system_;
    const boost::filesystem::path data_;
    const boost::filesystem::path applications_;
    const boost::filesystem::path output_;
    Pool_ABC& pool_;
    PortFactory_ABC& ports_;
    //@}
};

}

#endif // SESSION_FACTORY_H
