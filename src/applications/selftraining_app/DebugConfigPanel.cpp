// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "DebugConfigPanel.h"
#include "moc_DebugConfigPanel.cpp"
#include "clients_gui/FileDialog.h"
#include "clients_gui/tools.h"
#include "frontend/CommandLineTools.h"
#include "frontend/CreateSession.h"

namespace
{
    const int maxIntegrationDir = 5;

    QString ReadStringSetting( const QString& key )
    {
        QSettings settings( "MASA Group", "SWORD" );
        return settings.value( "/sword/" + key ).toString();
    }

    void WriteStringSetting( const QString& key, const QString& value )
    {
        QSettings settings( "MASA Group", "SWORD" );
        settings.setValue( "/sword/" + key, value );
    }

    int ReadIntSetting( const std::string& key, int defaultValue = 0 )
    {
        QSettings settings( "MASA Group", "SWORD" );
        int val =  settings.value( ( "/sword/" + key ).c_str(), defaultValue ).toInt();
        return val;
    }

    void WriteIntSetting( const std::string& key, int value )
    {
        QSettings settings( "MASA Group", "SWORD" );
        settings.setValue( ( "/sword/" + key ).c_str(), value );
    }
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel constructor
// Created: NPT 2013-01-03
// -----------------------------------------------------------------------------
DebugConfigPanel::DebugConfigPanel( QWidget* parent, const tools::GeneralConfig& config,
                                    bool legacy, bool timeline )
    : PluginConfig_ABC( parent )
    , config_( config )
    , profilingBox_( 0 )
    , decCallsBox_( 0 )
    , commandsBox_( 0 )
    , hooksBox_( 0 )
    , pathfindsBox_( 0 )
    , filterLabel_( 0 )
    , dumpLabel_( 0 )
    , dataDirectory_( 0 )
    , dataButton_( 0 )
    , exerciseNumber_( 1 )
{
    //legacy box
    legacyCheckBox_ = new QCheckBox();
    legacyCheckBox_->setCheckable( true );
    legacyCheckBox_->setChecked( legacy );
    connect( legacyCheckBox_, SIGNAL( stateChanged( int ) ), SLOT( SwordVersionChecked( int ) ) );

    //integration level label
    integrationLabel_ = new QLabel();

    //integration level combobox
    integrationComboBox_ = new QComboBox();
    integrationComboBox_->setEditable( true );
    QString pathValue = ReadStringSetting( "IntegrationLayerPaths" );
    if( !pathValue.isEmpty() )
    {
        pathList_ = pathValue.split( ';' );
        integrationComboBox_->addItems( pathList_ );
    }
    connect( integrationComboBox_, SIGNAL( editTextChanged( const QString& ) ), SLOT( OnEditIntegrationDirectory( const QString& ) ) );

    //integration level button
    integrationButton_ = new QPushButton();
    integrationButton_->setText( "..." );
    connect( integrationButton_, SIGNAL( clicked() ), SLOT( OnChangeIntegrationDirectory() ) );

    //integration level group box
    QHBoxLayout* integrationBoxLayout = new QHBoxLayout();
    integrationBoxLayout->addWidget( integrationLabel_ );
    integrationBoxLayout->addWidget( integrationComboBox_ );
    integrationBoxLayout->addWidget( integrationButton_ );
    integrationBoxLayout->setStretch( 0, 2 );
    integrationBoxLayout->setStretch( 1, 10 );
    integrationBoxLayout->setStretch( 2, 1 );
    integrationBoxLayout->setMargin( 5 );

    topBox_ = new QGroupBox();
    QVBoxLayout* commentBoxLayout = new QVBoxLayout( topBox_ );
    commentBoxLayout->setMargin( 5 );
    commentBoxLayout->addWidget( legacyCheckBox_ );
    commentBoxLayout->addLayout( integrationBoxLayout );

    //timeline box
    timelineBox_ = new QGroupBox();
    timelineBox_->setCheckable( true );
    timelineBox_->setChecked( timeline );
    timelineBox_->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
    connect( timelineBox_, SIGNAL( clicked( bool ) ), SLOT( OnTimelineChecked( bool ) ) );

    timelineDebugPortLabel_ = new QLabel();
    timelineDebugPortSpinBox_ = new QSpinBox();
    timelineDebugPortSpinBox_->setRange( 0, 65535 );
    timelineDebugPortSpinBox_->setValue( ReadIntSetting( "TimelineDebugPort" ) );
    connect( timelineDebugPortSpinBox_, SIGNAL( valueChanged( int ) ), SLOT( OnTimelineDebugPortChanged( int ) ) );

    QHBoxLayout* debugPortBox = new QHBoxLayout();
    debugPortBox->addWidget( timelineDebugPortLabel_ );
    debugPortBox->addWidget( timelineDebugPortSpinBox_ );

    QVBoxLayout* timelineGroupLayout = new QVBoxLayout( timelineBox_ );
    timelineGroupLayout->addLayout( debugPortBox );

    //profiling group box
    profilingBox_ = new QGroupBox();
    QGridLayout* profiling = new QGridLayout( profilingBox_, 2, 2 );
    profiling->setMargin( 10 );
    decCallsBox_ = new QCheckBox();
    commandsBox_ = new QCheckBox();
    hooksBox_ = new QCheckBox();
    profiling->addWidget( decCallsBox_, 0, 0 );
    profiling->addWidget( commandsBox_, 0, 1 );
    profiling->addWidget( hooksBox_, 1, 0 );

    //pathfinds group box
    pathfindsBox_ = new QGroupBox();
    QVBoxLayout* pathfinds = new QVBoxLayout( pathfindsBox_ );
    pathfinds->setMargin( 10 );
    QHBoxLayout* dumpLayout = new QHBoxLayout();
    dumpLayout->setSpacing( 10 );
    dumpLabel_ = new QLabel();
    dataButton_ = new QPushButton();
    connect( dataButton_, SIGNAL( clicked() ), SLOT( OnChangeDataDirectory() ) );
    dataDirectory_ = new QLineEdit();

    dumpLayout->addWidget( dumpLabel_ );
    dumpLayout->addWidget( dataDirectory_ );
    dumpLayout->addWidget( dataButton_ );

    QHBoxLayout* filterLayout = new QHBoxLayout();
    filterLayout->setSpacing( 10 );
    filterLabel_ = new QLabel();
    filterEdit_ = new QLineEdit();
    filterLayout->addWidget( filterLabel_ );
    filterLayout->addWidget( filterEdit_ );
    connect( filterEdit_, SIGNAL( editingFinished() ), SLOT( OnChangeDataFilter() ) );

    pathfinds->addLayout( dumpLayout );
    pathfinds->addLayout( filterLayout );

    //general Layout
    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    mainLayout->addWidget( topBox_ );
    mainLayout->addWidget( timelineBox_ );
    mainLayout->addWidget( profilingBox_ );
    mainLayout->addWidget( pathfindsBox_ );
    mainLayout->setAlignment( Qt::AlignTop );
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel destructor
// Created: NPT 2013-01-03
// -----------------------------------------------------------------------------
DebugConfigPanel::~DebugConfigPanel()
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::SwordVersionChecked
// Created: NPT 2013-01-03
// -----------------------------------------------------------------------------
void DebugConfigPanel::SwordVersionChecked( int state )
{
    emit SwordVersionSelected( state == Qt::Checked );
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::OnChangeIntegrationDirectory
// Created: NPT 2013-01-03
// -----------------------------------------------------------------------------
void DebugConfigPanel::OnChangeIntegrationDirectory()
{
    const tools::Path directory = gui::FileDialog::getExistingDirectory( this , "" );
    integrationComboBox_->setCurrentText( QString::fromStdWString( directory.ToUnicode() ) );
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::OnEditIntegrationDirectory
// Created: NPT 2013-01-03
// -----------------------------------------------------------------------------
void DebugConfigPanel::OnEditIntegrationDirectory( const QString& directory )
{
    tools::Path path = tools::Path::FromUnicode( directory.toStdWString() );
    if( path.IsDirectory() )
    {
        if ( !pathList_.contains( directory, Qt::CaseSensitive ) )
        {
            //maj of combobox
            pathList_.push_front( directory );
            if( pathList_.count() > maxIntegrationDir )
                pathList_.removeLast();
            integrationComboBox_->clear();
            integrationComboBox_->addItems( pathList_ );

            //save in registry
            WriteStringSetting( "IntegrationLayerPaths", pathList_.join( ";" ) );
        }
        emit IntegrationPathSelected( path );
    }
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::OnLanguageChanged
// Created: LGY 2013-02-05
// -----------------------------------------------------------------------------
void DebugConfigPanel::OnLanguageChanged()
{
    legacyCheckBox_->setText( tools::translate( "DebugConfigPanel", "Enable Legacy Mode" ) );
    timelineBox_->setTitle( tools::translate( "DebugConfigPanel", "Enable Web Timeline" ) );
    timelineDebugPortLabel_->setText( tools::translate( "DebugConfigPanel", "Debug port" ) );
    integrationLabel_->setText( tools::translate( "DebugConfigPanel", "Integration layer directory" ) );
    profilingBox_->setTitle( tools::translate( "DebugConfigPanel", "Profiling settings" ) );
    decCallsBox_->setText( tools::translate( "DebugConfigPanel", "Decisional functions" ) );
    commandsBox_->setText( tools::translate( "DebugConfigPanel", "Commands (non-legacy)" ) );
    hooksBox_->setText( tools::translate( "DebugConfigPanel", "Hooks (non-legacy)" ) );
    pathfindsBox_->setTitle( tools::translate( "DebugConfigPanel", "Pathfind settings" ) );
    filterLabel_->setText( tools::translate( "DebugConfigPanel", "Filter :" ) );
    dumpLabel_->setText( tools::translate( "DebugConfigPanel", "Dump pathfinds directory :" ) );
    dataButton_->setText( "..." );
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::GetName
// Created: LGY 2013-02-05
// -----------------------------------------------------------------------------
QString DebugConfigPanel::GetName() const
{
    return tools::translate( "DebugConfigPanel", "Debug" );
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::Commit
// Created: LGY 2013-02-05
// -----------------------------------------------------------------------------
void DebugConfigPanel::Commit( const tools::Path& exercise, const tools::Path& session )
{
    if( decCallsBox_->isChecked() || commandsBox_->isChecked() || hooksBox_->isChecked() || timelineBox_->isChecked() )
    {
        frontend::CreateSession action( config_, exercise, session );
        if( decCallsBox_->isChecked() )
            action.SetOption( "session/config/simulation/profiling/@decisional", "true" );
        if( commandsBox_->isChecked() )
           action.SetOption( "session/config/simulation/profiling/@command", "true" );
        if( hooksBox_->isChecked() )
            action.SetOption( "session/config/simulation/profiling/@hook", "true" );
        if( timelineBox_->isChecked() )
        {
            action.SetOption( "session/config/timeline/@debug-port", timelineDebugPortSpinBox_->value() );
            action.SetOption( "session/config/timeline/@url", "localhost:" +
                              boost::lexical_cast< std::string >( frontend::GetPort( exerciseNumber_, frontend::TIMELINE_WEB_PORT ) ) );
        }
        action.Commit();
    }
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::OnChangeDataDirectory
// Created: LGY 2013-02-05
// -----------------------------------------------------------------------------
void DebugConfigPanel::OnChangeDataDirectory()
{
    const tools::Path directory = gui::FileDialog::getExistingDirectory( this , "", tools::Path::FromUnicode( dataDirectory_->text().toStdWString() ) );
    if( directory.IsEmpty() )
        return;
    dataDirectory_->setText( QString::fromStdWString( directory.ToUnicode() ) );
    emit DumpPathfindOptionsChanged( filterEdit_->text(), tools::Path::FromUnicode( dataDirectory_->text().toStdWString() ) );
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::OnChangeDataFilter
// Created: LGY 2013-02-06
// -----------------------------------------------------------------------------
void DebugConfigPanel::OnChangeDataFilter()
{
    emit DumpPathfindOptionsChanged( filterEdit_->text(), tools::Path::FromUnicode( dataDirectory_->text().toStdWString() ) );
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::OnTimelineChecked
// Created: BAX 2013-04-16
// -----------------------------------------------------------------------------
void DebugConfigPanel::OnTimelineChecked( bool checked )
{
    emit TimelineEnabled( checked );
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::OnTimelineDebugPortChanged
// Created: ABR 2013-05-16
// -----------------------------------------------------------------------------
void DebugConfigPanel::OnTimelineDebugPortChanged( int port )
{
    WriteIntSetting( "TimelineDebugPort", port );
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::OnExerciseNumberChanged
// Created: ABR 2013-05-16
// -----------------------------------------------------------------------------
void DebugConfigPanel::OnExerciseNumberChanged( int exerciseNumber )
{
    exerciseNumber_ = exerciseNumber;
}

