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

#include "Container.h"
#include "runtime/Async.h"
#include "runtime/Timer_ABC.h"
#include <boost/filesystem/path.hpp>

namespace cpplog
{
    class BaseLogger;
}

namespace runtime
{
    struct FileSystem_ABC;
    struct Pool_ABC;
    struct Runtime_ABC;
}

namespace host
{
    struct NodeController_ABC;
    struct PortFactory_ABC;
    struct SessionFactory_ABC;
    struct UuidFactory_ABC;
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
                                const runtime::FileSystem_ABC& system,
                                const SessionFactory_ABC& sessions,
                                const NodeController_ABC& nodes,
                                const Path& root,
                                const Path& apps,
                                runtime::Pool_ABC& pool );
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
    virtual T_Session   Pause( const Uuid& id ) const;
    //@}

private:
    //! @name Private methods
    //@{
    void Update();
    void UpdateSession( T_Session session );
    void Save( const Session_ABC& session ) const;
    void Create( Session_ABC& session );
    //@}

    //! @name Private template helpers
    //@{
    template< typename T >
    T_Session Dispatch( const Uuid& id, const T& operand ) const;
    //@}

private:
    //! @name Private members
    //@{
    cpplog::BaseLogger& log_;
    const runtime::Runtime_ABC& runtime_;
    const runtime::FileSystem_ABC& system_;
    const SessionFactory_ABC& factory_;
    const NodeController_ABC& nodes_;
    const Path root_;
    const Path apps_;
    Container< Session_ABC > sessions_;
    runtime::Timer timer_;
    mutable runtime::Async async_;
    //@}
};
}

#endif // SESSION_CONTROLLER_H
