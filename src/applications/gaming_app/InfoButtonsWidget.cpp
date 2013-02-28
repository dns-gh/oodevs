// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoButtonsWidget.h"
#include "icons.h"
#include "InfoCompositionDialog.h"
#include "InfoMaintenanceDialog.h"
#include "InfoMedicalDialog.h"
#include "InfoSupplyDialog.h"
#include "InfoFuneralDialog.h"
#include "UnitStateDialog.h"
#include "tools/GeneralConfig.h"

namespace
{
    QPixmap MakePixmap( const std::string& name )
    {
        return QPixmap( tools::GeneralConfig::BuildResourceChildFile( std::string( "images/gaming/" ) + name + ".png" ).c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget constructor
// Created: SBO 2007-02-05
// -----------------------------------------------------------------------------
InfoButtonsWidget::InfoButtonsWidget( QWidget* widget, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory
                                    , kernel::DisplayExtractor_ABC& extractor, const StaticModel& staticModel
                                    , actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile )
    : Q3GroupBox( 2, Qt::Horizontal, widget, "InfoButtonsWidget" )
{
    setFlat( true );
    setFixedWidth( 100 );
    setFrameStyle( Q3Frame::Panel | Q3Frame::Sunken );
    layout()->setMargin( 0 );
    setInsideMargin( 0 );
    setInsideSpacing( 0 );

    UnitStateDialog* unitStateDialog = new UnitStateDialog( topLevelWidget(), controllers, staticModel, actionsModel, simulation, profile );
    AddButton< InfoCompositionDialog >( MakePixmap( "composition" ), controllers, factory );
    //AddButton( unitStateDialog, MakePixmap( "composition" ), unitStateDialog->GetEquipmentToolTip(), SLOT( ToggleEquipment( bool ) ), SIGNAL( OnToggleEquipment( bool ) ) );
    AddButton( unitStateDialog, MakePixmap( "ordnance" ), unitStateDialog->GetResourceToolTip(), SLOT( ToggleResource( bool ) ), SIGNAL( OnToggleResource( bool ) ) );
    AddButton< InfoMedicalDialog >    ( MakePixmap( "health"      ), controllers, factory, extractor );
    AddButton< InfoMaintenanceDialog >( MakePixmap( "maintenance" ), controllers, factory, extractor );
    AddButton< InfoSupplyDialog >     ( MakePixmap( "supply"      ), controllers, factory, extractor );
    AddButton< InfoFuneralDialog >    ( MakePixmap( "mortuary"    ), controllers, extractor );
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget destructor
// Created: SBO 2007-02-05
// -----------------------------------------------------------------------------
InfoButtonsWidget::~InfoButtonsWidget()
{
    // NOTHING
}

namespace
{
    QPushButton* CreateButton( QWidget* parent, const QPixmap& pixmap )
    {
        QPushButton* btn = new QPushButton( parent );
        btn->setToggleButton( true );
        btn->setPixmap( pixmap );
        btn->setFixedSize( 50, 50 );
        btn->setDisabled( true );
        return btn;
    }
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget::AddButton
// Created: SBO 2007-02-05
// -----------------------------------------------------------------------------
template< typename Dialog >
void InfoButtonsWidget::AddButton( const QPixmap& pixmap, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
{
    QPushButton* btn = CreateButton( this, pixmap );
    QDialog* dialog = new Dialog( topLevelWidget(), controllers, factory );
    QToolTip::add( btn, dialog->caption() );
    connect( btn, SIGNAL( toggled( bool ) ), dialog, SLOT( OnToggle( bool ) ) );
    connect( dialog, SIGNAL( Closed() ), btn, SLOT( toggle() ) );
    connect( dialog, SIGNAL( Disabled( bool ) ), btn, SLOT( setDisabled( bool ) ) );
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget::AddButton
// Created: SBO 2007-02-05
// -----------------------------------------------------------------------------
template< typename Dialog >
void InfoButtonsWidget::AddButton( const QPixmap& pixmap, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory
                                 , kernel::DisplayExtractor_ABC& extractor )
{
    QPushButton* btn = CreateButton( this, pixmap );
    QDialog* dialog = new Dialog( topLevelWidget(), controllers, factory, extractor );
    QToolTip::add( btn, dialog->caption() );
    connect( btn, SIGNAL( toggled( bool ) ), dialog, SLOT( OnToggle( bool ) ) );
    connect( dialog, SIGNAL( Closed() ), btn, SLOT( toggle() ) );
    connect( dialog, SIGNAL( Disabled( bool ) ), btn, SLOT( setDisabled( bool ) ) );
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget::AddButton
// Created: SBO 2007-02-05
// -----------------------------------------------------------------------------
template< typename Dialog >
void InfoButtonsWidget::AddButton( const QPixmap& pixmap, kernel::Controllers& controllers, kernel::DisplayExtractor_ABC& extractor )
{
    QPushButton* btn = CreateButton( this, pixmap );
    QDialog* dialog = new Dialog( topLevelWidget(), controllers, extractor );
    QToolTip::add( btn, dialog->caption() );
    connect( btn, SIGNAL( toggled( bool ) ), dialog, SLOT( OnToggle( bool ) ) );
    connect( dialog, SIGNAL( Closed() ), btn, SLOT( toggle() ) );
    connect( dialog, SIGNAL( Disabled( bool ) ), btn, SLOT( setDisabled( bool ) ) );
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget::AddButton
// Created: ABR 2011-07-07
// -----------------------------------------------------------------------------
void InfoButtonsWidget::AddButton( QDialog* dialog, const QPixmap& pixmap, const QString& tooltips, const char* toggleSlot, const char* toggleSignal )
{
    QPushButton* btn = CreateButton( this, pixmap );
    QToolTip::add( btn, tooltips );
    connect( btn, SIGNAL( toggled( bool ) ), dialog, toggleSlot );
    connect( dialog, toggleSignal, btn, SLOT( setOn( bool ) ) );
    connect( dialog, SIGNAL( Disabled( bool ) ), btn, SLOT( setDisabled( bool ) ) );
}
