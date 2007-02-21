// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoButtonsWidget.h"
#include "icons.h"
#include "AgentMaintenancePanel.h"
#include "AgentMedicalPanel.h"
#include "AgentSupplyPanel.h"
#include "InfoCompositionDialog.h"
#include "InfoDotationsDialog.h"
#include "InfoLendingsDialog.h"
#include "InfoMaintenanceDialog.h"
#include "InfoMedicalDialog.h"
#include "InfoSupplyDialog.h"

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget constructor
// Created: SBO 2007-02-05
// -----------------------------------------------------------------------------
InfoButtonsWidget::InfoButtonsWidget( QWidget* widget, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : QGroupBox( 2, Qt::Horizontal, widget )
{
    setFlat( true );
    setFixedWidth( 100 );
    setFrameStyle( QFrame::Panel | QFrame::Sunken );
    layout()->setMargin( 0 );
    setInsideMargin( 0 );
    setInsideSpacing( 0 );
    AddButton< InfoCompositionDialog >( MAKE_PIXMAP( components ) , controllers, factory );
    AddButton< InfoMaintenanceDialog >( MAKE_PIXMAP( maintenance ), controllers, factory );
    AddButton< InfoDotationsDialog >  ( MAKE_PIXMAP( dotations )  , controllers, factory );
    AddButton< InfoMedicalDialog >    ( MAKE_PIXMAP( medical )    , controllers, factory );
    AddButton< InfoLendingsDialog >   ( MAKE_PIXMAP( lendborrow ) , controllers, factory );
    AddButton< InfoSupplyDialog>      ( MAKE_PIXMAP( supply )     , controllers, factory );
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget destructor
// Created: SBO 2007-02-05
// -----------------------------------------------------------------------------
InfoButtonsWidget::~InfoButtonsWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget::AddButton
// Created: SBO 2007-02-05
// -----------------------------------------------------------------------------
template< typename Dialog >
void InfoButtonsWidget::AddButton( const QPixmap& pixmap, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
{
    QPushButton* btn = new QPushButton( this );
    btn->setToggleButton( true );
    btn->setPixmap( pixmap );
    btn->setFixedSize( 50, 50 );
    btn->setDisabled( true );
    QDialog* dialog = new Dialog( topLevelWidget(), controllers, factory );
    QToolTip::add( btn, dialog->caption() );
    widgets_[btn] = dialog;
    connect( btn, SIGNAL( toggled( bool ) ), dialog, SLOT( OnToggle( bool ) ) );
    connect( dialog, SIGNAL( Closed() ), btn, SLOT( toggle() ) );
    connect( dialog, SIGNAL( Disabled( bool ) ), btn, SLOT( setDisabled( bool ) ) );
}
