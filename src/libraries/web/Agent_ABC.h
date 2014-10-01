// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef AGENT_ABC_H
#define AGENT_ABC_H

#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <boost/uuid/uuid.hpp>
#include <string>
#include <vector>

namespace boost
{
    template< typename T > class optional;
namespace filesystem
{
    class path;
}
}

namespace io
{
    struct Reader_ABC;
}

namespace web
{
    struct Chunker_ABC;
    struct User;
    typedef boost::filesystem::path Path;
    typedef boost::property_tree::ptree Tree;
    typedef boost::uuids::uuid Uuid;
namespace node
{
    struct Config;
}
namespace session
{
    struct Config;
}
}

namespace web
{
// =============================================================================
/** @class  Agent_ABC
    @brief  Agent_ABC interface
*/
// Created: BAX 2012-03-07
// =============================================================================
struct Agent_ABC : public boost::noncopyable
{
    //! @name Constructors/Destructor
    //@{
             Agent_ABC() {}
    virtual ~Agent_ABC() {}
    //@}

    //! @name Cluster Methods
    //@{
    virtual Tree GetCluster  () const = 0;
    virtual Tree StartCluster() const = 0;
    virtual Tree StopCluster () const = 0;
    //@}

    //! @name Node Methods
    //@{
    virtual std::vector< Tree > ListNodes   ( int offset, int limit ) const = 0;
    virtual size_t              CountNodes  () const = 0;
    virtual Tree                GetNode     ( const Uuid& id ) const = 0;
    virtual Tree                CreateNode  ( const std::string& ident, const node::Config& cfg ) = 0;
    virtual Tree                DeleteNode  ( const Uuid& id ) = 0;
    virtual Tree                StartNode   ( const Uuid& id ) const = 0;
    virtual Tree                StopNode    ( const Uuid& id ) const = 0;
    virtual Tree                UpdateNode  ( const Uuid& id, const Tree& cfg ) = 0;
    virtual std::string         IdentifyNode( const Uuid& id ) const = 0;
    //@}

    //! @name Client Methods
    //@{
    virtual Tree GetClient     ( bool x64 ) const = 0;
    virtual void DownloadClient( Chunker_ABC& dst, bool x64 ) const = 0;
    //@}

    //! @name Install Methods
    //@{
    virtual Tree GetInstall     ( const Uuid& id ) const = 0;
    virtual Tree DeleteInstall  ( const Uuid& id, const std::vector< size_t >& list  ) = 0;
    virtual void DownloadInstall( const Uuid& id, Chunker_ABC& dst, size_t item ) = 0;
    virtual void DownloadInstall( const Uuid& id, Chunker_ABC& dst, const std::string& type, const std::string& name, const std::string& checksum ) = 0;
    //@}

    //! @name Cache Methods
    //@{
    virtual Tree UploadCache     ( const Uuid& id, io::Reader_ABC& src ) = 0;
    virtual Tree GetCache        ( const Uuid& id ) const = 0;
    virtual Tree DeleteCache     ( const Uuid& id ) = 0;
    virtual Tree InstallFromCache( const Uuid& id, const std::vector< size_t >& list ) = 0;
    //@}

    //! @name Session Methods
    //@{
    virtual std::vector< Tree > ListSessions      ( const User& user, int offset, int limit ) const = 0;
    virtual size_t              CountSessions     ( const User& user ) const = 0;
    virtual Tree                GetSession        ( const User& user, const Uuid& id ) const = 0;
    virtual Tree                CreateSession     ( const User& user, const session::Config& cfg, const std::string& exercise ) = 0;
    virtual Tree                DeleteSession     ( const User& user, const Uuid& id ) = 0;
    virtual void                DeleteUser        ( const User& user, int id ) = 0;
    virtual Tree                StartSession      ( const User& user, const Uuid& id, const std::string& checkpoint ) const = 0;
    virtual Tree                StopSession       ( const User& user, const Uuid& id ) const = 0;
    virtual Tree                PauseSession      ( const User& user, const Uuid& id ) const = 0;
    virtual Tree                UpdateSession     ( const User& user, const Uuid& id, const Tree& cfg ) const = 0;
    virtual Tree                ArchiveSession    ( const User& user, const Uuid& id ) const = 0;
    virtual Tree                RestoreSession    ( const User& user, const Uuid& id ) const = 0;
    virtual void                DownloadSession   ( const User& user, const Uuid& id, Chunker_ABC& dst ) const = 0;
    virtual Tree                ReplaySession     ( const User& user, const Uuid& id ) const = 0;
    virtual void                DownloadSessionLog( const User& user, const Uuid& id, Chunker_ABC& dst, const std::string& logFile, int limitSize,
                                                    bool deflate ) const = 0;
    //@}

    //! @name Exercise Methods
    //@{
    virtual std::vector< Path > ListExercises ( const Uuid& id, int offset, int limit ) const = 0;
    virtual size_t              CountExercises( const Uuid& id ) const = 0;
    //@}

    //! @name Plugin Methods
    //@{
    virtual std::vector< Path > ListPlugins ( int offset, int limit ) const = 0;
    virtual size_t              CountPlugins() const = 0;
    //@}

    //! @name Licenses Methods
    //@{
    virtual Tree ListLicenses( const Uuid& node ) const = 0;
    virtual Tree UploadLicenses( io::Reader_ABC& src ) = 0;
    //@}
};
}

#endif // AGENT_ABC_H
