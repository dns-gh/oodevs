// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "SessionConfigPanel.h"
#include "moc_SessionConfigPanel.cpp"

#include "clients_kernel/Tools.h"

#include "frontend/Ports.h"
#include "frontend/CreateSession.h"

#pragma warning( push, 0 )
#include <QtGui/qlabel.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qspinbox.h>
#pragma warning( pop )


using namespace frontend;

// -----------------------------------------------------------------------------
// Name: SessionConfigPanel constructor
// Created: JSR 2010-11-05
// -----------------------------------------------------------------------------
SessionConfigPanel::SessionConfigPanel( QWidget* parent, const tools::GeneralConfig& config )
    : PluginConfig_ABC( parent )
    , config_( config )
{
    //session box
    nameLabel_ = new QLabel();
    sessionName_ = new QLineEdit();
    sessionName_->setText( "" );

    QWidget* sessionBox = new QWidget();
    QHBoxLayout* sessionBoxLayout = new QHBoxLayout( sessionBox );
    sessionBoxLayout->addWidget( nameLabel_ );
    sessionBoxLayout->addWidget( sessionName_ );
    sessionBoxLayout->setStretchFactor( nameLabel_, 1 );
    sessionBoxLayout->setStretchFactor( sessionName_, 2 );

    commentLabel_ = new QLabel();
    sessionComment_ = new QTextEdit();
    sessionComment_->setText( "" );

    //exercise number box
    QWidget* commentBox = new QWidget();
    QHBoxLayout* commentBoxLayout = new QHBoxLayout( commentBox );
    commentBoxLayout->addWidget( commentLabel_ );
    commentBoxLayout->addWidget( sessionComment_ );
    commentBoxLayout->setStretchFactor( commentLabel_, 1 );
    commentBoxLayout->setStretchFactor( sessionComment_, 2 );

    portsGroup_ = new QGroupBox();

    simulationPortLabel_ = new QLabel();
    simulationPort_ = new QSpinBox();
    simulationPort_->setRange( 5000, 65535 - NUM_PORTS );
    simulationPort_->setSingleStep( NUM_PORTS );
    simulationPort_->setValue( 10000 );

    dispatcherPortLabel_ = new QLabel();
    dispatcherPort_ = new QSpinBox();
    dispatcherPort_->setRange( 5000, 65535 );
    dispatcherPort_->setValue( 10000 + DISPATCHER_PORT );
    dispatcherPort_->setEnabled( false );
    dispatcherPort_->setButtonSymbols( QAbstractSpinBox::NoButtons );

    timelinePortLabel_ = new QLabel();
    timelinePort_ = new QSpinBox();
    timelinePort_->setRange( 5000, 65535 );
    timelinePort_->setValue( 10000 + TIMELINE_PORT );
    timelinePort_->setEnabled( false );
    timelinePort_->setButtonSymbols( QAbstractSpinBox::NoButtons );

    //Exercise box
    QGridLayout* grid = new QGridLayout();
    portsGroup_->setLayout( grid );
    grid->setColumnStretch( 0, 1 );
    grid->setColumnStretch( 1, 2 );
    grid->addWidget( simulationPortLabel_, 0, 0 );
    grid->addWidget( simulationPort_, 0, 1 );
    grid->addWidget( dispatcherPortLabel_, 1, 0 );
    grid->addWidget( dispatcherPort_, 1, 1 );
    grid->addWidget( timelinePortLabel_, 2, 0 );
    grid->addWidget( timelinePort_, 2, 1 );

    QVBoxLayout* exerciseBoxLayout = new QVBoxLayout( this );
    exerciseBoxLayout->addWidget( sessionBox );
    exerciseBoxLayout->addWidget( commentBox );
    exerciseBoxLayout->addWidget( portsGroup_ );

    connect( simulationPort_, SIGNAL( valueChanged( int ) ), SLOT( OnSimulationPortChanged( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: SessionConfigPanel destructor
// Created: JSR 2010-11-05
// -----------------------------------------------------------------------------
SessionConfigPanel::~SessionConfigPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SessionConfigPanel::OnLanguageChanged
// Created: ABR 2011-11-10
// -----------------------------------------------------------------------------
void SessionConfigPanel::OnLanguageChanged()
{
    nameLabel_->setText( tools::translate( "SessionConfigPanel", "Session name:" ) );
    commentLabel_->setText( tools::translate( "SessionConfigPanel", "Session comments:" ) );
    portsGroup_->setTitle( tools::translate( "SessionConfigPanel", "Ports" ) );
    simulationPortLabel_->setText( tools::translate( "SessionConfigPanel", "Simulation:" ) );
    dispatcherPortLabel_->setText( tools::translate( "SessionConfigPanel", "Dispatcher:" ) );
    timelinePortLabel_->setText( tools::translate( "SessionConfigPanel", "Timeline:" ) );
}

// -----------------------------------------------------------------------------
// Name: SessionConfigPanel::GetName
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
QString SessionConfigPanel::GetName() const
{
    return tools::translate( "SessionConfigPanel", "Session" );
}

// -----------------------------------------------------------------------------
// Name: SessionConfigPanel::Commit
// Created: JSR 2010-11-05
// -----------------------------------------------------------------------------
void SessionConfigPanel::Commit( const tools::Path& exercise, const tools::Path& session )
{
    frontend::CreateSession action( config_, exercise, session );
    {
        action.SetOption( "session/meta/date", session );
        action.SetOption( "session/meta/name", sessionName_->text().toStdString() );
        action.SetOption( "session/meta/comment", sessionComment_->toPlainText() );
    }
    {
        action.SetOption( "session/config/simulation/network/@port", "localhost:" +  // $$$$ AGE 2007-10-09:
                            boost::lexical_cast< std::string >( simulationPort_->value() + SIMULATION_PORT ) );
    }
    {
        action.SetOption( "session/config/dispatcher/network/@client", "localhost:" +  // $$$$ AGE 2007-10-09:
                            boost::lexical_cast< std::string >( simulationPort_->value() + SIMULATION_PORT ) );
        action.SetOption( "session/config/dispatcher/network/@server", "0.0.0.0:" +  // $$$$ AGE 2007-10-09:
                            boost::lexical_cast< std::string >( simulationPort_->value() + DISPATCHER_PORT ) );
    }
    {
        action.SetOption( "session/config/gaming/network/@server", "localhost:" +  // $$$$ AGE 2007-10-09:
                                    boost::lexical_cast< std::string >( simulationPort_->value() + DISPATCHER_PORT ) );
    }
    {
        action.SetOption( "session/config/timeline/@url", "localhost:" +
                                    boost::lexical_cast< std::string >( simulationPort_->value() + TIMELINE_PORT ) );
    }
    action.Commit();
}

// -----------------------------------------------------------------------------
// Name: SessionConfigPanel::OnSimulationPortChanged
// Created: JSR 2014-12-02
// -----------------------------------------------------------------------------
void SessionConfigPanel::OnSimulationPortChanged( int value )
{
    dispatcherPort_->setValue( value + DISPATCHER_PORT );
    timelinePort_->setValue( value + TIMELINE_PORT );
}
