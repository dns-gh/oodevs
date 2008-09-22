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
#include "clients_gui/HelpSystem.h"
#include "tools/GeneralConfig.h"
#include "tools/Version.h"
#include "preparation/Tools.h"

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

    QPixmap MakePixmap( const std::string& name )
    {
        return QImage( tools::GeneralConfig::BuildResourceChildFile( std::string( "images/gui/" ) + name + ".png" ).c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: Menu constructor
// Created: SBO 2006-04-28
// -----------------------------------------------------------------------------
Menu::Menu( QMainWindow* pParent, Controllers& controllers, QDialog& prefDialog, QDialog& profileDialog, QDialog& profileWizardDialog, QDialog& importDialog, gui::ItemFactory_ABC& factory, const QString& license, const gui::HelpSystem& help )
    : QMenuBar( pParent )
{
    QPopupMenu* menu = new QPopupMenu( this );
    menu->insertItem( MAKE_ICON( new ) , tools::translate( "Menu", "&New..." ) , parent(), SLOT( New() ) , CTRL + Key_N );
    menu->insertItem( MAKE_ICON( open ), tools::translate( "Menu", "&Open..." ), parent(), SLOT( Open() ), CTRL + Key_O );
    menu->insertItem( tools::translate( "Menu", "Close" ), parent(), SLOT( Close() ) );
    menu->insertSeparator();
    menu->insertItem( tools::translate( "Menu", "&Import..." ), &importDialog, SLOT( exec() ), CTRL + Key_I );
    menu->insertSeparator();
    menu->insertItem( MAKE_ICON( save )  , tools::translate( "Menu", "&Save" )      , parent(), SLOT( Save() ), CTRL + Key_S );
    menu->insertSeparator();
    menu->insertItem( tools::translate( "Menu", "&Quit" ), pParent, SLOT( close() ), CTRL + Key_Q );
    insertItem( tools::translate( "Menu", "&File" ), menu );

    menu = new QPopupMenu( this );
    menu->insertItem( MAKE_ICON( profile ), tools::translate( "Menu", "View/Edit..." ), &profileDialog, SLOT( exec() ) );
    menu->insertSeparator();
    menu->insertItem( tools::translate( "Menu", "Creation wizard..." ), &profileWizardDialog, SLOT( exec() ) );
    insertItem( tools::translate( "Menu", "&Profiles" ), menu );

    menu = new QPopupMenu( this );
    QPopupMenu* subMenu = new QPopupMenu( menu );
    AddSubMenu4( subMenu, tools::translate( "Menu", "Links" )            , MakePixmap( "logistic_links" ), controllers.options_, "LogisticLinks" );
    AddSubMenu4( subMenu, tools::translate( "Menu", "Missing links" )    , MakePixmap( "logistic_missing_links" ), controllers.options_, "MissingLogisticLinks" );
    menu->insertItem( tools::translate( "Menu", "Logistic..." ), subMenu );

    subMenu = new QPopupMenu( menu );
    AddSubMenu3( subMenu, tools::translate( "Menu", "Small text" )    , MAKE_ICON( textsmall )    , controllers.options_, "SmallText" );
    AddSubMenu3( subMenu, tools::translate( "Menu", "Large text" )    , MAKE_ICON( textbig )      , controllers.options_, "BigText" );
    AddSubMenu4( subMenu, tools::translate( "Menu", "Tactical lines" ), MAKE_ICON( tacticallines ), controllers.options_, "TacticalLines" );

    subMenu->insertSeparator();

    OptionMenu< float >* gridMenu = new OptionMenu< float >( subMenu, controllers.options_, "GridSize" );
    gridMenu->AddItem( tools::translate( "Menu", "Off"    ),  -1 );
    gridMenu->AddItem( tools::translate( "Menu", "100m"  ),  0.1f );
    gridMenu->AddItem( tools::translate( "Menu", "250m" ),  0.25f );
    gridMenu->AddItem( tools::translate( "Menu", "500m"  ),  0.5f );
    gridMenu->AddItem( tools::translate( "Menu", "1km"  ),  1.0f );
    gridMenu->AddItem( tools::translate( "Menu", "2.5km"  ),  2.5f );
    gridMenu->AddItem( tools::translate( "Menu", "5km"  ),  5.0f );
    gridMenu->AddItem( tools::translate( "Menu", "10km" ), 10.0f );
    subMenu->insertItem( tools::translate( "Menu", "Grid" ), gridMenu );

    menu->insertItem( tools::translate( "Menu", "Terrain..." ), subMenu );
    menu->insertSeparator();

    OptionMenu< bool >* boolMenu = new OptionMenu< bool >( menu, controllers.options_, "3D" );
    boolMenu->AddItem( tools::translate( "Menu", "2D" ), false );
    boolMenu->AddItem( tools::translate( "Menu", "3D" ), true );
    menu->insertItem( MAKE_ICON( threed ), tools::translate( "Menu", "Display mode" ), boolMenu );

    menu->insertSeparator();
    menu->insertItem( tools::translate( "Menu", "&Preferences..." ), &prefDialog, SLOT( exec() ), CTRL + Key_P );
    insertItem( tools::translate( "Menu", "&Display" ), menu );

    menu = pParent->createDockWindowMenu();
    insertItem( tools::translate( "Menu", "&Windows" ), menu );

    menu = new QPopupMenu( this );
    menu->insertItem( tools::translate( "Menu", "Help" ), &help, SLOT( ShowHelp() ), Key_F1 );
    menu->insertSeparator();
    menu->insertItem( tools::translate( "Menu", "About" ), new AboutDialog( this, factory, tools::translate( "Application", "SWORD Officer Training - Preparation" ) + " " + QString( tools::AppVersion() ), license ), SLOT( exec() ) );
    insertItem( tools::translate( "Menu", "&?" ), menu );
}
    
// -----------------------------------------------------------------------------
// Name: Menu destructor
// Created: SBO 2006-04-28
// -----------------------------------------------------------------------------
Menu::~Menu()
{
    // NOTHING
}
