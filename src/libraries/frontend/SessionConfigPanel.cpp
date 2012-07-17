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
#pragma warning( push, 0 )
#include <QtGui/qlabel.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qspinbox.h>
#include <Qt3Support/q3textedit.h>
#include <Qt3Support/q3vbox.h>
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
    Q3VBox* exerciseBox = new Q3VBox( this );
    exerciseBox->setMargin( 5 );
    {
        Q3HBox* sessionBox = new Q3HBox( exerciseBox );
        nameLabel_ = new QLabel( sessionBox );
        sessionBox->setStretchFactor( nameLabel_, 1 );
        sessionName_ = new QLineEdit( sessionBox );
        sessionBox->setStretchFactor( sessionName_, 2 );
        sessionName_->setText( "" );
    }
    {
        Q3HBox* commentBox = new Q3HBox( exerciseBox );
        commentLabel_ = new QLabel( commentBox );
        commentBox->setStretchFactor( commentLabel_, 1 );
        sessionComment_ = new Q3TextEdit( commentBox );
        commentBox->setStretchFactor( sessionComment_, 2 );
        sessionComment_->setText( "" );
    }
    {
        Q3HBox* exerciseNumberBox = new Q3HBox( exerciseBox );
        exerciseLabel_ = new QLabel( exerciseNumberBox );
        exerciseNumberBox->setStretchFactor( exerciseLabel_, 1 );
        exerciseNumber_ = new QSpinBox( 1, 10, 1, exerciseNumberBox );
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
void SessionConfigPanel::Commit( const std::string& exercise, const std::string& session )
{
    frontend::CreateSession action( config_, exercise, session );
    {
        action.SetOption( "session/meta/date", session );
        action.SetOption( "session/meta/name", sessionName_->text().toUtf8().constData() );
        action.SetOption( "session/meta/comment", sessionComment_->text().toUtf8().constData() );
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
