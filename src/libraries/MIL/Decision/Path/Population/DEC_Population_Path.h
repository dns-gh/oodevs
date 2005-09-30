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

#include "Decision/Path/DEC_Path_ABC.h"

class DEC_PathPoint;
class MIL_Population;

//*****************************************************************************
// Created: JDY 03-02-11
// Last modified: JVT 03-11-26
//*****************************************************************************
class DEC_Population_Path : public DEC_Path_ABC
{
    friend class DEC_PathFind_ComputationThread;

public:
    //! @name Types
    //@{   
    typedef std::list< DEC_PathPoint* >                 T_PathPointList;
    typedef T_PathPointList::iterator                   IT_PathPointList;
    typedef T_PathPointList::const_iterator             CIT_PathPointList;    
    //@}

public:
     DEC_Population_Path( const MIL_Population& queryMaker, const MT_Vector2D& vPosEnd );
    ~DEC_Population_Path();

    //! @name Path calculation
    //@{
    virtual void Execute( TerrainPathfinder& pathfind );
    virtual void AddResultPoint( const MT_Vector2D& vPos, const TerrainData& nObjectTypes, const TerrainData& nObjectTypesToNextPoint );
    //@}

private:
    //! @name Init
    //@{
    void Initialize( const T_PointVector& pathPoints );
    //@}

    //! @name Tools
    //@{
    virtual void NotifySectionEnded();
    //@}

    //! @name Operators
    //@{
    DEC_Population_Path& operator=( const DEC_Population_Path& rhs );
    //@}
    
private:   
    const MIL_Population& queryMaker_;
          MT_Profiler     profiler_;   
          bool            bSectionJustEnded_;
          T_PathPointList resultList_;
};

#include "DEC_Population_Path.inl"

#endif // __DEC_Population_Path_h_