// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ModelsWidget.h"
#include "moc_ModelsWidget.cpp"
#include "Config.h"
#include "MessageDialog.h"
#include "clients_kernel/Tools.h"
#include "frontend/commands.h"

// -----------------------------------------------------------------------------
// Name: ModelsWidget constructor
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
ModelsWidget::ModelsWidget( QWidget* parent, const Config& config )
    : parent_  ( parent )
    , config_  ( config )
{
    models_ = new QListWidget();
    models_->setFont( QFont( "Calibri", 12, QFont::Bold ) );
    connect( models_, SIGNAL( itemPressed( QListWidgetItem* ) ), this, SLOT( OnButtonChanged() ) );
}

// -----------------------------------------------------------------------------
// Name: CreateTerrainPage destructor
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
ModelsWidget::~ModelsWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelsWidget::Update
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
void ModelsWidget::Update()
{
    models_->clear();
    QStringList physicalBase;
    const tools::Path::T_Paths decisionalModels = frontend::commands::ListModels( config_ );
    for( auto it = decisionalModels.begin(); it != decisionalModels.end(); ++it )
    {
        const tools::Path::T_Paths physicalModels = frontend::commands::ListPhysicalModels( config_, *it );
        for( auto itP = physicalModels.begin(); itP != physicalModels.end(); ++itP )
            physicalBase << QString( "%1/%2" ).arg( it->ToUTF8().c_str() ).arg( itP->ToUTF8().c_str() );
    }
    models_->addItems( physicalBase );
    OnButtonChanged();
}

// -----------------------------------------------------------------------------
// Name: ModelsWidget::CurrentItem
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
QListWidgetItem* ModelsWidget::CurrentItem() const
{
    return models_->currentItem();
}

// -----------------------------------------------------------------------------
// Name: ModelsWidget::OnButtonChanged
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
void ModelsWidget::OnButtonChanged()
{
    emit ButtonChanged( !!CurrentItem(), tools::translate( "Page_ABC", "Delete" ), false );
}

namespace
{
    std::pair< tools::Path, tools::Path > Extract( const QString& value )
    {
        QStringList values = value.split("/");
        if( values.size() == 2 )
            return std::make_pair( tools::Path::FromUnicode( values[ 0 ].toStdWString() ), tools::Path::FromUnicode( values[ 1 ].toStdWString() ) );
        return std::make_pair( tools::Path(), tools::Path() );
    }
}

// -----------------------------------------------------------------------------
// Name: ModelsWidget::OnDelete
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
void ModelsWidget::OnDelete()
{
    QListWidgetItem* item = CurrentItem();
    if( !item )
        return;

    const std::pair< tools::Path, tools::Path > info( Extract( item->text() ) );
    const auto path = config_.GetPhysicalsDir( info.first, info.second );

    MessageDialog message( parent_, tools::translate( "ModelsWidget", "Delete" ),
            tools::translate( "ModelsWidget", "Are you sure you want to delete?" ),
            QMessageBox::Yes, QMessageBox::No );
    if( message.exec() != QMessageBox::Yes )
        return;

    if( !path.Exists() || !path.IsDirectory() )
        return;

    path.RemoveAll();
    Update();
}

QWidget* ModelsWidget::GetMainWidget()
{
    return models_;
}
