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
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* okButton = new gui::RichPushButton( "itineraryEdition-dock-widget-ok-button", tr( "Ok" ) );
    QPushButton* cancelButton = new gui::RichPushButton( "itineraryEdition-dock-widget-cancel-button", tr( "Cancel" ) );
    buttonLayout->addStretch( 1 );
    buttonLayout->addWidget( okButton );
    buttonLayout->addWidget( cancelButton );
    QWidget* mainWidget = new QWidget( this );
    QVBoxLayout* mainLayout = new QVBoxLayout( mainWidget );
    QLabel* tipLabel = new QLabel( "<p>" + tr( "Set start and end points by right-clicking on the map. "
                                               "Once the initial itinerary has been calculated, waypoints can be added and"
                                               "modified repeatedly by dragging and dropping from any position on the current itinerary." ) + "</p>" +
                                   "<p>" + tr( "Tip: Press <i>Control</i> to drag and drop to the exact position of your choice instead of snapping to the nearest road." ) + "</p>" );
    tipLabel->setWordWrap( true );
    mainLayout->addWidget( tipLabel );
    mainLayout->addStretch( 1 );
    mainLayout->addLayout( buttonLayout );
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
