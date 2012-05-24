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
    virtual Reply GetNode   ( const Uuid& id ) const;
    virtual Reply CreateNode( const std::string& name );
    virtual Reply DeleteNode( const Uuid& id );
    virtual Reply StartNode ( const Uuid& id ) const;
    virtual Reply StopNode  ( const Uuid& id ) const;
    virtual Reply UploadPack( const Uuid& id, std::istream& src );
    virtual Reply GetPack   ( const Uuid& id ) const;
    virtual Reply DeletePack( const Uuid& id );
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
    Uuid clusterId_;
    NodeController_ABC& nodes_;
    SessionController_ABC& sessions_;
    //@}
};

}

#endif // AGENT_H
