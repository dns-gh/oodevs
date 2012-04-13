// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef NODE_FACTORY_H
#define NODE_FACTORY_H

#include "NodeFactory_ABC.h"

namespace cpplog
{
    class BaseLogger;
}

namespace runtime
{
    class Runtime_ABC;
}

namespace host
{
    class FileSystem_ABC;
    class PortFactory_ABC;
    class Proxy_ABC;
    class UuidFactory_ABC;

// =============================================================================
/** @class  NodeFactory
    @brief  NodeFactory interface
*/
// Created: BAX 2012-04-03
// =============================================================================
class NodeFactory : public NodeFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             NodeFactory( cpplog::BaseLogger& log, const runtime::Runtime_ABC& runtime, const UuidFactory_ABC& uuids,
                          const FileSystem_ABC& system, const Proxy_ABC& proxy, PortFactory_ABC& ports,
                          const boost::filesystem::path& java, const boost::filesystem::path& jar, const boost::filesystem::path& web );
    virtual ~NodeFactory();
    //@}

    //! @name Methods
    //@{
    virtual boost::shared_ptr< Node_ABC > Create( const std::string& name ) const;
    virtual T_Nodes Reload() const;
    //@}

private:
    //! @name Private members
    //@{
    mutable cpplog::BaseLogger& log_;
    const runtime::Runtime_ABC& runtime_;
    const UuidFactory_ABC& uuids_;
    const FileSystem_ABC& system_;
    const Proxy_ABC& proxy_;
    const boost::filesystem::path java_;
    const boost::filesystem::path jar_;
    const boost::filesystem::path web_;
    PortFactory_ABC& ports_;
    //@}
};

}

#endif // NODE_FACTORY_H
