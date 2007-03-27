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
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/AutomatType.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SupplyStates constructor
// Created: SBO 2006-10-24
// -----------------------------------------------------------------------------
SupplyStates::SupplyStates( Controller& controller, Entity_ABC& entity, const Resolver_ABC< DotationType, QString >& resolver, PropertiesDictionary& dico )
    : ::LogisticHierarchies< SupplySuperior, kernel::SupplyHierarchies >( controller, entity, resolver )
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
    ::LogisticHierarchies< SupplySuperior, kernel::SupplyHierarchies >::CreateDictionary( dico, owner, tools::translate( "SupplyStates", "Logistic/Supply/Quotas" ) );
    dico.Register( *(const kernel::SupplyHierarchies*)this, tools::translate( "SupplyStates", "Logistic/Supply/Superior" ), tc2_, *this, &SupplyStates::SetSuperior );
}


// -----------------------------------------------------------------------------
// Name: SupplyStates::SetSuperior
// Created: SBO 2006-11-16
// -----------------------------------------------------------------------------
void SupplyStates::SetSuperior( const SupplySuperior& automat )
{
    ::LogisticHierarchies< SupplySuperior, kernel::SupplyHierarchies >::SetSuperior( automat );
}

// -----------------------------------------------------------------------------
// Name: SupplyStates::Draw
// Created: SBO 2007-03-27
// -----------------------------------------------------------------------------
void SupplyStates::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    const bool displayLinks   = tools.ShouldDisplay( "LogisticLinks" );
    const bool displayMissing = tools.ShouldDisplay( "MissingLogisticLinks" ) && viewport.IsHotpointVisible();
    if( ! displayLinks && ! displayMissing )
        return;

    const kernel::Automat_ABC& automat = static_cast< const kernel::Automat_ABC& >( GetEntity() );

    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
    glLineWidth( 3.f );
    glColor4f( COLOR_ORANGE );
    DrawLink( where, tools, 0.5f, displayLinks, displayMissing && automat.GetType().IsLogisticSupply() );
    glPopAttrib();
}
