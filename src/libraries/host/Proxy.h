// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef PROXY_H
#define PROXY_H

#include "Proxy_ABC.h"

#include "runtime/Async.h"
#include "runtime/Timer_ABC.h"
#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <boost/thread/mutex.hpp>

namespace boost
{
    class mutex;
}

namespace cpplog
{
    class BaseLogger;
}

namespace runtime
{
    struct FileSystem_ABC;
    struct Pool_ABC;
    struct Process_ABC;
    struct Runtime_ABC;
}

namespace web
{
    struct Client_ABC;
}

namespace host
{
    typedef boost::filesystem::path Path;
    typedef boost::property_tree::ptree Tree;
namespace proxy
{
    struct Link;
}
}

namespace host
{
namespace proxy
{
struct Ssl
{
    Ssl( bool enabled, const Path& certificate, const Path& key )
        : enabled    ( enabled )
        , certificate( certificate )
        , key        ( key )
    {
        // NOTHING
    }
    bool enabled;
    const Path certificate;
    const Path key;
private:
    Ssl& operator=( const Ssl& );
};

struct Config
{
    Config( const Path& root, const Path& app, int http, int tcp, const Ssl& ssl )
        : root( root )
        , app ( app )
        , http( http )
        , tcp ( tcp )
        , ssl ( ssl )
    {
        // NOTHING
    }
    const Path root;
    const Path app;
    const int http;
    const int tcp;
    const Ssl ssl;
private:
    Config& operator=( const Config& );
};
}

// =============================================================================
/** @class  Proxy
    @brief  Proxy class definition
*/
// Created: BAX 2012-04-11
// =============================================================================
class Proxy : public Proxy_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Proxy( cpplog::BaseLogger& log, const runtime::Runtime_ABC& runtime,
                    const runtime::FileSystem_ABC& fs, const proxy::Config& config,
                    web::Client_ABC& client, runtime::Pool_ABC& pool );
    virtual ~Proxy();
    //@}

    //! @name Methods
    //@{
    virtual int GetPort() const;
    virtual void Register( const std::string& prefix, const std::string& host, int port );
    virtual void Unregister( const std::string& prefix );
    //@}

    //! @name Typedef helpers
    //@{
    typedef std::map< const std::string, proxy::Link > T_Links;
    typedef boost::shared_ptr< runtime::Process_ABC > T_Process;
    //@}

    //! @name Implementation methods
    void Update();
    //@}

private:
    //! @name Private methods
    Tree GetProperties() const;
    void Save() const;
    bool Reload( const Path& path );
    void Start();
    void Stop();
    void Restart();
    void RegisterMissingLinks();
    T_Process MakeProcess() const;
    void HttpRegister( const std::string& prefix, const proxy::Link& proxy );
    void HttpUnregister( const std::string& prefix );
    //@}

    //! @name Member data
    //@{
    cpplog::BaseLogger& log_;
    const runtime::Runtime_ABC& runtime_;
    const runtime::FileSystem_ABC& fs_;
    const proxy::Config config_;
    boost::mutex access_;
    web::Client_ABC& client_;
    T_Process process_;
    T_Links links_;
    runtime::Timer timer_;
    mutable runtime::Async async_;
    //@}
};
}

#endif // PROXY_H
