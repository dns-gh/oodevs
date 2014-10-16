// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
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
ObjectCreationPanel::ObjectCreationPanel( QWidget* parent,
                                          gui::PanelStack_ABC& panel,
                                          kernel::Controllers& controllers,
                                          actions::ActionsModel& actionsModel,
                                          const StaticModel& staticModel,
                                          const kernel::Time_ABC& simulation,
                                          const kernel::Team_ABC& noSideTeam,
                                          const std::shared_ptr< gui::ParametersLayer >& layer,
                                          const gui::GLView_ABC& tools,
                                          const tools::GeneralConfig& config )
    : gui::InfoPanel_ABC( parent, panel, tools::translate( "ObjectCreationPanel", "Objects" ), "ObjectCreationPanel" )
    , controllers_ ( controllers )
    , actionsModel_( actionsModel )
    , static_      ( staticModel )
    , simulation_  ( simulation )
    , tools_       ( tools )
{
    gui::SubObjectName subObject( "ObjectCreationPanel" );
    QWidget* box = new QWidget( this );
    QBoxLayout* layout = new QBoxLayout( box, QBoxLayout::TopToBottom, 0, 5 );
    layout->setMargin( 5 );
    layout->setAlignment( Qt::AlignTop );

    created_ = new ObjectPrototype( this, controllers, staticModel, simulation_, noSideTeam, layer, config );
    layout->addWidget( created_ );
    QPushButton* ok = new QPushButton( tools::translate( "ObjectCreationPanel", "Create" ), this );
    layout->addWidget( ok );
    connect( ok, SIGNAL( clicked() ), this, SLOT( Commit() ) );
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
// Name: ObjectCreationPanel::Commit
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
void ObjectCreationPanel::Commit()
{
    created_->SetCommitContext( actionsModel_, simulation_ );
    created_->Commit();
}

// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel::Draw
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void ObjectCreationPanel::Draw( const gui::Viewport_ABC& /*viewport*/ )
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
