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
    typedef web::Reply Reply;
    typedef boost::property_tree::ptree Tree;
    typedef boost::uuids::uuid Uuid;

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
    virtual Reply GetCluster() const;
    virtual Reply StartCluster() const;
    virtual Reply StopCluster() const;
    //@}

    //! @name Node Methods
    //@{
    virtual Reply ListNodes ( int offset, int limit ) const;
    virtual Reply CountNodes() const;
    virtual Reply GetNode   ( const Uuid& id ) const;
    virtual Reply CreateNode( const std::string& name );
    virtual Reply DeleteNode( const Uuid& id );
    virtual Reply StartNode ( const Uuid& id ) const;
    virtual Reply StopNode  ( const Uuid& id ) const;
    //@}

    //! @name Install Methods
    //@{
    virtual Reply GetInstall   ( const Uuid& id ) const;
    virtual Reply DeleteInstall( const Uuid& id, const std::vector< size_t >& list  );
    //@}

    //! @name Cache Methods
    //@{
    virtual Reply UploadCache     ( const Uuid& id, std::istream& src );
    virtual Reply GetCache        ( const Uuid& id ) const;
    virtual Reply DeleteCache     ( const Uuid& id );
    virtual Reply InstallFromCache( const Uuid& id, const std::vector< size_t >& list );
    //@}

    //! @name Session Methods
    //@{
    virtual Reply ListSessions ( const Uuid& node, int offset, int limit ) const;
    virtual Reply CountSessions( const Uuid& node ) const;
    virtual Reply GetSession   ( const Uuid& id ) const;
    virtual Reply CreateSession( const Uuid& node, const std::string& name, const std::string& exercise );
    virtual Reply DeleteSession( const Uuid& id );
    virtual Reply StartSession ( const Uuid& id ) const;
    virtual Reply StopSession  ( const Uuid& id ) const;
    virtual Reply PauseSession ( const Uuid& id ) const;
    //@}

    //! @name Other Methods
    //@{
    virtual Reply ListExercises ( const Uuid& id, int offset, int limit ) const;
    virtual Reply CountExercises( const Uuid& id ) const;
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
