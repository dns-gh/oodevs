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
#include "moc_Menu.cpp"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/TristateOption.h"
#include "clients_kernel/FourStateOption.h"
#include "clients_gui/OptionMenu.h"
#include "clients_gui/resources.h"
#include "clients_gui/AboutDialog.h"

using namespace kernel;
using namespace gui;

namespace
{
    // $$$$ AGE 2007-05-31: ^c^v
    void Populate( OptionMenu< TristateOption >& menu )
    {
        menu.AddItem( TristateOption::AutoName(), TristateOption::Auto() );
        menu.AddItem( TristateOption::OnName(), TristateOption::On() );
        menu.AddItem( TristateOption::OffName(), TristateOption::Off() );
    }
    void Populate( OptionMenu< FourStateOption >& menu )
    {
        menu.AddItem( FourStateOption::OnName(), FourStateOption::On() );
        menu.AddItem( FourStateOption::SuperiorSelectedName(), FourStateOption::SuperiorSelected() );
        menu.AddItem( FourStateOption::SelectedName(), FourStateOption::Selected() );
        menu.AddItem( FourStateOption::OffName(), FourStateOption::Off() );
    }

    void AddSubMenu3( QPopupMenu* parent, const QString& label, const QIconSet& iconSet, Options& options, const std::string& option )
    {
        OptionMenu< TristateOption >* optionMenu = new OptionMenu< TristateOption >( parent, options, option );
        Populate( *optionMenu );
        parent->insertItem( iconSet, label, optionMenu );
    }

    void AddSubMenu4( QPopupMenu* parent, const QString& label, const QIconSet& iconSet, Options& options, const std::string& option )
    {
        OptionMenu< FourStateOption >* optionMenu = new OptionMenu< FourStateOption >( parent, options, option );
        Populate( *optionMenu );
        parent->insertItem( iconSet, label, optionMenu );
    }
}

// -----------------------------------------------------------------------------
// Name: Menu constructor
// Created: SBO 2006-04-28
// -----------------------------------------------------------------------------
Menu::Menu( QMainWindow* pParent, Controllers& controllers, QDialog& prefDialog, QDialog& profileDialog, QDialog& profileWizardDialog, QDialog& importDialog, gui::ItemFactory_ABC& factory, const QString& license )
    : QMenuBar( pParent )
{
    QPopupMenu* menu = new QPopupMenu( this );
    menu->insertItem( MAKE_ICON( new ) , tr( "&New..." ) , parent(), SLOT( New() ) , CTRL + Key_N );
    menu->insertItem( MAKE_ICON( open ), tr( "&Open..." ), parent(), SLOT( Open() ), CTRL + Key_O );
    menu->insertItem( tr( "Close" ), parent(), SLOT( Close() ) );
    menu->insertSeparator();
    menu->insertItem( tr( "&Import..." ), &importDialog, SLOT( exec() ), CTRL + Key_I );
    menu->insertSeparator();
    menu->insertItem( MAKE_ICON( save )  , tr( "&Save" )      , parent(), SLOT( Save() ), CTRL + Key_S );
    menu->insertSeparator();
    menu->insertItem( tr( "&Quit" ), pParent, SLOT( close() ), CTRL + Key_Q );
    insertItem( tr( "&File" ), menu );

    menu = new QPopupMenu( this );
    menu->insertItem( tr( "View/Edit..." ), &profileDialog, SLOT( exec() ) );
    menu->insertSeparator();
    menu->insertItem( tr( "Creation wizard..." ), &profileWizardDialog, SLOT( exec() ) );
    insertItem( tr( "&Profiles" ), menu );

    menu = new QPopupMenu( this );
    QPopupMenu* subMenu = new QPopupMenu( menu );
    AddSubMenu4( subMenu, tr( "Links" )            , MAKE_ICON( loglink )    , controllers.options_, "LogisticLinks" );
    AddSubMenu4( subMenu, tr( "Missing links" )    , MAKE_ICON( missinglog ) , controllers.options_, "MissingLogisticLinks" );
    menu->insertItem( tr( "Logistic..." ), subMenu );

    subMenu = new QPopupMenu( menu );
    AddSubMenu3( subMenu, tr( "Small text" )    , MAKE_ICON( textsmall )    , controllers.options_, "SmallText" );
    AddSubMenu3( subMenu, tr( "Large text" )    , MAKE_ICON( textbig )      , controllers.options_, "BigText" );
    AddSubMenu4( subMenu, tr( "Tactical lines" ), MAKE_ICON( tacticallines ), controllers.options_, "TacticalLines" );

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

    OptionMenu< bool >* boolMenu = new OptionMenu< bool >( menu, controllers.options_, "3D" );
    boolMenu->AddItem( tr( "2D" ), false );
    boolMenu->AddItem( tr( "3D" ), true );
    menu->insertItem( MAKE_ICON( threed ), tr( "Display mode" ), boolMenu );

    menu->insertSeparator();
    menu->insertItem( tr( "&Preferences..." ), &prefDialog, SLOT( exec() ), CTRL + Key_P );
    insertItem( tr( "&Display" ), menu );

    menu = pParent->createDockWindowMenu();
    insertItem( tr( "&Windows" ), menu );

    menu = new QPopupMenu( this );
    menu->insertItem( tr( "About" ), new AboutDialog( this, factory, QString( APP_NAME ) + " " + QString( APP_VERSION ), license ), SLOT( exec() ) );
    insertItem( tr( "&Help" ), menu );
}
    
// -----------------------------------------------------------------------------
// Name: Menu destructor
// Created: SBO 2006-04-28
// -----------------------------------------------------------------------------
Menu::~Menu()
{
    // NOTHING
}
