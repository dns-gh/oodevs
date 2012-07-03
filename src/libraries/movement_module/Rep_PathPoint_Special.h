//*****************************************************************************
//
// $Created: JDY 03-05-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Knowledge/Rep_PathPoint_Special.h $
// $Author: Age $
// $Modtime: 3/02/05 18:20 $
// $Revision: 2 $
// $Workfile: Rep_PathPoint_Special.h $
//
//*****************************************************************************

#ifndef movement_module_Rep_PathPoint_Special_h_
#define movement_module_Rep_PathPoint_Special_h_

#include "Rep_PathPoint.h"

namespace sword
{
namespace movement
{
//*****************************************************************************
// Created: JDY 03-05-05
//*****************************************************************************
class Rep_PathPoint_Special : public Rep_PathPoint
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
             Rep_PathPoint_Special( const MT_Vector2D& vPos, E_TypePointParticulier nTypePoint, const TerrainData& nTypeTerrain );
    virtual ~Rep_PathPoint_Special();

private:
    E_TypePointParticulier nTypePointParticulier_;
};

}
}

#endif // movement_module_Rep_PathPoint_Special_h_
