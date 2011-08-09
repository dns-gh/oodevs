// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "CheckpointConfigPanel.h"
#include "moc_CheckpointConfigPanel.cpp"
#include "CheckpointList.h"
#include "commands.h"
#include "CreateSession.h"
#include "Exercise_ABC.h"
#include "clients_gui/Tools.h"
#include <Qt3Support/q3datetimeedit.h>
#include <Qt3Support/q3groupbox.h>
#include <QtGui/qlabel.h>
#include <Qt3Support/q3listbox.h>
#include <QtGui/qspinbox.h>
#include <Qt3Support/q3vbox.h>

using namespace frontend;

namespace
{
    template< typename T >
    T* Style( T* widget )
    {
        widget->setBackgroundOrigin( QWidget::WindowOrigin );
        return widget;
    }
}

// -----------------------------------------------------------------------------
// Name: CheckpointConfigPanel constructor
// Created: SBO 2010-04-19
// -----------------------------------------------------------------------------
CheckpointConfigPanel::CheckpointConfigPanel( QWidget* parent, const tools::GeneralConfig& config )
    : PluginConfig_ABC( parent )
    , config_( config )
    , exercise_( 0 )
{
    Q3VBox* vbox = Style( new Q3VBox( this ) );
    vbox->setMargin( 5 );
    {
        checkpointsGroup_ = Style( new Q3GroupBox( 2, Qt::Vertical, tools::translate( "CheckpointConfigPanel", "Checkpoint configuration: " ), vbox ) );
        checkpointsGroup_->setCheckable( true );
        checkpointsGroup_->setChecked( false );
        {
            Q3HBox* frequencyBox = Style( new Q3HBox( checkpointsGroup_ ) );
            Style( new QLabel( tools::translate( "CheckpointConfigPanel", "Frequency:" ), frequencyBox ) );
            frequency_ = Style( new Q3TimeEdit( frequencyBox ) );
            frequency_->setDisplay ( Q3TimeEdit::Hours | Q3TimeEdit::Minutes | Q3TimeEdit::Seconds  );
            frequency_->setTime( QTime().addSecs( 3600 ) );
        }
        {
            Q3HBox* keepBox = Style( new Q3HBox( checkpointsGroup_ ) );
            Style( new QLabel( tools::translate( "CheckpointConfigPanel", "Keep:" ), keepBox ) );
            keep_ = Style( new QSpinBox( 1, 100, 1, keepBox ) );
        }
    }
    {
        loadGroup_ = Style( new Q3GroupBox( 2, Qt::Horizontal, tools::translate( "CheckpointConfigPanel", "Load checkpoint: " ), vbox ) );
        loadGroup_->setCheckable( true );
        loadGroup_->setChecked( false );
        {
            Q3VBox* box = Style( new Q3VBox( loadGroup_ ) );
            Style( new QLabel( tools::translate( "CheckpointConfigPanel", "Session:" ), box ) );
            box->setSpacing( 5 );
            sessions_ = Style( new Q3ListBox( box ) );
            connect( sessions_, SIGNAL( highlighted( const QString& ) ), SLOT( SessionSelected( const QString& ) ) );
        }
        {
            Q3VBox* box = Style( new Q3VBox( loadGroup_ ) );
            checkpoints_ = Style( new CheckpointList( box, config_ ) );
            connect( checkpoints_, SIGNAL( Select( const QString& ) ), SLOT( OnCheckpointSelected( const QString& ) ) );
        }
        connect( loadGroup_, SIGNAL( toggled( bool ) ), checkpoints_, SLOT( Toggle( bool ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: CheckpointConfigPanel destructor
// Created: SBO 2010-04-19
// -----------------------------------------------------------------------------
CheckpointConfigPanel::~CheckpointConfigPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CheckpointConfigPanel::Select
// Created: SBO 2010-04-19
// -----------------------------------------------------------------------------
void CheckpointConfigPanel::Select( const frontend::Exercise_ABC& exercise )
{
    if( exercise_ != &exercise )
    {
        exercise_ = &exercise;
        sessions_->clear();
        sessions_->insertStringList( commands::ListSessionsWithCheckpoint( config_, exercise_->GetName().c_str() ) );
        sessions_->setEnabled( sessions_->count() );
        if( !sessions_->count() )
            sessions_->insertItem( tools::translate( "CheckpointConfigPanel", "No session" ) );
        sessions_->setSelected( 0, true );
    }
}

// -----------------------------------------------------------------------------
// Name: CheckpointConfigPanel::ClearSelection
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void CheckpointConfigPanel::ClearSelection()
{
    exercise_ = 0;
    checkpointsGroup_->setChecked( false );
    checkpoints_->ClearSelection();
    sessions_->clear();
    sessions_->setSelected( 0, true );
    sessions_->insertItem( tools::translate( "CheckpointConfigPanel", "No session" ) );
    sessions_->setDisabled( true );
}

// -----------------------------------------------------------------------------
// Name: CheckpointConfigPanel::SessionSelected
// Created: SBO 2010-04-19
// -----------------------------------------------------------------------------
void CheckpointConfigPanel::SessionSelected( const QString& session )
{
    if( exercise_ )
        checkpoints_->Update( exercise_->GetName().c_str(), session );
}

// -----------------------------------------------------------------------------
// Name: CheckpointConfigPanel::OnCheckpointSelected
// Created: SBO 2010-04-23
// -----------------------------------------------------------------------------
void CheckpointConfigPanel::OnCheckpointSelected( const QString& checkpoint )
{
    emit CheckpointSelected( checkpoint.isEmpty() ? "" : sessions_->currentText(), checkpoint );
}

// -----------------------------------------------------------------------------
// Name: CheckpointConfigPanel::GetName
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
QString CheckpointConfigPanel::GetName() const
{
    return tools::translate( "CheckpointConfigPanel", "Checkpoints" );
}

// -----------------------------------------------------------------------------
// Name: CheckpointConfigPanel::Commit
// Created: SBO 2010-04-19
// -----------------------------------------------------------------------------
void CheckpointConfigPanel::Commit( const std::string& exercise, const std::string& session )
{
    if( checkpointsGroup_->isChecked() )
    {
        frontend::CreateSession action( config_, exercise, session );
        action.SetOption( "session/config/simulation/checkpoint/@frequency", QString( "%1s" ).arg( QTime().secsTo( frequency_->time() ) ).ascii() );
        action.SetOption( "session/config/simulation/checkpoint/@keep", keep_->value() );
        action.SetOption( "session/config/simulation/checkpoint/@usecrc", true );
        action.Commit();
    }
}
