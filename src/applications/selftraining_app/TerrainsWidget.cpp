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
#include "MessageDialog.h"
#include "clients_kernel/Tools.h"
#include "frontend/commands.h"

// -----------------------------------------------------------------------------
// Name: TerrainsWidget constructor
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
TerrainsWidget::TerrainsWidget( QWidget* parent, const Config& config )
    : parent_  ( parent )
    , config_  ( config )
{
    terrains_ = new QListWidget();
    terrains_->setFont( QFont( "Calibri", 12, QFont::Bold ) );
    connect( terrains_, SIGNAL( itemPressed( QListWidgetItem* ) ), this, SLOT( OnButtonChanged() ) );
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
void TerrainsWidget::Update()
{
    terrains_->clear();
    const tools::Path::T_Paths paths = frontend::commands::ListTerrains( config_ );
    for( auto it = paths.begin(); it != paths.end(); ++it )
        terrains_->addItem( it->ToUTF8().c_str() );
    OnButtonChanged();
}

// -----------------------------------------------------------------------------
// Name: TerrainsWidget::CurrentItem
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
QListWidgetItem* TerrainsWidget::CurrentItem() const
{
    return terrains_->currentItem();
}

// -----------------------------------------------------------------------------
// Name: TerrainsWidget::OnButtonChanged
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
void TerrainsWidget::OnButtonChanged()
{
    emit ButtonChanged( !!CurrentItem(), tools::translate( "Page_ABC", "Delete" ) );
}

// -----------------------------------------------------------------------------
// Name: TerrainsWidget::OnDelete
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
void TerrainsWidget::OnDelete()
{
    QListWidgetItem* item = CurrentItem();
    if( !item )
        return;

    const auto path = config_.GetTerrainDir( tools::Path::FromUnicode( item->text().toStdWString() ) );
    MessageDialog message( parent_, tools::translate( "TerrainsWidget", "Delete" ),
            tools::translate( "TerrainsWidget", "Are you sure you want to delete?" ),
            QMessageBox::Yes, QMessageBox::No );
    if( message.exec() != QMessageBox::Yes )
        return;

    if( !path.Exists() || !path.IsDirectory() )
        return;

    path.RemoveAll();
    Update();
}

QWidget* TerrainsWidget::GetMainWidget()
{
    return terrains_;
}
