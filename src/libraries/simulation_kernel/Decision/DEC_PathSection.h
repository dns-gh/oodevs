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

#include "MT_Tools/MT_Vector2D.h"
#include <pathfind/TerrainPathPoint.h>
#include <pathfind/TerrainRule_ABC.h>
#include <pathfind/AStarManagementCallback_ABC.h>
#include <tools/thread/Handler_ABC.h>

class TER_Pathfinder_ABC;
class DEC_PathResult_ABC;

// =============================================================================
// @class  DEC_Tools
// Created: NLD 2005-02-22
// =============================================================================
class DEC_PathSection : private tools::thread::Handler_ABC< TerrainPathPoint >
                          , private pathfind::AStarManagementCallback_ABC
                          , private boost::noncopyable
{
public:
             DEC_PathSection(
                    DEC_PathResult_ABC& result, std::unique_ptr< TerrainRule_ABC > rule,
                    const MT_Vector2D& vStartPoint, const MT_Vector2D& vEndPoint,
                    bool needRefine, bool useStrictClosest );
    virtual ~DEC_PathSection();

    //! @name Operations
    //@{
    bool   Execute  ( TER_Pathfinder_ABC& pathfind, unsigned int nComputationEndTime );
    double GetLength() const;
    void   Cancel   ();
    //@}

    //! @name Accessors
    //@{
    bool                IsImpossible() const;
    const MT_Vector2D&  GetPosStart () const;
    const MT_Vector2D&  GetPosEnd   () const; // $$$$ AGE 2005-02-25: ...
    void                SetPosStart ( const MT_Vector2D& point );
    //@}

private:
    //! @name Tools
    //@{
    virtual void Handle( const TerrainPathPoint& point );

    bool ShouldEndComputation( float rCostToCurrentNode, float rCostToGoalNode );
    //@}

private:
    DEC_PathResult_ABC& result_;
    std::unique_ptr< TerrainRule_ABC > rule_;
    MT_Vector2D startPoint_;
    const MT_Vector2D endPoint_;
    const bool needRefine_;
    const bool useStrictClosest_;
    bool bCanceled_;
    unsigned int nComputationEndTime_;
    unsigned int nAddedPoints_;
};

#endif // __DEC_PathSection_h_
