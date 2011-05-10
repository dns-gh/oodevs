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
#include <qcheckbox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qvbox.h>

using namespace frontend;

namespace
{
    template< typename T >
    T* Style( T* widget )
    {
        widget->setBackgroundOrigin( QWidget::WindowOrigin );
        return widget;
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
    QVBox* box = Style( new QVBox( this ) );
    box->setMargin( 5 );
    QGroupBox* timeBox = Style( new QGroupBox( 2, Qt::Horizontal, tools::translate( "AdvancedConfigPanel", "Time" ), box ) );
    {
        QHBox* stepBox = Style( new QHBox( timeBox ) );
        Style( new QLabel( tools::translate( "AdvancedConfigPanel", "Time step:" ), stepBox ) );
        stepSpin_ = Style( new QSpinBox( 1, 100, 1, stepBox ) );
        stepSpin_->setValue( 10 );
    }
    {
        QHBox* factorBox = Style( new QHBox( timeBox ) );
        Style( new QLabel( tools::translate( "AdvancedConfigPanel", "Time factor:" ), factorBox ) );
        factorSpin_ = Style( new QSpinBox( 1, 100, 1, factorBox ) );
        factorSpin_->setValue( 10 );
    }
    {
        QHBox* endTickBox = Style( new QHBox( timeBox ) );
        Style( new QLabel( tools::translate( "AdvancedConfigPanel", "End tick:" ), endTickBox ) );
        endtickSpin_ = Style( new QSpinBox( 0, std::numeric_limits< int >::max(), 1, endTickBox ) );
        endtickSpin_->setValue( 0 );
    }
    {
        pausedCheckBox_ = Style( new QCheckBox( tools::translate( "AdvancedConfigPanel", "  Paused at startup" ), timeBox ) );
        pausedCheckBox_->setChecked( false );
    }
    QGroupBox* pathfindBox = Style( new QGroupBox( 2, Qt::Horizontal, tools::translate( "AdvancedConfigPanel", "Pathfind" ), box ) );
    {
        QHBox* threadBox = Style( new QHBox( pathfindBox ) );
        Style( new QLabel( tools::translate( "AdvancedConfigPanel", "Number of threads:" ), threadBox ) );
        pathThreads_ = Style( new QSpinBox( 0, 4, 1, threadBox ) );
        pathThreads_->setValue( 1 );
    }
    QGroupBox* recordBox = Style( new QGroupBox( 2, Qt::Horizontal, tools::translate( "AdvancedConfigPanel", "Recorder" ), box ) );
    {
        QHBox* freqBox = Style( new QHBox( recordBox ) );
        Style( new QLabel( tools::translate( "AdvancedConfigPanel", "Fragmentation frequency: " ), freqBox ) );
        fragmentsFrequency_ = Style( new QSpinBox( 0, std::numeric_limits< int >::max(), 1, freqBox ) );
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
