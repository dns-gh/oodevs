// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "TerrainCreationPanel.h"
#include "moc_TerrainCreationPanel.cpp"
#include "InfoBubble.h"
#include "resources.h"
#include "ActionsContext.h"
#include "frontend/CreateTerrain.h"
#include "frontend/commands.h"
#include "tools/GeneralConfig.h"
#include <QtGui/qaction.h>
#include <Qt3Support/q3hbox.h>
#include <Qt3Support/q3action.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: TerrainCreationPanel constructor
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
TerrainCreationPanel::TerrainCreationPanel( Q3WidgetStack* widget, Q3Action& action, const tools::GeneralConfig& config, ActionsContext& context )
    : Panel_ABC( widget, action, context, "TerrainCreationPanel" )
    , config_  ( config )
{
    Q3VBox* box = new Q3VBox( this );
    box->setMargin( 10 );
    box->setSpacing( 10 );

    Q3GroupBox* group = new Q3GroupBox( 2, Qt::Horizontal, action.text(), box );
    new QLabel( tr( "New terrain name:" ), group );
    name_ = new QLineEdit( tr( "Enter terrain name" ), group );
    connect( name_, SIGNAL( textChanged( const QString& ) ), SLOT( NameChanged( const QString& ) ) );

    bubble_ = new InfoBubble( box );
    {
        Q3HBox* btnBox = new Q3HBox( box );
        btnBox->layout()->setAlignment( Qt::AlignRight );
        okay_ = new QPushButton( MAKE_PIXMAP( next ), tr( "Create" ), btnBox );
        QFont font( "Arial", 10, QFont::Bold );
        okay_->setFont( font );
    }
    if( !CreateTerrain::IsAvailable() )
    {
        group->hide();
        okay_->hide();
        bubble_->ShowError( tr( "Terrain generation tool was not found on your system." ) );
    }
    connect( okay_, SIGNAL( pressed() ), SLOT( CreateTerrain() ) );
    Update();
}

// -----------------------------------------------------------------------------
// Name: TerrainCreationPanel destructor
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
TerrainCreationPanel::~TerrainCreationPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TerrainCreationPanel::showEvent
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
void TerrainCreationPanel::showEvent( QShowEvent* event )
{
    name_->selectAll();
    name_->setFocus();
    Panel_ABC::showEvent( event );
}

// -----------------------------------------------------------------------------
// Name: TerrainCreationPanel::CreateTerrain
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
void TerrainCreationPanel::CreateTerrain()
{
    Start( new ::CreateTerrain( config_, name_->text() ));
    context_.Save( "terrain", name_->text() );
    Update();
    ShowNext();
}

// -----------------------------------------------------------------------------
// Name: TerrainCreationPanel::NameChanged
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
void TerrainCreationPanel::NameChanged( const QString& name )
{
    const bool exists = existingTerrains_.contains( name ) != 0;
    okay_->setDisabled( exists );
    if( exists )
        bubble_->ShowError( tr( "A terrain with this name already exists." ) );
    else
        bubble_->ShowInfo( tr( "The new terrain will be created in:\n%1" ).arg( config_.GetTerrainDir( name.ascii() ).c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: TerrainCreationPanel::Update
// Created: AGE 2007-10-16
// -----------------------------------------------------------------------------
void TerrainCreationPanel::Update()
{
    existingTerrains_ = commands::ListTerrains( config_ );
}
