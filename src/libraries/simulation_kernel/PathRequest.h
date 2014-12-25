// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __PathRequest_h_
#define __PathRequest_h_

#include <boost/optional.hpp>

class ActionManager;
class TER_PathFuture;

namespace sword
{
    class Pathfind;
    class PathfindRequest;
    class PathfindResult;
}

// =============================================================================
/** @class  PathRequest
    @brief  Path request
*/
// Created: LGY 2014-04-16
// =============================================================================
class PathRequest : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
     PathRequest( const boost::shared_ptr< TER_PathFuture >& future,
                  unsigned int ctx,
                  unsigned int clientId,
                  uint32_t id,
                  const sword::PathfindRequest& message,
                  const boost::optional< uint32_t >& magic );
    virtual ~PathRequest();
    //@}

    //! @name Serialization
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    template< typename Archive >        void load( Archive&, const unsigned int );
    template< typename Archive >        void save( Archive&, const unsigned int ) const;
    template< typename Archive > friend void save_construct_data( Archive&, const PathRequest*, const unsigned int /*version*/ );
    template< typename Archive > friend void load_construct_data( Archive&, PathRequest*, const unsigned int /*version*/ );
    //@}

    //! @name Operations
    //@{
    bool Update( ActionManager& actions );
    bool Update( const sword::Pathfind& pathfind );
    void SendStateToNewClient();
    bool IsPublished() const;
    uint32_t GetUnitId() const;
    std::unique_ptr< sword::Pathfind > GetPathfind() const;
    //@}

private:
    void SendComputePathfindAck( bool ok );
    void SendPathfindCreation( ActionManager& actions, bool ok );
    void FillPathfindMessage( sword::Pathfind& msg ) const;

private:
    //! @name Member data
    //@{
    boost::shared_ptr< TER_PathFuture > future_;
    const unsigned int ctx_;
    const unsigned int clientId_;
    const uint32_t id_;
    sword::PathfindRequest request_;
    const boost::optional< uint32_t > magic_;
    boost::optional< sword::PathResult > path_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PathRequest )

#endif // __PathRequest_h_
