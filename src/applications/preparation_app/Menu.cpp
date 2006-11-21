// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "Menu.h"
//#include "AboutDialog.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/TristateOption.h"
#include "clients_gui/OptionMenu.h"
#include "clients_gui/resources.h"

using namespace kernel;
using namespace gui;

namespace
{
    void Populate( OptionMenu< TristateOption >& menu )
    {
        menu.AddItem( TristateOption::AutoName(), TristateOption::Auto() );
        menu.AddItem( TristateOption::OnName(), TristateOption::On() );
        menu.AddItem( TristateOption::OffName(), TristateOption::Off() );
    }

    void AddSubMenu( QPopupMenu* parent, const QString& label, const QIconSet& iconSet, Options& options, const std::string& option )
    {
        OptionMenu< TristateOption >* optionMenu = new OptionMenu< TristateOption >( parent, options, option );
        Populate( *optionMenu );
        parent->insertItem( iconSet, label, optionMenu );
    }
}

// -----------------------------------------------------------------------------
// Name: Menu constructor
// Created: SBO 2006-04-28
// -----------------------------------------------------------------------------
Menu::Menu( QMainWindow* pParent, Controllers& controllers, QDialog& prefDialog )
    : QMenuBar( pParent )
{
    QPopupMenu* menu = new QPopupMenu( this );
    menu->insertItem( tr( "&New..." ), parent(), SLOT( New() ), CTRL + Key_N );
    menu->insertItem( MAKE_ICON( open ), tr( "&Open..." ), parent(), SLOT( Open() ), CTRL + Key_O );
    menu->insertItem( tr( "Close" ), parent(), SLOT( Close() ) );
    menu->insertSeparator();
    menu->insertItem( MAKE_ICON( save ), tr( "&Save" ), parent(), SLOT( Save() ), CTRL + Key_S );
    menu->insertItem( MAKE_ICON( saveas ), tr( "&Save as..." ), parent(), SLOT( SaveAs() ) );
    menu->insertSeparator();
    menu->insertItem( tr( "&Quit" ), pParent, SLOT( close() ), CTRL + Key_Q );
    insertItem( tr( "&File" ), menu, ALT + Key_F );

    menu = new QPopupMenu( this );
    QPopupMenu* subMenu = new QPopupMenu( menu );

    AddSubMenu( subMenu, tr( "Logistic links" )        , MAKE_ICON( loglink )    , controllers.options_, "LogisticLinks" );
    AddSubMenu( subMenu, tr( "Missing logistic links" ), MAKE_ICON( missinglog ) , controllers.options_, "MissingLogisticLinks" );
    menu->insertItem( tr( "Logistic..." ), subMenu );

    subMenu = new QPopupMenu( menu );
    AddSubMenu( subMenu, tr( "Small text" )    , MAKE_ICON( textsmall )    , controllers.options_, "SmallText" );
    AddSubMenu( subMenu, tr( "Large text" )    , MAKE_ICON( textbig )      , controllers.options_, "BigText" );
    AddSubMenu( subMenu, tr( "Tactical lines" ), MAKE_ICON( tacticallines ), controllers.options_, "TacticalLines" );

    subMenu->insertSeparator();

    OptionMenu< float >* gridMenu = new OptionMenu< float >( subMenu, controllers.options_, "GridSize" );
    gridMenu->AddItem( tr( "Off"    ),  -1 );
    gridMenu->AddItem( tr( "100m"  ),  0.1f );
    gridMenu->AddItem( tr( "250m" ),  0.25f );
    gridMenu->AddItem( tr( "500m"  ),  0.5f );
    gridMenu->AddItem( tr( "1km"  ),  1.0f );
    gridMenu->AddItem( tr( "2.5km"  ),  2.5f );
    gridMenu->AddItem( tr( "5km"  ),  5.0f );
    gridMenu->AddItem( tr( "10km" ), 10.0f );
    subMenu->insertItem( tr( "Grid" ), gridMenu );

    menu->insertItem( tr( "Terrain..." ), subMenu );
    menu->insertSeparator();

    menu->insertItem( tr( "&Preferences..." ), &prefDialog, SLOT( exec() ), CTRL + Key_P );
    insertItem( tr( "&Display" ), menu, ALT + Key_D );

//    menu = new QPopupMenu( this );
//    menu->insertItem( tr( "A propos" ), new AboutDialog( this, factory ), SLOT( exec() ) );
//    itemId = insertItem( tr( "Ai&de" ), menu );
//    setAccel( ALT + Key_D, itemId );
}
    
// -----------------------------------------------------------------------------
// Name: Menu destructor
// Created: SBO 2006-04-28
// -----------------------------------------------------------------------------
Menu::~Menu()
{
    // NOTHING
}
