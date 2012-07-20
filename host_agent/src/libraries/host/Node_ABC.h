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

#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <vector>

namespace boost
{
namespace filesystem
{
    class path;
}
namespace posix_time
{
    class ptime;
}
namespace uuids
{
    struct uuid;
}
}

namespace runtime
{
    struct Async;
    struct FileSystem_ABC;
    struct Runtime_ABC;
}

namespace host
{
    typedef boost::filesystem::path Path;
    typedef boost::property_tree::ptree Tree;
    typedef boost::uuids::uuid Uuid;
namespace node
{
    struct Token;
}
}

namespace host
{
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

    //! @name Accessors
    //@{
    virtual Uuid GetId() const = 0;
    virtual Path GetRoot() const = 0;
    virtual int  GetPort() const = 0;
    virtual std::string GetName() const = 0;
    virtual Tree GetProperties() const = 0;
    //@}

    //! @name Public methods
    //@{
    virtual Tree Save() const = 0;
    virtual bool Start( const runtime::Runtime_ABC& runtime, const Path& java, const Path& jar,
                        const Path& web, const std::string& type, int host, bool weak ) = 0;
    virtual bool Stop( bool weak ) = 0;
    virtual void Remove( const runtime::FileSystem_ABC& system, runtime::Async& async ) = 0;
    virtual void Update( size_t num_sessions, size_t parallel_sessions ) = 0;
    //@}

    //! @name Install methods
    //@{
    virtual Tree GetInstall() const = 0;
    virtual Tree DeleteInstall( const std::vector< size_t >& list ) = 0;
    //@}

    //! @name Cache methods
    //@{
    virtual void UploadCache( std::istream& src ) = 0;
    virtual Tree GetCache() const = 0;
    virtual Tree DeleteCache() = 0;
    virtual Tree InstallFromCache( const std::vector< size_t >& list ) = 0;
    //@}

    //! @name Exercise methods
    //@{
    typedef std::vector< Path > T_Exercises;
    virtual T_Exercises GetExercises( int offset, int limit ) const = 0;
    virtual size_t      CountExercises() const = 0;
    virtual Tree        LinkExercise( const std::string& name ) const = 0;
    virtual Tree        LinkExercise( const Tree& tree ) const = 0;
    virtual void        UnlinkExercise( const Tree& tree ) = 0;
    //@}

    //! @name Session methods
    //@{
    typedef boost::shared_ptr< node::Token > T_Token;
    virtual T_Token StartSession( const boost::posix_time::ptime& start ) = 0;
    virtual void UpdateSessionSize( const Uuid& id, size_t size ) = 0;
    virtual void RemoveSession( const Uuid& id ) = 0;
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
    virtual Ptr Make( const Path& root, const std::string& name, size_t num_sessions, size_t parallel_sessions ) const = 0;
    virtual Ptr Make( const Path& tag ) const = 0;
    //@}
};
}

#endif // NODE_ABC_H
