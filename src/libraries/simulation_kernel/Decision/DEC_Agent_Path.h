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
#include <boost/shared_ptr.hpp>

class MIL_LimaOrder;
class MIL_Agent_ABC;
class DEC_Agent_PathClass;
class DEC_AgentContext_ABC;
class DEC_PathComputer_ABC;

//*****************************************************************************
// Created: JDY 03-02-11
// Last modified: JVT 03-11-26
//*****************************************************************************
class DEC_Agent_Path : public DEC_PathResult
{
public:
             DEC_Agent_Path( MIL_Agent_ABC& queryMaker, const T_PointVector& points, const DEC_PathType& pathType,
                 const boost::shared_ptr< DEC_PathComputer_ABC >& computer );
    virtual ~DEC_Agent_Path();

    virtual void Destroy();
    virtual void Cancel();

    void CancelPath();

    const DEC_Agent_PathClass& GetPathClass() const;
    const T_PointVector& GetNextWaypoints() const;

    virtual E_State GetState() const;
    virtual double GetLength() const;

private:
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

    virtual void Finalize();
    virtual void NotifyPointReached( const T_PathPoints::const_iterator& itCurrentPathPoint );
    virtual const MT_Vector2D& GetLastWaypoint() const;

private:
    //! @name Member data
    //@{
    MIL_Agent_ABC& queryMaker_;
    const DEC_Agent_PathClass& pathClass_;
    const T_PointVector initialWaypoints_;
    T_PointVector nextWaypoints_;
    bool destroyed_;
    boost::shared_ptr< DEC_AgentContext_ABC > context_;
    boost::shared_ptr< DEC_PathComputer_ABC > computer_;
    //@}
};

#endif // __DEC_Agent_Path_h_
