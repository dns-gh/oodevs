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
#include <map>

namespace boost
{
    class shared_mutex;
}

namespace host
{
    class Session_ABC;
    class SessionFactory_ABC;
    class Node_ABC;
    class NodeFactory_ABC;

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
             Agent( const NodeFactory_ABC& nodeFactory, const SessionFactory_ABC& sessionFactory );
    virtual ~Agent();
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
    virtual Reply CreateSession( const boost::uuids::uuid& node, const std::string& exercise, const std::string& name );
    virtual Reply DeleteSession( const boost::uuids::uuid& id );
    virtual Reply StartSession ( const boost::uuids::uuid& id ) const;
    virtual Reply StopSession  ( const boost::uuids::uuid& id ) const;
    //@}

    //! @name Other Methods
    //@{
    virtual Reply ListExercises ( int offset, int limit ) const;
    virtual Reply CountExercises() const;
    //@}

    //! @name Type helpers
    //@{
    typedef std::map< boost::uuids::uuid, boost::shared_ptr< Session_ABC > > T_Sessions;
    typedef std::map< boost::uuids::uuid, boost::shared_ptr< Node_ABC > > T_Nodes;
    //@}

private:
    //! @name Member data
    //@{
    const NodeFactory_ABC& nodeFactory_;
    const SessionFactory_ABC& sessionFactory_;
    std::auto_ptr< boost::shared_mutex > access_;
    T_Nodes nodes_;
    T_Sessions sessions_;
    //@}
};

}

#endif // AGENT_H
