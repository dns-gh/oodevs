// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "NBCAttributes.h"
#include "Controller.h"
#include "Displayer_ABC.h"

// -----------------------------------------------------------------------------
// Name: NBCAttributes constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
NBCAttributes::NBCAttributes( Controller& controller, const Resolver_ABC< NBCAgent >& resolver )
    : controller_( controller )
    , resolver_( resolver )
    , nbc_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NBCAttributes destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
NBCAttributes::~NBCAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NBCAttributes::UpdateData
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
template< typename T >
void NBCAttributes::UpdateData( const T& message )
{
    if( message.m.attributs_specifiquesPresent )
    {
        if( message.attributs_specifiques.t == T_AttrObjectSpecific_nuage_nbc )
        {
            nbc_ = & resolver_.Get( message.attributs_specifiques.u.nuage_nbc->agent_nbc );
            controller_.Update( *this );
        }
        else if( message.attributs_specifiques.t == T_AttrObjectSpecific_zone_nbc )
        {
            nbc_ = & resolver_.Get( message.attributs_specifiques.u.zone_nbc->agent_nbc );
            controller_.Update( *this );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: NBCAttributes::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void NBCAttributes::DoUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: NBCAttributes::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void NBCAttributes::DoUpdate( const ASN1T_MsgObjectUpdate& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: NBCAttributes::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void NBCAttributes::DoUpdate( const ASN1T_MsgObjectCreation& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: NBCAttributes::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void NBCAttributes::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( "Nuage/Zone NBC" )
        .Display( "Agent_ABC NBC:", nbc_ );
}
