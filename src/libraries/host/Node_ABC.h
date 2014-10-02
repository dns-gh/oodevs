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

namespace io
{
    struct Reader_ABC;
}

namespace runtime
{
    struct Async;
}

namespace web
{
    struct Chunker_ABC;
namespace node
{
    struct Config;
}
namespace session
{
    struct Config;
}
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
    virtual std::string GetIdent() const = 0;
    virtual Tree GetProperties() const = 0;
    //@}

    //! @name Public methods
    //@{
    virtual Tree Save() const = 0;
    virtual bool Start( const Path& app, const Path& web,
                        const std::string& type, int http, int tcp, bool weak ) = 0;
    virtual bool Stop( bool weak ) = 0;
    virtual void Remove( runtime::Async& async ) = 0;
    virtual bool Update( const Tree& cfg ) = 0;
    virtual void SoftKill() = 0;
    //@}

    //! @name Install methods
    //@{
    virtual Tree GetInstall     () const = 0;
    virtual Tree DeleteInstall  ( const std::vector< size_t >& list ) = 0;
    virtual void DownloadInstall( web::Chunker_ABC& dst, size_t item ) = 0;
    virtual void DownloadInstall( web::Chunker_ABC& dst, const std::string& type, const std::string& name, const std::string& checksum ) = 0;
    //@}

    //! @name Cache methods
    //@{
    virtual void UploadCache( io::Reader_ABC& src ) = 0;
    virtual Tree GetCache() const = 0;
    virtual Tree DeleteCache() = 0;
    virtual Tree InstallFromCache( const std::vector< size_t >& list ) = 0;
    //@}

    //! @name Exercise methods
    //@{
    typedef std::vector< Path > T_Exercises;
    virtual T_Exercises GetExercises( int offset, int limit ) const = 0;
    virtual Tree        GetExerciseProperties( const std::string& exercise ) const = 0;
    virtual size_t      CountExercises() const = 0;
    virtual Tree        LinkExercise( const std::string& name ) const = 0;
    virtual Tree        LinkExercise( const Tree& tree ) const = 0;
    virtual void        UnlinkExercise( const Tree& tree ) = 0;
    //@}

    //! @name Session methods
    //@{
    typedef boost::shared_ptr< node::Token > T_Token;
    virtual T_Token StartSession( const boost::posix_time::ptime& start, bool first_time ) = 0;
    virtual void UpdateSessionSize( const Uuid& id, size_t size ) = 0;
    virtual void RemoveSession( const Uuid& id ) = 0;
    virtual void FilterConfig( web::session::Config& cfg ) const = 0;
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
    virtual Ptr Make( const Path& root, const std::string& ident, const web::node::Config& cfg ) const = 0;
    virtual Ptr Make( const Path& tag ) const = 0;
    //@}
};
}

#endif // NODE_ABC_H
