// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "LogConfigPanel.h"
#include "moc_LogConfigPanel.cpp"
#include "CreateSession.h"
#include "clients_kernel/Tools.h"
#pragma warning( push, 0 )
#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qlabel.h>
#include <QtCore/qsettings.h>
#include <QtGui/qspinbox.h>
#pragma warning( pop )

using namespace frontend;

namespace
{
    enum EFileSizeTypes
    {
        eMB,
        eKB,
        eLines
    };

    enum ELevelTypes
    {
        eWarning,
        eInfo,
        eAll
    };

    bool ReadHasRotatingLogs()
    {
        QSettings settings( "MASA Group", "SWORD" );
        return settings.value( "/sword/LogsRotation", true ).toBool();
    }

    int ReadMaxFileSize()
    {
        QSettings settings( "MASA Group", "SWORD" );
        return settings.value( "/sword/LogsMaxFileSize", 100 ).toInt();
    }

    int ReadFileSizeType()
    {
        QSettings settings( "MASA Group", "SWORD" );
        return std::min( 2, std::max( 0, settings.value( "/sword/LogsFileSizeType", 0 ).toInt() ) );
    }

    bool ReadKeepLogs()
    {
        QSettings settings( "MASA Group", "SWORD" );
        return settings.value( "/sword/LogsKeep", false ).toBool();
    }

    int ReadKeepLogsNumber()
    {
        QSettings settings( "MASA Group", "SWORD" );
        return std::max( 1, settings.value( "/sword/LogsKeepNumber", 5 ).toInt() );
    }

    int ReadLevel()
    {
        QSettings settings( "MASA Group", "SWORD" );
        return std::min( 2, std::max( 0, settings.value( "/sword/LogsLevel", 2 ).toInt() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LogConfigPanel constructor
// Created: JSR 2014-09-11
// -----------------------------------------------------------------------------
LogConfigPanel::LogConfigPanel( QWidget* parent, const tools::GeneralConfig& config )
    : PluginConfig_ABC( parent )
    , config_( config )
{
    rotateLogsGroup_ = new QGroupBox();
    rotateLogsGroup_->setCheckable( true );
    fileSizeLabel_ = new QLabel();
    fileSizeSpinBox_ = new QSpinBox();
    fileSizeSpinBox_->setRange( 1, std::numeric_limits<int>::max() );
    sizeTypeComboBox_ = new QComboBox();
    keepCheckBox_ = new QCheckBox();
    keepSpinBox_ = new QSpinBox();
    keepSpinBox_->setRange( 1, std::numeric_limits<int>::max() );
    keepLabel_ = new QLabel();
    levelLabel_ = new QLabel();
    levelComboBox_ = new QComboBox();

    QGridLayout* groupLayout = new QGridLayout( rotateLogsGroup_ );
    groupLayout->addWidget( fileSizeLabel_, 0, 0 );
    groupLayout->addWidget( fileSizeSpinBox_, 0, 1 );
    groupLayout->addWidget( sizeTypeComboBox_, 0, 2 );
    groupLayout->addWidget( keepCheckBox_, 1, 0 );
    groupLayout->addWidget( keepSpinBox_, 1, 1 );
    groupLayout->addWidget( keepLabel_, 1, 2 );

    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget( levelLabel_ );
    layout->addWidget( levelComboBox_ );
    layout->addWidget( new QWidget() );

    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    mainLayout->addWidget( rotateLogsGroup_ );
    mainLayout->addLayout( layout );
    mainLayout->addStretch();
    mainLayout->setMargin( 5 );

    QStringList items;
    items << "" << "" << "";
    sizeTypeComboBox_->addItems( items );
    levelComboBox_->addItems( items );

    rotateLogsGroup_->setChecked( ReadHasRotatingLogs() );
    fileSizeSpinBox_->setValue( ReadMaxFileSize() );
    sizeTypeComboBox_->setCurrentIndex( ReadFileSizeType() );
    keepCheckBox_->setChecked( ReadKeepLogs() );
    keepSpinBox_->setValue( ReadKeepLogsNumber() );
    keepSpinBox_->setEnabled( keepCheckBox_->isChecked() );
    levelComboBox_->setCurrentIndex( ReadLevel() );

    connect( keepCheckBox_, SIGNAL( toggled( bool ) ), SLOT( OnKeepToggled( bool ) ) );

    connect( rotateLogsGroup_,  SIGNAL( toggled( bool ) ),     SLOT( OnWriteSettings() ) );
    connect( fileSizeSpinBox_,  SIGNAL( valueChanged( int ) ), SLOT( OnWriteSettings() ) );
    connect( sizeTypeComboBox_, SIGNAL( activated( int ) ),    SLOT( OnWriteSettings() ) );
    connect( keepCheckBox_,     SIGNAL( toggled( bool ) ),     SLOT( OnWriteSettings() ) );
    connect( keepSpinBox_,      SIGNAL( valueChanged( int ) ), SLOT( OnWriteSettings() ) );
    connect( levelComboBox_,    SIGNAL( activated( int ) ),    SLOT( OnWriteSettings() ) );
}

// -----------------------------------------------------------------------------
// Name: LogConfigPanel destructor
// Created: JSR 2014-09-11
// -----------------------------------------------------------------------------
LogConfigPanel::~LogConfigPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogConfigPanel::OnLanguageChanged
// Created: JSR 2014-09-11
// -----------------------------------------------------------------------------
void LogConfigPanel::OnLanguageChanged()
{
    rotateLogsGroup_->setTitle( tools::translate( "LogConfigPanel", "Logs Rotation" ) );
    fileSizeLabel_->setText( tools::translate( "LogConfigPanel", "Max File Size" ) );

    int currentIndex = ( sizeTypeComboBox_->count() ) ? sizeTypeComboBox_->currentIndex() : 0;
    sizeTypeComboBox_->clear();
    sizeTypeComboBox_->addItem( tools::translate( "LogConfigPanel", "MB" ), eMB );
    sizeTypeComboBox_->addItem( tools::translate( "LogConfigPanel", "kB" ), eKB );
    sizeTypeComboBox_->addItem( tools::translate( "LogConfigPanel", "Lines" ), eLines );
    sizeTypeComboBox_->setCurrentIndex( currentIndex );

    keepCheckBox_->setText( tools::translate( "LogConfigPanel", "Keep" ) );
    keepLabel_->setText( tools::translate( "LogConfigPanel", "Files" ) );
    levelLabel_->setText( tools::translate( "LogConfigPanel", "Level" ) );
    
    currentIndex = ( levelComboBox_->count() ) ? levelComboBox_->currentIndex() : 0;
    levelComboBox_->clear();
    levelComboBox_->addItem( tools::translate( "LogConfigPanel", "Warning" ), eWarning );
    levelComboBox_->addItem( tools::translate( "LogConfigPanel", "Info" ), eInfo );
    levelComboBox_->addItem( tools::translate( "LogConfigPanel", "All" ), eAll );
    levelComboBox_->setCurrentIndex( currentIndex );
}

// -----------------------------------------------------------------------------
// Name: LogConfigPanel::GetName
// Created: JSR 2014-09-11
// -----------------------------------------------------------------------------
QString LogConfigPanel::GetName() const
{
    return tools::translate( "LogConfigPanel", "Logs" );
}

namespace
{
    std::string GetSizeUnit( int unit )
    {
        // @see ExerciseConfig::LogSettings::SetLogSettings
        switch( unit )
        {
        case eMB:
            return "mbytes";
        case eKB:
            return "kbytes";
        case eLines:
        default:
            return "lines";
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogConfigPanel::Commit
// Created: JSR 2014-09-11
// -----------------------------------------------------------------------------
void LogConfigPanel::Commit( frontend::CreateSession& action, const std::string& path ) const
{
    action.SetOption( path + "@loglevel", levelComboBox_->currentIndex() );
    if( rotateLogsGroup_->isChecked() )
    {
        action.SetOption( path + "@logfiles", keepCheckBox_->isChecked() ? keepSpinBox_->value() : std::numeric_limits< int >::max() );
        action.SetOption( path + "@logsize", fileSizeSpinBox_->value() );
        action.SetOption( path + "@sizeunit", GetSizeUnit( sizeTypeComboBox_->currentIndex() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LogConfigPanel::Commit
// Created: JSR 2014-09-11
// -----------------------------------------------------------------------------
void LogConfigPanel::Commit( const tools::Path& exercise, const tools::Path& session )
{
    frontend::CreateSession action( config_, exercise, session );
    Commit( action, "session/config/simulation/debug/" );
    Commit( action, "session/config/dispatcher/log/" );
    Commit( action, "session/config/dispatcher/messages/" );
    Commit( action, "session/config/dispatcher/debug/" );
    action.Commit();
}

// -----------------------------------------------------------------------------
// Name: LogConfigPanel::OnKeepToggled
// Created: JSR 2014-09-11
// -----------------------------------------------------------------------------
void LogConfigPanel::OnKeepToggled( bool checked )
{
    keepSpinBox_->setEnabled( checked );
}

// -----------------------------------------------------------------------------
// Name: RandomPluginConfigPanel::OnWriteSettings
// Created: JSR 2014-09-11
// -----------------------------------------------------------------------------
void LogConfigPanel::OnWriteSettings()
{
    QSettings settings( "MASA Group", "SWORD" );
    settings.setValue( "/sword/LogsRotation", rotateLogsGroup_->isChecked() );
    settings.setValue( "/sword/LogsMaxFileSize", fileSizeSpinBox_->value() );
    settings.setValue( "/sword/LogsFileSizeType", sizeTypeComboBox_->currentIndex() );
    settings.setValue( "/sword/LogsKeep", keepCheckBox_->isChecked() );
    settings.setValue( "/sword/LogsKeepNumber", keepSpinBox_->value() );
    settings.setValue( "/sword/LogsLevel", levelComboBox_->currentIndex() );
}
