// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef NODE_ABC_H
#define NODE_ABC_H

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <boost/uuid/uuid.hpp>
#include <vector>

namespace boost
{
namespace filesystem3
{
    class path;
}
}

namespace runtime
{
    class Process_ABC;
}

namespace host
{
    typedef boost::filesystem3::path Path;
    typedef boost::property_tree::ptree Tree;
    typedef boost::uuids::uuid Uuid;

// =============================================================================
/** @class  Node_ABC
    @brief  Node_ABC interface
*/
// Created: BAX 2012-04-03
// =============================================================================
struct Node_ABC : public boost::noncopyable
{
    //! @name Constructors/Destructor
    //@{
             Node_ABC() {}
    virtual ~Node_ABC() {}
    //@}

    //! @name Typedef helpers
    //@{
    typedef boost::shared_ptr< runtime::Process_ABC > T_Process;
    typedef boost::function< T_Process( const Node_ABC& ) > T_Starter;
    //@}

    //! @name Accessors
    //@{
    virtual Uuid GetId() const = 0;
    virtual int  GetPort() const = 0;
    virtual std::string GetName() const = 0;
    virtual Tree GetProperties() const = 0;
    virtual Path GetCachePath() const = 0;
    virtual Path GetInstallPath() const = 0;
    //@}

    //! @name Public methods
    //@{
    virtual Tree Save() const = 0;
    virtual bool Start( const T_Starter& starter ) = 0;
    virtual bool Stop() = 0;
    //@}

    //! @name Cache methods
    //@{
    virtual void UploadCache( std::istream& src ) = 0;
    virtual Tree GetCache() const = 0;
    virtual Tree DeleteCache() = 0;
    virtual Tree InstallFromCache( const std::vector< size_t >& list ) = 0;
    //@}
};

// =============================================================================
/** @class  NodeFactory_ABC
    @brief  NodeFactory_ABC interface
*/
// Created: BAX 2012-05-23
// =============================================================================
struct NodeFactory_ABC : public boost::noncopyable
{
    //! @name Constructors/Destructor
    //@{
             NodeFactory_ABC() {}
    virtual ~NodeFactory_ABC() {}
    //@}

    //! @name Typedef helpers
    //@{
    typedef boost::shared_ptr< Node_ABC > Ptr;
    //@}

    //! @name Methods
    //@{
    virtual Ptr Make( const Path& root, const std::string& name ) const = 0;
    virtual Ptr Make( const Tree& tree ) const = 0;
    //@}
};

}

#endif // NODE_ABC_H
