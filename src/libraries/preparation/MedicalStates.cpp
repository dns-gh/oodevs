// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "MedicalStates.h"
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: MedicalStates constructor
// Created: SBO 2006-10-24
// -----------------------------------------------------------------------------
MedicalStates::MedicalStates( kernel::Controller& controller, kernel::Entity_ABC& entity, const kernel::Resolver_ABC< kernel::DotationType, QString >& resolver, kernel::PropertiesDictionary& dico )
    : LogisticHierarchies< MedicalSuperior, MedicalStates >( controller, entity, resolver )
{
    CreateDictionary( dico, entity );
}
    
// -----------------------------------------------------------------------------
// Name: MedicalStates destructor
// Created: SBO 2006-10-24
// -----------------------------------------------------------------------------
MedicalStates::~MedicalStates()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MedicalStates::CreateDictionary
// Created: SBO 2006-10-24
// -----------------------------------------------------------------------------
void MedicalStates::CreateDictionary( kernel::PropertiesDictionary& dico, kernel::Entity_ABC& owner )
{
    LogisticHierarchies< MedicalSuperior, MedicalStates >::CreateDictionary( dico, owner, tools::translate( "MedicalStates", "Logistic/Medical/Quotas" ) );
    dico.Register( *(const kernel::TacticalHierarchies*)this, tools::translate( "MedicalStates", "Logistic/Medical/Superior" ), tc2_, *this, &MedicalStates::SetSuperior );
}

// -----------------------------------------------------------------------------
// Name: MedicalStates::GetLinkType
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
QString MedicalStates::GetLinkType() const
{
    return "medical";
}

// -----------------------------------------------------------------------------
// Name: MedicalStates::SetSuperior
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
void MedicalStates::SetSuperior( const MedicalSuperior& automat )
{
    LogisticHierarchies< MedicalSuperior, MedicalStates >::SetSuperior( automat );
}
