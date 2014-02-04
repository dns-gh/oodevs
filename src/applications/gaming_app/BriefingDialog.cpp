// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "BriefingDialog.h"
#include "clients_kernel/tools.h"
#include "gaming/CommandPublisher.h"
#include "gaming/CommandHandler.h"
#include "gaming/Command.h"
#include "tools/ExerciseConfig.h"
#include <string>

// -----------------------------------------------------------------------------
// Name: BriefingDialog constructor
// Created: SBO 2008-07-02
// -----------------------------------------------------------------------------
BriefingDialog::BriefingDialog( QWidget* parent, kernel::Controllers& controllers, Publisher_ABC& publisher, CommandHandler& handler, const tools::ExerciseConfig& config )
    : QDialog( parent )
    , config_( config )
    , publisher_( new CommandPublisher( controllers, publisher ) )
    , handler_( handler )
{
    setCaption( tools::translate( "BriefingDialog", "Briefing" ) );
    Q3VBoxLayout* mainLayout = new Q3VBoxLayout( this );
    mainLayout->setMargin( 0 );

    browser_ = new Q3TextBrowser( this );
    browser_->setFrameStyle( Q3Frame::Plain );
    mainLayout->addWidget( browser_ );
    {
        Q3HBox* box = new Q3HBox( this );
        box->layout()->setAlignment( Qt::AlignRight );
        QPushButton* ok = new QPushButton( tools::translate( "BriefingDialog", "Ok" ), box );
        ok->setFocus();
        mainLayout->addWidget( box );
        connect( ok, SIGNAL( clicked() ), SLOT( accept() ) );
    }
    hide();
    handler_.Register( "briefing", *this );
}

// -----------------------------------------------------------------------------
// Name: BriefingDialog destructor
// Created: SBO 2008-07-02
// -----------------------------------------------------------------------------
BriefingDialog::~BriefingDialog()
{
    handler_.Unregister( "briefing", *this );
}

// -----------------------------------------------------------------------------
// Name: BriefingDialog::Receive
// Created: SBO 2008-07-02
// -----------------------------------------------------------------------------
void BriefingDialog::Receive( const Command& command )
{
    if( command.ArgumentCount() < 2 )
        return;
    dialogId_ = command.Argument( 1 );
    Load( config_.BuildExerciseChildFile( "scripts" ) / tools::Path::FromUTF8( command.Argument( 2 ) ) );
    setMinimumSize( 600, 700 );
    show();
}

// -----------------------------------------------------------------------------
// Name: BriefingDialog::Load
// Created: SBO 2008-07-02
// -----------------------------------------------------------------------------
void BriefingDialog::Load( const tools::Path& filename )
{
    browser_->setSource( filename.Absolute().ToUTF8().c_str() );
}

// -----------------------------------------------------------------------------
// Name: BriefingDialog::reject
// Created: SBO 2008-07-03
// -----------------------------------------------------------------------------
void BriefingDialog::reject()
{
    accept();
}

// -----------------------------------------------------------------------------
// Name: BriefingDialog::accept
// Created: SBO 2008-07-02
// -----------------------------------------------------------------------------
void BriefingDialog::accept()
{
    const std::string message = "/choose " + dialogId_ + " 'ok'";
    publisher_->Send( "", message );
    QDialog::accept();
}

