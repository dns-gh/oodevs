//*****************************************************************************
//
// $Created: JDY 03-05-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Knowledge/DEC_Rep_PathPoint_Lima.h $
// $Author: Age $
// $Modtime: 3/02/05 18:19 $
// $Revision: 2 $
// $Workfile: DEC_Rep_PathPoint_Lima.h $
//
//*****************************************************************************

#ifndef __DEC_Rep_PathPoint_Lima_h_
#define __DEC_Rep_PathPoint_Lima_h_

#include "DEC_Rep_PathPoint.h"

class MIL_LimaFunction;

//*****************************************************************************
// Created: JDY 03-05-05
//*****************************************************************************
class DEC_Rep_PathPoint_Lima : public DEC_Rep_PathPoint
{
public:
    DEC_Rep_PathPoint_Lima( const MT_Vector2D& vPos, const TerrainData& nTypeTerrain, unsigned int nLimaID, const MIL_LimaFunction& function );
    virtual ~DEC_Rep_PathPoint_Lima();

    //! @name Accessors
    //@{
    int GetTypeLima();
    unsigned int GetLimaID();
    //@}

private:
    int typeLima_;
    unsigned int limaID_;
};

#endif // __DEC_Rep_PathPoint_Lima_h_