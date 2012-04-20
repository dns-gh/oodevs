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
    class Node;
    class Pool_ABC;
    class PortFactory_ABC;
    class Proxy_ABC;
    class SecurePool;
    class UuidFactory_ABC;
    template< typename T > class Container;

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
                             const UuidFactory_ABC& uuids,
                             const Proxy_ABC& proxy,
                             const boost::filesystem::path& java,
                             const boost::filesystem::path& jar,
                             const boost::filesystem::path& web,
                             const std::string& type,
                             Pool_ABC& pool,
                             PortFactory_ABC& ports
                           );
    virtual ~NodeController();
    //@}

    //! @name Methods
    //@{
    virtual void    Reload();
    virtual T_Nodes List( int offset, int limit ) const;
    virtual size_t  Count() const;
    virtual bool    Has( const boost::uuids::uuid& id ) const;
    virtual T_Node  Get( const boost::uuids::uuid& id ) const;
    virtual T_Node  Create( const std::string& name );
    virtual T_Node  Delete( const boost::uuids::uuid& id );
    virtual T_Node  Start( const boost::uuids::uuid& id ) const;
    virtual T_Node  Stop( const boost::uuids::uuid& id ) const;
    //@}

private:
    //! @name Private methods
    //@{
    void Save( const Node& node ) const;
    boost::shared_ptr< runtime::Process_ABC > StartWith( const Node& node ) const;
    void Start( Node& node, bool mustSave ) const;
    void Stop( Node& node, bool skipSave ) const;
    //@}

private:
    //! @name Private members
    //@{
    mutable cpplog::BaseLogger& log_;
    const runtime::Runtime_ABC& runtime_;
    const FileSystem_ABC& system_;
    const UuidFactory_ABC& uuids_;
    const Proxy_ABC& proxy_;
    const boost::filesystem::path java_;
    const boost::filesystem::path jar_;
    const boost::filesystem::path web_;
    const std::string type_;
    const std::auto_ptr< SecurePool > pool_;
    const std::auto_ptr< Container< Node > > nodes_;
    PortFactory_ABC& ports_;
    //@}
};

}

#endif // NODE_CONTROLLER_H
