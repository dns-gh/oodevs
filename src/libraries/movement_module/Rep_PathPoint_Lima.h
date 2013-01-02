//*****************************************************************************
//
// $Created: JDY 03-05-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Knowledge/Rep_PathPoint_Lima.h $
// $Author: Age $
// $Modtime: 3/02/05 18:19 $
// $Revision: 2 $
// $Workfile: Rep_PathPoint_Lima.h $
//
//*****************************************************************************

#ifndef movement_module_Rep_PathPoint_Lima_h_
#define movement_module_Rep_PathPoint_Lima_h_

#include "Rep_PathPoint.h"

namespace sword
{
namespace movement
{
//*****************************************************************************
// Created: JDY 03-05-05
//*****************************************************************************
class Rep_PathPoint_Lima : public Rep_PathPoint
{
public:
             Rep_PathPoint_Lima( const MT_Vector2D& vPos, const TerrainData& nTypeTerrain, unsigned int nLimaID, int typeLima );
    virtual ~Rep_PathPoint_Lima();

    //! @name Accessors
    //@{
    int GetTypeLima() const;
    unsigned int GetLimaID() const;
    //@}

private:
    const int typeLima_;
    const unsigned int limaID_;
};

}
}

#endif // movement_module_Rep_PathPoint_Lima_h_
