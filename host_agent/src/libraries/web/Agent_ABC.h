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
#include <boost/property_tree/ptree_fwd.hpp>
#include <string>
#include <vector>

namespace web
{
    typedef boost::property_tree::ptree Tree;
    typedef boost::uuids::uuid Uuid;
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
struct Agent_ABC : public boost::noncopyable
{
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
    virtual Reply ListNodes ( int offset, int limit ) const = 0;
    virtual Reply CountNodes() const = 0;
    virtual Reply GetNode   ( const Uuid& id ) const = 0;
    virtual Reply CreateNode( const std::string& name ) = 0;
    virtual Reply DeleteNode( const Uuid& id ) = 0;
    virtual Reply StartNode ( const Uuid& id ) const = 0;
    virtual Reply StopNode  ( const Uuid& id ) const = 0;
    //@}

    //! @name Install Methods
    //@{
    virtual Reply GetInstall   ( const Uuid& id ) const = 0;
    virtual Reply DeleteInstall( const Uuid& id, const std::vector< size_t >& list  ) = 0;
    //@}

    //! @name Cache Methods
    //@{
    virtual Reply UploadCache     ( const Uuid& id, std::istream& src ) = 0;
    virtual Reply GetCache        ( const Uuid& id ) const = 0;
    virtual Reply DeleteCache     ( const Uuid& id ) = 0;
    virtual Reply InstallFromCache( const Uuid& id, const std::vector< size_t >& list ) = 0;
    //@}

    //! @name Session Methods
    //@{
    virtual Reply ListSessions ( const Uuid& node, int offset, int limit ) const = 0;
    virtual Reply CountSessions( const Uuid& node ) const = 0;
    virtual Reply GetSession   ( const Uuid& id ) const = 0;
    virtual Reply CreateSession( const Uuid& node, const std::string& name, const std::string& exercise ) = 0;
    virtual Reply DeleteSession( const Uuid& id ) = 0;
    virtual Reply StartSession ( const Uuid& id ) const = 0;
    virtual Reply StopSession  ( const Uuid& id ) const = 0;
    virtual Reply PauseSession ( const Uuid& id ) const = 0;
    //@}

    //! @name Exercise Methods
    //@{
    virtual Reply ListExercises ( const Uuid& id, int offset, int limit ) const = 0;
    virtual Reply CountExercises( const Uuid& id ) const = 0;
    //@}
};
}

#endif // AGENT_ABC_H
