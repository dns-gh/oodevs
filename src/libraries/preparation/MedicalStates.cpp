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
#include "Tools.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/AutomatType.h"

// -----------------------------------------------------------------------------
// Name: MedicalStates constructor
// Created: SBO 2006-10-24
// -----------------------------------------------------------------------------
MedicalStates::MedicalStates( kernel::Controller& controller, kernel::Entity_ABC& entity, kernel::PropertiesDictionary& dico )
    : ::LogisticHierarchies< MedicalSuperior, kernel::MedicalHierarchies >( controller, entity )
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
void MedicalStates::CreateDictionary( kernel::PropertiesDictionary& dico, kernel::Entity_ABC& )
{
    dico.Register( *(const kernel::MedicalHierarchies*)this, tools::translate( "MedicalStates", "Logistic/Medical/Superior" ), tc2_, *this, &MedicalStates::SetSuperior );
}

// -----------------------------------------------------------------------------
// Name: MedicalStates::SetSuperior
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
void MedicalStates::SetSuperior( const MedicalSuperior& automat )
{
    ::LogisticHierarchies< MedicalSuperior, kernel::MedicalHierarchies >::SetSuperior( automat );
}

// -----------------------------------------------------------------------------
// Name: MedicalStates::Draw
// Created: SBO 2007-03-27
// -----------------------------------------------------------------------------
void MedicalStates::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    const bool displayLinks   = tools.ShouldDisplay( "LogisticLinks" );
    const bool displayMissing = tools.ShouldDisplay( "MissingLogisticLinks" ) && viewport.IsHotpointVisible();
    if( ! displayLinks && ! displayMissing )
        return;

    const kernel::Automat_ABC& automat = static_cast< const kernel::Automat_ABC& >( GetEntity() );

    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
    glLineWidth( 3.f );
    glColor4f( COLOR_PINK );
    DrawLink( where, tools, 0.5f, displayLinks, displayMissing && automat.GetType().IsLogisticMedical() );
    glPopAttrib();
}
