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
};

namespace host
{
    struct NodeFactory_ABC;
    struct Proxy_ABC;
    typedef boost::filesystem::path Path;

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
                             const runtime::FileSystem_ABC& system,
                             const NodeFactory_ABC& nodes,
                             const Path& root,
                             const Path& app,
                             const Path& web,
                             const std::string& type,
                             int host,
                             runtime::Pool_ABC& pool,
                             Proxy_ABC& proxy );
    virtual ~NodeController();
    //@}

    //! @name Methods
    //@{
    virtual void    Reload();
    virtual T_Nodes List( int offset, int limit ) const;
    virtual size_t  Count() const;
    virtual bool    Has( const Uuid& id ) const;
    virtual T_Node  Get( const Uuid& id ) const;
    virtual T_Node  Create( const std::string& ident, const std::string& name, size_t num_sessions, size_t parallel_sessions );
    virtual T_Node  Delete( const Uuid& id );
    virtual T_Node  Start( const Uuid& id ) const;
    virtual T_Node  Stop( const Uuid& id ) const;
    virtual T_Node  Update( const Uuid& id, const boost::optional< std::string >& name, size_t num_sessions, size_t parallel_sessions );
    //@}

    //! @name Install Methods
    //@{
    virtual Tree    GetInstall( const Uuid& id ) const;
    virtual Tree    DeleteInstall( const Uuid& id, const std::vector< size_t >& list );
    //@}

    //! @name Cache Methods
    //@{
    virtual Tree    UploadCache( const Uuid& id, std::istream& src ) const;
    virtual Tree    GetCache( const Uuid& id ) const;
    virtual Tree    DeleteCache( const Uuid& id );
    virtual Tree    InstallFromCache( const Uuid& id, const std::vector< size_t >& list );
    //@}

    //! @name Exercise Methods
    //@{
    virtual T_Exercises GetExercises( const Uuid& id, int offset, int limit ) const;
    virtual size_t      CountExercises( const Uuid& id ) const;
    //@}

    //! @name NodeObserver_ABC Methods
    //@{
    void Notify( const Node_ABC& node ) const;
    //@}

    //! @name Typedef helpers
    //@{
    typedef boost::shared_ptr< runtime::Process_ABC > T_Process;
    //@}

private:
    //! @name Private methods
    //@{
    void ReloadNode( const Path& path );
    void Update();
    void Save( const Node_ABC& node ) const;
    void Create( Node_ABC& node, bool reload );
    T_Process StartWith( const Node_ABC& node ) const;
    void Start( Node_ABC& node, bool force, bool weak ) const;
    void Stop( Node_ABC& node, bool skip, bool weak ) const;
    //@}

private:
    //! @name Private members
    //@{
    cpplog::BaseLogger& log_;
    const runtime::Runtime_ABC& runtime_;
    const runtime::FileSystem_ABC& system_;
    const NodeFactory_ABC& factory_;
    const Path root_;
    const Path app_;
    const Path web_;
    const std::string type_;
    const int host_;
    Proxy_ABC& proxy_;
    Container< Node_ABC > nodes_;
    runtime::Timer timer_;
    mutable runtime::Async async_;
    //@}
};
}

#endif // NODE_CONTROLLER_H
