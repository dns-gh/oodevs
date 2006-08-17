// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-09 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "TacticalLine_Lima.h"

#include "MT/MT_IO/MT_DirectoryBrowser.h"
#include "MT/MT_IO/MT_Dir.h"
#include "MT/MT_XmlTools/MT_XXmlInputArchive.h"

using namespace TEST;

IDManager TacticalLine_Lima::idManager_( 137 );

// -----------------------------------------------------------------------------
// Name: TacticalLine_Lima constructor
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
TacticalLine_Lima::TacticalLine_Lima( const T_PositionVector& points )
    : TacticalLine_ABC ( points )
    , eLimaType_       ( EnumTypeLima::ligne_debouche )
{
    nId_ = idManager_.GetFreeIdentifier();
    bIsSyncWithSim_  = false;
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_Lima constructor
// Created: SBO 2005-08-24
// -----------------------------------------------------------------------------
TacticalLine_Lima::TacticalLine_Lima( XmlInputArchive& archive )
    : TacticalLine_ABC ()
    , eLimaType_       ( EnumTypeLima::ligne_debouche )
{
    nId_ = idManager_.GetFreeIdentifier();
    bIsSyncWithSim_  = false;
    archive.ReadField( "Type", ( uint& )eLimaType_ );
    archive.BeginList( "Points" );
    while( archive.NextListElement() )
    {
        archive.Section( "Point" );
        double rX;
        double rY;
        archive.ReadField( "X", rX );
        archive.ReadField( "Y", rY );
        Position& pos = *new Position();
        pos.SetSimCoordinates( rX, rY );
        points_.push_back( &pos );
        archive.EndSection(); // Point
    }
    archive.EndList(); // Points
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_Lima constructor
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
TacticalLine_Lima::TacticalLine_Lima( const ASN1T_MsgLimaCreation& asnMsg )
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
    bIsSyncWithSim_  = true;
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_Lima destructor
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
TacticalLine_Lima::~TacticalLine_Lima()
{
    idManager_.ReleaseIdentifier( nId_ );
}


// -----------------------------------------------------------------------------
// Name: TacticalLine_Lima::UpdateToSim
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void TacticalLine_Lima::UpdateToSim()
{
    if( bIsSyncWithSim_ )
        return;

    assert( !points_.empty() );

    MOS_ASN_MsgLimaCreation asnMsg;

    asnMsg.GetAsnMsg().oid                          = nId_;
    asnMsg.GetAsnMsg().fonction                     = eLimaType_;
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

    asnMsg.Send( (T_NetContextId)this );
    bIsSyncWithSim_ = true;

    delete[] asnMsg.GetAsnMsg().geometrie.vecteur_point.elem;
}