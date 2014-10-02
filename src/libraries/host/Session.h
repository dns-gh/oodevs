// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SESSION_H
#define SESSION_H

#include "Session_ABC.h"
#include "runtime/Async.h"
#include "web/Configs.h"
#include "web/User.h"

#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/uuid/uuid.hpp>
#include <set>

namespace boost
{
    class shared_mutex;
namespace posix_time
{
    class ptime;
}
}

namespace cpplog
{
    class BaseLogger;
}

namespace runtime
{
    struct FileSystem_ABC;
    struct Runtime_ABC;
    struct Process_ABC;
}

namespace web
{
    struct Client_ABC;
    class Plugins;
}

namespace host
{
    struct Node_ABC;
    struct NodeController_ABC;
    struct Port_ABC;
    struct PortFactory_ABC;
    typedef boost::shared_ptr< const Port_ABC > Port;
    struct UuidFactory_ABC;
namespace node
{
    struct Token;
}
}

namespace host
{
// -----------------------------------------------------------------------------
// Name: SessionDependencies
// Created: BAX 2012-08-10
// -----------------------------------------------------------------------------
struct SessionDependencies
{
    SessionDependencies( const runtime::FileSystem_ABC& fs,
                         const runtime::Runtime_ABC& runtime,
                         const web::Plugins& plugins,
                         const NodeController_ABC& nodes,
                         const UuidFactory_ABC& uuids,
                         cpplog::BaseLogger& log,
                         web::Client_ABC& client,
                         runtime::Pool_ABC& pool,
                         PortFactory_ABC& ports )
        : fs     ( fs )
        , runtime( runtime )
        , plugins( plugins )
        , nodes  ( nodes )
        , uuids  ( uuids )
        , log    ( log )
        , client ( client )
        , pool   ( pool )
        , ports  ( ports )
    {
        // NOTHING
    }
    const runtime::FileSystem_ABC& fs;
    const runtime::Runtime_ABC& runtime;
    const web::Plugins& plugins;
    const NodeController_ABC& nodes;
    const UuidFactory_ABC& uuids;
    cpplog::BaseLogger& log;
    web::Client_ABC& client;
    runtime::Pool_ABC& pool;
    PortFactory_ABC& ports;
private:
    SessionDependencies& operator=( const SessionDependencies& );
};

// -----------------------------------------------------------------------------
// Name: SessionPaths
// Created: BAX 2012-08-09
// -----------------------------------------------------------------------------
struct SessionPaths
{
    SessionPaths( const Path& root, const Path& trash )
        : root ( root )
        , trash( trash )
    {
        // NOTHING
    }
    Path root;
    Path trash;
};

// =============================================================================
/** @class  Session
    @brief  Session class definition
*/
// Created: BAX 2012-03-16
// =============================================================================
class Session : public Session_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Session( const SessionDependencies& deps,
                      boost::shared_ptr< Node_ABC > node,
                      const SessionPaths& paths,
                      const web::session::Config& cfg,
                      const std::string& exercise,
                      const Uuid& replay,
                      const web::User& owner );
             Session( const SessionDependencies& deps,
                      boost::shared_ptr< Node_ABC > node,
                      const SessionPaths& paths,
                      const Tree& tree );
    virtual ~Session();
    //@}

    //! @name Session_ABC methods
    //@{
    virtual Uuid GetId() const;
    virtual Path GetRoot() const;
    virtual Uuid GetNode() const;
    virtual Path GetExercise() const;
    virtual std::string GetName() const;
    virtual int GetPort() const;
    virtual Tree GetProperties() const;
    virtual bool IsReplay() const;
    virtual Uuid GetReplayId() const;
    virtual bool HasReplays() const;
    virtual Tree AvailableLogs() const;
    virtual bool IsAuthorized( const web::User& user ) const;
    //@}

    //! @name Public methods
    //@{
    virtual Path  GetPath( const std::string& type ) const;
    virtual Path  GetOutput() const;
    virtual Tree  Save() const;
    virtual bool  Start( const Path& app, const Path& timeline, const std::string& checkpoint );
    virtual bool  Stop();
    virtual bool  Refresh();
    virtual bool  RefreshSize();
    virtual bool  Poll();
    virtual bool  Pause();
    virtual void  Remove();
    virtual bool  Update( const Tree& cfg );
    virtual bool  Archive();
    virtual bool  Restore();
    virtual bool  Download( web::Chunker_ABC& dst ) const;
    virtual T_Ptr Replay( const web::User& owner );
    virtual void  AttachReplay( const Session_ABC& replay );
    virtual void  DetachReplay( const Session_ABC& replay );
    virtual void  NotifyNode();
    virtual bool  DownloadLog( web::Chunker_ABC& dst, const std::string& logFile, int limitSize, bool deflate ) const;
    virtual void DeleteUser( const web::User& user, int id );
    //@}

    //! @name Typedef helpers
    //@{
    typedef boost::shared_ptr< runtime::Process_ABC > T_Process;
    typedef std::vector< std::string > T_Clients;
    typedef std::vector< std::string > T_Checkpoints;
    typedef std::set< Uuid > T_Replays;
    //@}

    //! @name Status enumeration
    //@{
    enum Status
    {
        STATUS_STOPPED,
        STATUS_PLAYING,
        STATUS_REPLAYING,
        STATUS_PAUSED,
        STATUS_ARCHIVED,
        STATUS_WAITING,
        STATUS_COUNT
    };
    //@}

private:
    //! @name Private methods
    //@{
    Tree GetProperties( bool save ) const;
    bool StopWith( boost::unique_lock< boost::shared_mutex >& mutex, bool parse, bool error );
    void ClearOutput( const Path& path );
    void ParseCheckpoints();
    void ParseExerciseProperties();
    bool SendStatus( Status next );
    T_Process StartSimulation( const web::session::Config& cfg,
                               const std::string& checkpoint,
                               bool replay,
                               const Path& output,
                               const Path& app ) const;
    struct PrivateState;
    boost::shared_ptr< PrivateState > PrepareStart( const std::string& checkpoint );
    //@}

private:
    //! @name Private const members
    //@{
    const SessionDependencies deps_;
    const boost::shared_ptr< Node_ABC > node_;
    const Uuid id_;
    const SessionPaths paths_;
    const Tree links_;
    const Port port_;
    const Uuid replay_;
    //@}

    //! @name Private members
    //@{
    mutable boost::shared_mutex access_;
    T_Process process_;
    T_Process timeline_;
    boost::shared_ptr< node::Token > running_;
    Status status_;
    bool busy_;
    bool polling_;
    bool sizing_;
    size_t counter_;
    size_t size_;
    T_Clients clients_;
    std::string start_time_;
    std::string current_time_;
    std::string last_error_;
    web::session::Config cfg_;
    T_Checkpoints checkpoints_;
    bool first_time_;
    T_Replays replays_;
    mutable runtime::Async async_;
    web::User owner_;
    //@}
};
}

#endif // SESSION_H
