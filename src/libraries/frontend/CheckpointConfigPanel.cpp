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
#include "clients_kernel/Tools.h"
#include <QtGui/qlabel.h>
#include <QtGui/qspinbox.h>
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
    //------checkpoint group ------//
    //frequency box
    frequencyLabel_ = new QLabel();
    frequency_ = new QTimeEdit();
    frequency_->setDisplayFormat( "hh:mm:ss" );
    frequency_->setTime( QTime:: fromString( ReadTimeRegistryValue( "CheckpointFrequency" ) ) );
    connect( frequency_, SIGNAL( timeChanged ( const QTime & ) ), SLOT( OnFrequencyChanged( const QTime & ) ) );

    QHBoxLayout* frequencyBox = new QHBoxLayout();
    frequencyBox->addWidget( frequencyLabel_ );
    frequencyBox->addWidget( frequency_ );

    //keep box
    keepLabel_ = new QLabel();
    keep_ = new QSpinBox();
    keep_->setRange( 1, 100 );
    keep_->setValue( ReadIntRegistryValue( "CheckpointKept" ) );
    connect( keep_, SIGNAL( valueChanged ( int ) ), SLOT( OnCheckpointKeptChanged( int ) ) );

    QHBoxLayout* keepBox = new QHBoxLayout();
    keepBox->addWidget( keepLabel_ );
    keepBox->addWidget( keep_ );

    //checkpoint groupbox
    checkpointsGroup_ = new QGroupBox();
    checkpointsGroup_->setCheckable( true );
    checkpointsGroup_->setChecked( ReadBoolRegistryValue( "CheckpointChecked" ) );
    QVBoxLayout* checkpointsGroupLayout = new QVBoxLayout( checkpointsGroup_ );
    checkpointsGroup_->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
    checkpointsGroupLayout->addLayout( frequencyBox );
    checkpointsGroupLayout->addLayout( keepBox );
    connect( checkpointsGroup_, SIGNAL( clicked( bool ) ), SLOT( OnCheckpointCheckedChanged( bool ) ) );
    //------end checkpoint group------//

    //------load group------//
    //session box
    sessionLabel_ = new QLabel();
    sessions_ = new QListWidget();
    sessions_->setFont( QFont( "Calibri", 12, QFont::Bold ) );
    connect( sessions_, SIGNAL( currentTextChanged( const QString& ) ), SLOT( SessionSelected( const QString& ) ) );

    QVBoxLayout* sessionBoxLayout = new QVBoxLayout();
    sessionBoxLayout->setSpacing( 5 );
    sessionBoxLayout->addWidget( sessionLabel_ );
    sessionBoxLayout->addWidget( sessions_ );

    //checkpoint box
    QVBoxLayout* boxLayout = new QVBoxLayout();
    checkpoints_ = new CheckpointList( config_ );
    connect( checkpoints_, SIGNAL( Select( const tools::Path& ) ), SLOT( OnCheckpointSelected( const tools::Path& ) ) );
    boxLayout->addWidget( checkpoints_ );
    boxLayout->setMargin( 0 );
    boxLayout->setSpacing( 5 );

    //load group box
    loadGroup_ = new QGroupBox();
    loadGroup_->setCheckable( true );
    loadGroup_->setChecked( false );
    QHBoxLayout* loadGroupLayout = new QHBoxLayout( loadGroup_ );
    loadGroupLayout->addLayout( sessionBoxLayout );
    loadGroupLayout->addLayout( boxLayout );
    loadGroupLayout->setSpacing( 10 );
    connect( loadGroup_, SIGNAL( toggled( bool ) ), checkpoints_, SLOT( Toggle( bool ) ) );
    //------end load group------//

    //------main panel------//
    QVBoxLayout* vBoxLayout = new QVBoxLayout( this );
    vBoxLayout->setMargin( 5 );
    vBoxLayout->addWidget( checkpointsGroup_ );
    vBoxLayout->addWidget( loadGroup_ );
    //------end main panel------//
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
        tools::Path::T_Paths paths = commands::ListSessionsWithCheckpoint( config_, exercise_->GetName() );
        for( auto it = paths.begin(); it != paths.end(); ++it )
            sessions_->addItem( QString( it->ToUTF8().c_str() ) );
        sessions_->setEnabled( sessions_->count() > 0 );
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
    checkpoints_->ClearSelection();
    sessions_->clear();
    sessions_->setCurrentRow( 0 );
    sessions_->addItem( tools::translate( "CheckpointConfigPanel", "No session" ) );
    loadGroup_->setChecked( false );
    sessions_->setDisabled( true );
}

// -----------------------------------------------------------------------------
// Name: CheckpointConfigPanel::SessionSelected
// Created: SBO 2010-04-19
// -----------------------------------------------------------------------------
void CheckpointConfigPanel::SessionSelected( const QString& session )
{
    if( exercise_ )
        checkpoints_->Update( exercise_->GetName(), tools::Path::FromUnicode( session.toStdWString() ) );
}

// -----------------------------------------------------------------------------
// Name: CheckpointConfigPanel::OnCheckpointSelected
// Created: SBO 2010-04-23
// -----------------------------------------------------------------------------
void CheckpointConfigPanel::OnCheckpointSelected( const tools::Path& checkpoint )
{
    QListWidgetItem* current = checkpoint.IsEmpty() ? 0 : sessions_->currentItem();
    emit CheckpointSelected( current ? tools::Path::FromUnicode( current->text().toStdWString() ) : tools::Path(), checkpoint );
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
void CheckpointConfigPanel::Commit( const tools::Path& exercise, const tools::Path& session )
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
