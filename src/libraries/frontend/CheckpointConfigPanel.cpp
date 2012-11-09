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
#include <QtGui/qspinbox.h>
#include <Qt3Support/q3vbox.h>

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: CheckpointConfigPanel constructor
// Created: SBO 2010-04-19
// -----------------------------------------------------------------------------
CheckpointConfigPanel::CheckpointConfigPanel( QWidget* parent, const tools::GeneralConfig& config )
    : PluginConfig_ABC( parent )
    , config_( config )
    , exercise_( 0 )
{
    Q3VBox* vbox = new Q3VBox( this );
    vbox->setMargin( 5 );
    {
        checkpointsGroup_ = new Q3GroupBox( 2, Qt::Vertical, vbox );
        checkpointsGroup_->setCheckable( true );
        checkpointsGroup_->setChecked( false );
        {
            Q3HBox* frequencyBox = new Q3HBox( checkpointsGroup_ );
            frequencyLabel_ = new QLabel( frequencyBox );
            frequency_ = new Q3TimeEdit( frequencyBox );
            frequency_->setDisplay ( Q3TimeEdit::Hours | Q3TimeEdit::Minutes | Q3TimeEdit::Seconds  );
            frequency_->setTime( QTime().addSecs( 3600 ) );
        }
        {
            Q3HBox* keepBox = new Q3HBox( checkpointsGroup_ );
            keepLabel_ = new QLabel( keepBox );
            keep_ = new QSpinBox( 1, 100, 1, keepBox );
            keep_->setValue( 1 );
        }
    }
    {
        loadGroup_ = new Q3GroupBox( 2, Qt::Horizontal, vbox );
        loadGroup_->setCheckable( true );
        loadGroup_->setChecked( false );
        {
            Q3VBox* box = new Q3VBox( loadGroup_ );
            sessionLabel_ = new QLabel( box );
            box->setSpacing( 5 );
            sessions_ = new QListWidget( box );
            sessions_->setFont( QFont( "Calibri", 12, QFont::Bold ) );
            connect( sessions_, SIGNAL( currentTextChanged( const QString& ) ), SLOT( SessionSelected( const QString& ) ) );
        }
        {
            QWidget* box = new QWidget( loadGroup_ );
            QVBoxLayout* boxLayout = new QVBoxLayout( box );
            checkpoints_ = new CheckpointList( config_ );
            connect( checkpoints_, SIGNAL( Select( const QString& ) ), SLOT( OnCheckpointSelected( const QString& ) ) );
            boxLayout->addWidget( checkpoints_ );
            boxLayout->setSpacing( 5 );
            boxLayout->setMargin( 0 );
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
// Name: CheckpointConfigPanel::OnLanguageChanged
// Created: ABR 2011-11-10
// -----------------------------------------------------------------------------
void CheckpointConfigPanel::OnLanguageChanged()
{
    checkpointsGroup_->setTitle( tools::translate( "CheckpointConfigPanel", "Checkpoint configuration: " ) );
    loadGroup_->setTitle(        tools::translate( "CheckpointConfigPanel", "Load checkpoint: " ) );
    frequencyLabel_->setText(    tools::translate( "CheckpointConfigPanel", "Frequency:" ) );
    keepLabel_->setText(         tools::translate( "CheckpointConfigPanel", "Keep:" ) );
    sessionLabel_->setText(      tools::translate( "CheckpointConfigPanel", "Session:" ) );
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
        sessions_->addItems( commands::ListSessionsWithCheckpoint( config_, exercise_->GetName().c_str() ) );
        sessions_->setEnabled( sessions_->count() );
        if( !sessions_->count() )
            sessions_->addItem( tools::translate( "CheckpointConfigPanel", "No session" ) );
        sessions_->setCurrentRow( 0 );
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
    sessions_->setCurrentRow( 0 );
    sessions_->addItem( tools::translate( "CheckpointConfigPanel", "No session" ) );
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
    emit CheckpointSelected( checkpoint.isEmpty() ? "" : sessions_->currentItem()->text(), checkpoint );
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
        action.SetOption( "session/config/simulation/checkpoint/@frequency", QString( "%1s" ).arg( QTime().secsTo( frequency_->time() ) ).toStdString() );
        action.SetOption( "session/config/simulation/checkpoint/@keep", keep_->value() );
        action.SetOption( "session/config/simulation/checkpoint/@usecrc", true );
        action.Commit();
    }
}
