// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "TacticalLine_ABC.h"
#include "ASN_Messages.h"

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC()
    : nID_           ( MIL_NULL_LINE_ID )
    , nState_        ( eStateCreated )
    , nNetworkState_ ( eNetworkStateNotRegistered )
    , bCreatedBy     ( true )
    , nLevel_        ( eNatureLevel_None )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC destructor
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
TacticalLine_ABC::~TacticalLine_ABC()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::AddPoint
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
void TacticalLine_ABC::AddPoint( const MT_Vector2D& vPos )
{
    pointList_.push_back( vPos );
    nState_ = eStateModified;
}


// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::InsertPoint
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
void TacticalLine_ABC::InsertPoint( uint nIndex, const MT_Vector2D& vPos )
{
    assert( nIndex > 0 && nIndex < pointList_.size() );
    IT_PointVector it = pointList_.begin() + nIndex;
    pointList_.insert( it, vPos );
    nState_ = eStateModified;
}


// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::ModifyPoint
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
void TacticalLine_ABC::ModifyPoint( uint nIndex, const MT_Vector2D& vNewPos )
{
    assert( nIndex >= 0 && nIndex < pointList_.size() );
    pointList_[nIndex] = vNewPos;
    nState_ = eStateModified;
}


// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Translate
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Translate( const MT_Vector2D& vTranslation )
{
    for( IT_PointVector it = pointList_.begin(); it != pointList_.end(); ++it )
        (*it) += vTranslation;
    nState_ = eStateModified;
}


// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::DeletePoint
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
void TacticalLine_ABC::DeletePoint( uint nIndex )
{
    assert( nIndex > 0 && nIndex < pointList_.size() );
    IT_PointVector it = pointList_.begin() + nIndex;
    pointList_.erase( it );
    nState_ = eStateModified;
}


// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::PopPoint
// Created: APE 2004-04-16
// -----------------------------------------------------------------------------
void TacticalLine_ABC::PopPoint()
{
    if( ! pointList_.empty() )
    {
        pointList_.pop_back();
        nState_ = eStateModified;
    }
}


// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::DeleteAllPoints
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
void TacticalLine_ABC::DeleteAllPoints()
{
    pointList_.clear();
    nState_ = eStateModified;
}


// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Read
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Read( MT_InputArchive_ABC& archive )
{
    archive.BeginList( "Points" );
    while( archive.NextListElement() )
    {
        archive.Section( "Point" );
        MT_Vector2D point;
        archive.ReadField( "X", point.rX_ );
        archive.ReadField( "Y", point.rY_ );
        pointList_.push_back( point );
        archive.EndSection();
    }
    archive.EndList();
}


// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Write
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Write( MT_OutputArchive_ABC& archive ) const
{
    assert( pointList_.size() != 0 );

    archive.BeginList( "Points", pointList_.size() );
    for( CIT_PointVector it = pointList_.begin(); it != pointList_.end(); ++it )
    {
        archive.Section( "Point" );
        archive.WriteField( "X", it->rX_ );
        archive.WriteField( "Y", it->rY_ );
        archive.EndSection();
    }
    archive.EndList();
}