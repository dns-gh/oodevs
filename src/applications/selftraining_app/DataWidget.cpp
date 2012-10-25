// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "DataWidget.h"
#include "moc_DataWidget.cpp"
#include "Config.h"
#include "MessageDialog.h"
#include "clients_gui/Tools.h"
#include "frontend/commands.h"
#include <boost/filesystem/convenience.hpp>

namespace bfs = boost::filesystem;

namespace
{
    void AddTab( QWidget* target, QTabWidget* tabs )
    {
        QWidget* holder = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout( holder );
        layout->setMargin( 5 );
        layout->addWidget( target );
        tabs->addTab( holder, "" );
    }
}

// -----------------------------------------------------------------------------
// Name: DataWidget constructor
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
DataWidget::DataWidget( QWidget* parent, QTabWidget* tabs, const Config& config )
    : parent_  ( parent )
    , config_  ( config )
    , mainTabs_( tabs )
{
    terrains_ = new QListWidget();
    terrains_->setFont( QFont( "Calibri", 12, QFont::Bold ) );
    connect( terrains_, SIGNAL( itemPressed( QListWidgetItem* ) ), this, SLOT( OnButtonChanged() ) );

    models_ = new QListWidget();
    models_->setFont( QFont( "Calibri", 12, QFont::Bold ) );
    connect( models_, SIGNAL( itemPressed( QListWidgetItem* ) ), this, SLOT( OnButtonChanged() ) );

    AddTab( terrains_, mainTabs_ );
    AddTab( models_,   mainTabs_ );

    Update();
}

// -----------------------------------------------------------------------------
// Name: CreateTerrainPage destructor
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
DataWidget::~DataWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DataWidget::Update
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
void DataWidget::Update()
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
    OnButtonChanged();
}

// -----------------------------------------------------------------------------
// Name: DataWidget::CurrentItem
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
QListWidgetItem* DataWidget::CurrentItem() const
{
    const QWidget* current = mainTabs_->currentWidget();
    if( current->isAncestorOf( terrains_ ) )
        return terrains_->currentItem();
    if( current->isAncestorOf( models_ ) )
        return models_->currentItem();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DataWidget::OnButtonChanged
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
void DataWidget::OnButtonChanged()
{
    emit ButtonChanged( !!CurrentItem(), tools::translate( "Page_ABC", "Delete" ) );
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
// Name: DataWidget::OnDelete
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
void DataWidget::OnDelete()
{
    QListWidgetItem* item = CurrentItem();
    if( !item )
        return;

    std::string path;
    if( mainTabs_->currentWidget()->isAncestorOf( terrains_ ) )
    {
        path = config_.GetTerrainDir( item->text().toAscii().constData() );
    }
    else
    {
        std::pair< std::string, std::string > info( Extract( item->text().toAscii().constData() ) );
        path = config_.GetPhysicalsDir( info.first, info.second );
    }
    MessageDialog message( parent_, tools::translate( "DataWidget", "Delete" ), tools::translate( "DataWidget", "Are you sure you want to delete?" ), QMessageBox::Yes, QMessageBox::No );
    if( message.exec() != QMessageBox::Yes )
        return;

    if( !bfs::exists( path ) || !bfs::is_directory( path ) )
        return;

    bfs::remove_all( path );
    Update();
}