// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "TacticalLine_ABC.h"
#include "ASN_Messages.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC( const QString& baseName, unsigned long id, const CoordinateConverter_ABC& converter, Publisher_ABC& publisher )
    : converter_    ( converter )
    , publisher_    ( publisher )
    , id_           ( id )
    , nState_       ( eStateCreated )
    , nNetworkState_( eNetworkStateNotRegistered )
    , bCreatedBy    ( true )
{
    strName_ = ( baseName + " %1" ).arg( id_ & 0x3FFFFF );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC( const QString& baseName, unsigned long id, const T_PointVector& points, const CoordinateConverter_ABC& converter, Publisher_ABC& publisher )
    : converter_    ( converter )
    , publisher_    ( publisher )
    , id_           ( id )
    , nState_       ( eStateCreated )
    , nNetworkState_( eNetworkStateNotRegistered )
    , bCreatedBy    ( true )
    , pointList_    ( points )   
{
    strName_ = ( baseName + " %1" ).arg( id_ & 0x3FFFFF );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC( const QString& baseName, unsigned long id, const ASN1T_Line& line, const CoordinateConverter_ABC& converter, Publisher_ABC& publisher )
    : converter_    ( converter )
    , publisher_    ( publisher )
    , id_           ( id )
    , nState_       ( eStateOk )
    , nNetworkState_( eNetworkStateRegistered )
    , bCreatedBy    ( false )
{
    strName_ = ( baseName + " %1" ).arg( id_ & 0x3FFFFF );
    for( uint i = 0; i != line.vecteur_point.n ; ++i )
        pointList_.push_back( converter.ConvertToXY( line.vecteur_point.elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter, Publisher_ABC& publisher )
    : converter_( converter )
    , publisher_( publisher )
{
    std::string name;
    int id;
    xis >> attribute( "name", name )
        >> attribute( "id", id );
    strName_ = name.c_str(); id_ = id;
    xis >> list( "point", *this, &TacticalLine_ABC::ReadPoint );
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
// Name: TacticalLine_ABC::Delete
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Delete()
{
    if( nNetworkState_ == eNetworkStateRegistered )
    {
        nState_ = eStateDeleted;
        UpdateToSim();
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::ValidateAcknowledge
// Created: AGE 2006-07-05
// -----------------------------------------------------------------------------
template< typename Ack >
void TacticalLine_ABC::ValidateAcknowledge( const Ack& ack )
{
    if( ack.error_code == EnumInfoContextErrorCode::no_error )
    {
        nState_ = eStateOk;
        nNetworkState_ = eNetworkStateRegistered;
    }
    else
        ; // $$$$ AGE 2006-07-05: else what ? log ?
}   

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Update
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Update( const ASN1T_MsgLimitCreationAck& ack )
{
    ValidateAcknowledge( ack );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Update
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Update( const ASN1T_MsgLimitUpdateAck& ack )
{
    ValidateAcknowledge( ack );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Update
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Update( const ASN1T_MsgLimaCreationAck& ack )
{
    ValidateAcknowledge( ack );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Update
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Update( const ASN1T_MsgLimaUpdateAck& ack )
{
    ValidateAcknowledge( ack );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::GetId
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
unsigned long TacticalLine_ABC::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::GetName
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
QString TacticalLine_ABC::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::WriteGeometry
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TacticalLine_ABC::WriteGeometry( ASN1T_Line& line )
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
// Name: TacticalLine_ABC::SerializeGeometry
// Created: AGE 2006-09-06
// -----------------------------------------------------------------------------
void TacticalLine_ABC::SerializeGeometry( xml::xostream& xos ) const
{
    xos << attribute( "name", std::string( strName_.ascii() ) )
        << attribute( "id", int( id_ ) );
    for ( CIT_PointVector itPoint = pointList_.begin() ; itPoint != pointList_.end() ; ++itPoint )
        xos << start( "point" )
                << attribute( "x", itPoint->X() )
                << attribute( "y", itPoint->Y() )
            << end();
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::ReadPoint
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void TacticalLine_ABC::ReadPoint( xml::xistream& xis )
{
    float x, y;
    xis >> attribute( "x", x )
        >> attribute( "y", y );
    pointList_.push_back( geometry::Point2f( x, y ) );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::UpdateToSim
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TacticalLine_ABC::UpdateToSim()
{
    UpdateToSim( nState_ );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Draw
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Draw( const GlTools_ABC& tools ) const
{
    tools.DrawLines( pointList_ );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::DrawName
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void TacticalLine_ABC::DrawName( const GlTools_ABC& tools ) const
{
    if( ! pointList_.empty() )
        tools.Print( strName_.ascii(), pointList_.front() );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::IsAt
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
bool TacticalLine_ABC::IsAt( const geometry::Point2f& point, float precision /*= 100.f*/ ) const
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
