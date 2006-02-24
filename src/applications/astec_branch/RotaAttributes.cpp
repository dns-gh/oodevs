// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "RotaAttributes.h"
#include "Controller.h"
#include "Displayer_ABC.h"

// -----------------------------------------------------------------------------
// Name: RotaAttributes constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
RotaAttributes::RotaAttributes( Controller& controller )
    : controller_( controller )
    , set_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RotaAttributes destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
RotaAttributes::~RotaAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RotaAttributes::UpdateData
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
template< typename T >
void RotaAttributes::UpdateData( const T& message )
{
    if( message.m.attributs_specifiquesPresent 
     && message.attributs_specifiques.t == T_AttrObjectSpecific_rota )
    {
        set_ = true;
        danger_ = message.attributs_specifiques.u.rota->niveau_danger;
        agents_.resize( message.attributs_specifiques.u.rota->agents_nbc.n );
        for( uint i = 0; i < message.attributs_specifiques.u.rota->agents_nbc.n; ++i )
            agents_[ i ] = message.attributs_specifiques.u.rota->agents_nbc.elem[ i ];
        controller_.Update( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: RotaAttributes::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void RotaAttributes::DoUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: RotaAttributes::Update
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void RotaAttributes::DoUpdate( const ASN1T_MsgObjectUpdate& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: RotaAttributes::Update
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void RotaAttributes::DoUpdate( const ASN1T_MsgObjectCreation& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: RotaAttributes::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void RotaAttributes::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( "ROTA" )
                .Display( "Danger:", danger_ )
                .Display( "Agents NBC:", agents_ ); // $$$$ AGE 2006-02-23: resolve
}
