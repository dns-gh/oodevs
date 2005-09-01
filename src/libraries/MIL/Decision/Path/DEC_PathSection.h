//*****************************************************************************
//
// $Created: JDY 03-02-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_PathSection.h $
// $Author: Age $
// $Modtime: 10/05/05 16:48 $
// $Revision: 6 $
// $Workfile: DEC_PathSection.h $
//
//*****************************************************************************

#ifndef __DEC_PathSection_h_
#define __DEC_PathSection_h_

#include "MIL.h"
#include "DEC_PathType.h"
#include "pathfind/TerrainPathPoint.h"
#include "pathfind/TerrainData.h"
#include "pathfind/AStarManagementCallback_ABC.h"
#include "tools/thread/Handler_ABC.h"

class DEC_Path_ABC;
class DEC_Path;
class TerrainRule_ABC;

// =============================================================================
// @class  DEC_Tools
// Created: NLD 2005-02-22
// =============================================================================
class DEC_PathSection : private tools::thread::Handler_ABC< TerrainPathPoint >, pathfind::AStarManagementCallback_ABC
{
    MT_COPYNOTALLOWED( DEC_PathSection );

public:
             DEC_PathSection( DEC_Path& path, DEC_PathType pathType, const MT_Vector2D& vStartPoint, const MT_Vector2D& vEndPoint );
    virtual ~DEC_PathSection();

    //! @name Operations
    //@{
    bool Execute( TerrainPathfinder& pathfind );
    MT_Float GetLength() const;
    void Cancel();
    //@}

    //! @name Accessors
    //@{
    bool               IsImpossible() const;
    const MT_Vector2D& GetPosStart() const;
    const MT_Vector2D& GetPosEnd() const; // $$$$ AGE 2005-02-25: ...
    //@}

private:
    //! @name Tools
    //@{
    virtual void     Handle( const TerrainPathPoint& point );
    bool ShouldEndComputation( float rCostToCurrentNode, float rCostToGoalNode );
    bool ComputationTimedOut() const;
    //@}

private:
    MT_Vector2D      vStartPoint_;
    MT_Vector2D      vEndPoint_;
    DEC_Path_ABC&    path_;
    TerrainRule_ABC* pRule_;

    bool          bCanceled_;
    bool          bRefine_;
    unsigned int  nAddedPoints_;
    unsigned long nComputationStart_;
    unsigned int  nPendingRequests_;
};

#include "DEC_PathSection.inl"

#endif // __DEC_PathSection_h_