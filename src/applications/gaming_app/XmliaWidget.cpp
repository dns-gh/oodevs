// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "XmliaWidget.h"
#include "XmliaReport.h"
#include "moc_XmliaWidget.cpp"
#include "gaming/CommandPublisher.h"
#include "gaming/Command.h"
#include "gaming/CommandHandler.h"
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: XmliaWidget constructor
// Created: RPD 2009-06-11
// -----------------------------------------------------------------------------
XmliaWidget::XmliaWidget( QWidget* parent, kernel::Controllers& controllers, Publisher_ABC& publisher, CommandHandler& handler )
    : QListView( parent )
    , controllers_( controllers )
    , handler_( handler )
    , publisher_( new CommandPublisher( controllers, publisher ) )
{
    handler_.Register( "xmlia", *this );
    controllers_.Register( *this );

    setMinimumSize( 40, 40);
    setShowSortIndicator( true );
    setSorting( -1 );
    setRootIsDecorated( true );
    addColumn( tr( "Date" ) );
    addColumn( tr( "Message" ) );
    setResizeMode( QListView::LastColumn );
    setAllColumnsShowFocus ( true );
}

// -----------------------------------------------------------------------------
// Name: XmliaWidget destructor
// Created: RPD 2009-06-11
// -----------------------------------------------------------------------------
XmliaWidget::~XmliaWidget()
{
    controllers_.Unregister( *this );
    handler_.Unregister( "xmlia", *this );
}

// -----------------------------------------------------------------------------
// Name: XmliaWidget::Receive
// Created: RPD 2009-08-14
// -----------------------------------------------------------------------------
void XmliaWidget::Receive( const Command& command )
{
  if( command.ArgumentCount() != 1 )
    return;

  new XmliaReport( this, QString( command.Argument( 1 ).c_str() ) );
}
