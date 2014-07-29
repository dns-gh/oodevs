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

#include "frontend/CommandLineTools.h"
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

    exerciseLabel_ = new QLabel();
    exerciseNumber_ = new QSpinBox();
    exerciseNumber_->setRange( 1, 10 );
    exerciseNumber_->setSingleStep( 1 );
    exerciseNumber_->setValue( 1 );

    //Exercise box
    QWidget* exerciseNumberBox = new QWidget();
    QHBoxLayout* exerciseNumberBoxLayout = new QHBoxLayout( exerciseNumberBox );
    exerciseNumberBoxLayout->addWidget( exerciseLabel_ );
    exerciseNumberBoxLayout->addWidget( exerciseNumber_ );
    exerciseNumberBoxLayout->setStretchFactor( exerciseLabel_, 1 );
    exerciseNumberBoxLayout->setStretchFactor( exerciseNumber_, 2 );

    QVBoxLayout* exerciseBoxLayout = new QVBoxLayout( this );
    exerciseBoxLayout->addWidget( sessionBox );
    exerciseBoxLayout->addWidget( commentBox );
    exerciseBoxLayout->addWidget( exerciseNumberBox );
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
    exerciseLabel_->setText( tools::translate( "SessionConfigPanel", "Exercise number:" ) );
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
                            boost::lexical_cast< std::string >( frontend::GetPort( exerciseNumber_->value(), SIMULATION_PORT ) ) );
    }
    {
        action.SetOption( "session/config/dispatcher/network/@client", "localhost:" +  // $$$$ AGE 2007-10-09:
                            boost::lexical_cast< std::string >( frontend::GetPort( exerciseNumber_->value(), SIMULATION_PORT ) ) );
        action.SetOption( "session/config/dispatcher/network/@server", "0.0.0.0:" +  // $$$$ AGE 2007-10-09:
                            boost::lexical_cast< std::string >( frontend::GetPort( exerciseNumber_->value(), DISPATCHER_PORT ) ) );
    }
    {
        action.SetOption( "session/config/gaming/network/@server", "localhost:" +  // $$$$ AGE 2007-10-09:
                                    boost::lexical_cast< std::string >( frontend::GetPort( exerciseNumber_->value(), DISPATCHER_PORT ) ) );
    }
    {
        action.SetOption( "session/config/timeline/@url", "localhost:" +
                                    boost::lexical_cast< std::string >( frontend::GetPort( exerciseNumber_->value(), frontend::TIMELINE_PORT ) ) );
    }
    action.Commit();
}
