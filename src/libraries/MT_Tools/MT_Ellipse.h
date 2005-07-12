//*****************************************************************************
//
// $Created: JVT 04-02-26 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Ellipse.h $
// $Author: Age $
// $Modtime: 2/03/05 16:12 $
// $Revision: 7 $
// $Workfile: MT_Ellipse.h $
//
//*****************************************************************************

#ifndef __MT_Ellipse_h_
#define __MT_Ellipse_h_

#include "MT_Tools_Types.h"
#include "MT_Vector2D.h"
#include "MT_Rect.h"

//*****************************************************************************
// Created: JVT 04-02-26
// Last modified: JVT 04-03-25
//*****************************************************************************
class MT_Ellipse
{    
public:
             MT_Ellipse( const MT_Vector2D& vOrigin, MT_Float rMajorAxeLength, MT_Float rMinorAxeLength, MT_Float rTheta );
             MT_Ellipse( const MT_Vector2D& vOrigin, const MT_Vector2D& vMajorAxisHigh, const MT_Vector2D& vMinorAxisHigh );
             MT_Ellipse( const MT_Ellipse& );
    virtual ~MT_Ellipse();

    MT_Ellipse& operator = ( const MT_Ellipse& );

    bool               IsInside ( MT_Vector2D ) const;
    const MT_Vector2D& GetCenter()              const;
    const MT_Vector2D  GetMajorAxisHighPoint()  const;
    const MT_Vector2D  GetMinorAxisHighPoint()  const;
    MT_Float           GetMajorAxeLength    ()  const;
    MT_Rect            GetBoundingBox() const;

public:
//protected:
    // informations brutes
    MT_Float rMajorAxeLength_;
    MT_Float rMinorAxeLength_;
    MT_Float rTheta_;

    // représentation sous la forme de l'équation d'une cônique ( centrée en 0 ) : A.x^2 + B.y^2 + C.x.y + F = 0
    MT_Float    rA_;
    MT_Float    rB_;
    MT_Float    rC_;
    MT_Float    rF_;

    // origine
    MT_Vector2D vCenter_;
};

#include "MT_Ellipse.inl"

#endif // __MT_Ellipse_h_