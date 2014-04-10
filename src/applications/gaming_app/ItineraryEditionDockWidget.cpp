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
    : gui::RichDockWidget( controllers, parent, "ItineraryEdition-dock-widget" )
{
    // Init
    setCaption( tr( "Itinerary creation" ) );

    // Main Layout
    QWidget* mainWidget = new QWidget( this );
    QHBoxLayout* mainLayout = new QHBoxLayout( mainWidget );
    QPushButton* okButton = new gui::RichPushButton( "ItineraryEdition-dock-widget-ok-button", tr( "Ok" ), mainWidget );
    QPushButton* cancelButton = new gui::RichPushButton( "ItineraryEdition-dock-widget-cancel-button", tr( "Cancel" ), mainWidget );
    mainLayout->addWidget( okButton );
    mainLayout->addWidget( cancelButton );
    setWidget( mainWidget );

    // Connections
    connect( okButton,     SIGNAL( clicked( bool ) ), this, SLOT( OnItineraryAccepted() ) );
    connect( cancelButton, SIGNAL( clicked( bool ) ), this, SLOT( OnItineraryRejected() ) );
}

// -----------------------------------------------------------------------------
// Name: ItineraryEditionDockWidget destructor
// Created: SLI 2014-04-09
// -----------------------------------------------------------------------------
ItineraryEditionDockWidget::~ItineraryEditionDockWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ItineraryEditionDockWidget::OnItineraryAccepted
// Created: SLI 2014-04-10
// -----------------------------------------------------------------------------
void ItineraryEditionDockWidget::OnItineraryAccepted()
{
    controllers_.ChangeMode( eModes_Gaming );
    emit ItineraryAccepted();
}

// -----------------------------------------------------------------------------
// Name: ItineraryEditionDockWidget::OnItineraryRejected
// Created: SLI 2014-04-10
// -----------------------------------------------------------------------------
void ItineraryEditionDockWidget::OnItineraryRejected()
{
    controllers_.ChangeMode( eModes_Gaming );
    emit ItineraryRejected();
}
