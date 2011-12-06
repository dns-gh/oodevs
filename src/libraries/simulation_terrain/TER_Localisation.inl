// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

//-----------------------------------------------------------------------------
// Name: TER_Localisation::GetPoints
// Created: NLD 2003-07-24
//-----------------------------------------------------------------------------
inline
const T_PointVector& TER_Localisation::GetPoints() const
{
    return pointVector_;
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation::GetType
// Created: NLD 2003-07-24
//-----------------------------------------------------------------------------
inline
TER_Localisation::E_LocationType TER_Localisation::GetType() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::operator==
// Created: NLD 2003-09-01
// -----------------------------------------------------------------------------
inline
bool TER_Localisation::operator == ( const TER_Localisation& localisation ) const
{
    return localisation.nType_ == nType_ && localisation.pointVector_ == pointVector_;
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::operator!=
// Created: CMA 2011-10-13
// -----------------------------------------------------------------------------
inline
bool TER_Localisation::operator != ( const TER_Localisation& localisation ) const
{
    return ! ( *this == localisation );
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::ComputeBarycenter
// Created: NLD 2003-08-22
// -----------------------------------------------------------------------------
inline
MT_Vector2D TER_Localisation::ComputeBarycenter() const
{
    return MT_ComputeBarycenter( pointVector_ );
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation::GetBoundingBox
// Created: JVT 03-09-02
//-----------------------------------------------------------------------------
inline
const MT_Rect& TER_Localisation::GetBoundingBox() const
{
    return boundingBox_;
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation::Intersect2DWithCircle
// Created: NLD 2003-07-23
//-----------------------------------------------------------------------------
inline
bool TER_Localisation::Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius ) const
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
inline
bool TER_Localisation::Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius, T_PointVector& shape ) const
{
    switch( nType_ )
    {
        case ePoint:
        case ePolygon:
            {
                assert( false );
                return false;
            }
        case eLine: return polyline_.Intersect2DWithCircle( vCircleCenter, rRadius, shape );
        default:
            return false;
    }
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation::IsInside
// Created: NLD 2003-07-24
// Last modified: JVT 03-09-02
//-----------------------------------------------------------------------------
inline
bool TER_Localisation::IsInside( const MT_Vector2D& vPos, double rPrecision ) const
{
    switch( nType_ )
    {
//        case ePoint:   return vPos.SquareDistance( pointVector_[0] ) <= ( rPrecision_ * rPrecision_ );
        case ePoint:   return polygon_.IsNull() || ( boundingBox_.IsInside( vPos, rPrecision_ ) && polygon_ .IsInside( vPos, rPrecision ) );
        case ePolygon: return polygon_.IsNull() || ( boundingBox_.IsInside( vPos, rPrecision_ ) && polygon_ .IsInside( vPos, rPrecision ) );
        case eLine:    return boundingBox_.IsInside( vPos, rPrecision_ ) && polyline_.IsInside( vPos, rPrecision );
        default:
            return false;
    }
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::IsOnBorder
// Created: NLD 2003-10-03
// -----------------------------------------------------------------------------
inline
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
inline
bool TER_Localisation::Intersect2D( const MT_Line& orientedLine, T_PointSet& collisions, double rPrecision /*= rPrecision_*/ ) const
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
        case ePoint:   return polygon_ .Intersect2D( orientedLine, collisions, rPrecision );
        case ePolygon: return polygon_ .Intersect2D( orientedLine, collisions, rPrecision );
        case eLine:    return polyline_.Intersect2D( orientedLine, collisions, rPrecision );
        default:
            return false;
    }
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation::Intersect2D
// Created: NLD 2003-07-24
//-----------------------------------------------------------------------------
inline
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
inline
double TER_Localisation::GetArea() const
{
    switch( nType_ )
    {
        case ePoint:   return 1.f;
        case ePolygon: return polygon_ .GetArea  ();
        case eLine:    return polyline_.Magnitude();
        default:
            return 0.f;
    }
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::GetLength
// Created: NLD 2003-10-17
// -----------------------------------------------------------------------------
inline
double TER_Localisation::GetLength() const
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
inline
bool TER_Localisation::WasACircle() const
{
    return bWasCircle_;
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::GetCircleRadius
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
inline
double TER_Localisation::GetCircleRadius() const
{
    return rCircleRadius_;
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::GetCircleCenter
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& TER_Localisation::GetCircleCenter() const
{
    return vCircleCenter_;
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
template<class Archive>
void TER_Localisation::load( Archive& ar, const unsigned int )
{
    ar >> nType_
       >> pointVector_
       >> bWasCircle_
       >> vCircleCenter_
       >> rCircleRadius_;
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
template<class Archive>
void TER_Localisation::save( Archive& ar, const unsigned int ) const
{
    ar << nType_
       << pointVector_
       << bWasCircle_
       << vCircleCenter_
       << rCircleRadius_;
}
