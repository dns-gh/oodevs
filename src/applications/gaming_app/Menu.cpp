// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "Menu.h"
#include "UserProfileDialog.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/TristateOption.h"
#include "clients_kernel/FourStateOption.h"
#include "clients_gui/OptionMenu.h"
#include "clients_gui/resources.h"
#include "clients_gui/AboutDialog.h"
#include "gaming/Tools.h"

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
    void Populate( OptionMenu< FourStateOption >& menu )
    {
        menu.AddItem( FourStateOption::SelectedName(), FourStateOption::Selected() );
        menu.AddItem( FourStateOption::SuperiorSelectedName(), FourStateOption::SuperiorSelected() );
        menu.AddItem( FourStateOption::OnName(), FourStateOption::On() );
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
Menu::Menu( QMainWindow* pParent, Controllers& controllers, QDialog& prefDialog, UserProfileDialog& profileDialog, ItemFactory_ABC& factory, const QString& license )
    : QMenuBar( pParent )
    , controllers_( controllers )
    , profileDialog_( profileDialog )
{
    QPopupMenu* menu = new QPopupMenu( this );
    menu->insertItem( MAKE_ICON( open ), tools::translate( "Menu", "&Open exercice..." ), parent(), SLOT( Open() ), CTRL + Key_O );
    menu->insertItem( tools::translate( "Menu", "Close" ), parent(), SLOT( Close() ) );
    menu->insertSeparator();
    menu->insertItem( tools::translate( "Menu", "&Quit" ), pParent, SLOT( close() ), CTRL + Key_Q );
    insertItem( tools::translate( "Menu", "&File" ), menu );

    menu = new QPopupMenu( this );
    QPopupMenu* subMenu = new QPopupMenu( menu );

    AddSubMenu4( subMenu, tools::translate( "Menu", "Vision lines" )   , MAKE_ICON( visionlines )   , controllers.options_, "VisionLines" );
    AddSubMenu4( subMenu, tools::translate( "Menu", "Vision cones" )   , MAKE_ICON( visioncones )   , controllers.options_, "VisionCones" );
    AddSubMenu4( subMenu, tools::translate( "Menu", "Vision surfaces" ), MAKE_ICON( visionsurfaces ), controllers.options_, "VisionSurfaces" );
    subMenu->insertSeparator();
    AddSubMenu4( subMenu, tools::translate( "Menu", "Routes" )        , MAKE_ICON( path )          , controllers.options_, "Paths" );
    AddSubMenu4( subMenu, tools::translate( "Menu", "Covered routes" ), MAKE_ICON( oldpath )       , controllers.options_, "OldPaths" );
    AddSubMenu4( subMenu, tools::translate( "Menu", "Direction" )     , MAKE_ICON( oldpath )       , controllers.options_, "Direction" ); // $$$$ AGE 2007-12-17: Icon
    subMenu->insertSeparator();
    // $$$$ AGE 2007-05-30: icons
    AddSubMenu4( subMenu, tools::translate( "Menu", "Convex hulls" ),     MAKE_ICON( oldpath )       , controllers.options_, "ConvexHulls" );
    AddSubMenu4( subMenu, tools::translate( "Menu", "Current Mission" ),  MAKE_ICON( oldpath )       , controllers.options_, "MissionParameters" );
    AddSubMenu4( subMenu, tools::translate( "Menu", "Decisional State" ), MAKE_ICON( oldpath )       , controllers.options_, "DecisionalState" );
    AddSubMenu4( subMenu, tools::translate( "Menu", "Formations" ),       MAKE_ICON( oldpath )       , controllers.options_, "Formations" );
    menu->insertItem( tools::translate( "Menu", "Units..." ), subMenu );

    subMenu = new QPopupMenu( menu );
    AddSubMenu4( subMenu, tools::translate( "Menu", "Links" )            , MAKE_ICON( loglink )    , controllers.options_, "LogisticLinks" );
    AddSubMenu4( subMenu, tools::translate( "Menu", "Missing links" )    , MAKE_ICON( missinglog ) , controllers.options_, "MissingLogisticLinks" );
    AddSubMenu4( subMenu, tools::translate( "Menu", "Real time actions" ), MAKE_ICON( realtimelog ), controllers.options_, "RealTimeLogistic" );
    menu->insertItem( tools::translate( "Menu", "Logistic..." ), subMenu );

    subMenu = new QPopupMenu( menu );
    AddSubMenu3( subMenu, tools::translate( "Menu", "Small texts" )   , MAKE_ICON( textsmall )    , controllers.options_, "SmallText" );
    AddSubMenu3( subMenu, tools::translate( "Menu", "Large texts" )   , MAKE_ICON( textbig )      , controllers.options_, "BigText" );
    AddSubMenu4( subMenu, tools::translate( "Menu", "Tactical lines" ), MAKE_ICON( tacticallines ), controllers.options_, "TacticalLines" );

    OptionMenu< bool >* boolMenu = new OptionMenu< bool >( subMenu, controllers.options_, "Weather" );
    boolMenu->AddItem( tools::translate( "Menu", "On" ), true );
    boolMenu->AddItem( tools::translate( "Menu", "Off" ), false );
    subMenu->insertItem( MAKE_ICON( weather ), tools::translate( "Menu", "Weather" ), boolMenu );

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

    boolMenu = new OptionMenu< bool >( menu, controllers.options_, "3D" );
    boolMenu->AddItem( tools::translate( "Menu", "2D" ), false );
    boolMenu->AddItem( tools::translate( "Menu", "3D" ), true );
    menu->insertItem( MAKE_ICON( threed ), tools::translate( "Menu", "Display mode" ), boolMenu );

    menu->insertSeparator();
    menu->insertItem( tools::translate( "Menu", "&Preferences..." ), &prefDialog, SLOT( exec() ), CTRL + Key_P );
    insertItem( tools::translate( "Menu", "&Display" ), menu );

    menu = new QPopupMenu( this );
    menu->insertItem( MAKE_ICON( profile ), tools::translate( "Menu", "Profiles..." ), &profileDialog_, SLOT( exec() ) );
    profileMenu_ = insertItem( tools::translate( "Menu", "Profi&les" ), menu );
    setItemVisible( profileMenu_, false );

    menu = pParent->createDockWindowMenu();
    insertItem( tools::translate( "Menu", "&Windows" ), menu );

    menu = new QPopupMenu( this );
    menu->insertItem( tools::translate( "Menu", "About" ), new AboutDialog( this, factory, QString( APP_NAME ) + " " + QString( APP_VERSION ), license ), SLOT( exec() ) );
    insertItem( tools::translate( "Menu", "&Help" ), menu );

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
// Name: Menu::NotifyUpdated
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
void Menu::NotifyUpdated( const Profile& profile )
{
    setItemVisible( profileMenu_, profileDialog_.CanBeShown( profile ) );
}
