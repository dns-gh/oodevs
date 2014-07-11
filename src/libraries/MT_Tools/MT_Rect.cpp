// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MT_Rect.h"
#include <cassert>
#include <cmath>

//-----------------------------------------------------------------------------
// Name: MT_Rect constructor
// Created: FBD 02-11-07
//-----------------------------------------------------------------------------
MT_Rect::MT_Rect( double rPosX1, double rPosY1, double rPosX2, double rPosY2 )
    : rPosX1_        ( rPosX2 > rPosX1 ? rPosX1 : rPosX2 )
    , rPosY1_        ( rPosY2 > rPosY1 ? rPosY1 : rPosY2 )
    , rPosX2_        ( rPosX2 > rPosX1 ? rPosX2 : rPosX1 )
    , rPosY2_        ( rPosY2 > rPosY1 ? rPosY2 : rPosY1 )
    , rWidth_        ( rPosX2_ - rPosX1_ )
    , rHeight_       ( rPosY2_ - rPosY1_ )
    , pointUpLeft_   ( rPosX1_, rPosY1_ )
    , pointUpRight_  ( rPosX2_, rPosY1_ )
    , pointDownRight_( rPosX2_, rPosY2_ )
    , pointDownLeft_ ( rPosX1_, rPosY2_ )
    , lineUp_        ( pointUpLeft_, pointUpRight_ )
    , lineRight_     ( pointUpRight_, pointDownRight_ )
    , lineDown_      ( pointDownLeft_, pointDownRight_ )
    , lineLeft_      ( pointUpLeft_, pointDownLeft_ )
    , vCenter_       ( rPosX1_ + ( rPosX2_ - rPosX1_ ) / 2.0, rPosY1_ + ( rPosY2_ - rPosY1_ ) / 2.0 )
{
}


//-----------------------------------------------------------------------------
// Name: MT_Rect constructor
// Created: FBD 02-11-07
//-----------------------------------------------------------------------------
MT_Rect::MT_Rect( const MT_Rect& rhs )
    : rPosX1_        ( rhs.rPosX1_ )
    , rPosY1_        ( rhs.rPosY1_ )
    , rPosX2_        ( rhs.rPosX2_ )
    , rPosY2_        ( rhs.rPosY2_ )
    , rWidth_        ( rPosX2_ - rPosX1_ )
    , rHeight_       ( rPosY2_ - rPosY1_ )
    , pointUpLeft_   ( rPosX1_, rPosY1_ )
    , pointUpRight_  ( rPosX2_, rPosY1_ )
    , pointDownRight_( rPosX2_, rPosY2_ )
    , pointDownLeft_ ( rPosX1_, rPosY2_ )
    , lineUp_        ( pointUpLeft_, pointUpRight_ )
    , lineRight_     ( pointUpRight_, pointDownRight_ )
    , lineDown_      ( pointDownLeft_, pointDownRight_ )
    , lineLeft_      ( pointUpLeft_, pointDownLeft_ )
    , vCenter_       ( rPosX1_ + ( rPosX2_ - rPosX1_ ) / 2.0, rPosY1_ + ( rPosY2_ - rPosY1_ ) / 2.0 )
{

}


//-----------------------------------------------------------------------------
// Name: MT_Rect destructor
// Created: FBD 02-11-07
//-----------------------------------------------------------------------------
MT_Rect::~MT_Rect()
{

}


//-----------------------------------------------------------------------------
// Name: MT_Rect::Reset
// Created: JVT 03-09-03
//-----------------------------------------------------------------------------
void MT_Rect::Reset()
{
    *this = MT_Rect( 0., 0., 0., 0. );
}



//-----------------------------------------------------------------------------
// Name: MT_Rect::Set
// Created: JVT 03-09-02
//-----------------------------------------------------------------------------
void MT_Rect::Set( const MT_Vector2D& vLeftBottom, const MT_Vector2D& vRightTop )
{
    *this = MT_Rect( vLeftBottom.rX_, vLeftBottom.rY_, vRightTop.rX_, vRightTop.rY_ );
}



//=============================================================================
// INTERSECTION
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MT_Rect::Intersect2D
// Created: FBD 02-11-07
//-----------------------------------------------------------------------------
bool MT_Rect::Intersect2D( const MT_Line& line ) const
{
    const MT_Vector2D& vPosSrc  = line.GetPosStart();
    const MT_Vector2D& vPosDest = line.GetPosEnd();
    return Intersect2D( vPosSrc, vPosDest );
}

//-----------------------------------------------------------------------------
// Name: MT_Rect::Intersect2D
// Created: FBD 02-11-07
//-----------------------------------------------------------------------------
bool MT_Rect::Intersect2D( const MT_Vector2D& vPosSrc, const MT_Vector2D& vPosDest ) const
{
    if( ( vPosSrc.rX_ > rPosX2_ && vPosDest.rX_ > rPosX2_ )
        || ( vPosSrc.rX_ < rPosX1_ && vPosDest.rX_ < rPosX1_ )
        || ( vPosSrc.rY_ < rPosY1_ && vPosDest.rY_ < rPosY1_ )
        || ( vPosSrc.rY_ > rPosY2_ && vPosDest.rY_ > rPosY2_ )
        )
    {
        return false;
    }

    if( ( vPosSrc.rX_ >= rPosX1_ && vPosSrc.rX_ <= rPosX2_ && vPosSrc.rY_ >= rPosY1_ && vPosSrc.rY_ <= rPosY2_ )
        || ( vPosDest.rX_ >= rPosX1_ && vPosDest.rX_ <= rPosX2_ && vPosDest.rY_ >= rPosY1_ && vPosDest.rY_ <= rPosY2_ )
        )
    {
        return true;
    }
    MT_Line line( vPosSrc, vPosDest );
    if( line.IsClipped( lineUp_ ) )
    {
        // Upper Line of Patch
        if( line.Intersect2D( lineUp_ ) )
        {
            return true;
        }
    }

    if( line.IsClipped( lineRight_ ) )
    {
        // Righter Line of Patch
        if( line.Intersect2D( lineRight_ ) )
        {
            return true;
        }
    }

    // Lower Line of Patch
    if( line.IsClipped( lineDown_ ) )
    {
        if( line.Intersect2D( lineDown_ ) )
        {
            return true;
        }
    }

    // Lefter Line of Patch
    if( line.IsClipped( lineLeft_ ) )
    {
        if( line.Intersect2D( lineLeft_ ) )
        {
            return true;
        }
    }
    return false;
}



//-----------------------------------------------------------------------------
// Name: MT_Rect::Intersect2D
// Created: FBD 02-11-21
//-----------------------------------------------------------------------------
bool MT_Rect::Intersect2D( const MT_Line& orientedLine, T_PointSet& collisionSet, double rPrecision ) const
{
    //$$$$ TMP a refaire - NE MARCHE QUE POUR UN RECTANGLE 'DROIT'
    collisionSet.clear();

    //$$$ TMP
    if( orientedLine.GetPosStart() == orientedLine.GetPosEnd() )
        return false;

    MT_Vector2D vPosIntersect;

    bool bTestLineUp    = true;
    bool bTestLineDown  = true;
    bool bTestLineLeft  = true;
    bool bTestLineRight = true;

    // Edges
    if( orientedLine.IsInside( pointUpLeft_, rPrecision ) )
    {
        bTestLineUp   = false;
        bTestLineLeft = false;
        collisionSet.insert( pointUpLeft_ );
        assert( IsInside( pointUpLeft_ ) );
    }

    if( orientedLine.IsInside( pointDownLeft_, rPrecision ) )
    {
        bTestLineDown = false;
        bTestLineLeft = false;
        collisionSet.insert( pointDownLeft_ );
        assert( IsInside( pointDownLeft_ ) );
    }

    if( orientedLine.IsInside( pointUpRight_, rPrecision ) )
    {
        bTestLineUp   = false;
        bTestLineRight = false;
        collisionSet.insert( pointUpRight_ );
        assert( IsInside( pointUpRight_ ) );
    }

    if( orientedLine.IsInside( pointDownRight_, rPrecision ) )
    {
        bTestLineDown = false;
        bTestLineRight = false;
        collisionSet.insert( pointDownRight_ );
        assert( IsInside( pointDownRight_ ) );
    }


    // Lines
    if( bTestLineUp && orientedLine.Intersect2D( lineUp_, vPosIntersect ) == eDoIntersect )
    {
        vPosIntersect = lineUp_.ClosestPointOnLine( vPosIntersect );
        collisionSet.insert( vPosIntersect );
        assert( IsInside( vPosIntersect ) );
    }

    if( bTestLineDown && orientedLine.Intersect2D( lineDown_, vPosIntersect ) == eDoIntersect )
    {
        vPosIntersect = lineDown_.ClosestPointOnLine( vPosIntersect );
        collisionSet.insert( vPosIntersect );
        assert( IsInside( vPosIntersect ) );
    }

    if( bTestLineLeft && orientedLine.Intersect2D( lineLeft_, vPosIntersect ) == eDoIntersect )
    {
        vPosIntersect = lineLeft_.ClosestPointOnLine( vPosIntersect );
        collisionSet.insert( vPosIntersect );
        assert( IsInside( vPosIntersect ) );
    }

    if( bTestLineRight && orientedLine.Intersect2D( lineRight_, vPosIntersect ) == eDoIntersect )
    {
        vPosIntersect = lineRight_.ClosestPointOnLine( vPosIntersect );
        collisionSet.insert( vPosIntersect );
        assert( IsInside( vPosIntersect ) );
    }

    assert( collisionSet.size() <= 2 );

    /*if( collisionSet.empty() )
    {
        if( IsInside( orientedLine.GetPosStart(), rPrecision ) )
            collisionSet.insert( orientedLine.GetPosStart() );
        if( IsInside( orientedLine.GetPosEnd(), rPrecision ) )
            collisionSet.insert( orientedLine.GetPosEnd() );*/
        return !collisionSet.empty();
    /*}

    return true;*/
}

//-----------------------------------------------------------------------------
// Name: MT_Rect::Intersect2DWithCircle
// Created: NLD 2002-12-20
//-----------------------------------------------------------------------------
bool MT_Rect::Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius ) const
{
    double rSquareRadius = rRadius * rRadius;

    if( rHeight_ == 0 && rWidth_ == 0 )
        return vCircleCenter.SquareDistance( pointUpLeft_ ) <= rSquareRadius;

    MT_Vector2D vClosestPoint = lineUp_.ClosestPointOnLine( vCircleCenter );
    if( vCircleCenter.SquareDistance( vClosestPoint ) <= rSquareRadius )
        return true;

    vClosestPoint = lineDown_.ClosestPointOnLine( vCircleCenter );
    if( vCircleCenter.SquareDistance( vClosestPoint ) <= rSquareRadius )
        return true;

    vClosestPoint = lineLeft_.ClosestPointOnLine( vCircleCenter );
    if( vCircleCenter.SquareDistance( vClosestPoint ) <= rSquareRadius )
        return true;

    vClosestPoint = lineRight_.ClosestPointOnLine( vCircleCenter );
    if( vCircleCenter.SquareDistance( vClosestPoint ) <= rSquareRadius )
        return true;

    if( IsInside( vCircleCenter ) )
        return true;

    return false;
}

//=============================================================================
// TOOLS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MT_Rect::ClipPoint
// Created: NLD 2003-07-30
//-----------------------------------------------------------------------------
void MT_Rect::ClipPoint( MT_Vector2D& vPosToClip ) const
{
    if( vPosToClip.rX_ < rPosX1_ )
        vPosToClip.rX_ = rPosX1_;
    else if( vPosToClip.rX_ > rPosX2_ )
        vPosToClip.rX_ = rPosX2_;

    if( vPosToClip.rY_ < rPosY1_ )
        vPosToClip.rY_ = rPosY1_;
    else if( vPosToClip.rY_ > rPosY2_ )
        vPosToClip.rY_ = rPosY2_;
}

//-----------------------------------------------------------------------------
// Name: MT_Rect::GetPointUpLeft
// Created: NLD 2002-12-17
//-----------------------------------------------------------------------------
const MT_Vector2D& MT_Rect::GetPointUpLeft() const
{
    return pointUpLeft_;
}

//-----------------------------------------------------------------------------
// Name: MT_Rect::GetPointUpRight
// Created: NLD 2002-12-17
//-----------------------------------------------------------------------------
const MT_Vector2D& MT_Rect::GetPointUpRight() const
{
    return pointUpRight_;
}

//-----------------------------------------------------------------------------
// Name: MT_Rect::GetPointDwnRight
// Created: NLD 2002-12-17
//-----------------------------------------------------------------------------
const MT_Vector2D& MT_Rect::GetPointDownRight() const
{
    return pointDownRight_;
}

//-----------------------------------------------------------------------------
// Name: MT_Rect::GetPointDownLeft
// Created: NLD 2002-12-17
//-----------------------------------------------------------------------------
const MT_Vector2D& MT_Rect::GetPointDownLeft() const
{
    return pointDownLeft_;
}

//-----------------------------------------------------------------------------
//  Name  :  MT_Rect::GetTop
// Created: FBD 02-07-05
//-----------------------------------------------------------------------------
double MT_Rect::GetTop() const
{
    return rPosY2_;
}

//-----------------------------------------------------------------------------
//  Name  :  MT_Rect::GetBottom
// Created: FBD 02-07-05
//-----------------------------------------------------------------------------
double MT_Rect::GetBottom() const
{
    return rPosY1_;
}


//-----------------------------------------------------------------------------
//  Name  :  MT_Rect::GetLeft
// Created: FBD 02-07-05
//-----------------------------------------------------------------------------
double MT_Rect::GetLeft() const
{
    return rPosX1_;
}


//-----------------------------------------------------------------------------
//  Name  :  MT_Rect::GetRight
// Created: FBD 02-07-05
//-----------------------------------------------------------------------------
double MT_Rect::GetRight() const
{
    return rPosX2_;
}


//-----------------------------------------------------------------------------
// Name: MT_Rect::GetWidth
// Created: FBD 02-11-07
//-----------------------------------------------------------------------------
double MT_Rect::GetWidth() const
{
    return rWidth_;
}


//-----------------------------------------------------------------------------
// Name: MT_Rect::GetHeight
// Created: FBD 02-11-07
//-----------------------------------------------------------------------------
double MT_Rect::GetHeight() const
{
    return rHeight_;
}


//-----------------------------------------------------------------------------
// Name: MT_Rect::GetCenter
// Created: FBD 02-11-14
//-----------------------------------------------------------------------------
const MT_Vector2D& MT_Rect::GetCenter() const
{
    return vCenter_;
}

//-----------------------------------------------------------------------------
// Name: MT_Rect::operator=
// Created: FBD 02-11-07
//-----------------------------------------------------------------------------
MT_Rect& MT_Rect::operator=( const MT_Rect& rhs )
{
    rPosX1_         = rhs.rPosX1_;
    rPosY1_         = rhs.rPosY1_;
    rPosX2_         = rhs.rPosX2_;
    rPosY2_         = rhs.rPosY2_;
    rWidth_         = rPosX2_ - rPosX1_;
    rHeight_        = rPosY2_ - rPosY1_;
    vCenter_        = rhs.vCenter_;
    
    pointUpLeft_.rX_ = rPosX1_;
    pointUpLeft_.rY_ = rPosY1_;
    pointUpRight_.rX_ = rPosX2_;
    pointUpRight_.rY_ = rPosY1_;
    pointDownRight_.rX_ = rPosX2_;
    pointDownRight_.rY_ = rPosY2_;
    pointDownLeft_.rX_ = rPosX1_;
    pointDownLeft_.rY_ = rPosY2_;

    return *this;
}

//-----------------------------------------------------------------------------
// Name: MT_Rect::IsInside
// Created: FBD 02-11-07
//-----------------------------------------------------------------------------
bool MT_Rect::IsInside( const MT_Vector2D& point ) const
{
    return( point.rX_ >= rPosX1_ && point.rX_ <= rPosX2_ && point.rY_ >= rPosY1_ && point.rY_ <= rPosY2_  );
}

//-----------------------------------------------------------------------------
// Name: MT_Rect::IsInside
// Created: NLD 2003-01-27
//-----------------------------------------------------------------------------
bool MT_Rect::IsInside( const MT_Vector2D& point, double rPrecision ) const
{
    double rTmpX = point.rX_ - rPosX1_;
    if( rTmpX < 0 && fabs( rTmpX ) >= rPrecision )
        return false;

    rTmpX = rPosX2_ - point.rX_;
    if( rTmpX < 0 && fabs( rTmpX ) >= rPrecision )
        return false;

    double rTmpY = point.rY_ - rPosY1_;
    if( rTmpY < 0 && fabs( rTmpY ) >= rPrecision )
        return false;

    rTmpY = rPosY2_ - point.rY_;
    if( rTmpY < 0 && fabs( rTmpY ) >= rPrecision )
        return false;

    return true;
}

// -----------------------------------------------------------------------------
// Name: MT_Rect::Contains
// Created: BCI 2011-02-24
// -----------------------------------------------------------------------------
bool MT_Rect::Contains( const MT_Rect& rect ) const
{
    return IsInside( rect.GetPointUpLeft() ) && IsInside( rect.GetPointDownRight() );
}

//-----------------------------------------------------------------------------
// Name: MT_Rect::GetLineLeft
// Created: FBD 03-03-07
//-----------------------------------------------------------------------------
const MT_Line& MT_Rect::GetLineLeft() const
{
    return lineLeft_;
}


//-----------------------------------------------------------------------------
// Name: MT_Rect::GetLineRight
// Created: FBD 03-03-07
//-----------------------------------------------------------------------------
const MT_Line& MT_Rect::GetLineRight() const
{
    return lineRight_;
}


//-----------------------------------------------------------------------------
// Name: MT_Rect::GetLineUp
// Created: FBD 03-03-07
//-----------------------------------------------------------------------------
const MT_Line& MT_Rect::GetLineUp() const
{
    return lineUp_;
}


//-----------------------------------------------------------------------------
// Name: MT_Rect::GetLineDown
// Created: FBD 03-03-07
//-----------------------------------------------------------------------------
const MT_Line& MT_Rect::GetLineDown() const
{
    return lineDown_;
}


//-----------------------------------------------------------------------------
// Name: MT_Rect::Intersect2D
// Created: JDY 03-05-20
//-----------------------------------------------------------------------------
bool MT_Rect::Intersect2D( const MT_Rect& rect ) const
{
    return Intersect2D(MT_Line(rect.pointUpLeft_,rect.pointUpRight_) )
        || Intersect2D(MT_Line(rect.pointUpRight_,rect.pointDownRight_) )
        || Intersect2D(MT_Line(rect.pointDownRight_,rect.pointDownLeft_) )
        || Intersect2D(MT_Line(rect.pointDownLeft_,rect.pointUpLeft_) );
}
