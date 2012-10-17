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
#include <Qt3Support/q3groupbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qspinbox.h>
#include <Qt3Support/q3vbox.h>

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: AdvancedConfigPanel constructor
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
AdvancedConfigPanel::AdvancedConfigPanel( QWidget* parent, const tools::GeneralConfig& config )
    : PluginConfig_ABC( parent )
    , config_( config )
{
    Q3VBox* box = new Q3VBox( this );
    box->setMargin( 5 );
    timeBox_ = new Q3GroupBox( 2, Qt::Horizontal, box );
    {
        Q3HBox* stepBox = new Q3HBox( timeBox_ );
        stepLabel_ = new QLabel( stepBox );
        stepSpin_ = new QSpinBox( 1, 100, 1, stepBox );
        stepSpin_->setValue( 10 );
    }
    {
        Q3HBox* factorBox = new Q3HBox( timeBox_ );
        factorLabel_ = new QLabel( factorBox );
        factorSpin_ = new QSpinBox( 1, 100, 1, factorBox );
        factorSpin_->setValue( 10 );
    }
    {
        Q3HBox* endTickBox = new Q3HBox( timeBox_ );
        endtickLabel_ = new QLabel( endTickBox );
        endtickSpin_ = new QSpinBox( 0, std::numeric_limits< int >::max(), 1, endTickBox );
        endtickSpin_->setValue( 0 );
    }
    {
        Q3HBox* pausedBox = new Q3HBox( timeBox_ );
        pausedLabel_ = new QLabel( pausedBox );
        pausedCheckBox_ = new QCheckBox( pausedBox );
        pausedCheckBox_->setChecked( false );
    }
    pathfindBox_ = new Q3GroupBox( 2, Qt::Horizontal, box );
    {
        Q3HBox* threadBox = new Q3HBox( pathfindBox_ );
        pathThreadsLabel_ = new QLabel( threadBox );
        pathThreadsSpin_ = new QSpinBox( 0, 4, 1, threadBox );
        pathThreadsSpin_->setValue( 1 );
    }
    recordBox_ = new Q3GroupBox( 2, Qt::Horizontal, box );
    {
        Q3HBox* freqBox = new Q3HBox( recordBox_ );
        fragmentsFrequencyLabel_ = new QLabel( freqBox );
        fragmentsFrequencySpin_ = new QSpinBox( 0, std::numeric_limits< int >::max(), 1, freqBox );
        fragmentsFrequencySpin_->setValue( 200 );
    }
    clientBox_ = new Q3GroupBox( 2, Qt::Horizontal, box );
    {
        noClientLabel_ = new QLabel( clientBox_ );
        noClientCheckBox_ = new QCheckBox( clientBox_ );
        noClientCheckBox_->setChecked( false );
        connect( noClientCheckBox_, SIGNAL( stateChanged ( int ) ), SLOT( NoClientChecked( int ) ) );
    }
    legacyBox_ = new Q3GroupBox( 2, Qt::Horizontal, box );
    {
        legacyLabel_ = new QLabel( legacyBox_ );
        legacyCheckBox_ = new QCheckBox( legacyBox_ );
        legacyCheckBox_->setChecked( false );
        connect( legacyCheckBox_, SIGNAL( stateChanged ( int ) ), SLOT( SwordVersionChecked( int ) ) );
    }
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
    emit NoClientSelected( state == Qt::Checked );
}

// -----------------------------------------------------------------------------
// Name: AdvancedConfigPanel::SwordVersionChecked
// Created: SLI 2012-01-30
// -----------------------------------------------------------------------------
void AdvancedConfigPanel::SwordVersionChecked( int state )
{
    emit SwordVersionSelected( state == Qt::Checked );
}
