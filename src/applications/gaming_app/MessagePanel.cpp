// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MessagePanel.h"
#include "moc_MessagePanel.cpp"
#include "gaming/Command.h"
#include "gaming/CommandHandler.h"
#include "gaming/CommandPublisher.h"
#include <string>
#include <qhbuttongroup.h>

// -----------------------------------------------------------------------------
// Name: MessagePanel constructor
// Created: SBO 2009-03-04
// -----------------------------------------------------------------------------
MessagePanel::MessagePanel( QMainWindow* mainWindow, kernel::Controllers& controllers, Publisher_ABC& publisher, CommandHandler& handler )
    : QDockWindow( mainWindow, "message" )
    , mainWindow_( mainWindow )
    , handler_( handler )
    , publisher_( new CommandPublisher( controllers, publisher ) )
{
    setResizeEnabled( false );
    setMovingEnabled( false );
    setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Maximum );
    setPaletteBackgroundColor( QColor( 255, 255, 225 ) );

    QHBox* box = new QHBox( this );
    box->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    box->setMargin( 5 );
    {
        text_ = new QLabel( box );
        text_->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Maximum );
        QFont font( text_->font() );
        font.setPixelSize( 14 );
        text_->setFont( font );
        text_->setTextFormat( Qt::RichText );
    }
    {
        buttons_ = new QHButtonGroup( box );
        buttons_->setFrameStyle( QFrame::Plain );
        buttons_->setInsideMargin( 5 );
        buttons_->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
        buttons_->hide();
        connect( buttons_, SIGNAL( clicked( int ) ), this, SLOT( OnButtonPressed( int ) ) );
    }
    setWidget( box );
    hide();

    handler_.Register( "display", *this );
    handler_.Register( "prompt", *this );
}

// -----------------------------------------------------------------------------
// Name: MessagePanel destructor
// Created: SBO 2009-03-04
// -----------------------------------------------------------------------------
MessagePanel::~MessagePanel()
{
    handler_.Unregister( "display", *this );
    handler_.Unregister( "prompt", *this );
}

// -----------------------------------------------------------------------------
// Name: MessagePanel::Receive
// Created: SBO 2009-03-05
// -----------------------------------------------------------------------------
void MessagePanel::Receive( const Command& command )
{
    if( command.ArgumentCount() == 0 )
    {
        Clear();
        return;
    }
    if( command.Name() == "display" )
    {
        ClearButtons();
        Display( command.Argument( 1 ).c_str() );
    }
    else if( command.Name() == "prompt" && command.ArgumentCount() > 2 )
    {
        ClearButtons();
        activePrompt_ = command.Argument( 1 );
        Display( command.Argument( 2 ).c_str() );
        const QStringList buttons = QStringList::split( '|', command.Argument( 3 ).c_str() );
        for( QStringList::const_iterator it = buttons.begin(); it != buttons.end(); ++it )
            AddButton( *it );
        buttons_->show();
    }
}

// -----------------------------------------------------------------------------
// Name: MessagePanel::Display
// Created: SBO 2009-03-05
// -----------------------------------------------------------------------------
void MessagePanel::Display( const QString& message )
{
    QString formatted( message );
    formatted.replace( '\n', "<br>" );
    text_->setText( formatted );
    mainWindow_->moveDockWindow( this, Qt::DockTop, true, -1 );
    show();
}

// -----------------------------------------------------------------------------
// Name: MessagePanel::AddButton
// Created: SBO 2009-03-05
// -----------------------------------------------------------------------------
void MessagePanel::AddButton( const QString& title )
{
    buttons_->insert( new QPushButton( title, buttons_ ) );
}

// -----------------------------------------------------------------------------
// Name: MessagePanel::Clear
// Created: SBO 2009-03-05
// -----------------------------------------------------------------------------
void MessagePanel::Clear()
{
    Display( "" );
    ClearButtons();
    hide();
}

// -----------------------------------------------------------------------------
// Name: MessagePanel::ClearButtons
// Created: SBO 2009-03-05
// -----------------------------------------------------------------------------
void MessagePanel::ClearButtons()
{
    const int count = buttons_->count();
    for( int i = 0; i < count; ++i )
        if( QButton* button = buttons_->find( i ) )
        {
            buttons_->remove( button );
            button->deleteLater();
        }
    buttons_->hide();
}

// -----------------------------------------------------------------------------
// Name: MessagePanel::OnButtonPressed
// Created: SBO 2009-03-05
// -----------------------------------------------------------------------------
void MessagePanel::OnButtonPressed( int id )
{
    if( QButton* button = buttons_->find( id ) )
    {
        const std::string message = "/choose " + activePrompt_ + " \"" + button->text().ascii() + "\"";
        publisher_->Send( "", message );
    }
}

// -----------------------------------------------------------------------------
// Name: MessagePanel::sizeHint
// Created: SBO 2009-03-04
// -----------------------------------------------------------------------------
QSize MessagePanel::sizeHint() const
{
    return QSize( mainWindow_->width(), std::max( text_->height(), 40 ) );
}
