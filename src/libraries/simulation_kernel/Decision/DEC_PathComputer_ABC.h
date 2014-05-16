// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __DEC_PathComputer_ABC_h_
#define __DEC_PathComputer_ABC_h_

#include "DEC_Path_ABC.h"
#include "DEC_PathResult_ABC.h"
#include <boost/noncopyable.hpp>

class TER_Pathfinder_ABC;
class DEC_PathSection;
class DEC_PathPoint;
class MT_Vector2D;

// =============================================================================
/** @class  DEC_PathComputer_ABC
    @brief  DEC_PathComputer_ABC
*/
// Created: MCO 2014-05-15
// =============================================================================
class DEC_PathComputer_ABC : public DEC_PathResult_ABC
{
public:
             DEC_PathComputer_ABC() {}
    virtual ~DEC_PathComputer_ABC() {}

    virtual double GetLength() const = 0;
    virtual void Execute( TER_Pathfinder_ABC& pathfind ) = 0;
    virtual void Cancel() = 0;
    virtual DEC_Path_ABC::E_State GetState() const = 0;
    virtual void RegisterPathSection( DEC_PathSection& section ) = 0;

    virtual const MT_Vector2D& GetLastWaypoint() const = 0;
    virtual const std::vector< MT_Vector2D >& GetComputedWaypoints() const = 0;
    virtual void RemoveComputedWaypoint() = 0;

    typedef std::list< boost::shared_ptr< DEC_PathPoint > > T_PathPoints;
    virtual T_PathPoints GetResult() const = 0;

};

#endif // __DEC_PathComputer_ABC_h_
