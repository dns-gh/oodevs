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
#include "frontend/CreateTerrain.h"
#include "frontend/commands.h"
#include "tools/GeneralConfig.h"
#include <qaction.h>
#include <qlineedit.h>
#include <qpushbutton.h>

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: TerrainCreationPanel constructor
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
TerrainCreationPanel::TerrainCreationPanel( QWidgetStack* widget, QAction& action, const tools::GeneralConfig& config )
    : Panel_ABC        ( widget, action )
    , config_          ( config )
{
    QVBox* box = new QVBox( this );
    box->setMargin( 10 );
    box->setSpacing( 10 );

    QGroupBox* group = new QGroupBox( 2, Qt::Horizontal, action.text(), box );
    new QLabel( tr( "New terrain name:" ), group );
    name_ = new QLineEdit( tr( "Enter terrain name" ), group );
    connect( name_, SIGNAL( textChanged( const QString& ) ), SLOT( NameChanged( const QString& ) ) );

    bubble_ = new InfoBubble( box );
    {
        QHBox* btnBox = new QHBox( box );
        btnBox->layout()->setAlignment( Qt::AlignRight );
        okay_ = new QPushButton( MAKE_PIXMAP( next ), tr( "Create" ), btnBox );
        QFont font( "Arial", 10, QFont::Bold );
        okay_->setFont( font );
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
    new ::CreateTerrain( this, config_, name_->text() );
    Update();
    ShowNext();
}

// -----------------------------------------------------------------------------
// Name: TerrainCreationPanel::NameChanged
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
void TerrainCreationPanel::NameChanged( const QString& name )
{
    const bool exists = existingTerrains_.contains( name );
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
