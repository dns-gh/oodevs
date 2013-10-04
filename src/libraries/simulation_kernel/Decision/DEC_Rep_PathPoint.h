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

class DEC_Representations;

//*****************************************************************************
// Created: JDY 03-05-05
//*****************************************************************************
class DEC_Rep_PathPoint : public DEC_PathPoint
{
public:
             DEC_Rep_PathPoint( const MT_Vector2D& vPos, E_TypePoint nTypePoint, const TerrainData& nTypeTerrain, const char* szDIARepType = "Rep_Point" );
    virtual ~DEC_Rep_PathPoint();

    //! @name Accessors
    //@{
    virtual const TerrainData& GetTypeTerrain() const;
    //@}

    //! @name DIA
    //@{
    virtual void SendToDIA( DEC_Representations& agent, boost::shared_ptr< DEC_PathPoint > point ) const;
    virtual void RemoveFromDIA( boost::shared_ptr< DEC_PathPoint > self );
    //@}

private:
    TerrainData nTypeTerrain_;
    mutable DEC_Representations* pSentToDiaAgent_;
};

#endif // __DEC_Rep_PathPoint_h_