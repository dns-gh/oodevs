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
#include "ExerciseMenu.h"
#include "ConnectionMenu.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/TristateOption.h"
#include "clients_kernel/FourStateOption.h"
#include "clients_gui/OptionMenu.h"
#include "clients_gui/resources.h"
#include "clients_gui/AboutDialog.h"
#include "clients_gui/HelpSystem.h"
#include "gaming/Tools.h"
#include "tools/GeneralConfig.h"
#include "tools/Version.h"

using namespace kernel;
using namespace gui;

namespace
{
    template< typename T >
    struct CompositeMenu
    {
        CompositeMenu( QPopupMenu* menu, QToolBar* toolBar, const QString& label, const QIconSet& icons, Options& options, const std::string& option )
            : menu_( new OptionMenu< T >( menu, options, option ) )
        {
            {
                QToolButton* button = new QToolButton( toolBar );
                toolBarMenu_ = new OptionMenu< T >( button, options, option );
                button->setIconSet( icons );
                button->setTextLabel( label );
                button->setPopup( toolBarMenu_ );
                button->setPopupDelay( 1 );
            }
            {
                menu->insertItem( icons, label, menu_ );
            }
        }

        void AddItem( const QString& label, const T& value )
        {
            toolBarMenu_->AddItem( label, value );
            menu_->AddItem( label, value );
        }

        OptionMenu< T >* toolBarMenu_;
        OptionMenu< T >* menu_;
    };


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

    void AddSubMenu3( QToolBar* toolBar, QPopupMenu* parent, const QString& label, const QIconSet& iconSet, Options& options, const std::string& option )
    {
        {
            QToolButton* button = new QToolButton( toolBar );
            OptionMenu< TristateOption >* optionMenu = new OptionMenu< TristateOption >( button, options, option );
            Populate( *optionMenu );
            button->setIconSet( iconSet );
            button->setTextLabel( label );
            button->setPopup( optionMenu );
            button->setPopupDelay( 1 );
        }
        {
            OptionMenu< TristateOption >* optionMenu = new OptionMenu< TristateOption >( parent, options, option );
            Populate( *optionMenu );
            parent->insertItem( iconSet, label, optionMenu );
        }
    }

    void AddSubMenu4( QToolBar* toolBar, QPopupMenu* parent, const QString& label, const QIconSet& iconSet, Options& options, const std::string& option )
    {
        {
            QToolButton* button = new QToolButton( toolBar );
            OptionMenu< FourStateOption >* optionMenu = new OptionMenu< FourStateOption >( button, options, option );
            Populate( *optionMenu );
            button->setIconSet( iconSet );
            button->setTextLabel( label );
            button->setPopup( optionMenu );
            button->setPopupDelay( 1 );
        }
        {
            OptionMenu< FourStateOption >* optionMenu = new OptionMenu< FourStateOption >( parent, options, option );
            Populate( *optionMenu );
            parent->insertItem( iconSet, label, optionMenu );
        }
    }

    QPixmap MakePixmap( const std::string& name )
    {
        return QPixmap( tools::GeneralConfig::BuildResourceChildFile( std::string( "images/gui/" ) + name + ".png" ).c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: Menu constructor
// Created: SBO 2006-04-28
// -----------------------------------------------------------------------------
Menu::Menu( QMainWindow* pParent, Controllers& controllers, QDialog& prefDialog, UserProfileDialog& profileDialog, ItemFactory_ABC& factory, const QString& license, const gui::HelpSystem& help, gui::LinkInterpreter_ABC& interpreter, Network& network, kernel::Logger_ABC& logger )
    : QMenuBar( pParent )
    , controllers_( controllers )
    , profileDialog_( profileDialog )
{
    QPopupMenu* menu = new QPopupMenu( this );
    new ConnectionMenu( menu, controllers, network, logger );

    menu->insertSeparator();
    menu->insertItem( tools::translate( "Menu", "&Quit" ), pParent, SLOT( close() ), CTRL + Key_Q );
    insertItem( tools::translate( "Menu", "&File" ), menu );

    menu = new QPopupMenu( this );
    QPopupMenu* subMenu = new QPopupMenu( menu );
    QToolBar* toolBar = new QToolBar( pParent, "units toolbar" );
    toolBar->setLabel( tools::translate( "Menu", "Units toolbar" ) );

    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Vision lines" )   , MakePixmap( "vision_lines" )   , controllers.options_, "VisionLines" );
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Vision cones" )   , MakePixmap( "vision_cones" )   , controllers.options_, "VisionCones" );
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Vision surfaces" ), MakePixmap( "vision_surfaces" ), controllers.options_, "VisionSurfaces" );
    subMenu->insertSeparator();
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Weapon ranges" ) , MakePixmap( "weapon_ranges" ), controllers.options_, "WeaponRanges" );
    subMenu->insertSeparator();
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Routes" )        , MakePixmap( "path_ahead" ) , controllers.options_, "Paths" );
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Covered routes" ), MakePixmap( "path_behind" ), controllers.options_, "OldPaths" );
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Direction" )     , MakePixmap( "direction" )  , controllers.options_, "Direction" );
    subMenu->insertSeparator();
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Convex hulls" ),     MakePixmap( "convex_hulls" )    , controllers.options_, "ConvexHulls" );
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Current Mission" ),  MakePixmap( "current_mission" ) , controllers.options_, "MissionParameters" );
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Decisional State" ), MakePixmap( "decisional_state" ), controllers.options_, "DecisionalState" );
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Formations" ),       MakePixmap( "formations" )      , controllers.options_, "Formations" );
    menu->insertItem( tools::translate( "Menu", "Units..." ), subMenu );

    subMenu = new QPopupMenu( menu );
    toolBar = new QToolBar( pParent, "logistics toolbar" );
    toolBar->setLabel( tools::translate( "Menu", "Logistics toolbar" ) );
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Links" )            , MakePixmap( "logistic_links" )        , controllers.options_, "LogisticLinks" );
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Missing links" )    , MakePixmap( "logistic_missing_links" ), controllers.options_, "MissingLogisticLinks" );
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Real time actions" ), MAKE_ICON( realtimelog ), controllers.options_, "RealTimeLogistic" );
    menu->insertItem( tools::translate( "Menu", "Logistic..." ), subMenu );

    subMenu = new QPopupMenu( menu );
    toolBar = new QToolBar( pParent, "terrain toolbar" );
    toolBar->setLabel( tools::translate( "Menu", "Terrain toolbar" ) );
    AddSubMenu3( toolBar, subMenu, tools::translate( "Menu", "Small texts" )   , MAKE_ICON( textsmall )    , controllers.options_, "SmallText" );
    AddSubMenu3( toolBar, subMenu, tools::translate( "Menu", "Large texts" )   , MAKE_ICON( textbig )      , controllers.options_, "BigText" );
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Tactical lines" ), MakePixmap( "tacticallines" ), controllers.options_, "TacticalLines" );

    {
        CompositeMenu< bool > composite( subMenu, toolBar, tools::translate( "Menu", "Weather" ), MakePixmap( "weather" ), controllers.options_, "Weather" );
        composite.AddItem( tools::translate( "Menu", "On" ), true );
        composite.AddItem( tools::translate( "Menu", "Off" ), false );
    }
    subMenu->insertSeparator();

    {
        CompositeMenu< float > composite( subMenu, toolBar, tools::translate( "Menu", "Grid" ), MakePixmap( "grid" ), controllers.options_, "GridSize" );
        composite.AddItem( tools::translate( "Menu", "Off"    ),  -1 );
        composite.AddItem( tools::translate( "Menu", "100m"  ),  0.1f );
        composite.AddItem( tools::translate( "Menu", "250m" ),  0.25f );
        composite.AddItem( tools::translate( "Menu", "500m"  ),  0.5f );
        composite.AddItem( tools::translate( "Menu", "1km"  ),  1.0f );
        composite.AddItem( tools::translate( "Menu", "2.5km"  ),  2.5f );
        composite.AddItem( tools::translate( "Menu", "5km"  ),  5.0f );
        composite.AddItem( tools::translate( "Menu", "10km" ), 10.0f );
    }

    menu->insertItem( tools::translate( "Menu", "Terrain..." ), subMenu );
    menu->insertSeparator();

    {
        OptionMenu< bool >* boolMenu = new OptionMenu< bool >( menu, controllers.options_, "3D" );
        boolMenu->AddItem( tools::translate( "Menu", "2D" ), false );
        boolMenu->AddItem( tools::translate( "Menu", "3D" ), true );
        menu->insertItem( MAKE_ICON( threed ), tools::translate( "Menu", "Display mode" ), boolMenu );
    }

    menu->insertSeparator();
    menu->insertItem( tools::translate( "Menu", "&Preferences..." ), &prefDialog, SLOT( exec() ), CTRL + Key_P );
    insertItem( tools::translate( "Menu", "&Display" ), menu );

    menu = new QPopupMenu( this );
    menu->insertItem( MAKE_ICON( profile ), tools::translate( "Menu", "Profiles..." ), &profileDialog_, SLOT( exec() ) );
    profileMenu_ = insertItem( tools::translate( "Menu", "Profi&les" ), menu );
    setItemVisible( profileMenu_, false );

    menu = new ExerciseMenu( this, controllers, interpreter );
    insertItem( tools::translate( "Menu", "&Exercise" ), menu );

    menu = pParent->createDockWindowMenu();
    insertItem( tools::translate( "Menu", "&Windows" ), menu );

    menu = new QPopupMenu( this );
    menu->insertItem( tools::translate( "Menu", "Help" ), &help, SLOT( ShowHelp() ), Key_F1 );
    menu->insertSeparator();
    menu->insertItem( tools::translate( "Menu", "About" ), new AboutDialog( this, factory, tools::translate( "Application", "Gaming" ) + " " + QString( tools::AppVersion() ), license ), SLOT( exec() ) );
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
// Name: Menu::NotifyUpdated
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
void Menu::NotifyUpdated( const Profile& profile )
{
    setItemVisible( profileMenu_, profileDialog_.CanBeShown( profile ) );
}
