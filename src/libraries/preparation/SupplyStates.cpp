// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "SupplyStates.h"
#include "clients_gui/Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SupplyStates constructor
// Created: SBO 2006-10-24
// -----------------------------------------------------------------------------
SupplyStates::SupplyStates( Controller& controller, Entity_ABC& entity, const Resolver_ABC< DotationType, QString >& resolver, PropertiesDictionary& dico )
    : LogisticHierarchies< SupplySuperior, SupplyStates >( controller, entity, resolver )
{
    CreateDictionary( dico, entity );
}
    
// -----------------------------------------------------------------------------
// Name: SupplyStates destructor
// Created: SBO 2006-10-24
// -----------------------------------------------------------------------------
SupplyStates::~SupplyStates()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyStates::CreateDictionary
// Created: SBO 2006-10-24
// -----------------------------------------------------------------------------
void SupplyStates::CreateDictionary( kernel::PropertiesDictionary& dico, kernel::Entity_ABC& owner )
{
    LogisticHierarchies< SupplySuperior, SupplyStates >::CreateDictionary( dico, owner, tools::translate( "SupplyStates", "Logistic/Supply/Quotas" ) );
    dico.Register( *(const kernel::TacticalHierarchies*)this, tools::translate( "SupplyStates", "Logistic/Supply/Superior" ), tc2_, *this, &SupplyStates::SetSuperior );
}

// -----------------------------------------------------------------------------
// Name: SupplyStates::GetLinkType
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
QString SupplyStates::GetLinkType() const
{
    return "supply";
}

// -----------------------------------------------------------------------------
// Name: SupplyStates::SetSuperior
// Created: SBO 2006-11-16
// -----------------------------------------------------------------------------
void SupplyStates::SetSuperior( const SupplySuperior& automat )
{
    LogisticHierarchies< SupplySuperior, SupplyStates >::SetSuperior( automat );
}
