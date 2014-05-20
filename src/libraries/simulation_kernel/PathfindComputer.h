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

class MIL_AgentPion;
class DEC_PathFind_Manager;
class DEC_PathComputer;
class MIL_Population;
class PathRequest;
class PHY_ComposanteTypePion;

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
             PathfindComputer( DEC_PathFind_Manager& manager, const TER_World& world );
    virtual ~PathfindComputer();
    //@}

    //! @name Operations
    //@{
    void Compute( MIL_AgentPion& pion, const sword::PathfindRequest& message,
                  unsigned int ctx, unsigned int clientId, const boost::optional< uint32_t >& magic );
    void Compute( const MIL_Population& population, const sword::PathfindRequest& message,
                  unsigned int ctx, unsigned int clientId, const boost::optional< uint32_t >& magic );
    void Compute( const std::vector< const PHY_ComposanteTypePion* >& equipments,
                  const sword::PathfindRequest& message,
                  unsigned int ctx, unsigned int clientId, const boost::optional< uint32_t >& magic );
    bool Destroy( uint32_t pathfind );
    void Update();
    void SendStateToNewClient();
    //@}

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
    void Compute( const boost::shared_ptr< DEC_PathComputer >& computer, const sword::PathfindRequest& message,
                  unsigned int ctx, unsigned int clientId, const boost::optional< uint32_t >& magic );
    //@}

private:
    //! @name Member data
    //@{
    DEC_PathFind_Manager& manager_;
    const TER_World& world_;
    uint32_t ids_;
    T_Results results_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PathfindComputer )

#endif // __PathfindComputer_h_
