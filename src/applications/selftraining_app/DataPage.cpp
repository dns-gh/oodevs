// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "DataPage.h"
#include "moc_DataPage.cpp"
#include "Config.h"
#include "MessageDialog.h"
#include "clients_gui/Tools.h"
#include "frontend/commands.h"
#include <boost/filesystem/convenience.hpp>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: DataPage constructor
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
DataPage::DataPage( QWidget* parent, Q3WidgetStack* pages, Page_ABC& previous, const Config& config )
    : ContentPage( pages, previous, eButtonBack | eButtonDelete )
    , parent_( parent )
    , config_( config )
{
    setWindowTitle( "DataPage" );

    terrains_ = new QListWidget();
    terrains_->setFont( QFont( "Calibri", 12, QFont::Bold ) );
    connect( terrains_, SIGNAL( itemPressed( QListWidgetItem* ) ), this, SLOT( UpdateButton() ) );

    models_ = new QListWidget();
    models_->setFont( QFont( "Calibri", 12, QFont::Bold ) );
    connect( models_, SIGNAL( itemPressed( QListWidgetItem* ) ), this, SLOT( UpdateButton() ) );

    mainTabs_ = new QTabWidget();
    mainTabs_->addTab( terrains_, "" );
    mainTabs_->addTab( models_, "" );
    connect( mainTabs_, SIGNAL( currentChanged( int ) ), this, SLOT( UpdateButton() ) );

    layout()->addWidget( mainTabs_ );

    Update();
    AddContent( mainTabs_ );
}

// -----------------------------------------------------------------------------
// Name: CreateTerrainPage destructor
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
DataPage::~DataPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DataPage::OnLanguageChanged
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
void DataPage::OnLanguageChanged()
{
    SetTitle( tools::translate( "DataPage", "Data" ) );
    mainTabs_->setTabText( eTabs_Terrains, tools::translate( "DataPage", "Terrains" ) );
    mainTabs_->setTabText( eTabs_Models, tools::translate( "DataPage", "Models" ) );
    ContentPage::OnLanguageChanged();
}

// -----------------------------------------------------------------------------
// Name: DataPage::Update
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
void DataPage::Update()
{
    terrains_->clear();
    terrains_->addItems( frontend::commands::ListTerrains( config_ ) );
    models_->clear();
    QStringList physicalBase;
    QStringList decisionalModels = frontend::commands::ListModels( config_ );
    for( QStringList::const_iterator it = decisionalModels.begin(); it != decisionalModels.end(); ++it )
    {
        const QStringList physicalModels = frontend::commands::ListPhysicalModels( config_, (*it).toAscii().constData() );
        for( QStringList::const_iterator itP = physicalModels.begin(); itP != physicalModels.end(); ++itP )
            physicalBase << QString( "%1/%2" ).arg( *it ).arg( *itP );
    }
    models_->addItems( physicalBase );
    UpdateButton();
}

// -----------------------------------------------------------------------------
// Name: DataPage::CurrentItem
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
QListWidgetItem* DataPage::CurrentItem() const
{
    QListWidgetItem* item = 0;
    switch( mainTabs_->currentIndex() )
    {
    case eTabs_Terrains:
        item = terrains_->currentItem();
        break;
    case eTabs_Models:
        item = models_->currentItem();
        break;
    default:
        break;
    }
    return item;
}

// -----------------------------------------------------------------------------
// Name: DataPage::UpdateButton
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
void DataPage::UpdateButton()
{
    EnableButton( eButtonDelete, CurrentItem() != 0 );
}

namespace
{
    std::pair< std::string, std::string > Extract( const std::string& value )
    {
        QStringList values = QStringList::split( "/", value.c_str() );
        if( values.size() == 2 )
            return std::make_pair( values[ 0 ], values[ 1 ] );
        return std::make_pair( "", "" );
    }
}

// -----------------------------------------------------------------------------
// Name: DataPage::OnDelete
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
void DataPage::OnDelete()
{
    if( QListWidgetItem* item = CurrentItem() )
    {
        std::string path;
        if( mainTabs_->currentIndex() == eTabs_Terrains )
            path = config_.GetTerrainDir( item->text().toAscii().constData() );
        else
        {
            std::pair< std::string, std::string > info( Extract( item->text().toAscii().constData() ) );
            path = config_.GetPhysicalsDir( info.first, info.second );
        }
        MessageDialog message( parent_, tools::translate( "DataPage", "Delete" ), tools::translate( "DataPage", "Are you sure you want to delete?" ), QMessageBox::Yes, QMessageBox::No );
        if( message.exec() == QMessageBox::Yes )
            if( bfs::exists( path ) && bfs::is_directory( path ) )
            {
                bfs::remove_all( path );
                Update();
            }
    }
}

// -----------------------------------------------------------------------------
// Name: DataPage::SetTerrainsEnabled
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
void DataPage::SetTerrainsEnabled( bool value )
{
    mainTabs_->setTabEnabled( eTabs_Terrains, value );
}

// -----------------------------------------------------------------------------
// Name: DataPage::SetModelsEnabled
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
void DataPage::SetModelsEnabled( bool value )
{
    mainTabs_->setTabEnabled( eTabs_Models, value );
}
