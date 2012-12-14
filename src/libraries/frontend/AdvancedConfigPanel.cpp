// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "AdvancedConfigPanel.h"
#include "moc_AdvancedConfigPanel.cpp"
#include "clients_gui/tools.h"
#include "frontend/CreateSession.h"
#include <QtGui/qcheckbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qspinbox.h>
#include <QtCore/qsettings.h>

using namespace frontend;

namespace
{
    bool ReadRegistry( const std::string& key )
    {
        QSettings settings( "MASA Group", "SWORD" );
        return settings.readBoolEntry( ( "/sword/" + key ).c_str(), false );
    }
    void WriteRegistry( const std::string& key, bool value )
    {
        QSettings settings( "MASA Group", "SWORD" );
        settings.writeEntry( ( "/sword/" + key ).c_str(), value );
    }
}

// -----------------------------------------------------------------------------
// Name: AdvancedConfigPanel constructor
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
AdvancedConfigPanel::AdvancedConfigPanel( QWidget* parent, const tools::GeneralConfig& config )
    : PluginConfig_ABC( parent )
    , config_( config )
{
    //time box

    //---
    stepLabel_ = new QLabel();
    stepSpin_ = new QSpinBox();
    stepSpin_->setRange( 1, 100 );
    stepSpin_->setLineStep( 1 );
    stepSpin_->setValue( 10 );

    //---
    factorLabel_ = new QLabel();
    factorSpin_ = new QSpinBox();
    factorSpin_->setRange( 1, 100 );
    factorSpin_->setLineStep( 1 );
    factorSpin_->setValue( 10 );

    //---
    endtickLabel_ = new QLabel();
    endtickSpin_ = new QSpinBox();
    endtickSpin_->setRange( 0, std::numeric_limits< int >::max() );
    endtickSpin_->setLineStep( 1 );
    endtickSpin_->setValue( 0 );

    //---
    pausedLabel_ = new QLabel();
    pausedCheckBox_ = new QCheckBox();
    pausedCheckBox_->setChecked( false );

    //---
    timeBox_ = new QGroupBox();
    QGridLayout* timeBoxLayout = new QGridLayout( timeBox_ );
    timeBoxLayout->addWidget( stepLabel_, 0, 0, 1, 1 );
    timeBoxLayout->addWidget( stepSpin_, 0, 1, 1, 1 );
    timeBoxLayout->addWidget( factorLabel_, 0, 2, 1, 1 );
    timeBoxLayout->addWidget( factorSpin_, 0, 3, 1, 1 );
    timeBoxLayout->addWidget( endtickLabel_, 1, 0, 1, 1 );
    timeBoxLayout->addWidget( endtickSpin_, 1, 1, 1, 1 );
    timeBoxLayout->addWidget( pausedLabel_, 1, 2, 1, 1 );
    timeBoxLayout->addWidget( pausedCheckBox_, 1, 3, 1, 1 );


    // pathfind box
    pathThreadsLabel_ = new QLabel();
    pathThreadsSpin_ = new QSpinBox();
    pathThreadsSpin_->setRange( 0, 4 );
    pathThreadsSpin_->setLineStep( 1 );
    pathThreadsSpin_->setValue( 1 );

    QWidget* threadBox = new QWidget();
    QHBoxLayout* threadBoxLayout = new QHBoxLayout( threadBox );
    threadBoxLayout->addWidget( pathThreadsLabel_ );
    threadBoxLayout->addWidget( pathThreadsSpin_ );

    pathfindBox_ = new QGroupBox();
    QHBoxLayout* pathfindBoxLayout = new QHBoxLayout( pathfindBox_ );
    pathfindBoxLayout->addWidget( threadBox );

    //record box
    fragmentsFrequencyLabel_ = new QLabel();
    fragmentsFrequencySpin_ = new QSpinBox();
    stepSpin_->setRange( 0, std::numeric_limits< int >::max() );
    stepSpin_->setLineStep( 1 );
    fragmentsFrequencySpin_->setValue( 200 );

    QWidget* freqBox = new QWidget();
    QHBoxLayout* freqBoxLayout = new QHBoxLayout( freqBox );
    freqBoxLayout->addWidget( fragmentsFrequencyLabel_ );
    freqBoxLayout->addWidget( fragmentsFrequencySpin_ );

    recordBox_ = new QGroupBox();
    QHBoxLayout* recordBoxLayout = new QHBoxLayout( recordBox_ );
    recordBoxLayout->addWidget( freqBox );

    //client box
    noClientLabel_ = new QLabel();

    noClientCheckBox_ = new QCheckBox();
    noClientCheckBox_->setChecked( false );
    connect( noClientCheckBox_, SIGNAL( stateChanged ( int ) ), SLOT( NoClientChecked( int ) ) );

    clientBox_ = new QGroupBox();
    QHBoxLayout* clientBoxLayout = new QHBoxLayout( clientBox_ );
    clientBoxLayout->addWidget( noClientLabel_ );
    clientBoxLayout->addWidget( noClientCheckBox_ );
    clientBoxLayout->setMargin( 5 );

    //legacy box
    legacyLabel_ = new QLabel();

    legacyCheckBox_ = new QCheckBox();
    legacyCheckBox_->setChecked( false );
    connect( legacyCheckBox_, SIGNAL( stateChanged ( int ) ), SLOT( SwordVersionChecked( int ) ) );

    legacyBox_ = new QGroupBox();
    QHBoxLayout* commentBoxLayout = new QHBoxLayout( legacyBox_ );
    commentBoxLayout->addWidget( legacyLabel_ );
    commentBoxLayout->addWidget( legacyCheckBox_ );
    commentBoxLayout->setMargin( 5 );

    //general panel
    QVBoxLayout* boxLayout = new QVBoxLayout( this );
    boxLayout->setMargin( 5 );
    boxLayout->addWidget( timeBox_ );
    boxLayout->addWidget( pathfindBox_ );
    boxLayout->addWidget( recordBox_ );
    boxLayout->addWidget( clientBox_ );
    boxLayout->addWidget( legacyBox_ );
}

// -----------------------------------------------------------------------------
// Name: AdvancedConfigPanel destructor
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
AdvancedConfigPanel::~AdvancedConfigPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AdvancedConfigPanel::OnLanguageChanged
// Created: ABR 2011-11-10
// -----------------------------------------------------------------------------
void AdvancedConfigPanel::OnLanguageChanged()
{
    timeBox_->setTitle( tools::translate( "AdvancedConfigPanel", "Time" ) );
    stepLabel_->setText( tools::translate( "AdvancedConfigPanel", "Time step:" ) );
    factorLabel_->setText( tools::translate( "AdvancedConfigPanel", "Time factor:" ) );
    endtickLabel_->setText( tools::translate( "AdvancedConfigPanel", "End tick:" ) );
    pausedLabel_->setText( tools::translate( "AdvancedConfigPanel", "Paused at startup:" ) );

    pathfindBox_->setTitle( tools::translate( "AdvancedConfigPanel", "Pathfind" ) );
    pathThreadsLabel_->setText( tools::translate( "AdvancedConfigPanel", "Number of threads:" ) );

    recordBox_->setTitle( tools::translate( "AdvancedConfigPanel", "Recorder" ) );
    fragmentsFrequencyLabel_->setText( tools::translate( "AdvancedConfigPanel", "Fragmentation frequency: " ) );

    clientBox_->setTitle( tools::translate( "AdvancedConfigPanel", "Client" ) );
    noClientLabel_->setText( tools::translate( "AdvancedConfigPanel", "Do not start gaming client" ) );

    legacyBox_->setTitle( tools::translate( "AdvancedConfigPanel", "Legacy Mode" ) );
    legacyLabel_->setText( tools::translate( "AdvancedConfigPanel", "Enable Legacy Mode" ) );
}

// -----------------------------------------------------------------------------
// Name: AdvancedConfigPanel::GetName
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
QString AdvancedConfigPanel::GetName() const
{
    return tools::translate( "AdvancedConfigPanel", "Advanced" );
}

// -----------------------------------------------------------------------------
// Name: AdvancedConfigPanel::Commit
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
void AdvancedConfigPanel::Commit( const std::string& exercise, const std::string& session )
{
    frontend::CreateSession action( config_, exercise, session );
    action.SetOption( "session/config/simulation/time/@step", stepSpin_->value() );
    action.SetOption( "session/config/simulation/time/@factor", factorSpin_->value() );
    action.SetOption( "session/config/simulation/time/@end-tick", endtickSpin_->value() );
    action.SetOption( "session/config/simulation/time/@paused", pausedCheckBox_->isChecked() );
    action.SetOption( "session/config/simulation/pathfinder/@threads", pathThreadsSpin_->value() );
    action.SetOption( "session/config/dispatcher/plugins/recorder/@fragmentfreq", fragmentsFrequencySpin_->value() );
    action.Commit();
}

// -----------------------------------------------------------------------------
// Name: AdvancedConfigPanel::SwordVersionChecked
// Created: RBA 2012-01-30
// -----------------------------------------------------------------------------
void AdvancedConfigPanel::NoClientChecked( int state )
{
    const bool noClientSelected = state == Qt::Checked;
    WriteRegistry( "NoClientSelected", noClientSelected );
    emit NoClientSelected( noClientSelected );
}

// -----------------------------------------------------------------------------
// Name: AdvancedConfigPanel::SwordVersionChecked
// Created: SLI 2012-01-30
// -----------------------------------------------------------------------------
void AdvancedConfigPanel::SwordVersionChecked( int state )
{
    const bool isLegacy = state == Qt::Checked;
    WriteRegistry( "IsLegacy", isLegacy );
    emit SwordVersionSelected( isLegacy );
}
