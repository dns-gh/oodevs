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

#include "Mt_Vector2DTypes.h"

class MT_Line;
class MT_Droite;

//=============================================================================
// Created: NLD 2003-07-25
//=============================================================================
class MT_Polyline
{
public:
             MT_Polyline();
             MT_Polyline( const T_PointVector& points );
    virtual ~MT_Polyline();

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
    bool Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius ) const; // NB : return true if the circle is inside
    bool Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius, T_PointVector& shape ) const; // NB : return true if the circle is inside
    bool IsInside             ( const MT_Vector2D& vPos, double rPrecision ) const;
    bool Intersect2D          ( const MT_Line& orientedLine, T_PointSet& collisions, double rPrecision ) const;
    bool Intersect2D          ( const MT_Line& line, double rPrecision ) const;
    bool Intersect2D          ( const MT_Droite& droite, T_PointSet& collisions ) const;
    void Translate            ( const MT_Vector2D& );

    double    Magnitude () const;
    MT_Vector2D GetPointAt( double rDist ) const;
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

#endif // __MT_Polyline_h_
