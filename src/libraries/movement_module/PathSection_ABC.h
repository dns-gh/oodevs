//*****************************************************************************
//
// $Created: JDY 03-02-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/PathSection_ABC.h $
// $Author: Age $
// $Modtime: 10/05/05 16:48 $
// $Revision: 6 $
// $Workfile: PathSection_ABC.h $
//
//*****************************************************************************

#ifndef movement_module_PathSection_ABC_h_
#define movement_module_PathSection_ABC_h_

#include "MT_Tools/MT_Vector2D.h"
#include <boost/noncopyable.hpp>

class TerrainRule_ABC;
class TerrainPathfinder;
class TerrainPathPoint;
class TerrainData;

namespace sword
{
namespace movement
{
    class Path_ABC;

// =============================================================================
// @class  DEC_Tools
// Created: NLD 2005-02-22
// =============================================================================
class PathSection_ABC : /*private tools::thread::Handler_ABC< TerrainPathPoint >
                          , private pathfind::AStarManagementCallback_ABC
                          ,*/ private boost::noncopyable
{
public:
    virtual ~PathSection_ABC();

    //! @name Operations
    //@{
    bool   Execute  ( TerrainPathfinder& pathfind, unsigned int nComputationEndTime );
    double GetLength() const;
    void   Cancel   ();
    //@}

    //! @name Accessors
    //@{
    bool                IsImpossible() const;
    const MT_Vector2D&  GetPosStart () const;
    const MT_Vector2D&  GetPosEnd   () const; // $$$$ AGE 2005-02-25: ...
    const Path_ABC& GetPath     () const;
    void                SetPosStart ( const MT_Vector2D& point );
    //@}

    void HandlePathPoint( const MT_Vector2D& point, const TerrainData& dataAtPoint, const TerrainData& dataToNextPoint );
    bool ShouldEndComputation( float rCostToCurrentNode, float rCostToGoalNode );

protected:
    PathSection_ABC( Path_ABC& path, const MT_Vector2D& vStartPoint, const MT_Vector2D& vEndPoint );

    //! @name Accessors
    //@{
    virtual TerrainRule_ABC& GetRule() const = 0;
    //@}

private:
    Path_ABC& path_;
    MT_Vector2D startPoint_;
    MT_Vector2D endPoint_;
    unsigned int nComputationEndTime_;

    bool bCanceled_;
    unsigned int nAddedPoints_;
};

}
}

#endif // movement_module_PathSection_ABC_h_