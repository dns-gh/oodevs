// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "ObjectCreationPanel.h"
#include "moc_ObjectCreationPanel.cpp"
#include "ObjectPrototype.h"
#include "astec_kernel/ObjectType.h"
#include "astec_kernel/IDManager.h"

// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel constructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectCreationPanel::ObjectCreationPanel( QWidget* parent, Controllers& controllers, Publisher_ABC& publisher, const StaticModel& model, ParametersLayer& layer, const GlTools_ABC& tools )
    : QVBox( parent )
    , publisher_( publisher )
    , tools_( tools )
    , created_( new ObjectPrototype( this, controllers, model, layer ) )
{
    QPushButton* pOkButton = new QPushButton( tr( "Créer" ), this );
    connect( pOkButton, SIGNAL( clicked() ), this, SLOT( Commit() ) );
}
    
// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel destructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectCreationPanel::~ObjectCreationPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel::Commit
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
void ObjectCreationPanel::Commit()
{
    if( !created_->CheckValidity() )
        return;
    created_->Commit( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel::Draw
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void ObjectCreationPanel::Draw( const geometry::Rectangle2f& /*viewport*/ )
{
    created_->Draw( tools_ );
}
