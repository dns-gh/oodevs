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

// -----------------------------------------------------------------------------
// Name: CampAttributes constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
CampAttributes::CampAttributes( const Resolver_ABC< Agent >& resolver )
    : resolver_( resolver )
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
// Name: CampAttributes::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
template< typename T >
void CampAttributes::DoUpdate( const T& message )
{
    if( message.m.attributs_specifiquesPresent 
     && message.attributs_specifiques.t == T_AttrObjectSpecific_camp_prisonniers )
        tc2_ = resolver_.Find( message.attributs_specifiques.u.camp_prisonniers->tc2 );
    if( message.m.attributs_specifiquesPresent 
     && message.attributs_specifiques.t == T_AttrObjectSpecific_camp_refugies )
        tc2_ = resolver_.Find( message.attributs_specifiques.u.camp_refugies->tc2 );
}

// -----------------------------------------------------------------------------
// Name: CampAttributes::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void CampAttributes::Update( const ASN1T_MsgObjectKnowledgeUpdate& message )
{
    DoUpdate( message );
}

// -----------------------------------------------------------------------------
// Name: CampAttributes::Update
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void CampAttributes::Update( const ASN1T_MsgObjectUpdate& message )
{
    DoUpdate( message );
}

// -----------------------------------------------------------------------------
// Name: CampAttributes::Update
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void CampAttributes::Update( const ASN1T_MsgObjectCreation& message )
{
    DoUpdate( message );
}
