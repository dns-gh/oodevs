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
             Node( const runtime::Runtime_ABC& runtime, const UuidFactory_ABC& uuids,
                   const FileSystem_ABC& system, const boost::filesystem::wpath& java,
                   const boost::filesystem::wpath& jar, const boost::filesystem::wpath& web, int host,
                   const std::string& name, PortFactory_ABC& ports );
             Node( const runtime::Runtime_ABC& runtime, const FileSystem_ABC& system,
                   const boost::filesystem::wpath& java, const boost::filesystem::wpath& jar,
                   const boost::filesystem::wpath& web, xml::xistream& xis, PortFactory_ABC& ports );
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
    boost::filesystem::wpath GetPath() const;
    //@}

private:
    //! @name Member data
    //@{
    const runtime::Runtime_ABC& runtime_;
    const FileSystem_ABC& system_;
    const boost::filesystem::wpath java_;
    const boost::filesystem::wpath jar_;
    const boost::filesystem::wpath web_;
    const boost::uuids::uuid id_;
    const int host_;
    const std::string name_;
    std::auto_ptr< boost::shared_mutex > access_;
    boost::shared_ptr< runtime::Process_ABC > process_;
    std::auto_ptr< Port_ABC > port_;
    //@}
};

}

#endif // NODE_H
