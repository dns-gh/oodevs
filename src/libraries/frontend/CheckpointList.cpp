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
#include <qlabel.h>
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: CheckpointList constructor
// Created: SBO 2010-04-21
// -----------------------------------------------------------------------------
CheckpointList::CheckpointList( QWidget* parent, const tools::GeneralConfig& config )
    : QVBox( parent )
    , config_( config )
    , enabled_( false )
{
    setSpacing( 5 );
    setBackgroundOrigin( QWidget::WindowOrigin );
    {
        QLabel* label = new QLabel( tools::translate( "CheckpointList", "Checkpoint:" ), this );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        list_ = new QListBox( this );
        connect( list_, SIGNAL( highlighted( int ) ), this, SLOT( SelectCheckpoint( int ) ) );
    }
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
        bfs::path p( config.GetCheckpointsDir( exercise.ascii(), session.ascii() ) );
        p = p / checkpoint.ascii() / "MetaData.xml";
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
    if( exercise_ != exercise.ascii() || session_ != session.ascii() )
    {
        exercise_ = exercise;
        session_ = session;
        list_->clear();
        checkpoints_ = frontend::commands::ListCheckpoints( config_, exercise.ascii(), session.ascii() );
        QStringList visibleNames;
        for( QStringList::const_iterator it = checkpoints_.begin(); it != checkpoints_.end(); ++it )
            visibleNames.push_back( ReadName( config_, exercise, session, *it ) );
        list_->insertStringList( visibleNames );
        list_->setEnabled( list_->count() > 0 );
        if( !list_->count() )
            list_->insertItem( tools::translate( "CheckpointList", "No checkpoint" ) );
        emit Select( "" );
        list_->setSelected( 0, true );
    }
}

// -----------------------------------------------------------------------------
// Name: CheckpointList::ClearSelection
// Created: LGY 2011-06-10
// -----------------------------------------------------------------------------
void CheckpointList::ClearSelection()
{
    list_->clear();
    list_->insertItem( tools::translate( "CheckpointList", "No checkpoint" ) );
    list_->setDisabled( true );
    emit Select( "" );
    list_->setSelected( 0, true );
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
    emit Select( enabled && list_->selectedItem() ? checkpoints_[list_->currentItem()] : "" );
}
