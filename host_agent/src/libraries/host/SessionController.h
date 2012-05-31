// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SESSION_CONTROLLER_H
#define SESSION_CONTROLLER_H

#include "SessionController_ABC.h"

#include <boost/filesystem/path.hpp>

namespace cpplog
{
    class BaseLogger;
}

namespace runtime
{
    class Runtime_ABC;
    class Process_ABC;
}

namespace host
{
    struct Async;
    class FileSystem_ABC;
    class Pool_ABC;
    class PortFactory_ABC;
    struct SessionFactory_ABC;
    class UuidFactory_ABC;
    template< typename T > class Container;
    typedef boost::filesystem::path Path;

// =============================================================================
/** @class  SessionFactory
    @brief  SessionFactory interface
*/
// Created: BAX 2012-03-19
// =============================================================================
class SessionController : public SessionController_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SessionController( cpplog::BaseLogger& log,
                                const runtime::Runtime_ABC& runtime,
                                const FileSystem_ABC& system,
                                const SessionFactory_ABC& sessions,
                                const Path& logs,
                                const Path& data,
                                const Path& apps,
                                Pool_ABC& pool );
    virtual ~SessionController();
    //@}

    //! @name Methods
    //@{
    virtual void        Reload( T_Predicate predicate );
    virtual T_Sessions  List( T_Predicate predicate = T_Predicate(), int offset = 0, int limit = INT_MAX ) const;
    virtual size_t      Count( T_Predicate predicate = T_Predicate() ) const;
    virtual bool        Has( const Uuid& id ) const;
    virtual T_Session   Get( const Uuid& id ) const;
    virtual T_Session   Create( const Uuid& node, const std::string& name, const std::string& exercise );
    virtual T_Session   Delete( const Uuid& id );
    virtual T_Session   Start( const Uuid& id ) const;
    virtual T_Session   Stop( const Uuid& id ) const;
    //@}

private:
    //! @name Private methods
    //@{
    void Create( Session_ABC& session, bool isReload );
    void Save( const Session_ABC& session ) const;
    boost::shared_ptr< runtime::Process_ABC > StartWith( const Session_ABC& session ) const;
    void Start( Session_ABC& session, bool mustSave ) const;
    void Stop( Session_ABC& session, bool skipSave ) const;
    Path GetPath( const Session_ABC& session ) const;
    //@}

private:
    //! @name Private members
    //@{
    mutable cpplog::BaseLogger& log_;
    const runtime::Runtime_ABC& runtime_;
    const FileSystem_ABC& system_;
    const SessionFactory_ABC& factory_;
    const Path logs_;
    const Path data_;
    const Path apps_;
    const std::auto_ptr< Container< Session_ABC > > sessions_;
    const std::auto_ptr< Async > async_;
    //@}
};

}

#endif // SESSION_CONTROLLER_H
