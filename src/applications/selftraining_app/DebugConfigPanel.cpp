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
#include "Config.h"

#include "clients_gui/FileDialog.h"

#include "frontend/CommandLineTools.h"
#include "frontend/CreateSession.h"
#include "frontend/DebugConfig.h"

#include <QScrollArea>

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel constructor
// Created: NPT 2013-01-03
// -----------------------------------------------------------------------------
DebugConfigPanel::DebugConfigPanel( QWidget* parent, const Config& config,
        frontend::DebugConfig& debug )
    : gui::WidgetLanguageObserver_ABC< QWidget >( parent )
    , visible_( !!parent )
    , config_( config )
    , debug_( debug )
    , profilingBox_( 0 )
    , decCallsBox_( 0 )
    , pathfindsBox_( 0 )
    , filterLabel_( 0 )
    , dumpLabel_( 0 )
    , dataDirectory_( 0 )
    , dataButton_( 0 )
    , mapnikBox_( 0 )
    , mapnikLayerBox_( 0 )
    , mapnikThreads_( 0 )
{
    debug_ = frontend::LoadDebugConfig();

    //integration level label
    integrationLabel_ = new QLabel();

    //integration level combobox
    integrationDir_ = new QLineEdit();
    integrationDir_->setText( QString::fromStdWString( debug_.sim.integrationDir.ToUnicode() ) );
    connect( integrationDir_, SIGNAL( textEdited( const QString& ) ), SLOT( OnEditIntegrationDirectory( const QString& ) ) );

    //integration level button
    integrationButton_ = new QPushButton();
    integrationButton_->setText( "..." );
    connect( integrationButton_, SIGNAL( clicked() ), SLOT( OnChangeIntegrationDirectory() ) );

    //integration level group box
    QHBoxLayout* integrationBoxLayout = new QHBoxLayout();
    integrationBoxLayout->addWidget( integrationLabel_ );
    integrationBoxLayout->addWidget( integrationDir_ );
    integrationBoxLayout->addWidget( integrationButton_ );
    integrationBoxLayout->setStretch( 0, 2 );
    integrationBoxLayout->setStretch( 1, 10 );
    integrationBoxLayout->setStretch( 2, 1 );
    integrationBoxLayout->setMargin( 5 );

    topBox_ = new QGroupBox();
    QVBoxLayout* commentBoxLayout = new QVBoxLayout( topBox_ );
    commentBoxLayout->setMargin( 5 );
    commentBoxLayout->addLayout( integrationBoxLayout );

    //timeline box
    timelineBox_ = new QGroupBox();
    timelineBox_->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
    connect( timelineBox_, SIGNAL( clicked( bool ) ), SLOT( OnTimelineChecked( bool ) ) );

    timelineDebugPortLabel_ = new QLabel();
    timelineDebugPortSpinBox_ = new QSpinBox();
    timelineDebugPortSpinBox_->setRange( 0, 65535 );
    timelineDebugPortSpinBox_->setValue( debug_.timeline.debugPort );
    connect( timelineDebugPortSpinBox_, SIGNAL( valueChanged( int ) ), SLOT( OnTimelineDebugPortChanged( int ) ) );

    QHBoxLayout* debugPortBox = new QHBoxLayout();
    debugPortBox->addWidget( timelineDebugPortLabel_ );
    debugPortBox->addWidget( timelineDebugPortSpinBox_ );

    QHBoxLayout* timelineLogLayout = new QHBoxLayout();
    timelineLogLayout->setSpacing( 10 );
    timelineLogLabel_ = new QLabel();
    timelineLog_ = new QLineEdit();
    timelineLog_->setText( QString::fromStdWString( debug_.timeline.clientLogPath.ToUnicode() ) );
    connect( timelineLog_, SIGNAL( textEdited( const QString& ) ), SLOT( OnTimelineLogChanged( const QString& ) ) );
    timelineLogLayout->addWidget( timelineLogLabel_ );
    timelineLogLayout->addWidget( timelineLog_ );

    QHBoxLayout* debugLayout = new QHBoxLayout();
    debugLayout->setSpacing( 10 );
    timelineDebugLabel_ = new QLabel();
    timelineDebug_ = new QLineEdit();
    timelineDebug_->setText( QString::fromStdWString( debug_.timeline.debugWwwDir.ToUnicode() ) );
    connect( timelineDebug_, SIGNAL( textEdited( const QString& ) ), SLOT( OnTimelineDebugChanged( const QString& ) ) );
    debugLayout->addWidget( timelineDebugLabel_ );
    debugLayout->addWidget( timelineDebug_ );

    QHBoxLayout* cefLogLayout = new QHBoxLayout();
    cefLogLayout->setSpacing( 10 );
    cefLogLabel_ = new QLabel();
    cefLog_ = new QLineEdit();
    cefLog_->setText( QString::fromStdWString( debug_.timeline.cefLog.ToUnicode() ) );
    connect( cefLog_, SIGNAL( textEdited( const QString& ) ), SLOT( OnCefLogChanged( const QString& ) ) );
    cefLogLayout->addWidget( cefLogLabel_ );
    cefLogLayout->addWidget( cefLog_ );

    oldTimeline_ = new QCheckBox();
    oldTimeline_->setChecked( debug_.timeline.legacyTimeline );
    QHBoxLayout* oldTimelineLayout = new QHBoxLayout();
    oldTimelineLayout->addWidget( oldTimeline_ );
    connect( oldTimeline_, SIGNAL( toggled( bool ) ), this, SLOT( OnTimelineChecked( bool ) ) );

    autostartEvents_ = new QCheckBox();
    autostartEvents_->setChecked( debug_.timeline.autostart );
    QHBoxLayout* autostartLayout = new QHBoxLayout();
    autostartLayout->addWidget( autostartEvents_ );
    connect( autostartEvents_, SIGNAL( toggled( bool ) ), this, SLOT( OnTimelineAutostart( bool ) ) );

    QVBoxLayout* timelineGroupLayout = new QVBoxLayout( timelineBox_ );
    timelineGroupLayout->addLayout( debugPortBox );
    timelineGroupLayout->addLayout( timelineLogLayout );
    timelineGroupLayout->addLayout( debugLayout );
    timelineGroupLayout->addLayout( cefLogLayout );
    timelineGroupLayout->addLayout( oldTimelineLayout );
    timelineGroupLayout->addLayout( autostartLayout );

    //profiling group box
    profilingBox_ = new QGroupBox();
    QGridLayout* profiling = new QGridLayout( profilingBox_, 1, 1 );
    profiling->setMargin( 10 );
    decCallsBox_ = new QCheckBox();
    decCallsBox_->setChecked( debug_.sim.decProfiling );
    connect( decCallsBox_, SIGNAL( clicked( bool ) ), SLOT( OnDecProfilingChanged( bool ) ) );
    profiling->addWidget( decCallsBox_, 0, 0 );

    //pathfinds group box
    pathfindsBox_ = new QGroupBox();
    QVBoxLayout* pathfinds = new QVBoxLayout( pathfindsBox_ );
    pathfinds->setMargin( 10 );
    QHBoxLayout* dumpLayout = new QHBoxLayout();
    dumpLayout->setSpacing( 10 );
    dumpLabel_ = new QLabel();
    dataButton_ = new QPushButton();
    connect( dataButton_, SIGNAL( clicked() ), SLOT( OnSelectDataDirectory() ) );
    dataDirectory_ = new QLineEdit();
    dataDirectory_->setText( QString::fromStdWString( debug_.sim.pathfindDumpDir.ToUnicode() ) );
    connect( dataDirectory_, SIGNAL( textEdited( const QString& ) ), SLOT( OnChangeDataDirectory() ) );
    dumpLayout->addWidget( dumpLabel_ );
    dumpLayout->addWidget( dataDirectory_ );
    dumpLayout->addWidget( dataButton_ );

    QHBoxLayout* filterLayout = new QHBoxLayout();
    filterLayout->setSpacing( 10 );
    filterLabel_ = new QLabel();
    filterEdit_ = new QLineEdit();
    filterEdit_->setText( debug_.sim.pathfindFilter.c_str() );
    filterLayout->addWidget( filterLabel_ );
    filterLayout->addWidget( filterEdit_ );
    connect( filterEdit_, SIGNAL( editingFinished() ), SLOT( OnChangeDataDirectory() ) );

    pathfinds->addLayout( dumpLayout );
    pathfinds->addLayout( filterLayout );

    //mapnik group box
    mapnikBox_ = new QGroupBox();
    QGridLayout* mapnik = new QGridLayout( mapnikBox_, 2, 1 );
    mapnik->setMargin( 10 );
    mapnikLayerBox_ = new QCheckBox();
    mapnikLayerBox_->setChecked( debug_.gaming.hasMapnik );
    connect( mapnikLayerBox_, SIGNAL( clicked( bool ) ), SLOT( OnMapnikLayerChecked( bool ) ) );
    mapnikThreads_ = new QSpinBox();
    mapnikThreads_->setRange( 0, 128 );
    mapnikThreads_->setValue( debug_.gaming.mapnikThreads );
    connect( mapnikThreads_, SIGNAL( valueChanged( int ) ), SLOT( OnMapnikThreadsChanged( int ) ) );
    mapnikThreadsLabel_ = new QLabel();
    mapnik->addWidget( mapnikLayerBox_, 0, 0 );
    mapnik->addWidget( mapnikThreadsLabel_, 1, 0 );
    mapnik->addWidget( mapnikThreads_, 1, 1 );

    // development features
    const auto& availableFeatures = tools::GetAvailableFeatures();
    featuresBox_ = new QGroupBox();
    featuresBox_->setTitle( "Features" );
    QVBoxLayout* featuresLayout = new QVBoxLayout( featuresBox_ );
    for( auto it = availableFeatures.begin(); it != availableFeatures.end(); ++it )
    {
        QCheckBox* checkbox = new QCheckBox( QString::fromStdString( *it ) );
        checkbox->setChecked( debug_.features.count( *it ) != 0 );
        featuresLayout->addWidget( checkbox );
        features_.push_back( checkbox );
        connect( checkbox, SIGNAL( stateChanged( int ) ), SLOT( OnDevFeaturesChanged() ));
    }

    //general Layout
    QGroupBox* box = new QGroupBox();
    QVBoxLayout* layout = new QVBoxLayout( box );
    layout->addWidget( topBox_ );
    layout->addWidget( timelineBox_ );
    layout->addWidget( profilingBox_ );
    layout->addWidget( pathfindsBox_ );
    layout->addWidget( mapnikBox_ );
    layout->addWidget( featuresBox_ );
    layout->setAlignment( Qt::AlignTop );
    QScrollArea* scroll = new QScrollArea();
    scroll->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    scroll->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
    scroll->setWidgetResizable( true );
    scroll->setFrameStyle( QFrame::Box | QFrame::Sunken );
    scroll->setWidget( box );
    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    mainLayout->addWidget( scroll );
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
// Name: DebugConfigPanel::OnChangeIntegrationDirectory
// Created: NPT 2013-01-03
// -----------------------------------------------------------------------------
void DebugConfigPanel::OnChangeIntegrationDirectory()
{
    const tools::Path directory = gui::FileDialog::getExistingDirectory( this ,
            QString::fromStdWString( debug_.sim.integrationDir.ToUnicode() ) );
    integrationDir_->setText( QString::fromStdWString( directory.ToUnicode() ) );
    OnEditIntegrationDirectory( integrationDir_->text() );
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::OnEditIntegrationDirectory
// Created: NPT 2013-01-03
// -----------------------------------------------------------------------------
void DebugConfigPanel::OnEditIntegrationDirectory( const QString& directory )
{
    tools::Path path = tools::Path::FromUnicode( directory.toStdWString() );
    if( !path.IsEmpty() && !path.IsDirectory() )
        return;
    debug_.sim.integrationDir = path;
    frontend::SaveDebugConfig( debug_ );
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::OnLanguageChanged
// Created: LGY 2013-02-05
// -----------------------------------------------------------------------------
void DebugConfigPanel::OnLanguageChanged()
{
    timelineBox_->setTitle( tr( "Timeline" ) );
    timelineDebugPortLabel_->setText( tr( "Debug port" ) );
    timelineLogLabel_->setText( tr( "Client log file" ) );
    timelineDebugLabel_->setText( tr( "Debug directory" ) );
    cefLogLabel_->setText( tr( "Chrome embedded log file" ) );
    oldTimeline_->setText( tr( "Enable legacy timeline" ) );
    autostartEvents_->setText( tr( "Automatically start events" ) );
    integrationLabel_->setText( tr( "Integration layer directory" ) );
    profilingBox_->setTitle( tr( "Profiling settings" ) );
    decCallsBox_->setText( tr( "Decisional functions" ) );
    pathfindsBox_->setTitle( tr( "Pathfind settings" ) );
    filterLabel_->setText( tr( "Filter :" ) );
    dumpLabel_->setText( tr( "Dump pathfinds directory :" ) );
    mapnikBox_->setTitle( tr( "Mapnik settings" ) );
    mapnikLayerBox_->setText( tr( "Activate layer" ) );
    mapnikThreadsLabel_->setText( tr( "Rendering threads" ) );
    dataButton_->setText( "..." );
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::GetName
// Created: LGY 2013-02-05
// -----------------------------------------------------------------------------
QString DebugConfigPanel::GetName() const
{
    return tr( "Debug" );
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::OnChangeDataDirectory
// Created: LGY 2013-02-05
// -----------------------------------------------------------------------------
void DebugConfigPanel::OnChangeDataDirectory()
{
    debug_.sim.pathfindFilter = filterEdit_->text().toStdString();
    debug_.sim.pathfindDumpDir = tools::Path::FromUnicode( dataDirectory_->text().toStdWString() );
    frontend::SaveDebugConfig( debug_ );
}

void DebugConfigPanel::OnSelectDataDirectory()
{
    const tools::Path directory = gui::FileDialog::getExistingDirectory( this , "", tools::Path::FromUnicode( dataDirectory_->text().toStdWString() ) );
    if( directory.IsEmpty() )
        return;
    dataDirectory_->setText( QString::fromStdWString( directory.ToUnicode() ) );
    OnChangeDataDirectory();
}

void DebugConfigPanel::OnDecProfilingChanged( bool checked )
{
    debug_.sim.decProfiling = checked;
    frontend::SaveDebugConfig( debug_ );
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::OnTimelineChecked
// Created: BAX 2013-04-16
// -----------------------------------------------------------------------------
void DebugConfigPanel::OnTimelineChecked( bool checked )
{
    debug_.timeline.legacyTimeline = checked;
    frontend::SaveDebugConfig( debug_ );
}

void DebugConfigPanel::OnTimelineAutostart( bool checked )
{
    debug_.timeline.autostart = checked;
    frontend::SaveDebugConfig( debug_ );
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::OnTimelineDebugPortChanged
// Created: ABR 2013-05-16
// -----------------------------------------------------------------------------
void DebugConfigPanel::OnTimelineDebugPortChanged( int port )
{
    debug_.timeline.debugPort = port;
    frontend::SaveDebugConfig( debug_ );
}

void DebugConfigPanel::OnTimelineLogChanged( const QString& path )
{
    debug_.timeline.clientLogPath = tools::Path::FromUnicode( path.trimmed().toStdWString() );
    frontend::SaveDebugConfig( debug_ );
}

void DebugConfigPanel::OnTimelineDebugChanged( const QString& path )
{
    debug_.timeline.debugWwwDir = tools::Path::FromUnicode( path.trimmed().toStdWString() );
    frontend::SaveDebugConfig( debug_ );
}

void DebugConfigPanel::OnCefLogChanged( const QString& path )
{
    debug_.timeline.cefLog = tools::Path::FromUnicode( path.trimmed().toStdWString() );
    frontend::SaveDebugConfig( debug_ );
}

void DebugConfigPanel::OnMapnikLayerChecked( bool checked )
{
    debug_.gaming.hasMapnik = checked;
    frontend::SaveDebugConfig( debug_ );
}

void DebugConfigPanel::OnMapnikThreadsChanged( int threads )
{
    debug_.gaming.mapnikThreads = threads;
    frontend::SaveDebugConfig( debug_ );
}

void DebugConfigPanel::OnDevFeaturesChanged()
{
    debug_.features.clear();
    for( auto it = features_.begin(); it != features_.end(); ++it )
        if( (*it)->isChecked() )
            debug_.features.insert( (*it)->text() );
    frontend::SaveDebugConfig( debug_ );
}

