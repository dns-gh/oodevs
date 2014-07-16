//*****************************************************************************
//
// $Created: JDY 03-03-06 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Circle.h $
// $Author: Nld $
// $Modtime: 19/07/04 16:49 $
// $Revision: 4 $
// $Workfile: MT_Circle.h $
//
//*****************************************************************************

#ifndef __MT_Circle_h_
#define __MT_Circle_h_

#include "MT_Vector2DTypes.h"

class MT_Line;

//*****************************************************************************
// Created: JDY 03-03-06
//*****************************************************************************
class MT_Circle
{
public:
    MT_Circle();
    MT_Circle(const MT_Vector2D& c,double r);
    MT_Circle(const MT_Vector2D& a,const MT_Vector2D& b);
    MT_Circle(double x,double y,double r);
    MT_Circle(const MT_Circle& c);
    virtual ~MT_Circle();

    MT_Vector2D Center() const;
    double    Radius() const;
    MT_Vector2D PointOnCircle(double alpha);
    bool        IsTrivial() const;
    bool        Inside(const MT_Vector2D& p) const;
    bool        Outside(const MT_Vector2D& p) const;
    bool        Contains(const MT_Vector2D& p) const;
    MT_Circle&  operator =( const MT_Circle& c);
    MT_Circle   operator -( const MT_Vector2D& v ) const;
    MT_Circle   operator +( const MT_Vector2D& v ) const;
    MT_Circle&  operator +=( const MT_Vector2D& v );
    MT_Circle&  operator -=( const MT_Vector2D& v );
    T_PointVector Intersection( const MT_Line& line) const;
    T_PointVector Intersection( const MT_Vector2D& p1,const MT_Vector2D& p2) const;
    double    IntersectionArea( const MT_Circle& c ) const;

private:
    MT_Vector2D c_;
    double    r_;
};

#endif // __MT_Circle_h_
