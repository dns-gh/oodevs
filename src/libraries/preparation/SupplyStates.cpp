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
    : ::LogisticHierarchies< SupplySuperior, kernel::SupplyHierarchies >( controller, entity )
    , controller_( controller )
    , resolver_( resolver )
    , item_( 0 )
{
    CreateDictionary( dico, entity );
}
    
// -----------------------------------------------------------------------------
// Name: SupplyStates destructor
// Created: SBO 2006-10-24
// -----------------------------------------------------------------------------
SupplyStates::~SupplyStates()
{
    Resolver< Dotation >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: SupplyStates::CreateDictionary
// Created: SBO 2006-10-24
// -----------------------------------------------------------------------------
void SupplyStates::CreateDictionary( kernel::PropertiesDictionary& dico, kernel::Entity_ABC& owner )
{
    const QString name = tools::translate( "SupplyStates", "Logistic/Supply/Quotas" );
    item_ = new DotationsItem( controller_, owner, dico, name, *this );
    dico.Register( *(const kernel::SupplyHierarchies*)this, name + tools::translate( "SupplyStates", "/<Dotations>" ), item_ );
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

// -----------------------------------------------------------------------------
// Name: SupplyStates::Load
// Created: SBO 2007-03-29
// -----------------------------------------------------------------------------
void SupplyStates::Load( xml::xistream& xis )
{
    xis >> xml::start( "quotas" )
            >> xml::list( "dotation", *this, &SupplyStates::ReadDotation )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: SupplyStates::ReadDotation
// Created: SBO 2007-03-29
// -----------------------------------------------------------------------------
void SupplyStates::ReadDotation( xml::xistream& xis )
{
    Dotation* dotation = new Dotation( xis, resolver_ );
    item_->AddDotation( *dotation );
    kernel::Resolver< Dotation >::Register( dotation->type_->GetId(), *dotation );
}

// -----------------------------------------------------------------------------
// Name: SupplyStates::SerializeQuotas
// Created: SBO 2007-03-29
// -----------------------------------------------------------------------------
void SupplyStates::SerializeQuotas( xml::xostream& xos ) const
{
    kernel::Iterator< const Dotation& > it = Resolver< Dotation >::CreateIterator();
    xos << xml::start( "quotas" );
    while( it.HasMoreElements() )
    {
        xos << xml::start( "dotation" );
        it.NextElement().SerializeAttributes( xos );
        xos << xml::end();
    }
    xos << xml::end();
}
