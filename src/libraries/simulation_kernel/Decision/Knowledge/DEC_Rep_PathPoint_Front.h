//*****************************************************************************
//
// $Created: JDY 03-05-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Knowledge/DEC_Rep_PathPoint_Front.h $
// $Author: Jvt $
// $Modtime: 3/12/04 11:39 $
// $Revision: 2 $
// $Workfile: DEC_Rep_PathPoint_Front.h $
//
//*****************************************************************************

#ifndef __DEC_Rep_PathPoint_Front_h_
#define __DEC_Rep_PathPoint_Front_h_

#include "MIL.h"
#include "Decision/Path/DEC_PathPoint.h"

class DEC_Rep_PathPoint;
class DEC_Representations;

//*****************************************************************************
// Created: JDY 03-05-05
//*****************************************************************************
class DEC_Rep_PathPoint_Front  : public DEC_PathPoint
                               , public DIA_TypedObject
{
    MT_COPYNOTALLOWED( DEC_Rep_PathPoint_Front )

public:
    explicit DEC_Rep_PathPoint_Front( const MT_Vector2D& vPos, DEC_Rep_PathPoint& dest );
    virtual ~DEC_Rep_PathPoint_Front();

    //! @name Init
    //@{
    static void InitializeDIA();
    //@}

    //! @name DIA
    //@{
    virtual void SendToDIA( DEC_Representations& agent ) const;
    //@}

    //! @name Debug
    //@{
    virtual void Dump() const;
    //@}

private:
    DEC_Rep_PathPoint& destPoint_;

    mutable DEC_Representations* pSentToDiaAgent_;

private:
    static int nDIAPointIdx_;
    static int nDIAClsIdx_;
    static int nDIADestIdx_;
};

#endif // __DEC_Rep_PathPoint_Front_h_