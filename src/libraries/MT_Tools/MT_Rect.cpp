//*****************************************************************************
//
// $Created: FBD 02-11-07 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Rect.cpp $
// $Author: Nld $
// $Modtime: 1/09/04 16:44 $
// $Revision: 18 $
// $Workfile: MT_Rect.cpp $
//
//*****************************************************************************

#include "MT_Tools_pch.h"

#include "MT_Rect.h"

#include "MT_Line.h"

#include "MT/MT_Archive/MT_OutputArchive_ABC.h"
#include "MT/MT_Archive/MT_InputArchive_ABC.h"

//MT_BOOSTPOOLING_FUNCTIONS( MT_Rect )

//-----------------------------------------------------------------------------
// Name: MT_Rect constructor
// Created: FBD 02-11-07
//-----------------------------------------------------------------------------
MT_Rect::MT_Rect( MT_Float rPosX1, MT_Float rPosY1, MT_Float rPosX2, MT_Float rPosY2 )
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
// Created: FBD 02-11-21
//-----------------------------------------------------------------------------
bool MT_Rect::Intersect2D( const MT_Triangle& triangle ) const
{
    MT_Line line1( triangle.GetPos1(), triangle.GetPos2() );
    MT_Line line2( triangle.GetPos2(), triangle.GetPos3() );
    MT_Line line3( triangle.GetPos3(), triangle.GetPos1() );

    return Intersect2D( line1 ) || Intersect2D( line2 ) || Intersect2D( line3 );
}

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
    if (   ( vPosSrc.rX_ > rPosX2_ && vPosDest.rX_ > rPosX2_ )
        || ( vPosSrc.rX_ < rPosX1_ && vPosDest.rX_ < rPosX1_ )
        || ( vPosSrc.rY_ < rPosY1_ && vPosDest.rY_ < rPosY1_ )
        || ( vPosSrc.rY_ > rPosY2_ && vPosDest.rY_ > rPosY2_ )
        )
    {
        return false;
    }

    if (   ( vPosSrc.rX_ >= rPosX1_ && vPosSrc.rX_ <= rPosX2_ && vPosSrc.rY_ >= rPosY1_ && vPosSrc.rY_ <= rPosY2_ )
        || ( vPosDest.rX_ >= rPosX1_ && vPosDest.rX_ <= rPosX2_ && vPosDest.rY_ >= rPosY1_ && vPosDest.rY_ <= rPosY2_ )
        )
    {
        return true;
    }
    MT_Line line( vPosSrc, vPosDest );
    if ( line.IsClipped( lineUp_ ) )
    {
        // Upper Line of Patch
	    if ( line.Intersect2D( lineUp_ ) )
	    {
		    return true;
	    }
    }

    if ( line.IsClipped( lineRight_ ) )
    {
        // Righter Line of Patch
	    if ( line.Intersect2D( lineRight_ ) )
	    {
		    return true;
	    }
    }
    
    // Lower Line of Patch
    if ( line.IsClipped( lineDown_ ) )
    {
	    if ( line.Intersect2D( lineDown_ ) )
	    {
		    return true;
	    }
    }

    // Lefter Line of Patch
    if ( line.IsClipped( lineLeft_ ) )
    {
	    if ( line.Intersect2D( lineLeft_ ) )
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
bool MT_Rect::Intersect2D( const MT_Line& orientedLine, T_PointSet& collisionSet, MT_Float rPrecision ) const
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
bool MT_Rect::Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, MT_Float rRadius ) const
{
    MT_Float rSquareRadius = rRadius * rRadius;

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



//=============================================================================
// IN/OUT
//=============================================================================

//-----------------------------------------------------------------------------
//  Name  :  MT_Rect::Write
// Created: FBD 02-09-05
//-----------------------------------------------------------------------------
//int MT_Rect::Write( MT_OutputArchive_ABC& archive ) const
//{
//    archive.WriteField( "PosX1", rPosX1_ );
//    archive.WriteField( "PosY1", rPosY1_ );
//    archive.WriteField( "PosX2", rPosX2_ );
//    archive.WriteField( "PosY2", rPosY2_ );
//    archive.WriteField( "Height", rHeight_ );
//    archive.WriteField( "Width", rWidth_ );
//
//    archive.Section( "PointUpLeft" );
//    pointUpLeft_   .Write( archive );
//    archive.EndSection();
//
//    archive.Section( "PointUpRight" );
//    pointUpRight_  .Write( archive );
//    archive.EndSection();
//
//    archive.Section( "PointDownRight" );
//    pointDownRight_.Write( archive );
//    archive.EndSection();
//
//    archive.Section( "PointDownLeft" );
//    pointDownLeft_ .Write( archive );
//    archive.EndSection();
//
//    archive.Section( "Center" );
//    vCenter_       .Write( archive );
//    archive.EndSection();
//    return 0;
//}
//
//-----------------------------------------------------------------------------
//  Name  :  MT_Rect::Read
// Created: FBD 02-09-05
//-----------------------------------------------------------------------------
//int MT_Rect::Read( MT_InputArchive_ABC& archive )
//{
//    archive.ReadField( "PosX1", rPosX1_ );
//    archive.ReadField( "PosY1", rPosY1_ );
//    archive.ReadField( "PosX2", rPosX2_ );
//    archive.ReadField( "PosY2", rPosY2_ );
//    archive.ReadField( "Height", rHeight_ );
//    archive.ReadField( "Width", rWidth_ );
//    archive.Section( "PointUpLeft" );
//    pointUpLeft_   .Read( archive );
//    archive.EndSection();
//
//    archive.Section( "PointUpRight" );
//    pointUpRight_  .Read( archive );
//    archive.EndSection();
//
//    archive.Section( "PointDownRight" );
//    pointDownRight_.Read( archive );
//    archive.EndSection();
//
//    archive.Section( "PointDownLeft" );
//    pointDownLeft_ .Read( archive );
//    archive.EndSection();
//
//    archive.Section( "Center" );
//    vCenter_       .Read( archive );
//    archive.EndSection();
//
//    lineUp_    = MT_Line( pointUpLeft_,   pointUpRight_   );
//    lineRight_ = MT_Line( pointUpRight_,  pointDownRight_ );
//    lineDown_  = MT_Line( pointDownLeft_, pointDownRight_ );
//    lineLeft_  = MT_Line( pointUpLeft_,   pointDownLeft_  );
//    return 0;
//}

