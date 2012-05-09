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
#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree_fwd.hpp>

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
    class Runtime_ABC;
    class Process_ABC;
}

namespace web
{
    class Client_ABC;
}

namespace host
{
    class FileSystem_ABC;
    class Pool_ABC;
    class SecurePool;

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
                    const FileSystem_ABC& system, const boost::filesystem::path& logs,
                    const boost::filesystem::path& java, const boost::filesystem::path& jar,
                    int port, web::Client_ABC& client, Pool_ABC& pool );
    virtual ~Proxy();
    //@}

    //! @name Methods
    //@{
    virtual int GetPort() const;
    virtual void Register( const std::string& prefix, const std::string& host, int port ) const;
    virtual void Unregister( const std::string& prefix ) const;
    //@}

private:
    //! @name Private methods
    boost::filesystem::path GetPath() const;
    boost::property_tree::ptree GetProperties() const;
    void Save() const;
    bool Reload( const boost::filesystem::path& path );
    void Start();
    void Stop();
    //@}

    //! @name Member data
    //@{
    mutable cpplog::BaseLogger& log_;
    const runtime::Runtime_ABC& runtime_;
    const FileSystem_ABC& system_;
    const boost::filesystem::path logs_;
    const boost::filesystem::path java_;
    const boost::filesystem::path jar_;
    const int port_;
    const std::auto_ptr< SecurePool > pool_;
    const std::auto_ptr< boost::mutex > access_;
    web::Client_ABC& client_;
    boost::shared_ptr< runtime::Process_ABC > process_;
    //@}
};

}

#endif // PROXY_H
