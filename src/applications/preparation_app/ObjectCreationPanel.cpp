// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ObjectCreationPanel.h"
#include "moc_ObjectCreationPanel.cpp"
#include "ObjectPrototype.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Controllers.h"
#include "preparation/ObjectsModel.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel constructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectCreationPanel::ObjectCreationPanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, const StaticModel& model, ObjectsModel& objectsModel, ParametersLayer& layer, const GlTools_ABC& tools )
    : InfoPanel_ABC( parent, panel, tr( "Objets" ) )
    , controllers_( controllers )
    , tools_( tools )
    , created_( new ObjectPrototype( this, controllers, model, objectsModel, layer ) )
{
    QPushButton* pOkButton = new QPushButton( tr( "Créer" ), this );
    connect( pOkButton, SIGNAL( clicked() ), this, SLOT( Commit() ) );
    controllers_.Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel destructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectCreationPanel::~ObjectCreationPanel()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel::Commit
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
void ObjectCreationPanel::Commit()
{
    if( !created_->CheckValidity() )
        return;
    created_->Commit();
}

// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel::Draw
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void ObjectCreationPanel::Draw( const geometry::Rectangle2f& /*viewport*/ )
{
    created_->Draw( tools_ );
}

// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel::NotifyUpdated
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
void ObjectCreationPanel::NotifyUpdated( const ModelLoaded& )
{
    Show();
}