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

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

namespace boost
{
    class mutex;
    class shared_mutex;
}

namespace runtime
{
    class Runtime_ABC;
    class Process_ABC;
}

namespace host
{
    class Package_ABC;
    class Port_ABC;
    class PortFactory_ABC;

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
             Node( const boost::uuids::uuid& id, const std::string& name, std::auto_ptr< Port_ABC > port );
             Node( const boost::property_tree::ptree& tree, const runtime::Runtime_ABC& runtime, PortFactory_ABC& ports );
    virtual ~Node();
    //@}

    //! @name Node_ABC methods
    //@{
    virtual boost::uuids::uuid GetId() const;
    virtual boost::property_tree::ptree GetProperties() const;
    virtual void ParsePack( const FileSystem_ABC& system, const boost::filesystem3::path& path );
    virtual void ReadPack( const FileSystem_ABC& system, const boost::filesystem3::path& path, std::istream& src );
    virtual boost::property_tree::ptree GetPack() const;
    virtual boost::property_tree::ptree DeletePack();
    //@}

    //! @name Typedef helpers
    //@{
    typedef boost::shared_ptr< runtime::Process_ABC > T_Process;
    typedef boost::function< T_Process( const Node& ) > T_Starter;
    //@}

    //! @name Public methods
    //@{
    boost::property_tree::ptree Save() const;
    bool Start( const T_Starter& starter );
    bool Stop();
    //@}

    //! @name Public members
    //@{
    const boost::uuids::uuid id_;
    const std::string name_;
    const std::auto_ptr< Port_ABC > port_;
    //@}

private:
    //! @name Public methods
    //@{
    boost::property_tree::ptree GetCommonProperties() const;
    //@}

private:
    //! @name Private members
    //@{
    const std::auto_ptr< boost::shared_mutex > access_;
    const std::auto_ptr< boost::mutex > package_;
    boost::shared_ptr< Package_ABC > stash_;
    T_Process process_;
    //@}
};
}

#endif // NODE_H