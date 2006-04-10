// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Life.h"
#include "GlTools_ABC.h"

// -----------------------------------------------------------------------------
// Name: Life constructor
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
Life::Life()
    : life_( 1.f )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Life destructor
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
Life::~Life()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Life::Draw
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void Life::Draw( const geometry::Point2f& where, const GlTools_ABC& tools ) const
{
    glPushAttrib( GL_CURRENT_BIT );
    glColor4f( 1, 1, 1, 0.4f );
    tools.DrawRectangle( where, life_ );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: Life::DoUpdate
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void Life::DoUpdate( const ASN1T_MsgUnitAttributes& message )
{
    if( message.m.etat_operationnel_brutPresent )
        life_ = message.etat_operationnel_brut * 0.01f;
}
