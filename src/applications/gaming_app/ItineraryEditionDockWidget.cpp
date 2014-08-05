// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ItineraryEditionDockWidget.h"
#include "moc_ItineraryEditionDockWidget.cpp"
#include "clients_gui/RichPushButton.h"
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: ItineraryEditionDockWidget constructor
// Created: SLI 2014-04-09
// -----------------------------------------------------------------------------
ItineraryEditionDockWidget::ItineraryEditionDockWidget( QWidget* parent, kernel::Controllers& controllers )
    : gui::RichDockWidget( controllers, parent, "itineraryEdition-dock-widget" )
{
    setCaption( tr( "Itinerary creation" ) );
    setFloating( true );
    setVisible( false );
    QWidget* mainWidget = new QWidget( this );
    QHBoxLayout* mainLayout = new QHBoxLayout( mainWidget );
    mainLayout->setAlignment( Qt::AlignRight | Qt::AlignBottom );
    QPushButton* okButton = new gui::RichPushButton( "itineraryEdition-dock-widget-ok-button", tr( "Ok" ), mainWidget );
    QPushButton* cancelButton = new gui::RichPushButton( "itineraryEdition-dock-widget-cancel-button", tr( "Cancel" ), mainWidget );
    mainLayout->addWidget( okButton );
    mainLayout->addWidget( cancelButton );
    setWidget( mainWidget );
    connect( okButton, SIGNAL( clicked( bool ) ), this, SIGNAL( ItineraryAccepted() ) );
    connect( cancelButton, SIGNAL( clicked( bool ) ), this, SIGNAL( ItineraryRejected() ) );
}

// -----------------------------------------------------------------------------
// Name: ItineraryEditionDockWidget destructor
// Created: SLI 2014-04-09
// -----------------------------------------------------------------------------
ItineraryEditionDockWidget::~ItineraryEditionDockWidget()
{
    // NOTHING
}

void ItineraryEditionDockWidget::showEvent( QShowEvent* event )
{
    gui::RichDockWidget::showEvent( event );
    raise();
}
