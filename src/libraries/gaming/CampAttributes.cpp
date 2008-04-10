// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "CampAttributes.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: CampAttributes constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
CampAttributes::CampAttributes( Controller& controller, const Resolver_ABC< Automat_ABC >& resolver )
    : controller_( controller )
    , resolver_( resolver )
    , tc2_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CampAttributes destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
CampAttributes::~CampAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CampAttributes::UpdateData
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
template< typename T >
void CampAttributes::UpdateData( const T& message )
{
    if( message.m.specific_attributesPresent 
     && message.specific_attributes.t == T_ObjectAttributesSpecific_prisoner_camp )
    {
        tc2_ = resolver_.Find( message.specific_attributes.u.prisoner_camp->tc2 );
        controller_.Update( *(CampAttributes_ABC*)this );
    }
    if( message.m.specific_attributesPresent 
     && message.specific_attributes.t == T_ObjectAttributesSpecific_refugee_camp )
    {
        tc2_ = resolver_.Find( message.specific_attributes.u.refugee_camp->tc2 );
        controller_.Update( *(CampAttributes_ABC*)this );
    }
}

// -----------------------------------------------------------------------------
// Name: CampAttributes::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void CampAttributes::DoUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: CampAttributes::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void CampAttributes::DoUpdate( const ASN1T_MsgObjectUpdate& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: CampAttributes::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void CampAttributes::DoUpdate( const ASN1T_MsgObjectCreation& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: CampAttributes::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void CampAttributes::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Camp" ) )
                .Display( tools::translate( "Object", "TC2:" ), tc2_ );
}

// -----------------------------------------------------------------------------
// Name: CampAttributes::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void CampAttributes::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}
