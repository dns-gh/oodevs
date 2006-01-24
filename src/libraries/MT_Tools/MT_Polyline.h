//*****************************************************************************
//
// $Created: NLD 2003-07-25 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/src/MT_Polyline.h $
// $Author: Jvt $
// $Modtime: 16/05/05 13:25 $
// $Revision: 6 $
// $Workfile: MT_Polyline.h $
//
//*****************************************************************************

#ifndef __MT_Polyline_h_
#define __MT_Polyline_h_

#include "MT_Tools_Types.h"

#include "MT_Vector2D.h"

class MT_Line;

//=============================================================================
// Created: NLD 2003-07-25
//=============================================================================
class MT_Polyline
{
    MT_BOOSTPOOLING_MEMBERS( MT_Polyline )

public:
     MT_Polyline();
     MT_Polyline( const T_PointVector& points );
    ~MT_Polyline();

    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    MT_Polyline& operator=( const T_PointVector& points );
    //@}

    //-------------------------------------------------------------------------
    /** @name Operations */
    //-------------------------------------------------------------------------
    //@{
    bool Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, MT_Float rRadius ) const; // NB : return true if the circle is inside
    bool Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, MT_Float rRadius, T_PointVector& shape ) const; // NB : return true if the circle is inside
    bool IsInside             ( const MT_Vector2D& vPos, MT_Float rPrecision ) const;
    bool Intersect2D          ( const MT_Line& orientedLine, T_PointSet& collisions, MT_Float rPrecision ) const;
    bool Intersect2D          ( const MT_Line& line, MT_Float rPrecision ) const;
    bool Intersect2D          ( const MT_Droite& droite, T_PointSet& collisions ) const;
    void Translate            ( const MT_Vector2D& );

    MT_Float    Magnitude () const;
    MT_Vector2D GetPointAt( MT_Float rDist ) const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    const T_PointVector& GetPoints() const;
    //@}



private:
    T_PointVector points_;

};

#include "MT_Polyline.inl"

#endif // __MT_Polyline_h_
