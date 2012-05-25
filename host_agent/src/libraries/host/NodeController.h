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
#include <boost/filesystem/path.hpp>

namespace cpplog
{
    class BaseLogger;
}

namespace runtime
{
    class Runtime_ABC;
    class Process_ABC;
};

namespace host
{
    class FileSystem_ABC;
    struct NodeFactory_ABC;
    class Pool_ABC;
    class Proxy_ABC;
    class SecurePool;
    template< typename T > class Container;
    typedef boost::filesystem::path Path;

// =============================================================================
/** @class  NodeController
    @brief  NodeController interface
*/
// Created: BAX 2012-04-03
// =============================================================================
class NodeController : public NodeController_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             NodeController( cpplog::BaseLogger& log,
                             const runtime::Runtime_ABC& runtime,
                             const FileSystem_ABC& system,
                             const Proxy_ABC& proxy,
                             const NodeFactory_ABC& nodes,
                             const Path& root,
                             const Path& java,
                             const Path& jar,
                             const Path& web,
                             const std::string& type,
                             Pool_ABC& pool );
    virtual ~NodeController();
    //@}

    //! @name Methods
    //@{
    virtual void    Reload();
    virtual T_Nodes List( int offset, int limit ) const;
    virtual size_t  Count() const;
    virtual bool    Has( const Uuid& id ) const;
    virtual T_Node  Get( const Uuid& id ) const;
    virtual T_Node  Create( const std::string& name );
    virtual T_Node  Delete( const Uuid& id );
    virtual T_Node  Start( const Uuid& id ) const;
    virtual T_Node  Stop( const Uuid& id ) const;
    //@}

    //! @name Cache Methods
    //@{
    virtual Tree    UploadCache( const Uuid& id, std::istream& src ) const;
    virtual Tree    GetCache( const Uuid& id ) const;
    virtual Tree    DeleteCache( const Uuid& id );
    virtual Tree    InstallFromCache( const Uuid& id, const std::vector< size_t >& list );
    //@}

private:
    //! @name Private methods
    //@{
    void Save( const Node_ABC& node ) const;
    void Create( Node_ABC& node, bool isReload );
    boost::shared_ptr< runtime::Process_ABC > StartWith( const Node_ABC& node ) const;
    void Start( Node_ABC& node, bool mustSave ) const;
    void Stop( Node_ABC& node, bool skipSave ) const;
    //@}

private:
    //! @name Private members
    //@{
    mutable cpplog::BaseLogger& log_;
    const runtime::Runtime_ABC& runtime_;
    const FileSystem_ABC& system_;
    const Proxy_ABC& proxy_;
    const NodeFactory_ABC& factory_;
    const Path root_;
    const Path java_;
    const Path jar_;
    const Path web_;
    const std::string type_;
    const std::auto_ptr< SecurePool > pool_;
    const std::auto_ptr< Container< Node_ABC > > nodes_;
    //@}
};

}

#endif // NODE_CONTROLLER_H
