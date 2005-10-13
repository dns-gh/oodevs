//*****************************************************************************
//
// $Created: NLD 2003-07-22 $
// $Archive: /MVW_v10/Build/SDK/ter/src/TER_Localisation.inl $
// $Author: Age $
// $Modtime: 24/02/05 11:16 $
// $Revision: 1 $
// $Workfile: TER_Localisation.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: TER_Localisation::GetPoints
// Created: NLD 2003-07-24
//-----------------------------------------------------------------------------
const T_PointVector& TER_Localisation::GetPoints() const
{
    return pointVector_;
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation::GetType
// Created: NLD 2003-07-24
//-----------------------------------------------------------------------------
TER_Localisation::E_TypeLocalisation TER_Localisation::GetType() const
{
    return nType_;
}


// -----------------------------------------------------------------------------
// Name: TER_Localisation::operator==
// Created: NLD 2003-09-01
// -----------------------------------------------------------------------------
bool TER_Localisation::operator == ( const TER_Localisation& localisation ) const
{
    return localisation.nType_ == nType_ && localisation.pointVector_ == pointVector_;
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::ComputeBarycenter
// Created: NLD 2003-08-22
// -----------------------------------------------------------------------------
MT_Vector2D TER_Localisation::ComputeBarycenter() const
{
    return MT_ComputeBarycenter( pointVector_ );
}


//-----------------------------------------------------------------------------
// Name: TER_Localisation::GetBoundingBox
// Created: JVT 03-09-02
//-----------------------------------------------------------------------------
const MT_Rect& TER_Localisation::GetBoundingBox() const
{
    return boundingBox_;
}

//=============================================================================
// GEOMETRY
//=============================================================================

//-----------------------------------------------------------------------------
// Name: TER_Localisation::Intersect2DWithCircle
// Created: NLD 2003-07-23
//-----------------------------------------------------------------------------
bool TER_Localisation::Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, MT_Float rRadius ) const
{
    switch( nType_ )
    {
//        case ePoint:   return vCircleCenter.Distance( pointVector_[0] ) <= rRadius;
        case ePoint:   return polygon_ .Intersect2DWithCircle( vCircleCenter, rRadius );
        case ePolygon: return polygon_ .Intersect2DWithCircle( vCircleCenter, rRadius );
        case eLine:    return polyline_.Intersect2DWithCircle( vCircleCenter, rRadius );
        default:
            return false;
    }
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::Intersect2DWithCircle
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool TER_Localisation::Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, MT_Float rRadius, T_PointVector& shape ) const
{
    switch( nType_ )
    {
        case ePoint:   
        case ePolygon: 
            {
                assert( false ); 
                return false;
            }
        case eLine:    return polyline_.Intersect2DWithCircle( vCircleCenter, rRadius, shape );
        default:
            return false;
    }
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation::IsInside
// Created: NLD 2003-07-24
// Last modified: JVT 03-09-02
//-----------------------------------------------------------------------------
bool TER_Localisation::IsInside( const MT_Vector2D& vPos ) const
{
    switch( nType_ )
    {
//        case ePoint:   return vPos.SquareDistance( pointVector_[0] ) <= ( rPrecision_ * rPrecision_ );
        case ePoint:   return boundingBox_.IsInside( vPos, rPrecision_ ) && polygon_ .IsInside( vPos, rPrecision_ );
        case ePolygon: return boundingBox_.IsInside( vPos, rPrecision_ ) && polygon_ .IsInside( vPos, rPrecision_ );
        case eLine:    return boundingBox_.IsInside( vPos, rPrecision_ ) && polyline_.IsInside( vPos, rPrecision_ );
        default:
            return false;
    }
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::IsOnBorder
// Created: NLD 2003-10-03
// -----------------------------------------------------------------------------
bool TER_Localisation::IsOnBorder( const MT_Vector2D& vPos ) const
{
    switch( nType_ )
    {
//        case ePoint:   return vPos.SquareDistance( pointVector_[0] ) <= ( rPrecision_ * rPrecision_ );
        case ePoint:   return boundingBox_.IsInside( vPos, rPrecision_ ) && polygon_ .IsOnBorder( vPos, rPrecision_ );
        case ePolygon: return boundingBox_.IsInside( vPos, rPrecision_ ) && polygon_ .IsOnBorder( vPos, rPrecision_ );
        case eLine:    return boundingBox_.IsInside( vPos, rPrecision_ ) && polyline_.IsInside  ( vPos, rPrecision_ );
        default:
            return false;
    }
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation::Intersect2D
// Created: NLD 2003-07-24
//-----------------------------------------------------------------------------
bool TER_Localisation::Intersect2D( const MT_Line& orientedLine, T_PointSet& collisions ) const
{
    switch( nType_ )
    {
        /*case ePoint:   
            {
                if( orientedLine.IsInside( pointVector_[0], rPrecision_ ) )
                {
                    collisions.insert( pointVector_[0] );
                    return true;
                }
                return false;
            }                */
        case ePoint:   return polygon_ .Intersect2D( orientedLine, collisions, rPrecision_ );
        case ePolygon: return polygon_ .Intersect2D( orientedLine, collisions, rPrecision_ );
        case eLine:    return polyline_.Intersect2D( orientedLine, collisions, rPrecision_ );
        default:
            return false;
    }

}

//-----------------------------------------------------------------------------
// Name: TER_Localisation::Intersect2D
// Created: NLD 2003-07-24
//-----------------------------------------------------------------------------
bool TER_Localisation::Intersect2D( const MT_Line& line ) const
{
    switch( nType_ )
    {
//        case ePoint:   return line.IsInside( pointVector_[0], rPrecision_ );
        case ePoint:   return polygon_ .IntersectWithBorder( line, rPrecision_ );
        case ePolygon: return polygon_ .IntersectWithBorder( line, rPrecision_ );
        case eLine:    return polyline_.Intersect2D        ( line, rPrecision_ );
        default:
            return false;
    }
}


// -----------------------------------------------------------------------------
// Name: TER_Localisation::GetArea
// Created: NLD 2003-10-17
// -----------------------------------------------------------------------------
MT_Float TER_Localisation::GetArea() const
{
    switch( nType_ )
    {
        case ePoint:   return 1.;
        case ePolygon: return polygon_ .GetArea  (); 
        case eLine:    return polyline_.Magnitude();
        default:
            return false;
    }
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::GetLength
// Created: NLD 2003-10-17
// -----------------------------------------------------------------------------
MT_Float TER_Localisation::GetLength() const
{
    switch( nType_ )
    {
        case ePoint:   return 1.; // ???
        case ePolygon: return MT_Vector2D( boundingBox_.GetPointUpLeft() - boundingBox_.GetPointDownRight() ).Magnitude();
        case eLine:    return polyline_.Magnitude();
        default:
            return false;
    }
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::WasACircle
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
bool TER_Localisation::WasACircle() const
{
    return bWasCircle_;
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::GetCircleRadius
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
MT_Float TER_Localisation::GetCircleRadius() const
{
    return rCircleRadius_;
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::GetCircleCenter
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
const MT_Vector2D& TER_Localisation::GetCircleCenter() const
{
    return vCircleCenter_;
}
