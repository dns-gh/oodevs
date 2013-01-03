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
#include <Qt3Support/q3groupbox.h>
#include <QtGui/qlabel.h>
#include <Qt3Support/q3listbox.h>
#include <QtGui/qspinbox.h>
#include <Qt3Support/q3vbox.h>
#include <QtCore/qsettings.h>

using namespace frontend;

Q_DECLARE_METATYPE( QTime )

namespace
{
    int ReadIntRegistryValue( const std::string& key )
    {
        QSettings settings( "MASA Group", "SWORD" );
        int val =  settings.value( ( "/sword/" + key ).c_str(), 1 ).toInt();
        return val;
    }

    QString ReadTimeRegistryValue( const std::string& key )
    {
        QSettings settings( "MASA Group", "SWORD" );
        return settings.value( ( "/sword/" + key ).c_str(), QTime().addSecs( 3600 ) ).toString();
    }

    bool ReadBoolRegistryValue( const std::string& key )
    {
        QSettings settings( "MASA Group", "SWORD" );
        bool val =  settings.value( ( "/sword/" + key ).c_str(), false ).toBool();
        return val;
    }

    void WriteIntRegistryValue( const std::string& key, int value )
    {
        QSettings settings( "MASA Group", "SWORD" );
        settings.setValue( ( "/sword/" + key ).c_str(), value );
    }

    void WriteTimeRegistryValue( const std::string& key, QString date )
    {
        QSettings settings( "MASA Group", "SWORD" );
        settings.setValue( ( "/sword/" + key ).c_str(), date );
    }

    void WriteBoolRegistryValue( const std::string& key, bool checked )
    {
        QSettings settings( "MASA Group", "SWORD" );
        settings.setValue( ( "/sword/" + key ).c_str(), checked );
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
    Q3VBox* vbox = new Q3VBox( this );
    vbox->setMargin( 5 );
    {
        checkpointsGroup_ = new Q3GroupBox( 2, Qt::Vertical, vbox );
        checkpointsGroup_->setCheckable( true );
        checkpointsGroup_->setChecked( ReadBoolRegistryValue( "CheckpointChecked" ) );
        connect( checkpointsGroup_, SIGNAL( clicked( bool ) ), SLOT( OnCheckpointCheckedChanged( bool ) ) );
        {
            Q3HBox* frequencyBox = new Q3HBox( checkpointsGroup_ );
            frequencyLabel_ = new QLabel( frequencyBox );
            frequency_ = new QTimeEdit( frequencyBox );
            frequency_->setDisplayFormat( "hh:mm:ss" );
            frequency_->setTime( QTime:: fromString( ReadTimeRegistryValue( "CheckpointFrequency" ) ) );
            connect( frequency_, SIGNAL( timeChanged ( const QTime & ) ), SLOT( OnFrequencyChanged( const QTime & ) ) );
        }
        {
            Q3HBox* keepBox = new Q3HBox( checkpointsGroup_ );
            keepLabel_ = new QLabel( keepBox );
            keep_ = new QSpinBox( 1, 100, 1, keepBox );
            keep_->setValue( ReadIntRegistryValue( "CheckpointKept" ) );
            connect( keep_, SIGNAL( valueChanged ( int ) ), SLOT( OnCheckpointKeptChanged( int ) ) );
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
            sessions_ = new Q3ListBox( box );
            connect( sessions_, SIGNAL( highlighted( const QString& ) ), SLOT( SessionSelected( const QString& ) ) );
        }
        {
            Q3VBox* box = new Q3VBox( loadGroup_ );
            checkpoints_ = new CheckpointList( box, config_ );
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
        action.SetOption( "session/config/simulation/checkpoint/@frequency", QString( "%1s" ).arg( QTime().secsTo( frequency_->time() ) ).toAscii().constData() );
        action.SetOption( "session/config/simulation/checkpoint/@keep", keep_->value() );
        action.SetOption( "session/config/simulation/checkpoint/@usecrc", true );
        action.Commit();
    }
}

// -----------------------------------------------------------------------------
// Name: CheckpointConfigPanel::OnFrequencyChanged
// Created: NPT 2012-11-28
// -----------------------------------------------------------------------------
void CheckpointConfigPanel::OnFrequencyChanged( const QTime& time )
{
    WriteTimeRegistryValue( "CheckpointFrequency", time.toString( "hh:mm:ss" ) );
}

// -----------------------------------------------------------------------------
// Name: CheckpointConfigPanel::OnCheckpointKeptChanged
// Created: NPT 2012-11-28
// -----------------------------------------------------------------------------
void CheckpointConfigPanel::OnCheckpointKeptChanged( int value )
{
    WriteIntRegistryValue( "CheckpointKept", value );
}

// -----------------------------------------------------------------------------
// Name: CheckpointConfigPanel::OnCheckpointCheckedChanged
// Created: NPT 2012-12-20
// -----------------------------------------------------------------------------
void CheckpointConfigPanel::OnCheckpointCheckedChanged( bool value )
{
    WriteBoolRegistryValue( "CheckpointChecked", value );
}
