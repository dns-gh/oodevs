// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "MaintenanceStates.h"
#include "clients_gui/Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MaintenanceStates constructor
// Created: SBO 2006-10-24
// -----------------------------------------------------------------------------
MaintenanceStates::MaintenanceStates( Controller& controller, Entity_ABC& entity, const Resolver_ABC< DotationType, QString >& resolver, PropertiesDictionary& dico )
    : LogisticHierarchies< MaintenanceSuperior >( controller, entity, resolver )
{
    CreateDictionary( dico );
}
    
// -----------------------------------------------------------------------------
// Name: MaintenanceStates destructor
// Created: SBO 2006-10-24
// -----------------------------------------------------------------------------
MaintenanceStates::~MaintenanceStates()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MaintenanceStates::CreateDictionary
// Created: SBO 2006-10-24
// -----------------------------------------------------------------------------
void MaintenanceStates::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    dico.Register( *(const kernel::TacticalHierarchies*)this, tools::translate( "MaintenanceStates", "Logistic/Maintenance/Superior" ), tc2_, *this, &MaintenanceStates::SetSuperior );
}

// -----------------------------------------------------------------------------
// Name: MaintenanceStates::GetLinkType
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
QString MaintenanceStates::GetLinkType() const
{
    return "maintenance";
}

// -----------------------------------------------------------------------------
// Name: MaintenanceStates::SetSuperior
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
void MaintenanceStates::SetSuperior( const MaintenanceSuperior& automat )
{
    LogisticHierarchies< MaintenanceSuperior >::SetSuperior< MaintenanceStates >( automat );
}
