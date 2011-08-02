// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ChatRoom.h"
#include "moc_ChatRoom.cpp"
#include "gaming/CommandPublisher.h"
#include "gaming/CommandHandler.h"
#include "gaming/Command.h"
#include <string>

// -----------------------------------------------------------------------------
// Name: ChatRoom constructor
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
ChatRoom::ChatRoom( QWidget* parent, CommandPublisher& publisher, CommandHandler& handler, QString filter )
    : Q3VBox( parent )
    , publisher_( publisher )
    , handler_( handler )
    , filter_( filter )
{
    history_ = new Q3TextEdit( this );
    history_->setReadOnly( true );
    {
        Q3HBox* box = new Q3HBox( this );
        text_ = new QLineEdit( box );
        sendBtn_ = new QPushButton( tr( "Send" ), box );
        sendBtn_->setDisabled( true );
        box->setStretchFactor( text_, 5 );
        box->setStretchFactor( sendBtn_, 1 );
    }
    connect( text_, SIGNAL( textChanged( const QString& ) ), SLOT( OnTextChanged( const QString& ) ) );
    connect( text_, SIGNAL( returnPressed() ), SLOT( OnSend() ) );
    connect( sendBtn_, SIGNAL( clicked() ), SLOT( OnSend() ) );

    handler_.Register( "text", *this );
}

// -----------------------------------------------------------------------------
// Name: ChatRoom destructor
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
ChatRoom::~ChatRoom()
{
    handler_.Unregister( "text", *this );
}

// -----------------------------------------------------------------------------
// Name: ChatRoom::OnTextChanged
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
void ChatRoom::OnTextChanged( const QString& text )
{
    sendBtn_->setDisabled( text.isEmpty() );
}

// -----------------------------------------------------------------------------
// Name: ChatRoom::OnSend
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
void ChatRoom::OnSend()
{
    Send( filter_.ascii() );
}

// -----------------------------------------------------------------------------
// Name: ChatRoom::Send
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
void ChatRoom::Send( const std::string& target )
{
    if( !text_->text().isEmpty() )
    {
        const std::string baseMessage = text_->text().ascii();
        const std::string message = *baseMessage.begin() == '/' ? baseMessage : "/text " + baseMessage;
        publisher_.Send( target, message );
        text_->clear();
    }
}

// -----------------------------------------------------------------------------
// Name: ChatRoom::Matches
// Created: AGE 2008-08-14
// -----------------------------------------------------------------------------
bool ChatRoom::Matches( const QString& source, const QString& target ) const
{
    return ( source == filter_ && target == publisher_.SelfProfile() )
        || ( target == filter_ && source == publisher_.SelfProfile() )
        || ( filter_ == "" && target == "" );
}

// -----------------------------------------------------------------------------
// Name: ChatRoom::Receive
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
void ChatRoom::Receive( const Command& command )
{
    if( command.ArgumentCount() == 0 )
        return;
    if( Matches( command.Source().c_str(), command.Target().c_str() ) )
    {
        QString sender = command.Source().c_str();
        if( sender == publisher_.SelfProfile() )
            sender = tr( "Me" );
        QString text( "" );
        for( unsigned i = 1; i <= command.ArgumentCount(); ++i )
        {
            if( ! text.isEmpty() )
                text += " ";
            text += command.Argument( i ).c_str();
        }
        history_->append( "<b>" + sender + ":</b> " + text );
    }
}
