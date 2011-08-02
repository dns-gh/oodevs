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
#include "clients_kernel/Controllers.h"
#include "tools/GeneralConfig.h"

#pragma warning( disable : 4355 )

// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel constructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectCreationPanel::ObjectCreationPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, const StaticModel& model, ObjectsModel& objectsModel, const UrbanModel& urbanModel, gui::ParametersLayer& layer, const kernel::GlTools_ABC& tools, const tools::GeneralConfig& config )
    : InfoPanel_ABC( parent, panel, tr( "Objects" ), "ObjectCreationPanel" )
    , controllers_( controllers )
    , tools_      ( tools )
{
    Q3VBox* vbox = new Q3VBox( this );
    {
        created_ = new ObjectPrototype( vbox, controllers, model, objectsModel, urbanModel, layer, config );
        QPushButton* ok = new QPushButton( tr( "Create" ), vbox );
        connect( ok, SIGNAL( clicked() ), created_, SLOT( Commit() ) );
    }
    setWidget( vbox );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel destructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectCreationPanel::~ObjectCreationPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel::NotifyUpdated
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
void ObjectCreationPanel::NotifyUpdated( const kernel::ModelLoaded& )
{
    Show();
}

// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel::Draw
// Created: SBO 2009-05-29
// -----------------------------------------------------------------------------
void ObjectCreationPanel::Draw( kernel::Viewport_ABC& /*viewport*/ )
{
    created_->Draw( tools_ );
}
