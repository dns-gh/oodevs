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
#include "Registry.h"

#include "clients_gui/FileDialog.h"

#include "clients_kernel/tools.h"

#include "frontend/CommandLineTools.h"
#include "frontend/CreateSession.h"
#include "tools/GeneralConfig.h"

#include <QScrollArea>

namespace
{
    const int maxIntegrationDir = 5;
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel constructor
// Created: NPT 2013-01-03
// -----------------------------------------------------------------------------
DebugConfigPanel::DebugConfigPanel( QWidget* parent, const tools::GeneralConfig& config )
    : PluginConfig_ABC( parent )
    , visible_( !!parent )
    , config_( config )
    , profilingBox_( 0 )
    , decCallsBox_( 0 )
    , pathfindsBox_( 0 )
    , filterLabel_( 0 )
    , dumpLabel_( 0 )
    , dataDirectory_( 0 )
    , dataButton_( 0 )
    , exerciseNumber_( 1 )
    , mapnikBox_( 0 )
    , mapnikLayerBox_( 0 )
{
    //integration level label
    integrationLabel_ = new QLabel();

    //integration level combobox
    integrationComboBox_ = new QComboBox();
    integrationComboBox_->setEditable( true );
    QString pathValue = registry::ReadString( "IntegrationLayerPaths" );
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
    commentBoxLayout->addLayout( integrationBoxLayout );

    //timeline box
    timelineBox_ = new QGroupBox();
    timelineBox_->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
    connect( timelineBox_, SIGNAL( clicked( bool ) ), SLOT( OnTimelineChecked( bool ) ) );

    timelineDebugPortLabel_ = new QLabel();
    timelineDebugPortSpinBox_ = new QSpinBox();
    timelineDebugPortSpinBox_->setRange( 0, 65535 );
    timelineDebugPortSpinBox_->setValue( registry::ReadInt( "TimelineDebugPort" ) );
    connect( timelineDebugPortSpinBox_, SIGNAL( valueChanged( int ) ), SLOT( OnTimelineDebugPortChanged( int ) ) );

    QHBoxLayout* debugPortBox = new QHBoxLayout();
    debugPortBox->addWidget( timelineDebugPortLabel_ );
    debugPortBox->addWidget( timelineDebugPortSpinBox_ );

    QHBoxLayout* timelineLogLayout = new QHBoxLayout();
    timelineLogLayout->setSpacing( 10 );
    timelineLogLabel_ = new QLabel();
    timelineLog_ = new QLineEdit();
    timelineLog_->setText( registry::ReadString( "TimelineClientLog" ) );
    connect( timelineLog_, SIGNAL( textEdited( const QString& ) ), SLOT( OnTimelineLogChanged( const QString& ) ) );
    timelineLogLayout->addWidget( timelineLogLabel_ );
    timelineLogLayout->addWidget( timelineLog_ );

    oldTimeline_ = new QCheckBox();
    oldTimeline_->setChecked( registry::ReadBool( "EnableLegacyTimeline" ) );
    QHBoxLayout* oldTimelineLayout = new QHBoxLayout();
    oldTimelineLayout->addWidget( oldTimeline_ );
    connect( oldTimeline_, SIGNAL( toggled( bool ) ), this, SLOT( OnTimelineChecked( bool ) ) );

    QVBoxLayout* timelineGroupLayout = new QVBoxLayout( timelineBox_ );
    timelineGroupLayout->addLayout( debugPortBox );
    timelineGroupLayout->addLayout( timelineLogLayout );
    timelineGroupLayout->addLayout( oldTimelineLayout );

    //profiling group box
    profilingBox_ = new QGroupBox();
    QGridLayout* profiling = new QGridLayout( profilingBox_, 1, 1 );
    profiling->setMargin( 10 );
    decCallsBox_ = new QCheckBox();
    profiling->addWidget( decCallsBox_, 0, 0 );

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

    //mapnik group box
    mapnikBox_ = new QGroupBox();
    QGridLayout* mapnik = new QGridLayout( mapnikBox_, 1, 1 );
    mapnik->setMargin( 10 );
    mapnikLayerBox_ = new QCheckBox();
    mapnikLayerBox_->setChecked( registry::ReadBool( "HasMapnikLayer" ) );
    connect( mapnikLayerBox_, SIGNAL( clicked( bool ) ), SLOT( OnMapnikLayerChecked( bool ) ) );
    mapnik->addWidget( mapnikLayerBox_, 0, 0 );

    // development features
    const auto& availableFeatures = tools::GetAvailableFeatures();
    const auto savedFeatures = tools::SplitFeatures(
            registry::ReadFeatures().toStdString() );
    featuresBox_ = new QGroupBox();
    featuresBox_->setTitle( "Features" );
    QVBoxLayout* featuresLayout = new QVBoxLayout( featuresBox_ );
    for( auto it = availableFeatures.begin(); it != availableFeatures.end(); ++it )
    {
        QCheckBox* checkbox = new QCheckBox( QString::fromStdString( *it ) );
        checkbox->setChecked( savedFeatures.count( *it ) != 0 );
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
            registry::WriteString( "IntegrationLayerPaths", pathList_.join( ";" ) );
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
    timelineBox_->setTitle( tools::translate( "DebugConfigPanel", "Timeline" ) );
    timelineDebugPortLabel_->setText( tools::translate( "DebugConfigPanel", "Debug port" ) );
    timelineLogLabel_->setText( tools::translate( "DebugConfigPanel", "Client log file" ) );
    oldTimeline_->setText( tools::translate( "DebugConfigPanel", "Enable legacy timeline" ) );
    integrationLabel_->setText( tools::translate( "DebugConfigPanel", "Integration layer directory" ) );
    profilingBox_->setTitle( tools::translate( "DebugConfigPanel", "Profiling settings" ) );
    decCallsBox_->setText( tools::translate( "DebugConfigPanel", "Decisional functions" ) );
    pathfindsBox_->setTitle( tools::translate( "DebugConfigPanel", "Pathfind settings" ) );
    filterLabel_->setText( tools::translate( "DebugConfigPanel", "Filter :" ) );
    dumpLabel_->setText( tools::translate( "DebugConfigPanel", "Dump pathfinds directory :" ) );
    mapnikBox_->setTitle( tools::translate( "DebugConfigPanel", "Mapnik settings" ) );
    mapnikLayerBox_->setText( tools::translate( "DebugConfigPanel", "Activate layer" ) );
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
// Name: DebugConfigPanel::IsVisible
// Created: BAX 2013-11-25
// -----------------------------------------------------------------------------
bool DebugConfigPanel::IsVisible() const
{
    return visible_;
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::Commit
// Created: LGY 2013-02-05
// -----------------------------------------------------------------------------
void DebugConfigPanel::Commit( const tools::Path& exercise, const tools::Path& session )
{
    frontend::CreateSession action( config_, exercise, session );
    action.SetOption( "session/config/timeline/@debug-port", timelineDebugPortSpinBox_->value() );
    if( !timelineLog_->text().isEmpty() )
    {
        auto xml = frontend::ConfigurationManipulator::GetSessionXml( config_, exercise, session );
        xml = xml.Absolute();
        const auto log = xml.Parent() / tools::Path::FromUTF8( timelineLog_->text().toStdString() );
        action.SetOption( "session/config/timeline/@client-log", log.ToUTF8() );
    }
    action.SetOption( "session/config/timeline/@enabled", oldTimeline_->isChecked() );
    if( decCallsBox_->isChecked() )
        action.SetOption( "session/config/simulation/profiling/@decisional", "true" );
    if( mapnikLayerBox_->isChecked() )
        action.SetOption( "session/config/gaming/mapnik/@activate", "true" );
    action.Commit();
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
    registry::WriteBool( "EnableLegacyTimeline", checked );
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::OnTimelineDebugPortChanged
// Created: ABR 2013-05-16
// -----------------------------------------------------------------------------
void DebugConfigPanel::OnTimelineDebugPortChanged( int port )
{
    registry::WriteInt( "TimelineDebugPort", port );
}

void DebugConfigPanel::OnTimelineLogChanged( const QString& path )
{
    registry::WriteString( "TimelineClientLog", path );
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::OnExerciseNumberChanged
// Created: ABR 2013-05-16
// -----------------------------------------------------------------------------
void DebugConfigPanel::OnExerciseNumberChanged( int exerciseNumber )
{
    exerciseNumber_ = exerciseNumber;
}

void DebugConfigPanel::OnMapnikLayerChecked( bool checked )
{
    registry::WriteBool( "HasMapnikLayer", checked );
}

void DebugConfigPanel::OnDevFeaturesChanged()
{
    registry::WriteString( "DevFeatures", GetDevFeatures() );
}

QString DebugConfigPanel::GetDevFeatures() const
{
    std::unordered_set< std::string > checked;
    for( auto it = features_.begin(); it != features_.end(); ++it )
        if( (*it)->isChecked() )
            checked.insert( (*it)->text() );
    return tools::JoinFeatures( checked ).c_str();
}
