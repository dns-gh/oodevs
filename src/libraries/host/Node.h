// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef NODE_H
#define NODE_H

#include "Node_ABC.h"

#include "runtime/Async.h"
#include "web/Configs.h"
#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/uuid/uuid.hpp>

namespace boost
{
    class mutex;
    class shared_mutex;
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
    class Plugins;
}

namespace host
{
    struct NodeObserver_ABC;
    struct Package_ABC;
    struct PackageFactory_ABC;
    struct Port_ABC;
    struct PortFactory_ABC;
    struct UuidFactory_ABC;
    typedef boost::shared_ptr< const Port_ABC > Port;
}

namespace host
{
// -----------------------------------------------------------------------------
// Name: NodeDependencies
// Created: BAX 2012-08-09
// -----------------------------------------------------------------------------
struct NodeDependencies
{
    NodeDependencies( const PackageFactory_ABC& packages,
                      const runtime::FileSystem_ABC& fs,
                      const UuidFactory_ABC& uuids,
                      const NodeObserver_ABC& observer,
                      const runtime::Runtime_ABC& runtime,
                      const web::Plugins& plugins,
                      runtime::Pool_ABC& pool,
                      PortFactory_ABC& ports )
        : packages( packages )
        , fs      ( fs )
        , uuids   ( uuids )
        , observer( observer )
        , runtime ( runtime )
        , plugins ( plugins )
        , pool    ( pool )
        , ports   ( ports )
    {
        // NOTHING
    }
    const PackageFactory_ABC& packages;
    const runtime::FileSystem_ABC& fs;
    const UuidFactory_ABC& uuids;
    const NodeObserver_ABC& observer;
    const runtime::Runtime_ABC& runtime;
    const web::Plugins& plugins;
    runtime::Pool_ABC& pool;
    PortFactory_ABC& ports;
private:
    NodeDependencies& operator=( const NodeDependencies& );
};

// =============================================================================
/** @class  Node_ABC
    @brief  Node_ABC interface
*/
// Created: BAX 2012-04-03
// =============================================================================
class Node : public Node_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Node( const NodeDependencies& deps,
                   const Path& root,
                   int min_play_seconds,
                   const std::string& ident,
                   const web::node::Config& cfg );
             Node( const NodeDependencies& deps,
                   const Path& root,
                   int min_play_seconds,
                   const Tree& tree );
    virtual ~Node();
    //@}

    //! @name Node_ABC methods
    //@{
    virtual Uuid GetId() const;
    virtual Path GetRoot() const;
    virtual int  GetPort() const;
    virtual std::string GetIdent() const;
    virtual Tree GetProperties() const;
    //@}

    //! @name Public methods
    //@{
    virtual Tree Save() const;
    virtual bool Start( const Path& app, const Path& web,
                        const std::string& type, int http, int tcp, bool weak );
    virtual bool Stop( bool weak );
    virtual void Remove( runtime::Async& async );
    virtual bool Update( const Tree& cfg );
    virtual void SoftKill();
    //@}

    //! @name Node_ABC methods
    //@{
    virtual Tree GetInstall     () const;
    virtual Tree DeleteInstall  ( const std::vector< size_t >& list );
    virtual void DownloadInstall( web::Chunker_ABC& dst, size_t item );
    virtual void DownloadInstall( web::Chunker_ABC& dst, const std::string& type, const std::string& name, const std::string& checksum );
    //@}

    //! @name Node_ABC methods
    //@{
    virtual void UploadCache( io::Reader_ABC& src );
    virtual Tree GetCache() const;
    virtual Tree DeleteCache();
    virtual Tree InstallFromCache( const std::vector< size_t >& list );
    //@}

    //! @name Exercise methods
    //@{
    virtual T_Exercises GetExercises( int offset, int limit ) const;
    virtual Tree        GetExerciseProperties( const std::string& exercise ) const;
    virtual size_t      CountExercises() const;
    virtual Tree        LinkExercise( const std::string& name ) const;
    virtual Tree        LinkExercise( const Tree& tree ) const;
    virtual void        UnlinkExercise( const Tree& tree );
    //@}

    //! @name Session methods
    //@{
    virtual T_Token StartSession( const boost::posix_time::ptime& start, bool first_time );
    virtual void UpdateSessionSize( const Uuid& id, size_t size );
    virtual void RemoveSession( const Uuid& id );
    virtual void FilterConfig( web::session::Config& plugins ) const;
    void StopSession( const boost::posix_time::ptime& start );
    //@}

    //! @name Typedef helpers
    //@{
    typedef boost::shared_ptr< runtime::Process_ABC > T_Process;
    typedef std::map< Uuid, size_t > T_Sessions;
    //@}

private:
    //! @name Private methods
    //@{
    void ParsePackages( const Path& cache );
    Tree GetCommonProperties() const;
    std::pair< T_Process, bool > StopProcess( bool weak );
    //@}

private:
    //! @name Private constant members
    //@{
    const NodeDependencies deps_;
    const Uuid id_;
    const std::string ident_;
    const Path root_;
    const Port port_;
    const int min_play_seconds_;
    //@}

    //! @name Private members
    //@{
    mutable boost::shared_mutex access_;
    boost::shared_ptr< Package_ABC > install_;
    boost::shared_ptr< Package_ABC > cache_;
    T_Process process_;
    bool stopped_;
    mutable runtime::Async async_;
    web::node::Config cfg_;
    size_t num_play_;
    size_t num_parallel_;
    size_t num_exercises_;
    size_t install_size_;
    size_t cache_size_;
    size_t sessions_size_;
    T_Sessions sessions_;
    //@}
};
}

#endif // NODE_H