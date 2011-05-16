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
    // $$$$ AGE 2007-05-31:
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
Menu::Menu( QMainWindow* pParent, Controllers& controllers, QDialog& prefDialog, QDialog& profileDialog, QDialog& profileWizardDialog, QDialog& importDialog, QDialog& exportDialog, QDialog& scoreDialog, QDialog& successFactorDialog, QDialog& exerciseDialog, gui::ItemFactory_ABC& factory, const QString& license, const gui::HelpSystem& help )
    : QMenuBar    ( pParent )
    , controllers_( controllers )
{
    QPopupMenu* menu = new QPopupMenu( this );
    menu->insertItem( MAKE_ICON( new ) , tools::translate( "Menu", "&New..." ) , parent(), SLOT( New() ) , CTRL + Key_N );
    menu->insertItem( MAKE_ICON( open ), tools::translate( "Menu", "&Open..." ), parent(), SLOT( Open() ), CTRL + Key_O );
    Wrap( menu->insertItem( tools::translate( "Menu", "Close" ), parent(), SLOT( Close() ), CTRL + Key_W ) );
    menu->insertSeparator();
    Wrap( menu->insertItem( tools::translate( "Menu", "&Import..." ), &importDialog, SLOT( exec() ), CTRL + Key_I ) );
    Wrap( menu->insertItem( tools::translate( "Menu", "&Export..." ), &exportDialog, SLOT( exec() ), CTRL + Key_E ) );
    menu->insertSeparator();
    saveItem_ = menu->insertItem( MAKE_ICON( save ), tools::translate( "Menu", "&Save" )   , parent(), SLOT( Save() ),   CTRL + Key_S );
    Wrap( menu->insertItem( MAKE_ICON( saveas ), tools::translate( "Menu", "Save &As" ), parent(), SLOT( SaveAs() ), CTRL + SHIFT + Key_S ) );
    menu->insertSeparator();
    menu->insertItem( tools::translate( "Menu", "&Quit" ), pParent, SLOT( close() ), CTRL + Key_Q );
    insertItem( tools::translate( "Menu", "&File" ), menu );

    menu = new QPopupMenu( this );
    menu->insertItem( MAKE_ICON( profile ), tools::translate( "Menu", "View/Edit..." ), &profileDialog, SLOT( exec() ) );
    menu->insertSeparator();
    menu->insertItem( tools::translate( "Menu", "Creation wizard..." ), &profileWizardDialog, SLOT( exec() ) );
    Wrap( insertItem( tools::translate( "Menu", "&Profiles" ), menu ) );

    menu = new QPopupMenu( this );
    QPopupMenu* subMenu = new QPopupMenu( menu );
    AddSubMenu4( subMenu, tools::translate( "Menu", "Links" )            , MakePixmap( "logistic_links" ), controllers.options_, "LogisticLinks" );
    AddSubMenu4( subMenu, tools::translate( "Menu", "Missing links" )    , MakePixmap( "logistic_missing_links" ), controllers.options_, "MissingLogisticLinks" );
    {
        OptionMenu< int >* resourceMenu = new OptionMenu< int >( subMenu, controllers.options_, "ResourceNetworks" );
        resourceMenu->AddItem( tools::translate( "Menu", "On" ), 0 );
        resourceMenu->AddItem( tools::translate( "Menu", "Off" ), 1 );
        resourceMenu->AddItem( tools::translate( "Menu", "Selected: all links" ), 2 );
        resourceMenu->AddItem( tools::translate( "Menu", "Selected: outgoing links" ), 3 );
        subMenu->insertItem( MakePixmap( "logistic_links" ), tools::translate( "Menu", "Resource networks" ), resourceMenu );
    }
    Wrap( menu->insertItem( tools::translate( "Menu", "Logistic..." ), subMenu ) );

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

    Wrap( menu->insertItem( tools::translate( "Menu", "Terrain..." ), subMenu ) );
    menu->insertSeparator();

    OptionMenu< bool >* boolMenu = new OptionMenu< bool >( menu, controllers.options_, "3D" );
    boolMenu->AddItem( tools::translate( "Menu", "2D" ), false );
    boolMenu->AddItem( tools::translate( "Menu", "3D" ), true );
    menu->insertItem( MAKE_ICON( threed ), tools::translate( "Menu", "Display mode" ), boolMenu );
    menu->insertItem( tools::translate( "Menu", "Toggle fullscreen mode" ), pParent, SLOT( ToggleFullScreen() ), Key_F12 );
    menu->insertItem( tools::translate( "Menu", "Toggle dock windows" ), pParent, SLOT( ToggleDocks() ), Key_F11 );

    menu->insertSeparator();
    menu->insertItem( tools::translate( "Menu", "&Preferences..." ), &prefDialog, SLOT( exec() ), CTRL + Key_P );
    Wrap( insertItem( tools::translate( "Menu", "&Display" ), menu ) );

    menu = new QPopupMenu( this );
    menu->insertItem( tools::translate( "Menu", "Properties..." ), &exerciseDialog, SLOT( exec() ) );
    menu->insertItem( tools::translate( "Menu", "Scores..." ), &scoreDialog, SLOT( exec() ) );
    menu->insertItem( tools::translate( "Menu", "Success factors..." ), &successFactorDialog, SLOT( exec() ) );
    Wrap( insertItem( tools::translate( "Menu", "&Exercise" ), menu ) );

    menu = pParent->createDockWindowMenu();
    insertItem( tools::translate( "Menu", "&Windows" ), menu );

    menu = new QPopupMenu( this );
    menu->insertItem( tools::translate( "Menu", "Help" ), &help, SLOT( ShowHelp() ), Key_F1 );
    menu->insertSeparator();
    menu->insertItem( tools::translate( "Menu", "About" ), new AboutDialog( this, factory, tools::translate( "Application", "Preparation" ) + " " + QString( tools::AppVersion() ), license ), SLOT( exec() ) );
    insertItem( tools::translate( "Menu", "&?" ), menu );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Menu destructor
// Created: SBO 2006-04-28
// -----------------------------------------------------------------------------
Menu::~Menu()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: Menu EnableSaveItem
// Created: RPD 2010-02-05
// -----------------------------------------------------------------------------
void Menu::EnableSaveItem( bool status )
{
    setItemEnabled( saveItem_, status );
}

// -----------------------------------------------------------------------------
// Name: Menu::NotifyUpdated
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
void Menu::NotifyUpdated( const kernel::ModelLoaded& )
{
    EnableItems( true );
}

// -----------------------------------------------------------------------------
// Name: Menu::NotifyUpdated
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
void Menu::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    EnableItems( false );
}

// -----------------------------------------------------------------------------
// Name: Menu::EnableItems
// Created: SLG 2010-11-09
// -----------------------------------------------------------------------------
void Menu::EnableItems( bool status )
{
    for( std::vector< int >::const_iterator it = exerciseItems_.begin(); it != exerciseItems_.end(); ++it )
        setItemEnabled( *it, status );
}

// -----------------------------------------------------------------------------
// Name: Menu::Wrap
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
void Menu::Wrap( int item )
{
    exerciseItems_.push_back( item );
}
