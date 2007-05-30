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
#include "Tools.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/AutomatType.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MaintenanceStates constructor
// Created: SBO 2006-10-24
// -----------------------------------------------------------------------------
MaintenanceStates::MaintenanceStates( Controller& controller, Entity_ABC& entity, PropertiesDictionary& dico )
    : ::LogisticHierarchies< MaintenanceSuperior, kernel::MaintenanceHierarchies >( controller, entity )
{
    CreateDictionary( dico, entity );
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
void MaintenanceStates::CreateDictionary( kernel::PropertiesDictionary& dico, kernel::Entity_ABC& )
{
    dico.Register( *(const kernel::MaintenanceHierarchies*)this, tools::translate( "MaintenanceStates", "Logistic/Maintenance/Superior" ), tc2_, *this, &MaintenanceStates::SetSuperior );
}

// -----------------------------------------------------------------------------
// Name: MaintenanceStates::SetSuperior
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
void MaintenanceStates::SetSuperior( const MaintenanceSuperior& automat )
{
    ::LogisticHierarchies< MaintenanceSuperior, kernel::MaintenanceHierarchies >::SetSuperior( automat );
}

// -----------------------------------------------------------------------------
// Name: MaintenanceStates::Draw
// Created: SBO 2007-03-27
// -----------------------------------------------------------------------------
void MaintenanceStates::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    const bool displayLinks   = tools.ShouldDisplay( "LogisticLinks" );
    const bool displayMissing = tools.ShouldDisplay( "MissingLogisticLinks" ) && viewport.IsHotpointVisible();
    if( ! displayLinks && ! displayMissing )
        return;

    const kernel::Automat_ABC& automat = static_cast< const kernel::Automat_ABC& >( GetEntity() );

    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
    glLineWidth( 3.f );
    glColor4f( COLOR_MAROON );
    DrawLink( where, tools, 0.5f, displayLinks, displayMissing && automat.GetType().IsLogisticMaintenance() );
    glPopAttrib();
}
