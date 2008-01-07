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
#include "tools/GeneralConfig.h"
#include "tools/xmlcodecs.h"
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qdatetimeedit.h>

#pragma warning( disable: 4127 4244 4245 4511 4512 )
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/lexical_cast.hpp>
#include <xeumeuleu/xml.h>
#undef min

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: GameConfigPanel constructor
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
GameConfigPanel::GameConfigPanel( QWidget* parent, const tools::GeneralConfig& config )
    : QHBox( parent )
    , config_( config )
{
    setSpacing( 5 );
    CreateSimulationPanel( new QGroupBox( 1, Horizontal, tr( "Simulation" ), this ) );
    CreateDebugPanel     ( new QGroupBox( 1, Horizontal, tr( "Debug" ),      this ) );
    CreateSystemPanel    ( new QGroupBox( 1, Horizontal, tr( "System" ),     this ) );
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
        diaDebugPort_ = new QSpinBox( 1024, 65535, 1, diaDebugBox_ );
        diaDebugPort_->setValue( 15000 );
        connect( diaDebugBox_, SIGNAL( toggled( bool ) ), diaDebugPort_, SLOT( setEnabled( bool ) ) );
    }
    {
        netConBox_ = new QGroupBox( 1, Vertical, tr( "Network Console" ), box );
        netConBox_->setCheckable( true );
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

// -----------------------------------------------------------------------------
// Name: GameConfigPanel::SimulationPort
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
unsigned GameConfigPanel::SimulationPort( unsigned exerciseNumber )
{
    return 10000 + ( exerciseNumber - 1 ) * 2;
}

// -----------------------------------------------------------------------------
// Name: GameConfigPanel::DispatcherPort
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
unsigned GameConfigPanel::DispatcherPort( unsigned exerciseNumber )
{
    return SimulationPort( exerciseNumber ) + 1;
}

// -----------------------------------------------------------------------------
// Name: GameConfigPanel::Commit
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
void GameConfigPanel::Commit( const std::string& exercise, const std::string& session, const std::string& name, const std::string& comment, unsigned exerciseNumber )
{
    const std::string sessionXml = GetSessionXml( exercise, session );

    xml::xofstream xos( sessionXml );
    xos << xml::start( "session" )
            << xml::start( "meta" )
                << xml::content( "date", session )
                << xml::content( "name", xml::cdata( name ) )
                << xml::content( "comment", xml::cdata( comment ) )
            << xml::end()
            << xml::start( "config" )
                << xml::start( "simulation" )
                    << xml::start( "checkpoint" )
                        << xml::attribute( "frequency", QString( "%1s" ).arg( QTime().secsTo( checkFrequency_->time() ) ).ascii() )
                        << xml::attribute( "keep", keepSpin_->value() )
                        << xml::attribute( "usecrc", true )
                    << xml::end()
                    << xml::start( "debug" )
                        << xml::attribute( "decisional", decisionalLogs_->isChecked() )
                        << xml::attribute( "pathfind", pathfindLogs_->isChecked() )
                        << xml::attribute( "diadebugger", diaDebugBox_->isChecked() )
                        << xml::attribute( "diadebuggerport", diaDebugPort_->value() )
                        << xml::attribute( "networklogger", netConBox_->isChecked() )
                        << xml::attribute( "networkloggerport", netConPort_->value() )
                    << xml::end()
                    << xml::start( "decisional" )
                        << xml::attribute( "useonlybinaries", false ) // $$$$ AGE 2007-10-09: 
                    << xml::end()
                    << xml::start( "dispatcher" )
                        << xml::attribute( "embedded", true ) // $$$$ AGE 2007-10-09: 
                    << xml::end()
                    << xml::start( "network" )
                        << xml::attribute( "port", SimulationPort( exerciseNumber ) )
                    << xml::end()
                    << xml::start( "orbat" )
                        << xml::attribute( "checkcomposition", checkOdb_->isChecked() )
                    << xml::end()
                    << xml::start( "profiling" )
                        << xml::attribute( "enabled", profile_->isChecked() )
                    << xml::end()
                    << xml::start( "time" )
                        << xml::attribute( "step", stepSpin_->value() )
                        << xml::attribute( "factor", factorSpin_->value() )
                    << xml::end()
                    << xml::start( "pathfinder" )
                        << xml::attribute( "threads", pathThreads_->value() )
                    << xml::end()
                    << xml::start( "hla" ) 
                        << xml::attribute( "enabled", false ) // $$$$ AGE 2007-10-09: 
                        << xml::attribute( "federation", "MyFederation" )
                        << xml::attribute( "federate", "CSword power" )
                    << xml::end()
                << xml::end()
                << xml::start( "dispatcher" )
                    << xml::start( "network" )
                        << xml::attribute( "client", "localhost:" +  // $$$$ AGE 2007-10-09: 
                                    boost::lexical_cast< std::string >( SimulationPort( exerciseNumber ) ) )
                        << xml::attribute( "server", DispatcherPort( exerciseNumber ) )
                    << xml::end()
                    << xml::start( "plugins" )
                        << xml::start( "plugin" )
                            << xml::attribute( "name", "recorder" ) // $$$$ AGE 2007-10-09: 
                            << xml::attribute( "enabled", true )
                        << xml::end()
                    << xml::end()
                << xml::end()
                << xml::start( "gaming" )
                    << xml::start( "network" )
                        << xml::attribute( "server", "localhost:" +  // $$$$ AGE 2007-10-09: 
                                    boost::lexical_cast< std::string >( DispatcherPort( exerciseNumber ) ) )
                    << xml::end()
                << xml::end()
            << xml::end()
        << xml::end();
}

// -----------------------------------------------------------------------------
// Name: GameConfigPanel::GetSessionXml
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
std::string GameConfigPanel::GetSessionXml( const std::string& exercise, const std::string& session )
{
    const bfs::path dir( config_.BuildSessionDir( exercise, session ), bfs::native );
    bfs::create_directories( dir );
    return ( dir / "session.xml" ).native_file_string();
}

