// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "CreateTerrainPage.h"
#include "moc_CreateTerrainPage.cpp"
#include "Config.h"
#include "ProcessDialogs.h"
#include "ProgressPage.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/Controllers.h"
#include "frontend/CreateTerrain.h"
#include "frontend/commands.h"
#include "frontend/ProcessWrapper.h"

// -----------------------------------------------------------------------------
// Name: CreateTerrainPage constructor
// Created: JSR 2010-06-11
// -----------------------------------------------------------------------------
CreateTerrainPage::CreateTerrainPage( Q3WidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const Config& config )
    : ContentPage( pages, previous, eButtonBack | eButtonStart )
    , config_      ( config )
    , controllers_ ( controllers )
    , progressPage_( new ProgressPage( pages, *this ) )
    , available_   ( frontend::CreateTerrain::IsAvailable() )
{
    Q3VBox* mainBox = new Q3VBox( this );
    mainBox->setMargin( 5 );
    {
        Q3GroupBox* hbox = new Q3GroupBox( 1, Qt::Vertical, mainBox );
        terrainLabel_ = new QLabel( hbox );
        editName_ = new QLineEdit( hbox );
        connect( editName_, SIGNAL( textChanged( const QString& ) ), SLOT( EditNameChanged( const QString& ) ) );
        hbox->setEnabled( available_ );
        EnableButton( eButtonStart, available_ );
        errorLabel_ = new QLabel( mainBox );
    }
    AddContent( mainBox );
    EnableButton( eButtonStart, false );
    Update();
}

// -----------------------------------------------------------------------------
// Name: CreateTerrainPage destructor
// Created: JSR 2010-06-11
// -----------------------------------------------------------------------------
CreateTerrainPage::~CreateTerrainPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CreateTerrainPage::OnLanguageChanged
// Created: ABR 2011-11-09
// -----------------------------------------------------------------------------
void CreateTerrainPage::OnLanguageChanged()
{
    SetTitle( tools::translate( "CreateTerrainPage", "Create Terrain" ) );
    progressPage_->SetTitle( tools::translate( "CreateTerrainPage", "Starting terrain creation" ) );
    terrainLabel_->setText( tools::translate( "CreateTerrainPage", "Create new terrain:" ) );
    editName_->setPlaceholderText( tools::translate( "CreateTerrainPage", "Enter terrain name" ) );
    if( !available_ )
        errorLabel_->setText( tools::translate( "CreateTerrainPage", "Terrain generation tool was not found on your system." ) );
    ContentPage::OnLanguageChanged();
}

// -----------------------------------------------------------------------------
// Name: CreateTerrainPage::OnStart
// Created: JSR 2010-06-11
// -----------------------------------------------------------------------------
void CreateTerrainPage::OnStart()
{
    if( !dialogs::KillRunningProcesses( this ) )
        return;

    boost::shared_ptr< frontend::SpawnCommand > command( new frontend::CreateTerrain( config_, editName_->text(), true ) );
    boost::shared_ptr< frontend::ProcessWrapper > process( new frontend::ProcessWrapper( *progressPage_, command ) );
    progressPage_->Attach( process );
    process->Start();
    progressPage_->show();

    Update();
}

// -----------------------------------------------------------------------------
// Name: CreateTerrainPage::EditNameChanged
// Created: JSR 2010-06-11
// -----------------------------------------------------------------------------
void CreateTerrainPage::EditNameChanged( const QString& name )
{
    if( name.isEmpty() )
    {
        EnableButton( eButtonStart, false );
        errorLabel_->setText( "" );
    }
    else
    {
        bool exists = false;
        for( QStringList::iterator it = existingTerrains_.begin(); it != existingTerrains_.end(); ++it )
            if( (*it).lower() == name.lower() )
            {
                exists = true;
                break;
            }

        EnableButton( eButtonStart, !exists );
        if( exists )
            errorLabel_->setText( tools::translate( "CreateTerrainPage", "A terrain with this name already exists." ) );
        else
            errorLabel_->setText( tools::translate( "CreateTerrainPage", "The new terrain will be created in:\n%1" ).arg( config_.GetTerrainDir( name.ascii() ).c_str() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: CreateTerrainPage::Update
// Created: JSR 2010-06-11
// -----------------------------------------------------------------------------
void CreateTerrainPage::Update()
{
    editName_->clear();
    existingTerrains_ = frontend::commands::ListTerrains( config_ );
}
