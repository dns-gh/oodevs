//*****************************************************************************
// Name   : MT_Line.h
// Created: FBD 02-02-27
//*****************************************************************************

#ifndef __MT_Line_h_
#define __MT_Line_h_

#include "MT_Tools_Types.h"

#include "MT_Vector2D.h"

typedef enum E_CollisionState
{
	eDontIntersect = 0,
	eCollinear,
	eDoIntersect
};

//=============================================================================
// Created: FBD 02-02-27
//=============================================================================
class MT_Line
{
//    MT_BOOSTPOOLING_MEMBERS( MT_Line )

public:
    explicit MT_Line();
    MT_Line( const MT_Vector2D&, const MT_Vector2D& );
    ~MT_Line();

	const MT_Vector2D&  GetPosStart() const;
	const MT_Vector2D&  GetPosEnd  () const;
    MT_Vector2D         GetCenter  () const;
    MT_Float            Magnitude  () const;

    bool                IsClipped  ( const MT_Line& ) const;
	E_CollisionState    Intersect2D( const MT_Line& , MT_Vector2D& ) const;
    E_CollisionState    Intersect2D( const MT_Line& ) const;
    bool                Intersect2D( const T_PointVector& polyline, T_PointSet& orderedIntersectionSet, MT_Float rPrecision ) const; // Return the intersections between the line and the polyline, ordered
    bool                Intersect2D( const T_PointVector& polyline, MT_Float rPrecision ) const;

    MT_Vector2D         ProjectPointOnLine( const MT_Vector2D& vPoint ) const;
    MT_Float            ProjectPointOnLine( const MT_Vector2D& vPoint, MT_Vector2D& vResult ) const;
	MT_Vector2D		    ClosestPointOnLine( const MT_Vector2D& ) const;
    bool                IsInside( const MT_Vector2D&, MT_Float ) const;
    bool                IsInside( const MT_Vector2D&, MT_Float, MT_Vector2D&  ) const;
    MT_Line&            operator = ( const MT_Line& rhs );

private:
	const MT_Vector2D* pPosStart_;
	const MT_Vector2D* pPosEnd_;
};

typedef std::vector< MT_Line* >             T_LinePtrVector;
typedef const T_LinePtrVector               CT_LinePtrVector;
typedef T_LinePtrVector::iterator           IT_LinePtrVector;
typedef T_LinePtrVector::const_iterator     CIT_LinePtrVector;

typedef std::vector< MT_Line >              T_LineVector;
typedef const T_LineVector               CT_LineVector;
typedef T_LineVector::iterator           IT_LineVector;
typedef T_LineVector::const_iterator     CIT_LineVector;


#ifndef  _DEBUG
#   undef   INLINE                  
#   define  INLINE inline
#   include "MT_Line.inl"
#endif  // ! _DEBUG

#endif // __MT_Line_h_
