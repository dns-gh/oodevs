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
    Q3GroupBox* timeBox = new Q3GroupBox( 2, Qt::Horizontal, tools::translate( "AdvancedConfigPanel", "Time" ), box );
    {
        Q3HBox* stepBox = new Q3HBox( timeBox );
        new QLabel( tools::translate( "AdvancedConfigPanel", "Time step:" ), stepBox );
        stepSpin_ = new QSpinBox( 1, 100, 1, stepBox );
        stepSpin_->setValue( 10 );
    }
    {
        Q3HBox* factorBox = new Q3HBox( timeBox );
        new QLabel( tools::translate( "AdvancedConfigPanel", "Time factor:" ), factorBox );
        factorSpin_ = new QSpinBox( 1, 100, 1, factorBox );
        factorSpin_->setValue( 10 );
    }
    {
        Q3HBox* endTickBox = new Q3HBox( timeBox );
        new QLabel( tools::translate( "AdvancedConfigPanel", "End tick:" ), endTickBox );
        endtickSpin_ = new QSpinBox( 0, std::numeric_limits< int >::max(), 1, endTickBox );
        endtickSpin_->setValue( 0 );
    }
    {
        pausedCheckBox_ = new QCheckBox( tools::translate( "AdvancedConfigPanel", "  Paused at startup" ), timeBox );
        pausedCheckBox_->setChecked( false );
    }
    Q3GroupBox* pathfindBox = new Q3GroupBox( 2, Qt::Horizontal, tools::translate( "AdvancedConfigPanel", "Pathfind" ), box );
    {
        Q3HBox* threadBox = new Q3HBox( pathfindBox );
        new QLabel( tools::translate( "AdvancedConfigPanel", "Number of threads:" ), threadBox );
        pathThreads_ = new QSpinBox( 0, 4, 1, threadBox );
        pathThreads_->setValue( 1 );
    }
    Q3GroupBox* recordBox = new Q3GroupBox( 2, Qt::Horizontal, tools::translate( "AdvancedConfigPanel", "Recorder" ), box );
    {
        Q3HBox* freqBox = new Q3HBox( recordBox );
        new QLabel( tools::translate( "AdvancedConfigPanel", "Fragmentation frequency: " ), freqBox );
        fragmentsFrequency_ = new QSpinBox( 0, std::numeric_limits< int >::max(), 1, freqBox );
        fragmentsFrequency_->setValue( 200 );
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
    action.SetOption( "session/config/simulation/pathfinder/@threads", pathThreads_->value() );
    action.SetOption( "session/config/dispatcher/plugins/recorder/@fragmentfreq", fragmentsFrequency_->value() );
    action.Commit();
}
