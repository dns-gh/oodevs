// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "NBCAttributes.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "Tools.h"

using namespace kernel;

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
    if( message.m.specific_attributesPresent )
    {
        if( message.specific_attributes.t == T_ObjectAttributesSpecific_nbc_cloud )
        {
            nbc_ = & resolver_.Get( message.specific_attributes.u.nbc_cloud->nbc_agent );
            controller_.Update( *(NBCAttributes_ABC*)this );
        }
        else if( message.specific_attributes.t == T_ObjectAttributesSpecific_nbc_zone )
        {
            nbc_ = & resolver_.Get( message.specific_attributes.u.nbc_zone->nbc_agent );
            controller_.Update( *(NBCAttributes_ABC*)this );
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
    displayer.Group( tools::translate( "NBC", "NBC cloud/zone" ) )
        .Display( tools::translate( "NBC", "NBC agent:" ), nbc_ );
}

// -----------------------------------------------------------------------------
// Name: NBCAttributes::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void NBCAttributes::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}
