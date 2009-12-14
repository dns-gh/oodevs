// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "TacticalLinePositions.h"
#include "TacticalLine_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/Viewport_ABC.h"

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions constructor
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
TacticalLinePositions::TacticalLinePositions( const T_PointVector& pointList, const kernel::CoordinateConverter_ABC& converter, const TacticalLine_ABC& owner )
    : converter_( converter )
    , owner_    ( owner )
    , pointList_( pointList )
{
    for( CIT_PointVector it = pointList.begin(); it != pointList.end(); ++it )
        boundingBox_.Incorporate( *it );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions constructor
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
TacticalLinePositions::TacticalLinePositions( const ASN1T_Line& asnMsg, const kernel::CoordinateConverter_ABC& converter, const TacticalLine_ABC& owner )
    : converter_( converter )
    , owner_    ( owner )
{
    pointList_.reserve( asnMsg.coordinates.n );
    for( uint i = 0; i != asnMsg.coordinates.n; ++i )
    {
        pointList_.push_back( converter.ConvertToXY( asnMsg.coordinates.elem[i] ) );
        boundingBox_.Incorporate( pointList_.back() );
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions destructor
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
TacticalLinePositions::~TacticalLinePositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::GetPosition
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
geometry::Point2f TacticalLinePositions::GetPosition() const
{
    return pointList_.front();
}
    
// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::GetHeight
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
float TacticalLinePositions::GetHeight() const
{
    return 0.f;
}
    
// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::IsAt
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
bool TacticalLinePositions::IsAt( const geometry::Point2f& point, float precision /*= 100.f*/, float /*adaptiveFactor*/ ) const
{
    precision*=precision;
    if( pointList_.empty() )
        return false;
    if( pointList_.size() == 1 )
        return pointList_.front().SquareDistance( point ) <= precision;

    CIT_PointVector previous = pointList_.begin();
    for( CIT_PointVector current = previous + 1; current != pointList_.end(); ++current )
    {
        const geometry::Segment2f segment( *previous, *current );
        if( segment.SquareDistance( point ) < precision )
            return true;
        previous = current;
    }
    return false;
}
    
// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::IsIn
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
bool TacticalLinePositions::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    return ! rectangle.Intersect( boundingBox_ ).IsEmpty();
}
    
// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::GetBoundingBox
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
geometry::Rectangle2f TacticalLinePositions::GetBoundingBox() const
{
    return boundingBox_;
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::Accept
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void TacticalLinePositions::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    visitor.VisitLines( pointList_ );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::Draw
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
void TacticalLinePositions::Draw( const geometry::Point2f&, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( pointList_.empty() || !viewport.IsVisible( boundingBox_ ) )
        return;
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
        glLineWidth( 5.f );
        tools.DrawLines( pointList_ );
        glLineWidth( 3.f );
        if( owner_.IsLimit() )
            glColor3f( 0.1f, 0.1f, 0.1f );
        else
            glColor3f( 0.55f, 0.3f, 0.1f );
        tools.DrawLines( pointList_ );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::DoUpdate
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void TacticalLinePositions::DoUpdate( const ASN1T_MsgLimaUpdate& message )
{
    Update( message.tactical_line );
}
    
// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::DoUpdate
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void TacticalLinePositions::DoUpdate( const ASN1T_MsgLimitUpdate& message )
{
    Update( message.tactical_line );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::Update
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLinePositions::Update( const ASN1T_TacticalLine& message )
{
    pointList_.clear();
    pointList_.reserve( message.geometry.coordinates.n );
    boundingBox_ = geometry::Rectangle2f();
    for( unsigned int i = 0; i < message.geometry.coordinates.n; ++i )
    {
        pointList_.push_back( converter_.ConvertToXY( message.geometry.coordinates.elem[i] ) );
        boundingBox_.Incorporate( pointList_.back() );
    }
}
