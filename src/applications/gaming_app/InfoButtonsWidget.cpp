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
#include "moc_InfoButtonsWidget.cpp"
#include "icons.h"
#include "AgentResourcesPanel.h"
#include "AgentMaintenancePanel.h"
#include "AgentMedicalPanel.h"
#include "AgentStatePanel.h"
#include "AgentSupplyPanel.h"
#include "InfoDialog.h"

namespace
{
    template< typename T >
    InfoDialog* MakeDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const QString& title )
    {
        InfoDialog* dialog = new InfoDialog( parent, title );
        new T( dialog->RootWidget(), *dialog, controllers, factory );
        return dialog;
    }
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget constructor
// Created: SBO 2007-02-05
// -----------------------------------------------------------------------------
InfoButtonsWidget::InfoButtonsWidget( QWidget* widget, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : QGroupBox( 2, Qt::Horizontal, widget )
{
    setFlat( true );
    setFixedSize( 100, 150 );
    setFrameStyle( QFrame::Panel | QFrame::Sunken );
    layout()->setMargin( 0 );
    setInsideMargin( 0 );
    setInsideSpacing( 0 );
    AddButton( MAKE_PIXMAP( components ) , MakeDialog< AgentResourcesPanel   >( topLevelWidget(), controllers, factory, tr( "Equipments and humans" ) ) );
    AddButton( MAKE_PIXMAP( maintenance ), MakeDialog< AgentMaintenancePanel >( topLevelWidget(), controllers, factory, tr( "Maintenance system" ) ) );
    AddButton( MAKE_PIXMAP( dotations )  , MakeDialog< AgentResourcesPanel   >( topLevelWidget(), controllers, factory, tr( "Dotations" ) ) );
    AddButton( MAKE_PIXMAP( medical )    , MakeDialog< AgentMedicalPanel     >( topLevelWidget(), controllers, factory, tr( "Medical system" ) ) );
    AddButton( MAKE_PIXMAP( lendborrow ) , MakeDialog< AgentStatePanel       >( topLevelWidget(), controllers, factory, tr( "Lent and borrowed units" ) ) );
    AddButton( MAKE_PIXMAP( supply )     , MakeDialog< AgentSupplyPanel      >( topLevelWidget(), controllers, factory, tr( "Supply system" ) ) );
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
void InfoButtonsWidget::AddButton( const QPixmap& pixmap, InfoDialog* dialog )
{
    QPushButton* btn = new QPushButton( this );
    btn->setToggleButton( true );
    btn->setPixmap( pixmap );
    btn->setFixedSize( 50, 50 );
    QToolTip::add( btn, dialog->caption() );
    widgets_[btn] = dialog;
    connect( btn, SIGNAL( toggled( bool ) ), SLOT( OnToggle( bool ) ) );
    connect( dialog, SIGNAL( Closed() ), SLOT( OnClose() ) );
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget::OnToggle
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
void InfoButtonsWidget::OnToggle( bool state )
{
    QObject* obj = const_cast< QObject* >( sender() );
    if( obj->inherits( "QButton" ) )
    {
        QButton* btn = static_cast< QButton* >( obj );
        widgets_[btn]->setShown( state );
    }
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget::OnClose
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
void InfoButtonsWidget::OnClose()
{
    const QObject* obj = sender();
    if( obj->inherits( "QDialog" ) )
    {
        const QDialog* dlg = static_cast< const QDialog* >( obj );
        for( IT_ButtonWidgets it = widgets_.begin(); it != widgets_.end(); ++it )
            if( it->second == dlg )
            {
                if( it->first->isOn() )
                    it->first->toggle();
                return;
            }
    }
}
