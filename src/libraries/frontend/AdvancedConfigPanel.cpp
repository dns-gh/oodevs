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
        return settings.value( ( "/sword/" + key ).c_str() ).toBool();
    }
    void WriteRegistry( const std::string& key, bool value )
    {
        QSettings settings( "MASA Group", "SWORD" );
        settings.setValue( ( "/sword/" + key ).c_str(), value );
    }
    void AddSpinBox( QVBoxLayout* boxLayout, QLabel*& label, QSpinBox*& spin, int rangeMin, int rangeMax, int step, int spinValue )
    {
        label = new QLabel();
        spin = new QSpinBox();
        spin->setRange(rangeMin, rangeMax );
        spin->setSingleStep( step );
        spin->setValue( spinValue );

        QWidget* innerBox = new QWidget();
        QHBoxLayout* innerBoxLayout = new QHBoxLayout( innerBox );
        innerBoxLayout->addWidget( label );
        innerBoxLayout->addWidget( spin );

        boxLayout->addWidget( innerBox );
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
    stepSpin_->setSingleStep( 1 );
    stepSpin_->setValue( 10 );

    //---
    factorLabel_ = new QLabel();
    factorSpin_ = new QSpinBox();
    factorSpin_->setRange( 1, 100 );
    factorSpin_->setSingleStep( 1 );
    factorSpin_->setValue( 10 );

    //---
    endtickLabel_ = new QLabel();
    endtickSpin_ = new QSpinBox();
    endtickSpin_->setRange( 0, std::numeric_limits< int >::max() );
    endtickSpin_->setSingleStep( 1 );
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
    
    QGroupBox* miscellaneousBox = new QGroupBox();
    QVBoxLayout* miscellaneousBoxLayout = new QVBoxLayout( miscellaneousBox );
    miscellaneousBoxLayout->setMargin( 5 );
    AddSpinBox( miscellaneousBoxLayout, pathThreadsLabel_, pathThreadsSpin_, 0, 4, 1, 1 );
    AddSpinBox( miscellaneousBoxLayout, fragmentsFrequencyLabel_, fragmentsFrequencySpin_, 0, std::numeric_limits< int >::max(), 1, 200 );

    //client box
    QWidget* clientBox = new QWidget();
    QHBoxLayout* clientBoxLayout = new QHBoxLayout( clientBox );
    noClientLabel_ = new QLabel();
    noClientCheckBox_ = new QCheckBox();
    noClientCheckBox_->setChecked( false );
    connect( noClientCheckBox_, SIGNAL( stateChanged ( int ) ), SLOT( NoClientChecked( int ) ) );

    clientBoxLayout->addWidget( noClientLabel_ );
    clientBoxLayout->addWidget( noClientCheckBox_ );
    miscellaneousBoxLayout->addWidget( clientBox );

    AddSpinBox( miscellaneousBoxLayout, reportsFrequencyLabel_, reportsFrequencySpin_, 1, std::numeric_limits< int >::max(), 1, 100 );

    //general panel
    QVBoxLayout* boxLayout = new QVBoxLayout( this );
    boxLayout->setMargin( 5 );
    boxLayout->addWidget( timeBox_ );
    boxLayout->addWidget( miscellaneousBox );
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

    pathThreadsLabel_->setText( tools::translate( "AdvancedConfigPanel", "Number of pathfind threads:" ) );
    fragmentsFrequencyLabel_->setText( tools::translate( "AdvancedConfigPanel", "Recorder fragmentation frequency: " ) );
    noClientLabel_->setText( tools::translate( "AdvancedConfigPanel", "Do not start gaming client" ) );
    reportsFrequencyLabel_->setText( tools::translate( "AdvancedConfigPanel", "Report clean frequency: " ) );
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
    action.SetOption( "session/config/dispatcher/reports/@frequency", reportsFrequencySpin_->value() );
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
