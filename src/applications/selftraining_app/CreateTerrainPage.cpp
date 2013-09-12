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
#include "clients_kernel/Tools.h"
#include "clients_kernel/Controllers.h"
#include "frontend/CreateTerrain.h"
#include "frontend/commands.h"
#include "frontend/ProcessWrapper.h"

// -----------------------------------------------------------------------------
// Name: CreateTerrainPage constructor
// Created: JSR 2010-06-11
// -----------------------------------------------------------------------------
CreateTerrainPage::CreateTerrainPage( Application& app, QStackedWidget* pages, Page_ABC& previous, kernel::Controllers& controllers, const Config& config )
    : ContentPage( pages, previous, eButtonBack | eButtonStart )
    , config_      ( config )
    , controllers_ ( controllers )
    , progressPage_( new ProgressPage( app, pages, *this ) )
    , available_   ( frontend::IsTerrainAvailable() )
{
    setWindowTitle( "CreateTerrainPage" );

    terrainLabel_ = new QLabel();
    editName_ = new QLineEdit();
    connect( editName_, SIGNAL( textChanged( const QString& ) ), SLOT( EditNameChanged( const QString& ) ) );

    QGroupBox* hbox = new QGroupBox();
    QHBoxLayout* hboxLayout = new QHBoxLayout( hbox );
    hboxLayout->addWidget( terrainLabel_ );
    hboxLayout->addWidget( editName_ );
    hboxLayout->setSizeConstraint( QLayout::SetMinAndMaxSize );
    hboxLayout->setEnabled( available_ );
    EnableButton( eButtonStart, available_ );

    errorLabel_ = new QLabel();

    QWidget* mainBox = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout( mainBox );
    mainLayout->addWidget( hbox );
    mainLayout->addWidget( errorLabel_ );
    mainLayout->setMargin( 5 );

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
    auto process = frontend::CreateTerrain( *progressPage_, config_, tools::Path::FromUnicode( editName_->text().toStdWString() ) );
    progressPage_->Attach( process );
    frontend::ProcessWrapper::Start( process );
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
        for( auto it = existingTerrains_.begin(); it != existingTerrains_.end(); ++it )
        {
            std::string terrain = it->ToUTF8();
            std::transform( terrain.begin(), terrain.end(), terrain.begin(), ::tolower);
            if( terrain == name.toLower().toStdString() )
            {
                exists = true;
                break;
            }
        }

        EnableButton( eButtonStart, !exists );
        if( exists )
            errorLabel_->setText( tools::translate( "CreateTerrainPage", "A terrain with this name already exists." ) );
        else
        {
            tools::Path newTerrain = config_.GetTerrainDir( tools::Path::FromUnicode( name.toStdWString() ) );
            newTerrain.MakePreferred();
            errorLabel_->setText( tools::translate( "CreateTerrainPage", "The new terrain will be created in:\n%1" ).arg(
                QString::fromStdWString( newTerrain.ToUnicode().c_str() ) ) );
        }
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
