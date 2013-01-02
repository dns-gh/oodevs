// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef movement_module_Agent_Path_h_
#define movement_module_Agent_Path_h_

#include "PathResult.h"
#include "Speeds.h"
#include "wrapper/View.h"
#include "MT_Tools/MT_Profiler.h"

class MIL_LimaOrder;

namespace sword
{
namespace movement
{
    class ModuleFacade;
    class Agent_PathClass;

//*****************************************************************************
// Created: JDY 03-02-11
// Last modified: JVT 03-11-26
//*****************************************************************************
class Agent_Path : public PathResult
{
public:
    //! @name Constructors/Destructor
    //@{
             Agent_Path( ModuleFacade& module, const wrapper::View& entity, const MT_Vector2D& vPosEnd, const PathType& pathType );
             Agent_Path( ModuleFacade& module, const wrapper::View& entity, std::vector< boost::shared_ptr< MT_Vector2D > >& points , const PathType& pathType );
             Agent_Path( ModuleFacade& module, const wrapper::View& entity, const T_PointVector& points , const PathType& pathType );
    virtual ~Agent_Path();
    //@}

    //! @name Path calculation
    //@{
    virtual void Execute( TER_Pathfinder_ABC& pathfind );
    virtual bool NeedRefine() const;
    virtual bool UseStrictClosest() const;
    void Initialize();
    //@}

    //! @name Accessors
    //@{
    const Speeds& GetUnitSpeeds() const;
    const Agent_PathClass& GetPathClass() const;
    const T_PointVector& GetNextWaypoints() const;

    virtual const Agent_PathClass& GetClass() const;
    virtual const MT_Vector2D* GetFirstPoint() const;
    //@}

    //! @name Tools
    //@{
    virtual void InsertDecPoints();
    virtual void NotifyPointReached( const CIT_PathPointList& itCurrentPathPoint );
    virtual bool IsWaypoint( const MT_Vector2D& point ) const;

    void InsertLima( const MIL_LimaOrder& lima );
    void InsertLima( unsigned int limaID, int functionID, const MT_Vector2D& posIntersect, IT_PathPointList itPoint );
    //@}

private:
    //! @name Init
    //@{
    void ComputePath();
    //@}

    //! @name Points insertion Tools
    //@{
    int IsPointAvant( PathPoint* pBefore, PathPoint& current, PathPoint* pAfter, const TerrainData& nTypeTerrain ) const;
    bool IsPointAvant( const TerrainData& nObjectTypesBefore, const TerrainData& nObjectTypesToNextPoint, const TerrainData& nTypeTerrain ) const;
    bool IsPointAvantOut( const TerrainData& nObjectTypesBefore, const TerrainData& nObjectTypesToNextPoint, const TerrainData& nTypeTerrain ) const;
    bool IsPointAvantIn( const TerrainData& nObjectTypesBefore, const TerrainData& nObjectTypesToNextPoint, const TerrainData& nTypeTerrain ) const;

    void InsertPointAvants();
    void InsertLimas();

    void InsertPointAvant        ( boost::shared_ptr< PathPoint > spottedWaypoint, IT_PathPointList itCurrent );
    void InsertPointAvant        ( boost::shared_ptr< PathPoint > spottedWaypoint, IT_PathPointList itCurrent, double& rDistSinceLastPointAvant );
    bool InsertPoint             ( boost::shared_ptr< PathPoint > spottedWaypoint, IT_PathPointList itCurrent, double& rDistSinceLastPoint );
    void InsertPointAndPointAvant( boost::shared_ptr< PathPoint > spottedWaypoint, IT_PathPointList itCurrent, double& rDistSinceLastPoint, double& rDistSinceLastPointAvant );

    IT_PathPointList GetPreviousWaypointOnDifferentLocation( IT_PathPointList );
    //@}

private:
    //! @name Member data
    //@{
    ModuleFacade& module_;
    const wrapper::View entity_;
    // Path calculation parameters
    const Agent_PathClass& pathClass_;
    T_PointVector initialWaypoints_;
    T_PointVector nextWaypoints_;
    Speeds unitSpeeds_;
    MT_Profiler profiler_;
    bool bDecPointsInserted_;
    //@}
};

}
}

#endif // movement_module_Agent_Path_h_
