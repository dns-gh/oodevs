//*****************************************************************************
//
// $Created: JDY 03-05-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Knowledge/DEC_Rep_PathPoint.h $
// $Author: Age $
// $Modtime: 3/02/05 18:16 $
// $Revision: 4 $
// $Workfile: DEC_Rep_PathPoint.h $
//
//*****************************************************************************

#ifndef __DEC_Rep_PathPoint_h_
#define __DEC_Rep_PathPoint_h_

#include "Decision/DEC_PathPoint.h"
#include <boost/shared_ptr.hpp>

class DEC_Representations;

// Base class for path points returned to DIA.
class DEC_DIA_PathPoint : public DEC_PathPoint
{
public:
             DEC_DIA_PathPoint( const MT_Vector2D& vPos, E_Type type, E_TypePoint nTypePoint,
                     const char* szDIARepType );
    virtual ~DEC_DIA_PathPoint() {}

    void SendToDIA( DEC_Representations& agent,
            boost::shared_ptr< DEC_DIA_PathPoint > point );
    void RemoveFromDIA( boost::shared_ptr< DEC_DIA_PathPoint > self );

private:
    DEC_Representations* pSentToDiaAgent_;
};

class DEC_Rep_PathPoint : public DEC_DIA_PathPoint
{
public:
             DEC_Rep_PathPoint( const MT_Vector2D& vPos, E_TypePoint nTypePoint, const TerrainData& nTypeTerrain, const char* szDIARepType = "Rep_Point" );
    virtual ~DEC_Rep_PathPoint();

    //! @name Accessors
    //@{
    const TerrainData& GetTypeTerrain() const;
    //@}

private:
    TerrainData nTypeTerrain_;
};

boost::shared_ptr< DEC_Rep_PathPoint > CreateSpecialPoint(
        const MT_Vector2D& pos, const TerrainData& terrain );

#endif // __DEC_Rep_PathPoint_h_
