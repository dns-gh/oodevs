// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "CampAttributes.h"
#include "Controller.h"

// -----------------------------------------------------------------------------
// Name: CampAttributes constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
CampAttributes::CampAttributes( Controller& controller, const Resolver_ABC< Agent >& resolver )
    : controller_( controller )
    , resolver_( resolver )
    , tc2_( 0 )
{

}

// -----------------------------------------------------------------------------
// Name: CampAttributes destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
CampAttributes::~CampAttributes()
{

}

// -----------------------------------------------------------------------------
// Name: CampAttributes::UpdateData
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
template< typename T >
void CampAttributes::UpdateData( const T& message )
{
    if( message.m.attributs_specifiquesPresent 
     && message.attributs_specifiques.t == T_AttrObjectSpecific_camp_prisonniers )
    {
        tc2_ = resolver_.Find( message.attributs_specifiques.u.camp_prisonniers->tc2 );
        controller_.Update( *this );
    }
    if( message.m.attributs_specifiquesPresent 
     && message.attributs_specifiques.t == T_AttrObjectSpecific_camp_refugies )
    {
        tc2_ = resolver_.Find( message.attributs_specifiques.u.camp_refugies->tc2 );
        controller_.Update( *this );
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
