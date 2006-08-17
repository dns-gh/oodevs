// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-08 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Object.h"
#include "Team.h"
#include "Workspace.h"

using namespace TEST;

//-----------------------------------------------------------------------------
// Name: Object::Object
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
Object::Object( const Workspace& workspace, const ASN1T_MsgObjectCreation& asnMsg )
    : nId_               ( asnMsg.oid  )
    , nType_             ( asnMsg.type )
    , pTeam_             ( workspace.GetEntityManager().FindTeam( asnMsg.camp ) )
    , nTypeLocalisation_ ( asnMsg.localisation.type  )
{
    assert( pTeam_ );

    for( uint i = 0; i < asnMsg.localisation.vecteur_point.n; ++i )
    {
        Position* pos = new Position( std::string( (const char*)asnMsg.localisation.vecteur_point.elem[i].data, 15 ) );
        positionVector_.push_back( pos );
    }
}

//-----------------------------------------------------------------------------
// Name: Object::~Object
// Created: SBO 2005-08-08
//-----------------------------------------------------------------------------
Object::~Object()
{
    for( CIT_PositionVector it = positionVector_.begin(); it != positionVector_.end(); ++it )
        delete *it;
    positionVector_.clear();
}
