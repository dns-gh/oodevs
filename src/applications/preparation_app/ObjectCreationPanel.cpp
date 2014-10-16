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
#include "clients_gui/RichPushButton.h"

#pragma warning( disable : 4355 )

// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel constructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectCreationPanel::ObjectCreationPanel( QWidget* parent,
                                          gui::PanelStack_ABC& panel,
                                          kernel::Controllers& controllers,
                                          const StaticModel& model,
                                          ObjectsModel& objectsModel,
                                          const UrbanModel& urbanModel,
                                          const WeatherModel& weatherModel,
                                          const kernel::Team_ABC& noSideTeam,
                                          const std::shared_ptr< gui::ParametersLayer >& layer,
                                          const gui::GLView_ABC& tools,
                                          const tools::GeneralConfig& config )
    : InfoPanel_ABC( parent, panel, tr( "Objects" ), "ObjectCreationPanel" )
    , controllers_( controllers )
    , tools_      ( tools )
{
    gui::SubObjectName subObject( "ObjectCreationPanel" );
    this->setObjectName( "ObjectCreationPanel" );
    QWidget* box = new QWidget( this );
    QVBoxLayout* layout = new QVBoxLayout( box );
    layout->setMargin( 5 );
    layout->setAlignment( Qt::AlignTop );
    {
        created_ = new ObjectPrototype( "created", this, controllers, model, objectsModel, urbanModel, weatherModel, noSideTeam, layer, config );
        layout->addWidget( created_ );
        gui::RichPushButton* ok = new gui::RichPushButton( "ok", tr( "Create" ), this );
        layout->addWidget( created_ );
        layout->addWidget( ok );
        connect( ok, SIGNAL( clicked() ), created_, SLOT( Commit() ) );
    }
    layout->addStretch( 1 );
    setWidget( box );
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
void ObjectCreationPanel::Draw( gui::Viewport_ABC& /*viewport*/ )
{
    created_->Draw( tools_ );
}
