// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-04-14 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/TacticalLine_ABC.cpp $
// $Author: Ape $
// $Modtime: 26/07/04 16:29 $
// $Revision: 4 $
// $Workfile: TacticalLine_ABC.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "TacticalLine_ABC.h"
#include "ASN_Messages.h"
#include "CoordinateConverter.h"

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC( const std::string& baseName, unsigned long id, const CoordinateConverter& converter )
    : converter_    ( converter )
    , id_           ( id )
    , nState_       ( eStateCreated )
    , nNetworkState_( eNetworkStateNotRegistered )
    , bCreatedBy    ( true )
{
    strName_ = ( QString( baseName.c_str() ) + " %1" ).arg( id_ & 0x3FFFFF );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC( const std::string& baseName, unsigned long id, const T_PointVector& points, const CoordinateConverter& converter )
    : converter_    ( converter )
    , id_           ( id )
    , nState_       ( eStateCreated )
    , nNetworkState_( eNetworkStateNotRegistered )
    , bCreatedBy    ( true )
    , pointList_    ( points )   
{
    strName_ = ( QString( baseName.c_str() ) + " %1" ).arg( id_ & 0x3FFFFF );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC( const std::string& baseName, unsigned long id, const ASN1T_Line& line, const CoordinateConverter& converter )
    : converter_    ( converter )
    , id_           ( id )
    , nState_       ( eStateOk )
    , nNetworkState_( eNetworkStateRegistered )
    , bCreatedBy    ( false )
{
    strName_ = ( QString( baseName.c_str() ) + " %1" ).arg( id_ & 0x3FFFFF );
    for( uint i = 0; i != line.vecteur_point.n ; ++i )
        pointList_.push_back( converter.ConvertToXY( line.vecteur_point.elem[i] ) );
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
// Name: TacticalLine_ABC::Update
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Update( const ASN1T_MsgLimitCreationAck& )
{
    nState_ = eStateOk;
    nNetworkState_ = eNetworkStateRegistered;
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Update
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Update( const ASN1T_MsgLimitUpdateAck& )
{
    nState_ = eStateOk;
    nNetworkState_ = eNetworkStateRegistered;
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Update
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Update( const ASN1T_MsgLimaCreationAck& )
{
    nState_ = eStateOk;
    nNetworkState_ = eNetworkStateRegistered;
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Update
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Update( const ASN1T_MsgLimaUpdateAck& )
{
    nState_ = eStateOk;
    nNetworkState_ = eNetworkStateRegistered;
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
std::string TacticalLine_ABC::GetName() const
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
// Name: TacticalLine_ABC::UpdateToSim
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TacticalLine_ABC::UpdateToSim()
{
    if ( App::GetApp().GetNetwork().IsConnected() )
        UpdateToSim( nState_ );
}
