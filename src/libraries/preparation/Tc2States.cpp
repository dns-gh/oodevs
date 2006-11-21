// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Tc2States.h"
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: Tc2States constructor
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
Tc2States::Tc2States( kernel::Controller& controller, kernel::Entity_ABC& entity, const kernel::Resolver_ABC< kernel::DotationType, QString >& resolver, kernel::PropertiesDictionary& dico )
    : ::LogisticHierarchies< TC2, kernel::TC2Hierarchies >( controller, entity, resolver )
{
    CreateDictionary( dico, entity );
}

// -----------------------------------------------------------------------------
// Name: Tc2States destructor
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
Tc2States::~Tc2States()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Tc2States::CreateDictionary
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
void Tc2States::CreateDictionary( kernel::PropertiesDictionary& dico, kernel::Entity_ABC& owner )
{
    ::LogisticHierarchies< TC2, kernel::TC2Hierarchies >::CreateDictionary( dico, owner, tools::translate( "TC2States", "Logistic/TC2/Quotas" ) );
    dico.Register( *(const kernel::TC2Hierarchies*)this, tools::translate( "Tc2States", "Logistic/TC2/Superior" ), tc2_, *this, &Tc2States::SetSuperior );
}

// -----------------------------------------------------------------------------
// Name: Tc2States::SetSuperior
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
void Tc2States::SetSuperior( const TC2& automat )
{
    ::LogisticHierarchies< TC2, kernel::TC2Hierarchies >::SetSuperior( automat );
}
