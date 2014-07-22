// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef NODE_CONTROLLER_H
#define NODE_CONTROLLER_H

#include "NodeController_ABC.h"
#include "NodeObserver_ABC.h"

#include "Container.h"
#include "runtime/Async.h"
#include "runtime/Timer_ABC.h"
#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree.hpp>

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
    class Plugins;
}

namespace host
{
    struct NodeFactory_ABC;
    struct Package_ABC;
    struct Proxy_ABC;
    typedef boost::filesystem::path Path;
}

namespace host
{
// =============================================================================
/** @class  NodeController
    @brief  NodeController interface
*/
// Created: BAX 2012-04-03
// =============================================================================
class NodeController : public NodeController_ABC, public NodeObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             NodeController( cpplog::BaseLogger& log,
                             const runtime::Runtime_ABC& runtime,
                             const runtime::FileSystem_ABC& fs,
                             const web::Plugins& plugins,
                             const NodeFactory_ABC& nodes,
                             const Path& root,
                             const Path& app,
                             const Path& web,
                             const Path& client,
                             const Path& licenses,
                             const std::string& type,
                             int host,
                             int tcp,
                             runtime::Pool_ABC& pool,
                             Proxy_ABC& proxy );
    virtual ~NodeController();
    //@}

    //! @name Methods
    //@{
    virtual void    Reload();
    virtual T_Nodes List  ( int offset, int limit ) const;
    virtual size_t  Count () const;
    virtual bool    Has   ( const Uuid& id ) const;
    virtual T_Node  Get   ( const Uuid& id ) const;
    virtual T_Node  Create( const std::string& ident, const web::node::Config& cfg );
    virtual T_Node  Delete( const Uuid& id );
    virtual T_Node  Start ( const Uuid& id ) const;
    virtual T_Node  Stop  ( const Uuid& id ) const;
    virtual T_Node  Update( const Uuid& id, const Tree& cfg );
    //@}

    //! @name Client Methods
    //@{
    virtual Tree    GetClient     ( bool x64 ) const;
    virtual void    DownloadClient( web::Chunker_ABC& dst, bool x64 ) const;
    //@}

    //! @name Install Methods
    //@{
    virtual Tree    GetInstall     ( const Uuid& id ) const;
    virtual Tree    DeleteInstall  ( const Uuid& id, const std::vector< size_t >& list );
    virtual void    DownloadInstall( const Uuid& id, web::Chunker_ABC& dst, size_t item );
    virtual void    DownloadInstall( const Uuid& id, web::Chunker_ABC& dst, const std::string& type, const std::string& name, const std::string& checksum );
    //@}

    //! @name Cache Methods
    //@{
    virtual Tree    UploadCache     ( const Uuid& id, io::Reader_ABC& src ) const;
    virtual Tree    GetCache        ( const Uuid& id ) const;
    virtual Tree    DeleteCache     ( const Uuid& id );
    virtual Tree    InstallFromCache( const Uuid& id, const std::vector< size_t >& list );
    //@}

    //! @name Exercise Methods
    //@{
    virtual T_Exercises GetExercises  ( const Uuid& id, int offset, int limit ) const;
    virtual size_t      CountExercises( const Uuid& id ) const;
    //@}

    //! @name Exercise Methods
    //@{
    virtual T_Plugins GetPlugins  ( int offset, int limit ) const;
    virtual size_t    CountPlugins() const;
    //@}

    //! @name NodeObserver_ABC Methods
    //@{
    void Notify( const Node_ABC& node ) const;
    //@}

    //! @name Node Methods
    //@{
    virtual Tree  LinkExercise  ( const Node_ABC& node, const std::string& name ) const;
    virtual Tree  LinkExercise  ( const Node_ABC& node, const Tree& tree ) const;
    //@}

    //! @name Node Methods
    //@{
    virtual Tree ListLicenses ( const Uuid& id ) const;
    virtual Tree UploadLicenses( io::Reader_ABC& src );
    //@}

    //! @name Typedef helpers
    //@{
    typedef boost::shared_ptr< runtime::Process_ABC > T_Process;
    //@}

private:
    //! @name Private methods
    //@{
    void      ReloadNode     ( const Path& path );
    bool      ReloadDirectory( runtime::Async& reload, const Path& dir );
    void      Refresh        ();
    void      Save           ( const Node_ABC& node ) const;
    void      Create         ( Node_ABC& node, bool reload );
    T_Process StartWith      ( const Node_ABC& node ) const;
    void      Start          ( Node_ABC& node, bool force, bool weak ) const;
    void      Stop           ( Node_ABC& node, bool skip, bool weak ) const;
    bool      IsValid        ( const web::node::Config& config ) const;
    void      GetAvailableLicences();
    //@}

private:
    //! @name Private members
    //@{
    cpplog::BaseLogger& log_;
    const runtime::Runtime_ABC& runtime_;
    const runtime::FileSystem_ABC& fs_;
    const web::Plugins& plugins_;
    const NodeFactory_ABC& factory_;
    const Path root_;
    const Path app_;
    const Path web_;
    const Path licensesDir_;
    const std::string type_;
    const int host_;
    const int tcp_;
    Proxy_ABC& proxy_;
    Container< Node_ABC > nodes_;
    runtime::Timer timer_;
    boost::shared_ptr< Package_ABC > client_;
    mutable runtime::Async async_;
    Tree licenses_;
    //@}
};
}

#endif // NODE_CONTROLLER_H
