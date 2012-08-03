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
#include "web/SessionConfig.h"

#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/uuid/uuid.hpp>

namespace boost
{
    class shared_mutex;
namespace posix_time
{
    class ptime;
}
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
}

namespace host
{
    struct Node_ABC;
    struct Port_ABC;
    struct PortFactory_ABC;
    typedef boost::shared_ptr< const Port_ABC > Port;
namespace node
{
    struct Token;
}
}

namespace host
{
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
             Session( const runtime::FileSystem_ABC& system,
                      web::Client_ABC& client,
                      boost::shared_ptr< Node_ABC > node,
                      const Path& root,
                      const Uuid& id,
                      const web::session::Config& cfg,
                      const std::string& exercise,
                      const Port& port
                      );
             Session( const runtime::FileSystem_ABC& system,
                      web::Client_ABC& client,
                      boost::shared_ptr< Node_ABC > node,
                      const Path& root,
                      const Tree& tree,
                      const runtime::Runtime_ABC& runtime,
                      PortFactory_ABC& ports
                      );
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
    //@}

    //! @name Public methods
    //@{
    virtual Path GetPath( const std::string& type ) const;
    virtual Path GetOutput() const;
    virtual Tree Save() const;
    virtual bool Start( const runtime::Runtime_ABC& runtime, const Path& apps );
    virtual bool Stop();
    virtual bool Refresh();
    virtual bool RefreshSize();
    virtual bool Poll();
    virtual bool Pause();
    virtual void Remove();
    virtual bool Update( const Tree& cfg );
    //@}

    //! @name Typedef helpers
    //@{
    typedef boost::shared_ptr< runtime::Process_ABC > T_Process;
    typedef std::vector< std::string > T_Clients;
    //@}

    //! @name Status enumeration
    //@{
    enum Status
    {
        STATUS_STOPPED,
        STATUS_PLAYING,
        STATUS_REPLAYING,
        STATUS_PAUSED,
        STATUS_COUNT
    };
    //@}

private:
    //! @name Private methods
    //@{
    Tree GetProperties( bool save ) const;
    bool StopProcess( boost::upgrade_lock< boost::shared_mutex >& lock );
    bool ModifyStatus( boost::upgrade_lock< boost::shared_mutex >& lock, Status next );
    //@}

private:
    //! @name Private members
    //@{
    const runtime::FileSystem_ABC& system_;
    web::Client_ABC& client_;
    const boost::shared_ptr< Node_ABC > node_;
    const Uuid id_;
    const Path root_;
    const Tree links_;
    const Port port_;
    mutable boost::shared_mutex access_;
    T_Process process_;
    boost::shared_ptr< node::Token > running_;
    Status status_;
    bool polling_;
    size_t counter_;
    bool sizing_;
    size_t size_;
    T_Clients clients_;
    std::string start_time_;
    std::string current_time_;
    web::session::Config cfg_;
    //@}
};
}

#endif // SESSION_H
