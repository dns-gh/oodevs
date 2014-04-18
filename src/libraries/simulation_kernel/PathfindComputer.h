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
class DEC_PathResult;
class MIL_Population;
class MT_Vector2D;
class MIL_IDManager;
class PathRequest;

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
             PathfindComputer( DEC_PathFind_Manager& pathfindManager, const TER_World& world );
    virtual ~PathfindComputer();
    //@}

    //! @name Operations
    //@{
    unsigned long Compute( MIL_AgentPion& pion, const sword::PathfindRequest& message,
                           unsigned int nCtx, unsigned int clientId, bool stored );
    unsigned long Compute( const MIL_Population& population, const sword::PathfindRequest& message,
                           unsigned int nCtx, unsigned int clientId, bool stored );
    bool Destroy( unsigned int pathfind );
    void Update();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned long, boost::shared_ptr< PathRequest > > T_Results;
    //@}

    //! @name Helpers
    //@{
    unsigned long Compute( boost::shared_ptr< DEC_PathResult > path, const sword::PathfindRequest& request,
                           unsigned int unitId, unsigned int nCtx, unsigned int clientId, bool stored );
    //@}

private:
    //! @name Member data
    //@{
    DEC_PathFind_Manager& pathfindManager_;
    const TER_World& world_;
    std::unique_ptr< MIL_IDManager > idPathManager_;
    T_Results results_;
    //@}
};


#endif // __PathfindComputer_h_
