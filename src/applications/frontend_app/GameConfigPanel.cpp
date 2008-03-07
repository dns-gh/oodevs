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
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qdatetimeedit.h>
#include <xeumeuleu/xml.h>

#pragma warning( disable: 4127 4511 4512 )
#include <boost/lexical_cast.hpp>

// -----------------------------------------------------------------------------
// Name: GameConfigPanel constructor
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
GameConfigPanel::GameConfigPanel( QWidget* parent, const tools::GeneralConfig& config )
    : QVBox( parent )
    , config_( config )
{
    setSpacing( 5 );
    QHBox* box = new QHBox( this );
    box->setSpacing( 5 );
    CreateSimulationPanel( new QGroupBox( 1, Horizontal, tr( "Simulation" ), box ) );
    QWidget* debugPanel = new QGroupBox( 1, Horizontal, tr( "Debug" ), box );
    CreateDebugPanel( debugPanel );
    QWidget* systemPanel = new QGroupBox( 1, Horizontal, tr( "System" ), box );
    CreateSystemPanel( systemPanel );
    {
        box = new QHBox( this );
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
    QHBox* box = new QVBox( parent );
    box->setSpacing( 5 );
    {
        QGroupBox* time = new QGroupBox( 2, Vertical, tr( "Time" ), box );
        {
            QHBox* stepBox = new QHBox( time );
            new QLabel( tr( "Time step:" ), stepBox );
            stepSpin_ = new QSpinBox( 1, 100, 1, stepBox );
            stepSpin_->setValue( 10 );
        }
        {
            QHBox* factorBox = new QHBox( time );
            new QLabel( tr( "Time factor:" ), factorBox );
            factorSpin_ = new QSpinBox( 1, 100, 1, factorBox );
            factorSpin_->setValue( 10 );
        }
    }
    {
        QGroupBox* checkpoints = new QGroupBox( 2, Vertical, tr( "Checkpoints" ), box );
        {
            QHBox* frequencyBox = new QHBox( checkpoints );
            new QLabel( tr( "Frequency:" ), frequencyBox );
            checkFrequency_ = new QTimeEdit( frequencyBox );
            checkFrequency_->setTime( QTime().addSecs( 3600 ) );
        }
        {
            QHBox* keepBox = new QHBox( checkpoints );
            new QLabel( tr( "Rotations:" ), keepBox );
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
    QHBox* box = new QVBox( parent );
    box->setSpacing( 5 );
    {
        QGroupBox* logs = new QGroupBox( 2, Vertical, tr( "Logs" ), box );
        decisionalLogs_ = new QCheckBox( tr( "Decisional" ), logs );
        pathfindLogs_   = new QCheckBox( tr( "Pathfind" ), logs );
    }
    {
        diaDebugBox_ = new QGroupBox( 1, Vertical, tr( "DIA Debugger" ), box );
        diaDebugBox_->setCheckable( true );
        diaDebugBox_->setChecked( false );
        diaDebugPort_ = new QSpinBox( 1024, 65535, 1, diaDebugBox_ );
        diaDebugPort_->setValue( 15000 );
        connect( diaDebugBox_, SIGNAL( toggled( bool ) ), diaDebugPort_, SLOT( setEnabled( bool ) ) );
    }
    {
        netConBox_ = new QGroupBox( 1, Vertical, tr( "Network Console" ), box );
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
    QHBox* box = new QVBox( parent );
    box->setSpacing( 5 );
    {
        QHBox* pathThreads = new QHBox( box );
        new QLabel( tr( "Pathfind threads:" ), pathThreads );
        pathThreads_ = new QSpinBox( 1, 4, 1, pathThreads );
    }
    {
        profile_ = new QCheckBox( tr( "Profile" ), box );
    }
    {
        checkOdb_ = new QCheckBox( tr( "Check ODB Composition" ), box );
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
        action.SetOption( "session/config/simulation/checkpoint/@frequency", QString( "%1s" ).arg( QTime().secsTo( checkFrequency_->time() ) ).ascii() );
        action.SetOption( "session/config/simulation/checkpoint/@keep", keepSpin_->value() );
        action.SetOption( "session/config/simulation/checkpoint/@usecrc", true );
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
        action.SetOption( "session/config/simulation/pathfinder/@threads", pathThreads_->value() );
        action.SetOption( "session/config/simulation/hla/@enabled", false ); // $$$$ AGE 2007-10-09:
        action.SetOption( "session/config/simulation/hla/@federation", "MyFederation" );
        action.SetOption( "session/config/simulation/hla/@federate", "Sword OT Power" );
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
}
