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
TacticalLinePositions::TacticalLinePositions( const ASN1T_Line& asnMsg, const kernel::CoordinateConverter_ABC& converter, const TacticalLine_ABC& owner )
    : converter_( converter )
    , owner_    ( owner )
{
    pointList_.reserve( asnMsg.vecteur_point.n );
    for( uint i = 0; i != asnMsg.vecteur_point.n; ++i )
    {
        pointList_.push_back( converter.ConvertToXY( asnMsg.vecteur_point.elem[i] ) );
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
        xos << start( "point" )
                << attribute( "x", itPoint->X() )
                << attribute( "y", itPoint->Y() )
            << end();
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::WriteGeometry
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
void TacticalLinePositions::WriteGeometry( ASN1T_Line& line ) const
{
    line.type               = EnumTypeLocalisation::line;
    line.vecteur_point.n    = pointList_.size();
    line.vecteur_point.elem = new ASN1T_CoordUTM[ pointList_.size() ];

    unsigned int i = 0;
    for ( CIT_PointVector itPoint = pointList_.begin() ; itPoint != pointList_.end() ; ++itPoint )
    {
        const std::string strMGRS = converter_.ConvertToMgrs( *itPoint );
        line.vecteur_point.elem[i] = strMGRS.c_str();
        ++i;
    }
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
    pointList_.reserve( message.geometrie.vecteur_point.n );
    for( unsigned int i = 0; i < message.geometrie.vecteur_point.n; ++i )
        pointList_.push_back( converter_.ConvertToXY( message.geometrie.vecteur_point.elem[i] ) );
}
