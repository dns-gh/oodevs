// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "TacticalLinePositions.h"
#include "TacticalLine_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions constructor
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
TacticalLinePositions::TacticalLinePositions( const T_PointVector& pointList, const kernel::CoordinateConverter_ABC& converter, const TacticalLine_ABC& owner )
    : converter_( converter )
    , owner_    ( owner )
    , pointList_( pointList )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions constructor
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
TacticalLinePositions::TacticalLinePositions( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter, const TacticalLine_ABC& owner )
    : converter_( converter )
    , owner_    ( owner )
{
    xis >> list( "point", *this, &TacticalLinePositions::ReadPoint );
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
// Name: TacticalLinePositions::ReadPoint
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void TacticalLinePositions::ReadPoint( xml::xistream& xis )
{
    std::string mgrs;
    xis >> mgrs;
    pointList_.push_back( converter_.ConvertToXY( mgrs ) );
    boundingBox_.Incorporate( pointList_.back() );
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
bool TacticalLinePositions::IsAt( const geometry::Point2f& point, float precision /*= 100.f*/ ) const
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
    return rectangle.IsInside( GetPosition() );
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
// Name: TacticalLinePositions::Draw
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
void TacticalLinePositions::Draw( const geometry::Point2f&, const geometry::Rectangle2f&, const kernel::GlTools_ABC& tools ) const
{
    // $$$$ SBO 2006-11-07: viewport
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
        glLineWidth( 5.f );
        tools.DrawLines( pointList_ );
        glLineWidth( 3.f );
        if( owner_.IsLimit() )
            glColor3f( 0.1f, 0.1f, 0.1f );
//            if( nLevel_ == eNatureLevel_ooo )
//            {
//                glLineWidth( 4.f );
//                tools.DrawLines( pointList_ );
//                glColor3f( 0.5f, 0.5f, 0.5f );
//                glLineWidth( 2.f );
//            }
        else
            glColor4f( 0.55f, 0.3f, 0.1f, 1.0f );
        tools.DrawLines( pointList_ );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::SerializeAttributes
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
void TacticalLinePositions::SerializeAttributes( xml::xostream& xos ) const
{
    for ( CIT_PointVector itPoint = pointList_.begin() ; itPoint != pointList_.end() ; ++itPoint )
        xos << start( "point" ) << converter_.ConvertToMgrs( *itPoint ) << end();
}
