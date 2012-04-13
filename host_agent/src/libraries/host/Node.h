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
#include "NodeFactory_ABC.h"

namespace boost
{
    class shared_mutex;
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

namespace xml
{
    class xistream;
}

namespace host
{
    class FileSystem_ABC;
    class PortFactory_ABC;
    class Port_ABC;
    class Proxy_ABC;
    class UuidFactory_ABC;

// =============================================================================
/** @class  Node
    @brief  Node class definition
*/
// Created: BAX 2012-04-03
// =============================================================================
class Node : public Node_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Node( cpplog::BaseLogger& log,
                   const runtime::Runtime_ABC& runtime, const UuidFactory_ABC& uuids,
                   const FileSystem_ABC& system, const Proxy_ABC& proxy,
                   const boost::filesystem::path& java, const boost::filesystem::path& jar,
                   const boost::filesystem::path& web, const std::string& type, const std::string& name, PortFactory_ABC& ports );
             Node( cpplog::BaseLogger& log,
                   const runtime::Runtime_ABC& runtime, const FileSystem_ABC& system, const Proxy_ABC& proxy,
                   const boost::filesystem::path& java, const boost::filesystem::path& jar,
                   const boost::filesystem::path& web, xml::xistream& xis, PortFactory_ABC& ports );
    virtual ~Node();
    //@}

    //! @name Overrided methods
    //@{
    virtual boost::uuids::uuid GetTag() const;
    virtual std::string ToJson() const;
    virtual void Save() const;
    virtual void Start();
    virtual void Stop();
    //@}

private:
    //! @name Private methods
    //@{
    void CheckPaths() const;
    std::string ToXml() const;
    boost::filesystem::path GetPath() const;
    //@}

private:
    //! @name Member data
    //@{
    mutable cpplog::BaseLogger& log_;
    const runtime::Runtime_ABC& runtime_;
    const FileSystem_ABC& system_;
    const Proxy_ABC& proxy_;
    const boost::filesystem::path java_;
    const boost::filesystem::path jar_;
    const boost::filesystem::path web_;
    const std::string type_;
    const boost::uuids::uuid id_;
    const std::string name_;
    std::auto_ptr< boost::shared_mutex > access_;
    boost::shared_ptr< runtime::Process_ABC > process_;
    std::auto_ptr< Port_ABC > port_;
    //@}
};

}

#endif // NODE_H
