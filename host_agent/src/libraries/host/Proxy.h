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
#include "runtime/Event.h"
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
    typedef boost::filesystem3::path Path;
    typedef boost::property_tree::ptree Tree;
    struct ProxyLink;

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
                    const runtime::FileSystem_ABC& system, const Path& logs,
                    const Path& java, const Path& jar,
                    int port, web::Client_ABC& client, runtime::Pool_ABC& pool );
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
    typedef std::map< const std::string, ProxyLink > T_Links;
    typedef boost::shared_ptr< runtime::Process_ABC > T_Process;
    //@}

private:
    //! @name Private methods
    Path GetPath() const;
    Tree GetProperties() const;
    void Save() const;
    bool Reload( const Path& path );
    void Start();
    void Stop();
    void Update();
    void Restart();
    void RegisterMissingLinks();
    T_Process MakeProcess() const;
    void HttpRegister( const std::string& prefix, const ProxyLink& proxy );
    void HttpUnregister( const std::string& prefix );
    //@}

    //! @name Member data
    //@{
    cpplog::BaseLogger& log_;
    const runtime::Runtime_ABC& runtime_;
    const runtime::FileSystem_ABC& system_;
    const Path logs_;
    const Path java_;
    const Path jar_;
    const int port_;
    boost::mutex access_;
    web::Client_ABC& client_;
    T_Process process_;
    T_Links links_;
    runtime::Event end_;
    mutable runtime::Async async_;
    //@}
};

}

#endif // PROXY_H
