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
    : ContentPage( pages, tools::translate( "CreateTerrainPage", "Create Terrain" ), previous, eButtonBack | eButtonStart )
    , config_( config )
    , controllers_( controllers )
    , progressPage_( new ProgressPage( pages, *this, tools::translate( "CreateTerrainPage", "Starting terrain creation" ) ) )
{
    const bool available = frontend::CreateTerrain::IsAvailable();

    Q3VBox* mainBox = new Q3VBox( this );
    mainBox->setBackgroundOrigin( QWidget::WindowOrigin );
    mainBox->setMargin( 5 );
    {
        Q3GroupBox* hbox = new Q3GroupBox( 1, Qt::Vertical, mainBox );
        hbox->setBackgroundOrigin( QWidget::WindowOrigin );
        QLabel* label = new QLabel( tools::translate( "CreateTerrainPage", "Create new terrain:" ), hbox );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        editName_ = new QLineEdit( tools::translate( "CreateTerrainPage", "Enter terrain name" ), hbox );
        connect( editName_, SIGNAL( textChanged( const QString& ) ), SLOT( EditNameChanged( const QString& ) ) );

        hbox->setEnabled( available );
        EnableButton( eButtonStart, available );

        label_ = new QLabel( mainBox );
        label_->setBackgroundOrigin( QWidget::WindowOrigin );
    }

    if( !available )
        label_->setText( tools::translate( "CreateTerrainPage", "Terrain generation tool was not found on your system." ) );

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
    if( name.isEmpty() || name == tools::translate( "CreateTerrainPage", "Enter terrain name" ) )
    {
        EnableButton( eButtonStart, false );
        label_->setText( "" );
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
            label_->setText( tools::translate( "CreateTerrainPage", "A terrain with this name already exists." ) );
        else
            label_->setText( tools::translate( "CreateTerrainPage", "The new terrain will be created in:\n%1" ).arg( config_.GetTerrainDir( name.ascii() ).c_str() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: CreateTerrainPage::Update
// Created: JSR 2010-06-11
// -----------------------------------------------------------------------------
void CreateTerrainPage::Update()
{
    existingTerrains_ = frontend::commands::ListTerrains( config_ );
}
