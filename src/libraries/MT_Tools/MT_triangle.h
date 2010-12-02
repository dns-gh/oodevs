//*****************************************************************************
// Name   : MT_Triangle.h
// Created: FBD 02-07-01
//*****************************************************************************

#ifndef __MT_Triangle_h_
#define __MT_Triangle_h_

#include "MT_Vector2DTypes.h"
#include "MT_Line.h"
#include <boost/noncopyable.hpp>

//=============================================================================
// Created: FBD 02-07-01
//=============================================================================
class MT_Triangle : private boost::noncopyable
{
public:
             MT_Triangle();
             MT_Triangle( const MT_Vector2D& vPos1, const MT_Vector2D& vPos2, const MT_Vector2D& vPos3 );
    virtual ~MT_Triangle();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    const MT_Vector2D& GetPos1  () const;
    const MT_Vector2D& GetPos2  () const;
    const MT_Vector2D& GetPos3  () const;
    MT_Vector2D        GetCenter() const;
    double             GetArea  () const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Geo */
    //-------------------------------------------------------------------------
    //@{
    bool IsInside             ( const MT_Vector2D& vtxPoint ) const;
    bool Intersect2D          ( const MT_Line& line ) const;
    bool Intersect2D          ( const MT_Line& line, T_PointSet& collisions ) const;
    bool Intersect2D          ( const MT_Line& line, T_PointVector& collisions ) const;
    bool Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius ) const;
    //@}

private:
    const MT_Vector2D* pPos1_;
    const MT_Vector2D* pPos2_;
    const MT_Vector2D* pPos3_;
};

#ifndef  _DEBUG
#   undef   INLINE
#   define  INLINE inline
#   include "MT_Triangle.inl"
#endif  // ! _DEBUG

#endif // __MT_Triangle_h_
