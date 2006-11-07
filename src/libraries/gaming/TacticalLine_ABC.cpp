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
#include "TacticalLinePositions.h"
#include "clients_kernel/GlTools_ABC.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC( const QString& baseName, unsigned long id, Publisher_ABC& publisher )
    : publisher_    ( publisher )
    , id_           ( id )
    , nState_       ( eStateCreated )
    , nNetworkState_( eNetworkStateNotRegistered )
    , bCreatedBy    ( true )
{
    RegisterSelf( *this );
    strName_ = ( baseName + " %1" ).arg( id_ & 0x3FFFFF );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC( xml::xistream& xis, Publisher_ABC& publisher )
    : publisher_( publisher )
{
    RegisterSelf( *this );
    std::string name;
    int id;
    xis >> attribute( "name", name )
        >> attribute( "id", id );
    strName_ = name.c_str(); id_ = id;
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
// Name: TacticalLine_ABC::DoUpdate
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Polish()
{
    UpdateToSim();
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
    // $$$$ SBO 2006-11-06: visitor or something
    static_cast< TacticalLinePositions& >( Get< kernel::Positions >() ).WriteGeometry( line );
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
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const
{
//    if( ! pointList_.empty() ) // $$$$ SBO 2006-11-07: Get< kernel::Positions >().IsSet()
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
        glColor3f( 0.f, 0.f, 0.f );
        tools.Print( strName_.ascii(), Get< kernel::Positions >().GetPosition() );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Serialize
// Created: AGE 2006-09-06
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Serialize( xml::xostream& xos ) const
{
    xos << attribute( "name", std::string( strName_.ascii() ) )
        << attribute( "id", int( id_ ) );
}
