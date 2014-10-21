// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "TerrainsWidget.h"
#include "moc_TerrainsWidget.cpp"
#include "Config.h"
#include "LogProgressPage.h"
#include "MessageDialog.h"
#include "clients_kernel/Tools.h"
#include "frontend/commands.h"
#include "MT_Tools/MT_Logger.h"
#include <graphics/MapnikLevel.h>
#include <graphics/MapnikProcess.h>
#include <QtGui/QHeaderView>
#include <QtGui/QTableView>
#include <boost/optional.hpp>

namespace
{

const int PathRole = Qt::UserRole + 1;
const int UpgradeRole = Qt::UserRole + 2;

}  // namespace

// -----------------------------------------------------------------------------
// Name: TerrainsWidget constructor
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
TerrainsWidget::TerrainsWidget( QWidget* parent, const Config& config, Application& app,
       QStackedWidget* pages, Page_ABC& parentPage )
    : parent_  ( parent )
    , config_  ( config )
    , progress_( new LogProgressPage( app, pages, parentPage ) )
{
    model_ = new QStandardItemModel();
    model_->setColumnCount( 2 );
    terrains_ = new QTableView();
    terrains_->setModel( model_ );
    terrains_->setFont( QFont( "Calibri", 12, QFont::Bold ) );
    terrains_->setEditTriggers( QAbstractItemView::NoEditTriggers );
    terrains_->setSelectionBehavior( QAbstractItemView::SelectRows );
    terrains_->horizontalHeader()->hide();
    terrains_->verticalHeader()->hide();
    terrains_->setShowGrid( false );
    terrains_->setFocusPolicy( Qt::NoFocus );
    connect( terrains_, SIGNAL( pressed( const QModelIndex& ) ), this,
                        SLOT( OnButtonChanged( const QModelIndex& ) ) );
}

// -----------------------------------------------------------------------------
// Name: CreateTerrainPage destructor
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
TerrainsWidget::~TerrainsWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TerrainsWidget::Update
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
void TerrainsWidget::OnUpdate()
{
    model_->clear();
    const tools::Path::T_Paths paths = frontend::commands::ListTerrains( config_ );
    for( auto it = paths.begin(); it != paths.end(); ++it )
    {
        const auto terrainDir = config_.GetTerrainDir( *it );
        const auto version = graphics::GetMapnikDataVersion( terrainDir );
        const bool upgradable = graphics::CanUpgradeMapnikData( version, false );

        QList< QStandardItem* > items;
        QString label = QString::fromStdString( it->ToUTF8() );
        items << new QStandardItem( QString::fromStdString( it->ToUTF8() ) );
        if( upgradable )
        {
            auto item = new QStandardItem( tools::translate( "TerrainsWidget", "upgrade" ) );
            item->setFont( QFont( "Calibri", 12, QFont::Normal ) );
            items << item;
        }
        for( int i = 0; i < items.size(); ++i )
        {
            items[i]->setData( QString::fromStdString( it->ToUTF8() ), PathRole );
            items[i]->setData( upgradable, UpgradeRole );
        }

        model_->appendRow( items );
    }
    if( !paths.empty() )
    {
        terrains_->horizontalHeader()->setResizeMode( QHeaderView::Stretch );
        terrains_->horizontalHeader()->setResizeMode( 1, QHeaderView::ResizeToContents );
    }
    OnButtonChanged( QModelIndex() );
}

// -----------------------------------------------------------------------------
// Name: TerrainsWidget::OnButtonChanged
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
void TerrainsWidget::OnButtonChanged( const QModelIndex& index )
{
    UpdateButtons( index.isValid() && index.model(),
                   index.isValid() && index.data( UpgradeRole ).toBool() );
}

void TerrainsWidget::OnButtonChanged()
{
    bool remove = false, upgrade = false;
    const auto selected = terrains_->selectionModel()->selectedIndexes();
    if( selected.size() > 0 )
    {
        remove = true;
        upgrade = selected[0].data( UpgradeRole ).toBool();
    }
    UpdateButtons( remove, upgrade );
}

void TerrainsWidget::UpdateButtons( bool remove, bool upgrade )
{
    emit ButtonChanged( remove, tools::translate( "Page_ABC", "Delete" ), upgrade );
}

boost::optional< tools::Path > TerrainsWidget::GetSelectedTerrain() const
{
    const auto selected = terrains_->selectionModel()->selectedIndexes();
    if( selected.size() <= 0 )
        return boost::none;
    const auto data = selected[0].data( PathRole ).toString().toStdString();
    return config_.GetTerrainDir( tools::Path::FromUTF8( data ) );
}

void TerrainsWidget::OnUpgrade()
{
    const auto terrain = GetSelectedTerrain();
    if( !terrain )
        return;
    MT_LOG_INFO_MSG( "generating " << *terrain );
    progress_->Start( [this]()
    {
        generator_.reset();
    });
    generator_ = graphics::StartMapnikGen( *terrain, false, false,
        [this]( const std::string& line, graphics::E_MapnikProcess s )
        {
            switch( s )
            {
                case graphics::MapnikLogInfo:
                    MT_LOG_INFO_MSG( "mapnikgen: " << line );
                    progress_->Log( line );
                    break;
                case graphics::MapnikLogError:
                    MT_LOG_ERROR_MSG( "mapnikgen: " << line );
                    progress_->Log( line );
                    break;
                case graphics::MapnikSuccess:
                    progress_->NotifyDone( "" );
                    // Notify Qt main thread
                    QMetaObject::invokeMethod( this, "OnUpdate", Qt::QueuedConnection );
                    break;
                case graphics::MapnikFailure:
                    progress_->NotifyDone( "Mapnik generation failed" );
                    QMetaObject::invokeMethod( this, "OnUpdate", Qt::QueuedConnection );
                    break;
            };
        });
}

// -----------------------------------------------------------------------------
// Name: TerrainsWidget::OnDelete
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
void TerrainsWidget::OnDelete()
{
    const auto path = GetSelectedTerrain();
    if( !path )
        return;
    MessageDialog message( parent_, tools::translate( "TerrainsWidget", "Delete" ),
            tools::translate( "TerrainsWidget", "Are you sure you want to delete?" ),
            QMessageBox::Yes, QMessageBox::No );
    if( message.exec() != QMessageBox::Yes )
        return;

    if( !path->IsDirectory() )
        return;

    path->RemoveAll();
    OnUpdate();
}

QWidget* TerrainsWidget::GetMainWidget()
{
    return terrains_;
}
