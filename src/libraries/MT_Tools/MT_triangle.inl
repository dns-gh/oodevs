//*****************************************************************************
//
// $Created: AGN 03-06-12 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_triangle.inl $
// $Author: Nld $
// $Modtime: 14/10/03 16:36 $
// $Revision: 7 $
// $Workfile: MT_triangle.inl $
//
//*****************************************************************************



//-----------------------------------------------------------------------------
//  Name  :  MT_Triangle::GetPos1
// Created: FBD 02-07-02
//-----------------------------------------------------------------------------
INLINE
const MT_Vector2D& MT_Triangle::GetPos1() const
{
    assert( pPos1_ );
    return *pPos1_;
}


//-----------------------------------------------------------------------------
//  Name  :  MT_Triangle::GetPos2
// Created: FBD 02-07-02
//-----------------------------------------------------------------------------
INLINE
const MT_Vector2D& MT_Triangle::GetPos2() const
{
    assert( pPos2_ );
    return *pPos2_;
}


//-----------------------------------------------------------------------------
//  Name  :  MT_Triangle::GetPos3
// Created: FBD 02-07-02
//-----------------------------------------------------------------------------
INLINE
const MT_Vector2D& MT_Triangle::GetPos3() const
{
    assert( pPos3_ );
    return *pPos3_;
}



//-----------------------------------------------------------------------------
//  Name  :  MT_Triangle::IsInside
// Created: FBD 02-09-11
//-----------------------------------------------------------------------------
INLINE
bool MT_Triangle::IsInside( const MT_Vector2D& vPos ) const
{

    const MT_Vector2D& vec1 = GetPos1();
    const MT_Vector2D& vec2 = GetPos2();
    const MT_Vector2D& vec3 = GetPos3();

    if ( (vec1.rX_ - vec2.rX_) * vPos.rY_ + (vec2.rY_ - vec1.rY_) * vPos.rX_ + (vec2.rX_ * vec1.rY_) - (vec1.rX_ * vec2.rY_) <= 0. )
    {
        if ( (vec2.rX_ - vec3.rX_) * vPos.rY_ + (vec3.rY_ - vec2.rY_) * vPos.rX_ + (vec3.rX_ * vec2.rY_) - (vec2.rX_ * vec3.rY_) <= 0. )
        {
            if ( (vec3.rX_ - vec1.rX_) * vPos.rY_ + ( vec1.rY_ - vec3.rY_ ) * vPos.rX_ + (vec1.rX_ * vec3.rY_) - (vec3.rX_ * vec1.rY_) <= 0. )
            {
                return true;
            }
        }
    }
    return false;
}


//-----------------------------------------------------------------------------
// Name: MT_Triangle::Intersect2D
// Created: NLD 2003-01-14
//-----------------------------------------------------------------------------
MT_INLINE
bool MT_Triangle::Intersect2D( const MT_Line& line ) const
{
    if( line.Intersect2D( MT_Line( *pPos1_, *pPos2_ ) ) )
        return true;

    if( line.Intersect2D( MT_Line( *pPos2_, *pPos3_ ) ) )
        return true;

    if( line.Intersect2D( MT_Line( *pPos3_, *pPos1_ ) ) )
        return true;
    
    if( IsInside( line.GetPosStart() ) )
    {
        assert( IsInside( line.GetPosEnd() ) );
        return true;
    }

    return false;
}

// -----------------------------------------------------------------------------
// Name: MT_Triangle::Intersect2D
// Created: NLD 2003-10-14
// -----------------------------------------------------------------------------
INLINE
bool MT_Triangle::Intersect2D( const MT_Line& line, T_PointSet& collisions ) const
{
    MT_Vector2D vIntersect;
    if( line.Intersect2D( MT_Line( *pPos1_, *pPos2_ ), vIntersect ) )
        collisions.insert( vIntersect );

    if( line.Intersect2D( MT_Line( *pPos2_, *pPos3_ ), vIntersect ) )
        collisions.insert( vIntersect );

    if( line.Intersect2D( MT_Line( *pPos3_, *pPos1_ ), vIntersect ) )
        collisions.insert( vIntersect );
    
    if( IsInside( line.GetPosStart() ) )
    {
        assert( IsInside( line.GetPosEnd() ) );
        return true;
    }

    return !collisions.empty();
}

// -----------------------------------------------------------------------------
// Name: MT_Triangle::Intersect2D
// Created: NLD 2003-10-14
// -----------------------------------------------------------------------------
INLINE
bool MT_Triangle::Intersect2D( const MT_Line& line, T_PointVector& collisions ) const
{
    MT_Vector2D vIntersect;
    if( line.Intersect2D( MT_Line( *pPos1_, *pPos2_ ), vIntersect ) )
        collisions.push_back( vIntersect );

    if( line.Intersect2D( MT_Line( *pPos2_, *pPos3_ ), vIntersect ) )
        collisions.push_back( vIntersect );

    if( line.Intersect2D( MT_Line( *pPos3_, *pPos1_ ), vIntersect ) )
        collisions.push_back( vIntersect );
    
    if( IsInside( line.GetPosStart() ) )
    {
        assert( IsInside( line.GetPosEnd() ) );
        return true;
    }

    return !collisions.empty();
}


//-----------------------------------------------------------------------------
// Name: MT_Triangle::GetCenter
// Created: FBD 02-12-09
//-----------------------------------------------------------------------------
INLINE
MT_Vector2D MT_Triangle::GetCenter() const
{
    return ( GetPos1() + GetPos2() + GetPos3() ) / 3.0;
}

