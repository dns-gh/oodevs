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

class MIL_AgentPion;
class DEC_PathFind_Manager;
class DEC_PathComputer;
class MIL_Population;
class MT_Vector2D;
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
    uint32_t Compute( MIL_AgentPion& pion, const sword::PathfindRequest& message,
                      unsigned int ctx, unsigned int clientId, bool store );
    uint32_t Compute( const MIL_Population& population, const sword::PathfindRequest& message,
                      unsigned int ctx, unsigned int clientId, bool store );
    uint32_t Compute( const std::vector< const PHY_ComposanteTypePion* >& equipments,
                      const sword::PathfindRequest& message,
                      unsigned int ctx, unsigned int clientId, bool store );
    bool Destroy( uint32_t pathfind );
    void Update();
    //@}

private:
    //! @name Helpers
    //@{
    uint32_t Compute( const boost::shared_ptr< DEC_PathComputer >& computer, const sword::PathfindRequest& message,
                      unsigned int ctx, unsigned int clientId, bool store );
    //@}

private:
    //! @name Member data
    //@{
    DEC_PathFind_Manager& manager_;
    const TER_World& world_;
    uint32_t ids_;
    std::map< uint32_t, boost::shared_ptr< PathRequest > > results_;
    //@}
};


#endif // __PathfindComputer_h_
