// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "CheckpointList.h"
#include "moc_CheckpointList.cpp"
#include "commands.h"
#include "clients_kernel/Tools.h"
#include "tools/GeneralConfig.h"
#include <QtGui/qlabel.h>
#include <xeumeuleu/xml.hpp>

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: CheckpointList constructor
// Created: SBO 2010-04-21
// -----------------------------------------------------------------------------
CheckpointList::CheckpointList( const tools::GeneralConfig& config )
    : gui::WidgetLanguageObserver_ABC< QWidget >()
    , config_( config )
    , enabled_( false )
{
    label_ = new QLabel();
    list_ = new QListWidget();
    list_->setFont( QFont( "Calibri", 12, QFont::Bold ) );
    connect( list_, SIGNAL( currentRowChanged( int ) ), this, SLOT( SelectCheckpoint( int ) ) );

    QVBoxLayout* verticalLayout = new QVBoxLayout( this );
    verticalLayout->addWidget( label_ );
    verticalLayout->addWidget( list_ );
    verticalLayout->setSpacing( 5 );
    verticalLayout->setMargin( 0 );
}

// -----------------------------------------------------------------------------
// Name: CheckpointList destructor
// Created: SBO 2010-04-21
// -----------------------------------------------------------------------------
CheckpointList::~CheckpointList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CheckpointList::OnLanguageChanged
// Created: LGY 2014-01-23
// -----------------------------------------------------------------------------
void CheckpointList::OnLanguageChanged()
{
    label_->setText( tools::translate( "CheckpointList", "Checkpoint:" ) );
}

namespace
{
    QString ReadName( const tools::GeneralConfig& config, const tools::Path& exercise, const tools::Path& session, const tools::Path& checkpoint )
    {
        tools::Path metaDataFile = config.GetCheckpointsDir( exercise, session ) / checkpoint / "MetaData.xml";
        try
        {
            if( metaDataFile.Exists() )
            {
                std::string name;
                tools::Xifstream xis( metaDataFile );
                xis >> xml::start( "checkpoint" ) >> xml::content( "name", name );
                return name.empty() ? checkpoint.ToUTF8().c_str() : name.c_str();
            }
        }
        catch( ... )
        {
            // NOTHING
        }
        return checkpoint.ToUTF8().c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: CheckpointList::Update
// Created: SBO 2010-04-21
// -----------------------------------------------------------------------------
void CheckpointList::Update( const tools::Path& exercise, const tools::Path& session )
{
    if( exercise_ != exercise || session_ != session )
    {
        exercise_ = exercise;
        session_ = session;
        list_->clear();
        checkpoints_ = frontend::commands::ListCheckpoints( config_, exercise, session );
        QStringList visibleNames;
        for( auto it = checkpoints_.begin(); it != checkpoints_.end(); ++it )
            visibleNames.push_back( ReadName( config_, exercise, session, *it ) );
        list_->addItems( visibleNames );
        list_->setEnabled( list_->count() > 0 );
        if( !list_->count() )
            list_->addItem( tools::translate( "CheckpointList", "No checkpoint" ) );
        emit Select( "" );
        list_->setCurrentRow( 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: CheckpointList::ClearSelection
// Created: LGY 2011-06-10
// -----------------------------------------------------------------------------
void CheckpointList::ClearSelection()
{
    list_->clear();
    list_->addItem( tools::translate( "CheckpointList", "No checkpoint" ) );
    list_->setDisabled( true );
    emit Select( "" );
    list_->setCurrentRow( 0 );
}

// -----------------------------------------------------------------------------
// Name: CheckpointList::SelectCheckpoint
// Created: SBO 2010-04-21
// -----------------------------------------------------------------------------
void CheckpointList::SelectCheckpoint( int index )
{
    if( index >= 0 && index < int( checkpoints_.size() ) )
        emit Select( enabled_ ? checkpoints_[index] : "" );
}

// -----------------------------------------------------------------------------
// Name: CheckpointList::Toggle
// Created: SBO 2010-04-21
// -----------------------------------------------------------------------------
void CheckpointList::Toggle( bool enabled )
{
    enabled_ = enabled;
    if( checkpoints_.size() )
        emit Select( enabled && list_->currentItem() ? checkpoints_[ list_->currentRow() ] : "" );
}
