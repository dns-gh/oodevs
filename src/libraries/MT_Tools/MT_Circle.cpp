//*****************************************************************************
//
// $Created: JDY 03-03-06 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Circle.cpp $
// $Author: Nld $
// $Modtime: 19/07/04 16:50 $
// $Revision: 4 $
// $Workfile: MT_Circle.cpp $
//
//*****************************************************************************

#include "MT_Tools_pch.h"
#include "MT_Circle.h"


//-----------------------------------------------------------------------------
// Name: MT_Circle constructor
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
MT_Circle::MT_Circle()
: c_()
, r_(0.0)
{
}


//-----------------------------------------------------------------------------
// Name: MT_Circle constructor
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
MT_Circle::MT_Circle(const MT_Vector2D& c,MT_Float r)
: c_(c)
, r_(r)
{
}


//-----------------------------------------------------------------------------
// Name: MT_Circle constructor
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
MT_Circle::MT_Circle(const MT_Vector2D& a,const MT_Vector2D& b)
: c_(a)
, r_(a.Distance(b))
{
}


//-----------------------------------------------------------------------------
// Name: MT_Circle constructor
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
MT_Circle::MT_Circle(MT_Float x,MT_Float y,MT_Float r)
: c_(MT_Vector2D(x,y))
,  r_(r)
{
}


//-----------------------------------------------------------------------------
// Name: MT_Circle constructor
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
MT_Circle::MT_Circle(const MT_Circle& c)
{
    c_=c.c_;
    r_=c.r_;
}

//-----------------------------------------------------------------------------
// Name: MT_Circle destructor
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
MT_Circle::~MT_Circle()
{
}


#define square(x) (x)*(x)

//-----------------------------------------------------------------------------
// Name: MT_Circle::Intersection
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
T_PointVector MT_Circle::Intersection( const MT_Line& line) const
{
    T_PointVector res;
    MT_Float a, b, c, mu, i ;
    MT_Vector2D p1=line.GetPosStart();
    MT_Vector2D p2=line.GetPosEnd();

    if( p1 == p2 )
    {
        if( Inside( p1 ) )
            res.push_back( p1 );
        return res;
    }

    a=  (p2-p1).SquareMagnitude();
    b=  2   *   DotProduct(p2-p1,p1-c_);
    c=  c_.SquareMagnitude() + p1.SquareMagnitude() - 2* DotProduct(c_,p1) - square(r_) ;
    i=  b * b - 4 * a * c ;

    assert( a != 0 );

    if ( i < 0.0 )
    {
        // no intersection
    }
    else if ( i == 0.0)
    {
        // one intersection
        mu = -b/(2*a) ;
        res.push_back(MT_Vector2D(p1.rX_+mu*(p2.rX_-p1.rX_),p1.rY_+mu*(p2.rY_-p1.rY_)));

    }
    else if ( i > 0.0 )
    {
        MT_Float  ts=sqrt( square(b) - 4*a*c );
        
        // first intersection
        mu = (-b + ts) / (2*a);
        res.push_back(MT_Vector2D(p1.rX_+mu*(p2.rX_-p1.rX_),p1.rY_+mu*(p2.rY_-p1.rY_)));
        
        // second intersection
        mu = (-b - ts) / (2*a);
        res.push_back(MT_Vector2D(p1.rX_+mu*(p2.rX_-p1.rX_),p1.rY_+mu*(p2.rY_-p1.rY_)));
    }
    return res;

}


//-----------------------------------------------------------------------------
// Name: MT_Circle::Intersection
// Created: JDY 03-03-06
//-----------------------------------------------------------------------------
T_PointVector MT_Circle::Intersection( const MT_Vector2D& p1,const MT_Vector2D& p2) const
{
    T_PointVector vect=Intersection(MT_Line(p1,p2));
    T_PointVector res;
    MT_Float      d=p2.SquareDistance(p1);
    for (CIT_PointVector cit=vect.begin();cit!=vect.end();cit++)
    {
        MT_Float rDist1 = p1.SquareDistance(*cit);
        MT_Float rDist2 = p2.SquareDistance(*cit);

        if( rDist1 <= d && rDist2 <= d)
            res.push_back( *cit );
        else if( rDist1 < rDist2 && Inside( p1 ) )
            res.push_back( p1 );
        else if( rDist2 <= rDist1 && Inside( p2 ) )
            res.push_back( p2 );
    }
    return res;
}

#undef square