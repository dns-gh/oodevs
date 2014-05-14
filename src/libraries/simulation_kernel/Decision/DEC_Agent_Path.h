// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_Agent_Path_h_
#define __DEC_Agent_Path_h_

#include "Decision/DEC_PathResult.h"
#include "Entities/Orders/MIL_Fuseau.h"
#include "MT_Tools/MT_Profiler.h"
#include <boost/shared_ptr.hpp>

class MIL_LimaOrder;
class MIL_Agent_ABC;
class DEC_Agent_PathClass;
class DEC_Agent_PathfinderPath;

//*****************************************************************************
// Created: JDY 03-02-11
// Last modified: JVT 03-11-26
//*****************************************************************************
class DEC_Agent_Path : public DEC_PathResult
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Agent_Path( MIL_Agent_ABC& queryMaker, const MT_Vector2D& vPosStart, const MT_Vector2D& vPosEnd, const DEC_PathType& pathType );
             DEC_Agent_Path( MIL_Agent_ABC& queryMaker, const std::vector< boost::shared_ptr< MT_Vector2D > >& points, const DEC_PathType& pathType );
             DEC_Agent_Path( MIL_Agent_ABC& queryMaker, const T_PointVector& points , const DEC_PathType& pathType );
    virtual ~DEC_Agent_Path();
    //@}

    //! @name Destroy
    //@{
    virtual void Destroy();
    //@}

    //! @name Path calculation
    //@{
    virtual void Execute( TER_Pathfinder_ABC& pathfind );
    virtual void CleanAfterComputation();
    virtual bool IsPathForUnit( MIL_Agent_ABC* pion ) const;
    void CancelPath();
    //@}

    //! @name Helper to bypass lack of check of terrain in heuristics.
    //@{
    bool IsDestinationTrafficable() const;
    //@}

    //! @name Accessors
    //@{
    const DEC_Agent_PathClass& GetPathClass() const;
    const T_PointVector& GetNextWaypoints() const;
    //@}

    //! @name Tools
    //@{
    virtual void InsertDecPoints();
    virtual void NotifyPointReached( const T_PathPoints::const_iterator& itCurrentPathPoint );
    virtual bool IsWaypoint( const MT_Vector2D& point ) const;
    //@}

private:
    //! @name Init
    //@{
    void Initialize();
    //@}

    //! @name Points insertion Tools
    //@{
    int IsPointAvant( DEC_PathPoint* pBefore, DEC_PathPoint& current, DEC_PathPoint* pAfter, const TerrainData& nTypeTerrain ) const;
    bool IsPointAvant( const TerrainData& nObjectTypesBefore, const TerrainData& nObjectTypesToNextPoint, const TerrainData& nTypeTerrain ) const;
    bool IsPointAvantOut( const TerrainData& nObjectTypesBefore, const TerrainData& nObjectTypesToNextPoint, const TerrainData& nTypeTerrain ) const;
    bool IsPointAvantIn( const TerrainData& nObjectTypesBefore, const TerrainData& nObjectTypesToNextPoint, const TerrainData& nTypeTerrain ) const;

    void InsertPointAvants();
    void InsertLimas();

    void InsertPointAvant        ( boost::shared_ptr< DEC_PathPoint > spottedPathPoint, T_PathPoints::iterator itCurrent );
    void InsertPointAvant        ( boost::shared_ptr< DEC_PathPoint > spottedPathPoint, T_PathPoints::iterator itCurrent, double& rDistSinceLastPointAvant );
    bool InsertPoint             ( boost::shared_ptr< DEC_PathPoint > spottedPathPoint, T_PathPoints::iterator itCurrent, double& rDistSinceLastPoint );
    void InsertPointAndPointAvant( boost::shared_ptr< DEC_PathPoint > spottedPathPoint, T_PathPoints::iterator itCurrent, double& rDistSinceLastPoint, double& rDistSinceLastPointAvant );
    void InsertLima              ( const MIL_LimaOrder& lima );

    T_PathPoints::iterator GetPreviousPathPointOnDifferentLocation( T_PathPoints::iterator );
    //@}

private:
    //! @name Member data
    //@{
    MIL_Agent_ABC& queryMaker_;
    const DEC_Agent_PathClass& pathClass_;
    bool bRefine_;
    T_PointVector initialWaypoints_;
    T_PointVector nextWaypoints_;
    MIL_Fuseau fuseau_;
    MIL_Fuseau automateFuseau_;
    MT_Profiler profiler_;
    bool bDecPointsInserted_;
    bool destroyed_;
    std::unique_ptr< DEC_Agent_PathfinderPath > path_;
    //@}
};

#endif // __DEC_Agent_Path_h_
