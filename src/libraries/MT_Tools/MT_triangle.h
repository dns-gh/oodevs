//*****************************************************************************
// Name   : MT_Triangle.h
// Created: FBD 02-07-01
//*****************************************************************************

#ifndef __MT_Triangle_h_
#define __MT_Triangle_h_

#include "MT_Tools_Types.h"

#include "MT_Vector2D.h"
#include "MT_Line.h"

//=============================================================================
// Created: FBD 02-07-01
//=============================================================================
class MT_Triangle
{
    MT_COPYNOTALLOWED( MT_Triangle )

    MT_BOOSTPOOLING_MEMBERS( MT_Triangle )

public:
    explicit MT_Triangle();
    MT_Triangle( const MT_Vector2D& vPos1, const MT_Vector2D& vPos2, const MT_Vector2D& vPos3 );
    ~MT_Triangle();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    const MT_Vector2D& GetPos1  () const;
	const MT_Vector2D& GetPos2  () const;
	const MT_Vector2D& GetPos3  () const;
    MT_Vector2D        GetCenter() const;
    MT_Float           GetArea  () const;
    //@}    
    
    //-------------------------------------------------------------------------
    /** @name Geo */
    //-------------------------------------------------------------------------
    //@{
    bool IsInside             ( const MT_Vector2D& vtxPoint ) const;
    bool Intersect2D          ( const MT_Line& line ) const;
    bool Intersect2D          ( const MT_Line& line, T_PointSet& collisions ) const;
    bool Intersect2D          ( const MT_Line& line, T_PointVector& collisions ) const;
    bool Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, MT_Float rRadius ) const;
    //@}

private:
	const MT_Vector2D* pPos1_;
	const MT_Vector2D* pPos2_;
	const MT_Vector2D* pPos3_;
};


typedef std::vector< MT_Triangle >           T_TriangleVector;
typedef const T_TriangleVector               CT_TriangleVector;
typedef T_TriangleVector::iterator           IT_TriangleVector;
typedef T_TriangleVector::const_iterator     CIT_TriangleVector;


typedef std::vector< MT_Triangle* >             T_TrianglePtrVector;
typedef const T_TrianglePtrVector               CT_TrianglePtrVector;
typedef T_TrianglePtrVector::iterator           IT_TrianglePtrVector;
typedef T_TrianglePtrVector::const_iterator     CIT_TrianglePtrVector;


#ifndef  _DEBUG
#   undef   INLINE                  
#   define  INLINE inline
#   include "MT_Triangle.inl"
#endif  // ! _DEBUG

#endif // __MT_Triangle_h_
