//*****************************************************************************
//
// $Created: AGN 03-06-12 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_triangle.cpp $
// $Author: Nld $
// $Modtime: 17/10/03 14:17 $
// $Revision: 5 $
// $Workfile: MT_triangle.cpp $
//
//*****************************************************************************

#include "MT_Tools_pch.h"
#include "MT_Triangle.h"

#ifdef  _DEBUG
#   undef   INLINE                  
#   define  INLINE
#   include "MT_Triangle.inl"
#endif  // _DEBUG

//MT_BOOSTPOOLING_FUNCTIONS( MT_Triangle )

//-----------------------------------------------------------------------------
//  Name  :  MT_Triangle constructor
// Created: FBD 02-07-01
//-----------------------------------------------------------------------------
MT_Triangle::MT_Triangle( const MT_Vector2D& vPos1, const MT_Vector2D& vPos2, const MT_Vector2D& vPos3 )
:    pPos1_( &vPos1 )
,    pPos2_( &vPos2 )
,    pPos3_( &vPos3 )
{
}

//-----------------------------------------------------------------------------
//  Name  :  MT_Triangle constructor
// Created: FBD 02-07-01
//-----------------------------------------------------------------------------
MT_Triangle::MT_Triangle()
:    pPos1_()
,    pPos2_()
,    pPos3_()
{
    
}


//-----------------------------------------------------------------------------
//  Name  :  MT_Triangle::~MT_Triangle
// Created: FBD 02-07-01
//-----------------------------------------------------------------------------
MT_Triangle::~MT_Triangle()
{

}


//-----------------------------------------------------------------------------
// Name: MT_Triangle::Intersect2DWithCircle
// Created: NLD 2003-07-25
//-----------------------------------------------------------------------------
bool MT_Triangle::Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, MT_Float rRadius ) const
{
    if( IsInside( vCircleCenter ) )
        return true;

    MT_Line line1( *pPos1_, *pPos2_ );
    MT_Line line2( *pPos2_, *pPos3_ );
    MT_Line line3( *pPos3_, *pPos1_ );

    if( line1.ClosestPointOnLine( vCircleCenter ).Distance( vCircleCenter ) <= rRadius )
        return true;

    if( line2.ClosestPointOnLine( vCircleCenter ).Distance( vCircleCenter ) <= rRadius )
        return true;

    if( line3.ClosestPointOnLine( vCircleCenter ).Distance( vCircleCenter ) <= rRadius )
        return true;

    return false;
}

// -----------------------------------------------------------------------------
// Name: MT_Triangle::GetArea
// Created: NLD 2003-10-17
// -----------------------------------------------------------------------------
MT_Float MT_Triangle::GetArea() const
{
    MT_Line lineAB( *pPos1_, *pPos2_ );

    MT_Vector2D vProjectionC = lineAB.ProjectPointOnLine( *pPos3_ );

    return ( lineAB.Magnitude() * ( vProjectionC - *pPos3_ ).Magnitude() ) / 2.;
}
