// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_PathResult_h_
#define __DEC_PathResult_h_

#include "Knowledge/DEC_Knowledge_Def.h"
#include "MT_Tools/Mt_Vector2DTypes.h"
#include "simulation_terrain/TER_Path_ABC.h"
#include <geometry/Types.h>
#include <list>

namespace sword
{
    class Path;
}

class DEC_PathType;
class MIL_Agent_ABC;
class TER_Localisation;
class TER_PathPoint;
class TER_PathFuture;
class TER_PathSection;
class TER_Polygon;

// Enumerates the edges in "points" and introduces intermediate points to force
// the path walker to evaluate non-zero slopes. Note input points are not
// duplicated, instances are preserved and their slope information updated in
// place.
std::list< boost::shared_ptr< TER_PathPoint > > SplitEdgesOnElevationGrid(
    const std::list< boost::shared_ptr< TER_PathPoint > >& points );

//*****************************************************************************
// Created: JDY 03-02-11
// Last modified: JVT 03-11-26
//*****************************************************************************
class DEC_PathResult : public TER_Path_ABC
{
public:
    //! @name Types
    //@{
    typedef std::list< boost::shared_ptr< TER_PathPoint > > T_PathPoints;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             DEC_PathResult( const DEC_PathType& pathType, unsigned int callerId );
    virtual ~DEC_PathResult();
    //@}

    //! @name Accessors
    //@{
    const T_PathPoints& GetResult() const;
    const DEC_PathType& GetPathType() const;
    //@}

    //! @name Tools
    //@{
    void StartCompute();
    void StartCompute( const std::vector< geometry::Point2f >& itinerary );
    bool IsOnPath( const MT_Vector2D& vPos ) const;
    T_PathPoints::const_iterator GetCurrentKeyOnPath() const;
    MT_Vector2D GetFuturePosition( const MT_Vector2D& vStartPos, double rDist, bool bBoundOnPath ) const;
    bool ComputeFutureObjectCollision( const MT_Vector2D& vStartPos, const T_KnowledgeObjectVector& objectsToTest,
        double& rDistance, boost::shared_ptr< DEC_Knowledge_Object >& pObject,
        const MIL_Agent_ABC& agent, bool blockedByObject, bool applyScale ) const;
    virtual void NotifyPointReached( const T_PathPoints::const_iterator& itCurrentPathPoint );
    virtual TER_Path_ABC::E_State GetState() const;
    virtual void Cancel();
    virtual const MT_Vector2D& GetLastWaypoint() const;
    virtual void Finalize();
    //@}

    //! @name Network
    //@{
    void Serialize( sword::Path& asn, int firstPoint, int pathSizeThreshold ) const;
    //@}

private:
    //! @name Helpers
    //@{
    MT_Vector2D InternalGetFuturePosition( const T_PathPoints::const_iterator& itCurrentPos, double rDist, bool bBoundOnPath ) const;
    std::pair< TER_Polygon, std::size_t > ComputePathHull( const T_PathPoints::const_iterator& itCurrentPathPoint ) const;
    void ComputeFutureObjectCollision( const MT_Vector2D& vStartPos, double& rDistance,
        boost::shared_ptr< DEC_Knowledge_Object >& pObject, const boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge,
        const T_PathPoints::const_iterator& itCurrentPathPoint, const TER_Localisation& location ) const;
    //@}

protected:
    void AddSection( const boost::shared_ptr< TER_PathSection >& section );
    void SetResult( T_PathPoints points );
    const std::vector< MT_Vector2D >& GetComputedWaypoints() const;
    void RemoveComputedWaypoint();
    virtual void DoFinalize();

protected:
    T_PathPoints resultList_; //$$$

private:
    //! @name Member data
    //@{
    T_PathPoints::const_iterator itCurrentPathPoint_;
    const DEC_PathType& pathType_;
    bool bSectionJustStarted_;
    boost::shared_ptr< TER_PathFuture > future_;
    TER_Path_ABC::E_State state_;
    MT_Vector2D lastWaypoint_;
    std::vector< MT_Vector2D > computedWaypoints_;
    bool finalized_;
    std::vector< boost::shared_ptr< TER_PathSection > > sections_;
    const unsigned int callerId_;
    //@}
};

#endif // __DEC_PathResult_h_
