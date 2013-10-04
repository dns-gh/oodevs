//*****************************************************************************
//
// $Created: JDY 03-05-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Knowledge/DEC_Rep_PathPoint_Special.h $
// $Author: Age $
// $Modtime: 3/02/05 18:20 $
// $Revision: 2 $
// $Workfile: DEC_Rep_PathPoint_Special.h $
//
//*****************************************************************************

#ifndef __DEC_Rep_PathPoint_Special_h_
#define __DEC_Rep_PathPoint_Special_h_

#include "DEC_Rep_PathPoint.h"


//*****************************************************************************
// Created: JDY 03-05-05
//*****************************************************************************
class DEC_Rep_PathPoint_Special : public DEC_Rep_PathPoint
{
public:
    enum E_TypePointParticulier
    {
        eTypePointParticulierPont       = 1,
        eTypePointParticulierCarrefour  = 2,
        eTypePointParticulierVillage    = 4,
        eTypePointParticulierPointRoute = 8,
        eTypePointParticulierForet      = 16
    };

public:
             DEC_Rep_PathPoint_Special( const MT_Vector2D& vPos, E_TypePointParticulier nTypePoint, const TerrainData& nTypeTerrain );
    virtual ~DEC_Rep_PathPoint_Special();

private:
    E_TypePointParticulier nTypePointParticulier_;
};

#endif // __DEC_Rep_PathPoint_Special_h_