//*****************************************************************************
//
// $Created: JDY 03-02-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_PathSection_ABC.h $
// $Author: Age $
// $Modtime: 10/05/05 16:48 $
// $Revision: 6 $
// $Workfile: DEC_PathSection_ABC.h $
//
//*****************************************************************************

#ifndef __DEC_PathSection_ABC_h_
#define __DEC_PathSection_ABC_h_

#include "MIL.h"

#include "pathfind/TerrainPathPoint.h"
#include "pathfind/AStarManagementCallback_ABC.h"
#include "tools/thread/Handler_ABC.h"

class DEC_Path_ABC;
class TerrainRule_ABC;

// =============================================================================
// @class  DEC_Tools
// Created: NLD 2005-02-22
// =============================================================================
class DEC_PathSection_ABC : private tools::thread::Handler_ABC< TerrainPathPoint >
                          , private pathfind::AStarManagementCallback_ABC
{
    MT_COPYNOTALLOWED( DEC_PathSection_ABC );

public:
    virtual ~DEC_PathSection_ABC();

    //! @name Operations
    //@{
    bool     Execute  ( TerrainPathfinder& pathfind, uint nComputationEndTime );
    MT_Float GetLength() const;
    void     Cancel   ();
    //@}

    //! @name Accessors
    //@{
    bool               IsImpossible() const;
    const MT_Vector2D& GetPosStart () const;
    const MT_Vector2D& GetPosEnd   () const; // $$$$ AGE 2005-02-25: ...
    //@}

protected:
    DEC_PathSection_ABC( DEC_Path_ABC& path, const MT_Vector2D& vStartPoint, const MT_Vector2D& vEndPoint );

    //! @name Accessors
    //@{
    virtual TerrainRule_ABC& GetRule() const = 0;
    //@}

private:
    //! @name Tools
    //@{
    virtual void Handle              ( const TerrainPathPoint& point );
            bool ShouldEndComputation( float rCostToCurrentNode, float rCostToGoalNode );
    //@}

private:
    DEC_Path_ABC& path_;
    MT_Vector2D   startPoint_;
    MT_Vector2D   endPoint_;
    uint          nComputationEndTime_;

    bool          bCanceled_;
    unsigned int  nAddedPoints_;
};

#include "DEC_PathSection_ABC.inl"

#endif // __DEC_PathSection_ABC_h_