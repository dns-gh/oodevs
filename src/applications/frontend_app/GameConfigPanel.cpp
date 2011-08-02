// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "GameConfigPanel.h"
#include "AdvancedButton.h"
#include "frontend/CommandLineTools.h"
#include "frontend/CreateSession.h"
#include "tools/GeneralConfig.h"
#include "tools/xmlcodecs.h"
#include "clients_gui/Tools.h"
#include <xeumeuleu/xml.hpp>

#include <QtGui/qcheckbox.h>
#include <Qt3Support/q3datetimeedit.h>
#include <Qt3Support/q3groupbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qspinbox.h>

#pragma warning( push )
#pragma warning( disable : 4702 )
#include <boost/lexical_cast.hpp>
#pragma warning( pop )

#pragma warning( disable: 4127 4511 4512 )

// -----------------------------------------------------------------------------
// Name: GameConfigPanel constructor
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
GameConfigPanel::GameConfigPanel( QWidget* parent, const tools::GeneralConfig& config )
    : Q3VBox( parent, "GameConfigPanel" )
    , config_( config )
{
    setSpacing( 5 );
    Q3HBox* box = new Q3HBox( this );
    box->setSpacing( 5 );
    CreateSimulationPanel( new Q3GroupBox( 1, Qt::Horizontal, tools::translate( "GameConfigPanel", "Simulation" ), box ) );
    QWidget* debugPanel = new Q3GroupBox( 1, Qt::Horizontal, tools::translate( "GameConfigPanel", "Debug" ), box );
    CreateDebugPanel( debugPanel );
    QWidget* systemPanel = new Q3GroupBox( 1, Qt::Horizontal, tools::translate( "GameConfigPanel", "System" ), box );
    CreateSystemPanel( systemPanel );
    {
        box = new Q3HBox( this );
        box->layout()->setAlignment( Qt::AlignRight );
        AdvancedButton* button = new AdvancedButton( box );
        button->Link( debugPanel );
        button->Link( systemPanel );
    }
}

// -----------------------------------------------------------------------------
// Name: GameConfigPanel destructor
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
GameConfigPanel::~GameConfigPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GameConfigPanel::CreateSimulationPanel
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
QWidget* GameConfigPanel::CreateSimulationPanel( QWidget* parent )
{
    Q3HBox* box = new Q3VBox( parent );
    box->setSpacing( 5 );
    {
        Q3GroupBox* time = new Q3GroupBox( 2, Qt::Vertical, tools::translate( "GameConfigPanel", "Time" ), box );
        {
            Q3HBox* stepBox = new Q3HBox( time );
            new QLabel( tools::translate( "GameConfigPanel", "Time step:" ), stepBox );
            stepSpin_ = new QSpinBox( 1, 100, 1, stepBox );
            stepSpin_->setValue( 10 );
        }
        {
            Q3HBox* factorBox = new Q3HBox( time );
            new QLabel( tools::translate( "GameConfigPanel", "Time factor:" ), factorBox );
            factorSpin_ = new QSpinBox( 1, 100, 1, factorBox );
            factorSpin_->setValue( 10 );
        }
    }
    {
        checkpoints_ = new Q3GroupBox( 2, Qt::Vertical, tools::translate( "GameConfigPanel", "Checkpoints" ), box );
        checkpoints_->setCheckable( true );
        checkpoints_->setChecked( false );
        {
            Q3HBox* frequencyBox = new Q3HBox( checkpoints_ );
            new QLabel( tools::translate( "GameConfigPanel", "Frequency:" ), frequencyBox );
            checkFrequency_ = new Q3TimeEdit( frequencyBox );
            checkFrequency_->setDisplay ( Q3TimeEdit::Hours | Q3TimeEdit::Minutes | Q3TimeEdit::Seconds  );
            checkFrequency_->setTime( QTime().addSecs( 3600 ) );
        }
        {
            Q3HBox* keepBox = new Q3HBox( checkpoints_ );
            new QLabel( tools::translate( "GameConfigPanel", "Rotations:" ), keepBox );
            keepSpin_ = new QSpinBox( 1, 100, 1, keepBox );
        }
    }
    return box;
}

// -----------------------------------------------------------------------------
// Name: GameConfigPanel::CreateDebugPanel
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
QWidget* GameConfigPanel::CreateDebugPanel( QWidget* parent )
{
    Q3HBox* box = new Q3VBox( parent );
    box->setSpacing( 5 );
    {
        Q3GroupBox* logs = new Q3GroupBox( 4, Qt::Horizontal, tools::translate( "GameConfigPanel", "Logs" ), box );
        decisionalLogs_ = new QCheckBox( tools::translate( "GameConfigPanel", "Decisional" ), logs );
        pathfindLogs_   = new QCheckBox( tools::translate( "GameConfigPanel", "Pathfind" ), logs );
    }
    {
        diaDebugBox_ = new Q3GroupBox( 1, Qt::Vertical, tools::translate( "GameConfigPanel", "DIA Debugger" ), box );
        diaDebugBox_->setCheckable( true );
        diaDebugBox_->setChecked( false );
        diaDebugPort_ = new QSpinBox( 1024, 65535, 1, diaDebugBox_ );
        diaDebugPort_->setValue( 15000 );
        connect( diaDebugBox_, SIGNAL( toggled( bool ) ), diaDebugPort_, SLOT( setEnabled( bool ) ) );
    }
    {
        netConBox_ = new Q3GroupBox( 1, Qt::Vertical, tools::translate( "GameConfigPanel", "Network Console" ), box );
        netConBox_->setCheckable( true );
        netConBox_->setChecked( false );
        netConPort_ = new QSpinBox( 1024, 65535, 1, netConBox_ );
        netConPort_->setValue( 20000 );
        connect( netConBox_, SIGNAL( toggled( bool ) ), netConPort_, SLOT( setEnabled( bool ) ) );
    }
    return box;
}

// -----------------------------------------------------------------------------
// Name: GameConfigPanel::CreateSystemPanel
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
QWidget* GameConfigPanel::CreateSystemPanel( QWidget* parent )
{
    Q3HBox* box = new Q3VBox( parent );
    box->setSpacing( 5 );
    {
        Q3HBox* pathThreads = new Q3HBox( box );
        new QLabel( tools::translate( "GameConfigPanel", "Pathfind threads:" ), pathThreads );
        pathThreads_ = new QSpinBox( 1, 4, 1, pathThreads );
    }
    {
        profile_ = new QCheckBox( tools::translate( "GameConfigPanel", "Profile" ), box );
    }
    {
        checkOdb_ = new QCheckBox( tools::translate( "GameConfigPanel", "Check ODB Composition" ), box );
    }
    return box;
}

// $$$$ AGE 2008-02-22: bouger dans la lib frontend

// -----------------------------------------------------------------------------
// Name: GameConfigPanel::Commit
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
void GameConfigPanel::Commit( const std::string& exercise, const std::string& session, const std::string& name, const std::string& comment, unsigned exerciseNumber )
{
    frontend::CreateSession action( config_, exercise, session );
    action.SetDefaultValues();
    {
        action.SetOption( "session/meta/date", session );
        action.SetOption( "session/meta/name", name );
        action.SetOption( "session/meta/comment", comment );
    }
    {
        if( checkpoints_->isChecked() )
        {
            action.SetOption( "session/config/simulation/checkpoint/@frequency", QString( "%1s" ).arg( QTime().secsTo( checkFrequency_->time() ) ).ascii() );
            action.SetOption( "session/config/simulation/checkpoint/@keep", keepSpin_->value() );
            action.SetOption( "session/config/simulation/checkpoint/@usecrc", true );
        }
        action.SetOption( "session/config/simulation/debug/@decisional", decisionalLogs_->isChecked() );
        action.SetOption( "session/config/simulation/debug/@pathfind", pathfindLogs_->isChecked() );
        action.SetOption( "session/config/simulation/debug/@diadebugger", diaDebugBox_->isChecked() );
        action.SetOption( "session/config/simulation/debug/@diadebuggerport", diaDebugPort_->value() );
        action.SetOption( "session/config/simulation/debug/@networklogger", netConBox_->isChecked() );
        action.SetOption( "session/config/simulation/debug/@networkloggerport", netConPort_->value() );
        action.SetOption( "session/config/simulation/decisional/@useonlybinaries", false ); // $$$$ AGE 2007-10-09:
        action.SetOption( "session/config/simulation/dispatcher/@embedded", true ); // $$$$ AGE 2007-10-09:
        action.SetOption( "session/config/simulation/network/@port", frontend::SimulationPort( exerciseNumber ) );
        action.SetOption( "session/config/simulation/orbat/@checkcomposition", checkOdb_->isChecked() );
        action.SetOption( "session/config/simulation/profiling/@enabled", profile_->isChecked() );
        action.SetOption( "session/config/simulation/time/@step", stepSpin_->value() );
        action.SetOption( "session/config/simulation/time/@factor", factorSpin_->value() );

        //Réglage des paramètres du GC
        action.SetOption( "session/config/simulation/GarbageCollector/@setpause"  , 100 );
        action.SetOption( "session/config/simulation/GarbageCollector/@setstepmul"  , 100 );
        //
        action.SetOption( "session/config/simulation/pathfinder/@threads", pathThreads_->value() );
    }
    {
        action.SetOption( "session/config/dispatcher/network/@client", "localhost:" +  // $$$$ AGE 2007-10-09:
                            boost::lexical_cast< std::string >( frontend::SimulationPort( exerciseNumber ) ) );
        action.SetOption( "session/config/dispatcher/network/@server", frontend::DispatcherPort( exerciseNumber ) );
        action.SetOption( "session/config/dispatcher/plugins/recorder", "" ); // $$$$ AGE 2008-02-22:
    }
    {
        action.SetOption( "session/config/gaming/network/@server", "localhost:" +  // $$$$ AGE 2007-10-09:
                                    boost::lexical_cast< std::string >( frontend::DispatcherPort( exerciseNumber ) ) );
    }
    action.Commit();
}
