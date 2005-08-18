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

#ifdef __GNUG__
#   pragma implementation
#endif

#include "Tester_pch.h"
#include "TacticalLine_Lima.h"

using namespace TEST;

IDManager TacticalLine_Lima::idManager_( 137 );

// -----------------------------------------------------------------------------
// Name: TacticalLine_Lima constructor
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
TacticalLine_Lima::TacticalLine_Lima( const T_PositionVector& points )
    : TacticalLine_ABC ( points )
{
    nId_ = idManager_.GetFreeIdentifier();
    bIsSyncWithSim_  = false;
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
    // NOTHING
}