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

// -----------------------------------------------------------------------------
// Name: RotaAttributes constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
RotaAttributes::RotaAttributes()
    : set_( false )
{

}

// -----------------------------------------------------------------------------
// Name: RotaAttributes destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
RotaAttributes::~RotaAttributes()
{

}

// -----------------------------------------------------------------------------
// Name: RotaAttributes::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
template< typename T >
void RotaAttributes::DoUpdate( const T& message )
{
    if( message.m.attributs_specifiquesPresent 
     && message.attributs_specifiques.t == T_AttrObjectSpecific_rota )
    {
        set_ = true;
        danger_ = message.attributs_specifiques.u.rota->niveau_danger;
        agents_.resize( message.attributs_specifiques.u.rota->agents_nbc.n );
        for( uint i = 0; i < message.attributs_specifiques.u.rota->agents_nbc.n; ++i )
            agents_[ i ] = message.attributs_specifiques.u.rota->agents_nbc.elem[ i ];
    }
}

// -----------------------------------------------------------------------------
// Name: RotaAttributes::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void RotaAttributes::Update( const ASN1T_MsgObjectKnowledgeUpdate& message )
{
    DoUpdate( message );
}

// -----------------------------------------------------------------------------
// Name: RotaAttributes::Update
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void RotaAttributes::Update( const ASN1T_MsgObjectUpdate& message )
{
    DoUpdate( message );
}

// -----------------------------------------------------------------------------
// Name: RotaAttributes::Update
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void RotaAttributes::Update( const ASN1T_MsgObjectCreation& message )
{
    DoUpdate( message );
}
