// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __PathfindComputer_h_
#define __PathfindComputer_h_

#include <boost/noncopyable.hpp>
#include <boost/optional/optional_fwd.hpp>

namespace sword
{
    class PathfindRequest;
    class MagicAction;
}

class ActionManager;
class MIL_AgentPion;
class DEC_PathComputer;
class MIL_Population;
class PathRequest;
class PHY_ComposanteTypePion;
class TER_Pathfinder;
class TER_PathSection;

// =============================================================================
/** @class  PathfindComputer
    @brief  Pathfind computer
*/
// Created: LGY 2014-03-03
// =============================================================================
class PathfindComputer : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PathfindComputer( TER_Pathfinder& manager, const TER_World& world );
    virtual ~PathfindComputer();
    //@}

    //! @name Operations
    //@{
    void Update( ActionManager& actions );
    void SendStateToNewClient();
    //@}

    bool OnReceivePathfindCreation   ( const sword::MagicAction& message,
                                       unsigned int nCtx, unsigned int clientId, uint32_t magicId );
    void OnReceivePathfindUpdate     ( const sword::MagicAction& message );
    void OnReceivePathfindDestruction( const sword::MagicAction& message, sword::MagicActionAck& ack );
    void OnPathfindRequest           ( const sword::PathfindRequest& message, unsigned int nCtx, unsigned int clientId );
    void DeletePathfindsFromUnit     ( uint32_t unitId );

    //! @name Serialization
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    template< typename Archive >        void load( Archive&, const unsigned int );
    template< typename Archive >        void save( Archive&, const unsigned int ) const;
    template< typename Archive > friend void save_construct_data( Archive&, const PathfindComputer*, const unsigned int /*version*/ );
    template< typename Archive > friend void load_construct_data( Archive&, PathfindComputer*, const unsigned int /*version*/ );
    //@}

private:
    //! @name typedef helpers
    //@{
    typedef std::map< uint32_t, boost::shared_ptr< PathRequest > > T_Results;
    //@}

    //! @name Helpers
    //@{
    bool Destroy( uint32_t pathfind );
    void Compute( MIL_AgentPion& pion, const sword::PathfindRequest& message,
                  unsigned int ctx, unsigned int clientId, const boost::optional< uint32_t >& magic );
    void Compute( const MIL_Population& population, const sword::PathfindRequest& message,
                  unsigned int ctx, unsigned int clientId, const boost::optional< uint32_t >& magic );
    void Compute( const std::vector< const PHY_ComposanteTypePion* >& equipments,
                  const sword::PathfindRequest& message,
                  unsigned int ctx, unsigned int clientId, const boost::optional< uint32_t >& magic );
    void Compute( unsigned int callerId,
           const std::vector< boost::shared_ptr< TER_PathSection > >& sections,
           const sword::PathfindRequest& message, unsigned int ctx,
           unsigned int clientId, const boost::optional< uint32_t >& magic );
    //@}

private:
    //! @name Member data
    //@{
    TER_Pathfinder& manager_;
    const TER_World& world_;
    uint32_t ids_;
    T_Results results_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PathfindComputer )

#endif // __PathfindComputer_h_
