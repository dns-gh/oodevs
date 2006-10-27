// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "TacticalLine_Limit.h"

using namespace TEST;

IDManager TacticalLine_Limit::idManager_( 138 );

// -----------------------------------------------------------------------------
// Name: TacticalLine_Limit constructor
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
TacticalLine_Limit::TacticalLine_Limit( const T_PositionVector& points )
    : TacticalLine_ABC ( points )
{
    nId_ = idManager_.GetFreeIdentifier();
    bIsSyncWithSim_ = false;
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_Limit constructor
// Created: SBO 2005-08-24
// -----------------------------------------------------------------------------
TacticalLine_Limit::TacticalLine_Limit( XmlInputArchive& archive )
    : TacticalLine_ABC ()
{
    bIsSyncWithSim_ = false;
    archive.ReadAttribute( "id", nId_ );
    ReadPoints( archive );
    idManager_.LockIdentifier( nId_ );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_Limit constructor
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
TacticalLine_Limit::TacticalLine_Limit( const ASN1T_MsgLimitCreation& asnMsg )
    : TacticalLine_ABC ()
{
    nId_ = asnMsg.oid;
    idManager_.LockIdentifier( nId_ );

    assert( asnMsg.geometrie.type == EnumTypeLocalisation::line );
    for( uint i = 0; i != asnMsg.geometrie.vecteur_point.n ; ++i )
    {
        Position* pPos = new Position( std::string( (const char*)asnMsg.geometrie.vecteur_point.elem[i].data, 15 ) );
        points_.push_back( pPos );
    }
    bIsSyncWithSim_ = true;
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_Limit destructor
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
TacticalLine_Limit::~TacticalLine_Limit()
{
    idManager_.ReleaseIdentifier( nId_ );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_Limit::UpdateToSim
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void TacticalLine_Limit::UpdateToSim()
{
    if( bIsSyncWithSim_ )
        return;

    assert( !points_.empty() );

    MOS_ASN_MsgLimitCreation asnMsg;

    asnMsg.GetAsnMsg().oid                          = nId_;
    asnMsg.GetAsnMsg().level                        = EnumNatureLevel::ooo;
    asnMsg.GetAsnMsg().geometrie.type               = EnumTypeLocalisation::line;
    asnMsg.GetAsnMsg().geometrie.vecteur_point.n    = points_.size();
    asnMsg.GetAsnMsg().geometrie.vecteur_point.elem = new ASN1T_CoordUTM[ points_.size() ];

    uint i = 0;
    for ( CIT_PositionVector it = points_.begin() ; it != points_.end() ; ++it )
    {
        std::string strMGRS = ( *it )->GetMgrsCoordinate();
        asnMsg.GetAsnMsg().geometrie.vecteur_point.elem[i] = strMGRS.c_str();
        ++i;
    }

    asnMsg.Send( (unsigned int)this );
    bIsSyncWithSim_ = true;

    delete[] asnMsg.GetAsnMsg().geometrie.vecteur_point.elem;
}
