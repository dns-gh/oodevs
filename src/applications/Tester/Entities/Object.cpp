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

#ifdef __GNUG__
#   pragma implementation
#endif

#include "Tester_pch.h"
#include "Object.h"
#include "Team.h"

using namespace TEST;

Object::T_ObjectMap   Object::objects_;

//-----------------------------------------------------------------------------
// Name: Object::Object
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
Object::Object( const ASN1T_MsgObjectCreation& asnMsg )
    : nId_               ( asnMsg.oid  )
    , nType_             ( asnMsg.type )
    , pTeam_             ( Team::Find( asnMsg.camp ) )
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
    for( Path::CIT_PositionVector it = positionVector_.begin(); it != positionVector_.end(); ++it )
        delete *it;
    positionVector_.clear();
}

//-----------------------------------------------------------------------------
// Name: Object::Initialize
// Created: SBO 2005-08-08
//-----------------------------------------------------------------------------
void Object::Initialize()
{
}

//-----------------------------------------------------------------------------
// Name: Object::Terminate
// Created: SBO 2005-08-08
//-----------------------------------------------------------------------------
void Object::Terminate()
{
    for( CIT_ObjectMap it = objects_.begin(); it != objects_.end(); ++it )
        delete it->second;
    objects_.clear();
}