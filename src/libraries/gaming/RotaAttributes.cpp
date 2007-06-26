// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "RotaAttributes.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: RotaAttributes constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
RotaAttributes::RotaAttributes( Controller& controller, const Resolver_ABC< NBCAgent >& resolver )
    : controller_( controller )
    , resolver_( resolver )
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
    if( message.m.specific_attributesPresent
     && message.specific_attributes.t == T_ObjectAttributesSpecific_rota )
    {
        danger_ = message.specific_attributes.u.rota->danger_level;
        agents_.resize( message.specific_attributes.u.rota->nbc_agents.n );
        for( uint i = 0; i < message.specific_attributes.u.rota->nbc_agents.n; ++i )
            agents_[ i ] = & resolver_.Get( message.specific_attributes.u.rota->nbc_agents.elem[ i ] );
        controller_.Update( *(RotaAttributes_ABC*)this );
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
    displayer.Group( tools::translate( "NBC", "ROTA" ) )
                .Display( tools::translate( "NBC", "Danger level:" ), danger_ )
                .Display( tools::translate( "NBC", "NBC agent(s):" ), agents_ );
}

// -----------------------------------------------------------------------------
// Name: RotaAttributes::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void RotaAttributes::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}
