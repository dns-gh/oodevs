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

#include "Agent_ABC.h"
#include <boost/shared_ptr.hpp>

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
    class NodeController_ABC;
    class SessionController_ABC;

// =============================================================================
/** @class  Agent
    @brief  Agent class definition
*/
// Created: BAX 2012-03-07
// =============================================================================
class Agent : public Agent_ABC
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
    virtual Reply GetNode   ( const boost::uuids::uuid& id ) const;
    virtual Reply CreateNode( const std::string& name );
    virtual Reply DeleteNode( const boost::uuids::uuid& id );
    virtual Reply StartNode ( const boost::uuids::uuid& id ) const;
    virtual Reply StopNode  ( const boost::uuids::uuid& id ) const;
    //@}

    //! @name Session Methods
    //@{
    virtual Reply ListSessions ( const boost::uuids::uuid& node, int offset, int limit ) const;
    virtual Reply CountSessions( const boost::uuids::uuid& node ) const;
    virtual Reply GetSession   ( const boost::uuids::uuid& id ) const;
    virtual Reply CreateSession( const boost::uuids::uuid& node, const std::string& name, const std::string& exercise );
    virtual Reply DeleteSession( const boost::uuids::uuid& id );
    virtual Reply StartSession ( const boost::uuids::uuid& id ) const;
    virtual Reply StopSession  ( const boost::uuids::uuid& id ) const;
    //@}

    //! @name Other Methods
    //@{
    virtual Reply ListExercises ( int offset, int limit ) const;
    virtual Reply CountExercises() const;
    //@}

private:
    //! @name Member data
    //@{
    mutable cpplog::BaseLogger& log_;
    const std::auto_ptr< boost::mutex > access_;
    NodeController_ABC* cluster_;
    boost::uuids::uuid clusterId_;
    NodeController_ABC& nodes_;
    SessionController_ABC& sessions_;
    //@}
};

}

#endif // AGENT_H
