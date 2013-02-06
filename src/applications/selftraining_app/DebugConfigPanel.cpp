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
#include "clients_gui/tools.h"
#include "frontend/CreateSession.h"

namespace
{
    const int maxIntegrationDir = 5;

    bool ReadBoolRegistry( const std::string& key )
    {
        QSettings settings( "MASA Group", "SWORD" );
        return settings.value( ( "/sword/" + key ).c_str() ).toBool();
    }
    void WriteBoolRegistry( const std::string& key, bool value )
    {
        QSettings settings( "MASA Group", "SWORD" );
        settings.setValue( ( "/sword/" + key ).c_str(), value );
    }

    QString ReadStringRegistryValue( const std::string& key )
    {
        QSettings settings( "MASA Group", "SWORD" );
        return settings.value( ( "/sword/" + key ).c_str(), "" ).toString();
    }

    void WriteStringRegistryValue( const std::string& key, QString stringValue )
    {
        QSettings settings( "MASA Group", "SWORD" );
        settings.setValue( ( "/sword/" + key ).c_str(), stringValue );
    }
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel constructor
// Created: NPT 2013-01-03
// -----------------------------------------------------------------------------
DebugConfigPanel::DebugConfigPanel( QWidget* parent, const tools::GeneralConfig& config )
    : PluginConfig_ABC( parent )
    , config_( config )
    , pathList_( new QStringList() )
    , profilingBox_( 0 )
    , decCallsBox_( 0 )
    , commandsBox_( 0 )
    , hooksBox_( 0 )
    , pathfindsBox_( 0 )
    , filterLabel_( 0 )
    , dumpLabel_( 0 )
    , dataDirectory_( 0 )
    , dataButton_( 0 )
{
    //legacy box
    legacyLabel_ = new QLabel();

    legacyCheckBox_ = new QCheckBox();
    legacyCheckBox_->setCheckable( true );
    legacyCheckBox_->setChecked( ReadBoolRegistry( "IsLegacy" ) );
    connect( legacyCheckBox_, SIGNAL( clicked ( bool ) ), SLOT( SwordVersionChecked( bool ) ) );

    legacyBox_ = new QGroupBox();
    QHBoxLayout* commentBoxLayout = new QHBoxLayout( legacyBox_ );
    commentBoxLayout->addWidget( legacyLabel_ );
    commentBoxLayout->addWidget( legacyCheckBox_ );
    commentBoxLayout->setMargin( 5 );

    //integration level label
    integrationLabel_ = new QLabel();

    //integration level combobox
    integrationComboBox_ = new QComboBox();
    integrationComboBox_->setEditable( true );
    QString pathValue = ReadStringRegistryValue( "IntegrationLayerPaths" );
    if( !pathValue.isEmpty() )
    {
        pathList_ = new QStringList( QStringList::split( ";", pathValue ) );
        integrationComboBox_->addItems( *pathList_ );
    }
    connect( integrationComboBox_, SIGNAL( editTextChanged( const QString& ) ), SLOT( OnEditIntegrationDirectory( const QString& ) ) );

    //integration level button
    integrationButton_ = new QPushButton();
    integrationButton_->setText( "..." );
    connect( integrationButton_, SIGNAL( clicked() ), SLOT( OnChangeIntegrationDirectory() ) );

    //integration level group box
    integrationBox_ = new QGroupBox();
    QHBoxLayout* integrationBoxLayout = new QHBoxLayout( integrationBox_ );
    integrationBoxLayout->addWidget( integrationLabel_ );
    integrationBoxLayout->addWidget( integrationComboBox_ );
    integrationBoxLayout->addWidget( integrationButton_ );
    integrationBoxLayout->setStretch( 0, 2 );
    integrationBoxLayout->setStretch( 1, 10 );
    integrationBoxLayout->setStretch( 2, 1 );
    integrationBoxLayout->setMargin( 5 );

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
    mainLayout->addWidget( legacyBox_ );
    mainLayout->addWidget( integrationBox_ );
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
void DebugConfigPanel::SwordVersionChecked( bool state )
{
    WriteBoolRegistry( "IsLegacy", state );
    emit SwordVersionSelected( state );
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::OnChangeIntegrationDirectory
// Created: NPT 2013-01-03
// -----------------------------------------------------------------------------
void DebugConfigPanel::OnChangeIntegrationDirectory()
{
    const QString directory = QDir::convertSeparators( QFileDialog::getExistingDirectory( this , "" ) );
    integrationComboBox_->setCurrentText( directory );
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::OnEditIntegrationDirectory
// Created: NPT 2013-01-03
// -----------------------------------------------------------------------------
void DebugConfigPanel::OnEditIntegrationDirectory( const QString& directory )
{
    if( boost::filesystem::is_directory( directory.toStdString() ) )
    {
        if ( !pathList_->contains( directory, Qt::CaseSensitive ) )
        {
            //maj of combobox
            pathList_->push_front( directory );
            if( pathList_->count() > maxIntegrationDir )
                pathList_->removeLast();
            integrationComboBox_->clear();
            integrationComboBox_->addItems( *pathList_ );

            //save in registry
            WriteStringRegistryValue( "IntegrationLayerPaths", pathList_->join( ";" ) );
        }
    }
    emit IntegrationPathSelected( directory );
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::OnLanguageChanged
// Created: LGY 2013-02-05
// -----------------------------------------------------------------------------
void DebugConfigPanel::OnLanguageChanged()
{
    legacyLabel_->setText( tools::translate( "DebugConfigPanel", "Enable Legacy Mode" ) );
    integrationLabel_->setText( tools::translate( "DebugConfigPanel", "Integration layer directory" ) );
    profilingBox_->setTitle( tools::translate( "DebugConfigPanel", "Profiling settings" ) );
    decCallsBox_->setText( tools::translate( "DebugConfigPanel", "Decisional function calls" ) );
    commandsBox_->setText( tools::translate( "DebugConfigPanel", "Commands start / stop" ) );
    hooksBox_->setText( tools::translate( "DebugConfigPanel", "Hooks function calls" ) );
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
void DebugConfigPanel::Commit( const std::string& exercise, const std::string& session )
{
    frontend::CreateSession action( config_, exercise, session );
    if( decCallsBox_->isChecked() )
        action.SetOption( "session/config/simulation/profiling/@decisional", "true" );
    if( commandsBox_->isChecked() )
        action.SetOption( "session/config/simulation/profiling/@command", "true" );
    if( hooksBox_->isChecked() )
        action.SetOption( "session/config/simulation/profiling/@hook", "true" );
    action.Commit();
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::OnChangeDataDirectory
// Created: LGY 2013-02-05
// -----------------------------------------------------------------------------
void DebugConfigPanel::OnChangeDataDirectory()
{
    const QString directory = QDir::convertSeparators( QFileDialog::getExistingDirectory( this , "", dataDirectory_->text() ) );
    if( directory.isEmpty() )
        return;
    dataDirectory_->setText( directory );
    emit DumpPathfindOptionsChanged( filterEdit_->text(), dataDirectory_->text() );
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::OnChangeDataFilter
// Created: LGY 2013-02-06
// -----------------------------------------------------------------------------
void DebugConfigPanel::OnChangeDataFilter()
{
    emit DumpPathfindOptionsChanged( filterEdit_->text(), dataDirectory_->text() );
}
