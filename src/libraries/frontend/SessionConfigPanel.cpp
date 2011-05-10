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
#include "clients_gui/tools.h"
#include "frontend/CommandLineTools.h"
#include "frontend/CreateSession.h"
#include <qlabel.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qtextedit.h>
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
// Name: SessionConfigPanel constructor
// Created: JSR 2010-11-05
// -----------------------------------------------------------------------------
SessionConfigPanel::SessionConfigPanel( QWidget* parent, const tools::GeneralConfig& config )
    : PluginConfig_ABC( parent )
    , config_( config )
{
    QVBox* exerciseBox = Style( new QVBox( this ) );
    exerciseBox->setMargin( 5 );
    {
        QHBox* sessionBox = Style( new QHBox( exerciseBox ) );
        sessionBox->setStretchFactor( Style( new QLabel( tools::translate( "SessionConfigPanel", "Session name:" ), sessionBox ) ), 1 );
        sessionName_ = Style( new QLineEdit( sessionBox ) );
        sessionBox->setStretchFactor( sessionName_, 2 );
        sessionName_->setText( "" );
    }
    {
        QHBox* commentBox = Style( new QHBox( exerciseBox ) );
        commentBox->setStretchFactor( Style( new QLabel( tools::translate( "SessionConfigPanel", "Session comments:" ), commentBox ) ), 1 );
        sessionComment_ = Style( new QTextEdit( commentBox ) );
        commentBox->setStretchFactor( sessionComment_, 2 );
        sessionComment_->setText( "" );
    }
    {
        QHBox* exerciseNumberBox = Style( new QHBox( exerciseBox ) );
        exerciseNumberBox->setStretchFactor( Style( new QLabel( tools::translate( "SessionConfigPanel", "Exercise number:" ), exerciseNumberBox ) ), 1 );
        exerciseNumber_ = Style( new QSpinBox( 1, 10, 1, exerciseNumberBox ) );
        exerciseNumberBox->setStretchFactor( exerciseNumber_, 2 );
        exerciseNumber_->setValue( 1 );
    }
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
void SessionConfigPanel::Commit( const std::string& exercise, const std::string& session )
{
    frontend::CreateSession action( config_, exercise, session );
    {
        action.SetOption( "session/meta/date", session );
        action.SetOption( "session/meta/name", sessionName_->text().ascii() );
        action.SetOption( "session/meta/comment", sessionComment_->text().ascii() );
    }
    {
        action.SetOption( "session/config/simulation/network/@port", frontend::SimulationPort( exerciseNumber_->value() ) );
    }
    {
        action.SetOption( "session/config/dispatcher/network/@client", "localhost:" +  // $$$$ AGE 2007-10-09:
                            boost::lexical_cast< std::string >( frontend::SimulationPort( exerciseNumber_->value() ) ) );
        action.SetOption( "session/config/dispatcher/network/@server", frontend::DispatcherPort( exerciseNumber_->value() ) );
    }
    {
        action.SetOption( "session/config/gaming/network/@server", "localhost:" +  // $$$$ AGE 2007-10-09:
                                    boost::lexical_cast< std::string >( frontend::DispatcherPort( exerciseNumber_->value() ) ) );
    }
    action.Commit();
}
