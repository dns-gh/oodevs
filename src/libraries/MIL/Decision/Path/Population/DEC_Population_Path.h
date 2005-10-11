//*****************************************************************************
//
// $Created: JDY 03-02-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Population_Path.h $
// $Author: Age $
// $Modtime: 16/06/05 15:09 $
// $Revision: 10 $
// $Workfile: DEC_Population_Path.h $
//
//*****************************************************************************

#ifndef __DEC_Population_Path_h_
#define __DEC_Population_Path_h_

#include "MIL.h"

#include "Decision/Path/DEC_PathResult.h"
#include "MT_Tools/MT_Profiler.h"

class MIL_PopulationFlow;

//*****************************************************************************
// Created: JDY 03-02-11
// Last modified: JVT 03-11-26
//*****************************************************************************
class DEC_Population_Path : public DEC_PathResult
{
public:
     DEC_Population_Path( const MIL_PopulationFlow& flow, const MT_Vector2D& destination );
    ~DEC_Population_Path();

    //! @name Path calculation
    //@{
    virtual void Execute( TerrainPathfinder& pathfind );
    //@}

private:
    DEC_Population_Path( const DEC_Population_Path& rhs ); // Copy only query parameters, not the result !

    //! @name Tools
    //@{
            void Initialize     ( const T_PointVector& pathPoints );
    virtual void InsertDecPoints();
    //@}
   
private:   
    const MIL_PopulationFlow& flow_;
          MT_Profiler         profiler_;   
};

#include "DEC_Population_Path.inl"

#endif // __DEC_Population_Path_h_