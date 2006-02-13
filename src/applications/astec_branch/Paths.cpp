// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Paths.h"
#include "App.h"
#include "World.h"

// -----------------------------------------------------------------------------
// Name: Paths constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Paths::Paths()
{

}

// -----------------------------------------------------------------------------
// Name: Paths destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Paths::~Paths()
{

}

// -----------------------------------------------------------------------------
// Name: Paths::Update
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Paths::Update( const ASN1T_MsgUnitAttributes& message )
{
    // $$$$ AGE 2006-02-13: Jamais vidé ??
    if( message.m.positionPresent )
    {
        MT_Vector2D vNewPos;
        App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)message.position.data, vNewPos );
        previousPath_.push_back( vNewPos );
    }
}   

// -----------------------------------------------------------------------------
// Name: Paths::Update
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Paths::Update( const ASN1T_MsgUnitPathFind& message )
{
    plannedPath_.clear(); plannedPath_.reserve( message.itineraire.vecteur_point.n );
    for( uint i = 0; i < message.itineraire.vecteur_point.n; ++i )
    {
        MT_Vector2D vPos;
        // $$$$ AGE 2006-02-13: 
        App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)message.itineraire.vecteur_point.elem[i].data, vPos );
        plannedPath_.push_back( vPos );
    }
}
