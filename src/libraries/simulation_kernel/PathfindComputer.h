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
class MIL_Population;
class DEC_PathResult;
class MT_Vector2D;

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
             PathfindComputer();
    virtual ~PathfindComputer();
    //@}

    //! @name Operations
    //@{
    void Compute( MIL_AgentPion& pion, const MT_Vector2D& start, const MT_Vector2D& end,
                  unsigned int nCtx, unsigned int clientId );
    void Compute( const MIL_Population& population, const MT_Vector2D& start, const MT_Vector2D& end,
                  unsigned int nCtx, unsigned int clientId );
    void Update();
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< unsigned int, boost::shared_ptr< DEC_PathResult > > T_Result;
    typedef std::map< unsigned int, T_Result >                             T_Results;
    //@}

    //! @name Helpers
    //@{
    void Compute( const boost::shared_ptr< DEC_PathResult >& path, unsigned int nCtx, unsigned int clientId );
    bool PathComputed( unsigned int clientId, const T_Result& content );
    //@}

private:
    //! @name Member data
    //@{
    T_Results results_;
    //@}
};


#endif // __PathfindComputer_h_
