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
#include "clients_gui/Tools.h"
#include "tools/GeneralConfig.h"
#include <boost/filesystem/operations.hpp>
#include <QtGui/qlabel.h>
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: CheckpointList constructor
// Created: SBO 2010-04-21
// -----------------------------------------------------------------------------
CheckpointList::CheckpointList( const tools::GeneralConfig& config )
    : config_( config )
    , enabled_( false )
{
        QLabel* label = new QLabel( tools::translate( "CheckpointList", "Checkpoint:" ) );

        list_ = new QListWidget();
        list_->setFont( QFont( "Calibri", 12, QFont::Bold ) );
        connect( list_, SIGNAL( currentRowChanged( int ) ), this, SLOT( SelectCheckpoint( int ) ) );

        QVBoxLayout* verticalLayout = new QVBoxLayout( this );
        verticalLayout->addWidget( label );
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

namespace
{
    QString ReadName( const tools::GeneralConfig& config, const QString& exercise, const QString& session, const QString& checkpoint )
    {
        bfs::path p( config.GetCheckpointsDir( exercise.toStdString(), session.toStdString() ) );
        p = p / checkpoint.toStdString() / "MetaData.xml";
        try
        {
            if( bfs::exists( p ) )
            {
                std::string name;
                xml::xifstream xis( p.string() );
                xis >> xml::start( "checkpoint" ) >> xml::content( "name", name );
                return name.empty() ? checkpoint : name.c_str();
            }
        }
        catch( ... )
        {
            // NOTHING
        }
        return checkpoint;
    }
}

// -----------------------------------------------------------------------------
// Name: CheckpointList::Update
// Created: SBO 2010-04-21
// -----------------------------------------------------------------------------
void CheckpointList::Update( const QString& exercise, const QString& session )
{
    if( exercise_ != exercise || session_ != session )
    {
        exercise_ = exercise;
        session_ = session;
        list_->clear();
        checkpoints_ = frontend::commands::ListCheckpoints( config_, exercise.toStdString(), session.toStdString() );
        QStringList visibleNames;
        for( QStringList::const_iterator it = checkpoints_.begin(); it != checkpoints_.end(); ++it )
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
    if( checkpoints_.count() )
        emit Select( enabled && list_->currentItem() ? checkpoints_[ list_->currentRow() ] : "" );
}
