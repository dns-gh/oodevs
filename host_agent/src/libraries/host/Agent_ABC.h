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
#include <boost/uuid/uuid.hpp>
#include <string>
#include <vector>

namespace host
{
// =============================================================================
/** @class  Reply
    @brief  Reply struct definition
*/
// Created: BAX 2012-03-07
// =============================================================================
struct Reply
{
    std::string data;
    bool valid;
    Reply( const std::string& data, bool valid = true )
        : data ( data )
        , valid( valid )
    {
        // NOTHING
    }
};

// =============================================================================
/** @class  Agent_ABC
    @brief  Agent_ABC interface
*/
// Created: BAX 2012-03-07
// =============================================================================
class Agent_ABC : public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Agent_ABC() {}
    virtual ~Agent_ABC() {}
    //@}

    //! @name Cluster Methods
    //@{
    virtual Reply GetCluster() const = 0;
    virtual Reply StartCluster() const = 0;
    virtual Reply StopCluster() const = 0;
    //@}

    //! @name Node Methods
    //@{
    virtual Reply ListNodes( int offset, int limit ) const = 0;
    virtual Reply CountNodes() const = 0;
    virtual Reply GetNode( const boost::uuids::uuid& id ) const = 0;
    virtual Reply CreateNode( const std::string& name ) = 0;
    virtual Reply DeleteNode( const boost::uuids::uuid& id ) = 0;
    virtual Reply StartNode( const boost::uuids::uuid& id ) const = 0;
    virtual Reply StopNode( const boost::uuids::uuid& id ) const = 0;
    //@}

    //! @name Session Methods
    //@{
    virtual Reply ListSessions ( const boost::uuids::uuid& node, int offset, int limit ) const = 0;
    virtual Reply CountSessions( const boost::uuids::uuid& node ) const = 0;
    virtual Reply GetSession   ( const boost::uuids::uuid& id ) const = 0;
    virtual Reply CreateSession( const boost::uuids::uuid& node, const std::string& name, const std::string& exercise ) = 0;
    virtual Reply DeleteSession( const boost::uuids::uuid& id ) = 0;
    virtual Reply StartSession ( const boost::uuids::uuid& id ) const = 0;
    virtual Reply StopSession  ( const boost::uuids::uuid& id ) const = 0;
    //@}

    //! @name Other Methods
    //@{
    virtual Reply ListExercises ( int offset, int limit ) const = 0;
    virtual Reply CountExercises() const = 0;
    //@}
};

}

#endif // AGENT_ABC_H
