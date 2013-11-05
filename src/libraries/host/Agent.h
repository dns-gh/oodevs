// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef AGENT_H
#define AGENT_H

#include "web/Agent_ABC.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

namespace boost
{
    class mutex;
}

namespace cpplog
{
    class BaseLogger;
}

namespace host
{
    struct NodeController_ABC;
    struct SessionController_ABC;
    struct UserController_ABC;
    typedef boost::filesystem::path Path;
    typedef boost::property_tree::ptree Tree;
    typedef boost::uuids::uuid Uuid;
}

namespace host
{
// =============================================================================
/** @class  Agent
    @brief  Agent class definition
*/
// Created: BAX 2012-03-07
// =============================================================================
class Agent : public web::Agent_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Agent( cpplog::BaseLogger& log, NodeController_ABC* cluster, NodeController_ABC& nodes, SessionController_ABC& sessions );
    virtual ~Agent();
    //@}

    //! @name Cluster Methods
    //@{
    virtual Tree GetCluster  () const;
    virtual Tree StartCluster() const;
    virtual Tree StopCluster () const;
    //@}

    //! @name Node Methods
    //@{
    virtual std::vector< Tree > ListNodes   ( int offset, int limit ) const;
    virtual size_t              CountNodes  () const;
    virtual Tree                GetNode     ( const Uuid& id ) const;
    virtual Tree                CreateNode  ( const std::string& ident, const web::node::Config& cfg );
    virtual Tree                DeleteNode  ( const Uuid& id );
    virtual Tree                StartNode   ( const Uuid& id ) const;
    virtual Tree                StopNode    ( const Uuid& id ) const;
    virtual Tree                UpdateNode  ( const Uuid& id, const Tree& cfg );
    virtual std::string         IdentifyNode( const Uuid& id ) const;
    //@}

    //! @name Client Methods
    //@{
    virtual Tree GetClient     () const;
    virtual void DownloadClient( web::Chunker_ABC& dst ) const;
    //@}

    //! @name Install Methods
    //@{
    virtual Tree GetInstall     ( const Uuid& id ) const;
    virtual Tree DeleteInstall  ( const Uuid& id, const std::vector< size_t >& list  );
    virtual void DownloadInstall( const Uuid& id, web::Chunker_ABC& dst, size_t item );
    virtual void DownloadInstall( const Uuid& id, web::Chunker_ABC& dst, const std::string& type, const std::string& name, const std::string& checksum );
    //@}

    //! @name Cache Methods
    //@{
    virtual Tree UploadCache     ( const Uuid& id, io::Reader_ABC& src );
    virtual Tree GetCache        ( const Uuid& id ) const;
    virtual Tree DeleteCache     ( const Uuid& id );
    virtual Tree InstallFromCache( const Uuid& id, const std::vector< size_t >& list );
    //@}

    //! @name Session Methods
    //@{
    virtual std::vector< Tree > ListSessions      ( const web::User& user, int offset, int limit ) const;
    virtual size_t              CountSessions     ( const web::User& user ) const;
    virtual Tree                GetSession        ( const web::User& user, const Uuid& id ) const;
    virtual Tree                CreateSession     ( const web::User& user, const web::session::Config& cfg, const std::string& exercise );
    virtual Tree                DeleteSession     ( const web::User& user, const Uuid& id );
    virtual void                DeleteUser        ( const web::User& user, int id );
    virtual Tree                StartSession      ( const web::User& user, const Uuid& id, const std::string& checkpoint ) const;
    virtual Tree                StopSession       ( const web::User& user, const Uuid& id ) const;
    virtual Tree                PauseSession      ( const web::User& user, const Uuid& id ) const;
    virtual Tree                UpdateSession     ( const web::User& user, const Uuid& id, const Tree& cfg ) const;
    virtual Tree                ArchiveSession    ( const web::User& user, const Uuid& id ) const;
    virtual Tree                RestoreSession    ( const web::User& user, const Uuid& id ) const;
    virtual void                DownloadSession   ( const web::User& user, const Uuid& id, web::Chunker_ABC& dst ) const;
    virtual Tree                ReplaySession     ( const web::User& user, const Uuid& id ) const;
    virtual void                DownloadSessionLog( const web::User& user, const Uuid& id, web::Chunker_ABC& dst, const std::string& logFile, int limitSize,
                                                    bool deflate ) const;
    //@}

    //! @name Exercise Methods
    //@{
    virtual std::vector< Path > ListExercises ( const Uuid& id, int offset, int limit ) const;
    virtual size_t              CountExercises( const Uuid& id ) const;
    //@}

    //! @name Plugin Methods
    //@{
    virtual std::vector< Path > ListPlugins ( int offset, int limit ) const;
    virtual size_t              CountPlugins() const;
    //@}

    //! @name Licenses Methods
    //@{
    virtual Tree ListLicenses( const Uuid& node ) const;
    virtual Tree UploadLicenses( io::Reader_ABC& src );
    //@}

private:
    //! @name Member data
    //@{
    cpplog::BaseLogger& log_;
    boost::mutex access_;
    NodeController_ABC* cluster_;
    Uuid clusterId_;
    NodeController_ABC& nodes_;
    SessionController_ABC& sessions_;
    //@}
};
}

#endif // AGENT_H
