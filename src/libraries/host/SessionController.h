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
#include <tools/Path.h>

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
                                const runtime::FileSystem_ABC& fs,
                                const SessionFactory_ABC& sessions,
                                const NodeController_ABC& nodes,
                                const tools::Path& root,
                                const tools::Path& cwd,
                                const tools::Path& simulation,
                                const tools::Path& replayer,
                                const tools::Path& timeline,
                                runtime::Pool_ABC& pool );
    virtual ~SessionController();
    //@}

    //! @name Methods
    //@{
    virtual void        Reload     ( T_Predicate predicate );
    virtual T_Sessions  List       ( T_Predicate predicate = T_Predicate(), int offset = 0, int limit = INT_MAX ) const;
    virtual size_t      Count      ( T_Predicate predicate = T_Predicate() ) const;
    virtual bool        Filter     ( T_Predicate predicate, const Uuid& id ) const;
    virtual T_Session   Get        ( const Uuid& node, const Uuid& id ) const;
    virtual T_Session   Create     ( const web::User& user, const web::session::Config& cfg, const std::string& exercise );
    virtual T_Session   Delete     ( const Uuid& node, const Uuid& id );
    virtual void        DeleteUser ( const web::User& user, int id );
    virtual T_Session   Start      ( const web::User& user, const Uuid& id, const std::string& checkpoint ) const;
    virtual T_Session   Stop       ( const web::User& user, const Uuid& id ) const;
    virtual T_Session   Pause      ( const web::User& user, const Uuid& id ) const;
    virtual T_Session   Update     ( const web::User& user, const Uuid& id, const Tree& cfg ) const;
    virtual T_Session   Archive    ( const web::User& user, const Uuid& id ) const;
    virtual T_Session   Restore    ( const web::User& user, const Uuid& id ) const;
    virtual T_Session   Download   ( const web::User& user, const Uuid& id, web::Chunker_ABC& dst ) const;
    virtual T_Session   Replay     ( const web::User& user, const Uuid& id );
    virtual void        NotifyNode ( const Uuid& node );
    virtual T_Session   DownloadLog( const web::User& user, const Uuid& id, web::Chunker_ABC& dst, const std::string& logFile, int limitSize,
                                     bool deflate ) const;

    //@}

private:
    //! @name Private methods
    //@{
    void ReloadSession( const tools::Path& path, T_Predicate predicate );
    bool ReloadDirectory( runtime::Async& reload, const tools::Path& dir, T_Predicate predicate );
    void ReloadReplay( Session_ABC& session );
    void Refresh();
    void RefreshSession( T_Session session );
    void DeleteUser( T_Session session, const web::User& user, int id );
    void RefreshSize();
    void RefreshSize( T_Session session );
    void Save( const Session_ABC& session ) const;
    void Create( Session_ABC& session );
    //@}

    //! @name Private template helpers
    //@{
    template< typename T >
    T_Session Dispatch( const web::User& user, const Uuid& id, const T& operand ) const;
    typedef std::function< bool( const T_Session& ) > T_Operand;
    void Apply( T_Session session, const T_Operand& operand ) const;
    //@}

private:
    //! @name Private members
    //@{
    cpplog::BaseLogger& log_;
    const runtime::Runtime_ABC& runtime_;
    const runtime::FileSystem_ABC& fs_;
    const SessionFactory_ABC& factory_;
    const NodeController_ABC& nodes_;
    const tools::Path root_;
    const tools::Path trash_;
    const tools::Path cwd_;
    const tools::Path simulation_;
    const tools::Path replayer_;
    const tools::Path timeline_;
    Container< Session_ABC > sessions_;
    runtime::Timer timer_;
    runtime::Timer sizes_;
    mutable runtime::Async async_;
    //@}
};
}

#endif // SESSION_CONTROLLER_H
