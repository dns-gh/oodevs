// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ObjectCreationPanel.h"
#include "moc_ObjectCreationPanel.cpp"
#include "ObjectPrototype.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Controllers.h"

#pragma warning( disable : 4355 )

// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel constructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectCreationPanel::ObjectCreationPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const kernel::Time_ABC& simulation, gui::ParametersLayer& layer, const kernel::GlTools_ABC& tools )
    : gui::InfoPanel_ABC( parent, panel, tools::translate( "ObjectCreationPanel", "Objects" ), "ObjectCreationPanel" )
    , controllers_( controllers )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation )
    , tools_( tools )
    , created_( new ObjectPrototype( this, controllers, staticModel, layer ) )
{
    QPushButton* ok = new QPushButton( tools::translate( "ObjectCreationPanel", "Create" ), this );
    connect( ok, SIGNAL( clicked() ), this, SLOT( Commit() ) );
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
// Name: ObjectCreationPanel::Commit
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
void ObjectCreationPanel::Commit()
{
    created_->Commit( actionsModel_, simulation_ );
}

// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel::Draw
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void ObjectCreationPanel::Draw( const kernel::Viewport_ABC& /*viewport*/ )
{
    created_->Draw( tools_ );
}

// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel::NotifyUpdated
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
void ObjectCreationPanel::NotifyUpdated( const kernel::ModelLoaded& )
{
    Show();
}
