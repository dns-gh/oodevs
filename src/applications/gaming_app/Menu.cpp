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
#include "ExerciseMenu.h"
#include "ConnectionMenu.h"
#include "PopulationOptionChooser.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/TristateOption.h"
#include "clients_kernel/FourStateOption.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Tools.h"
#include "clients_gui/OptionMenu.h"
#include "clients_gui/resources.h"
#include "clients_gui/AboutDialog.h"
#include "clients_gui/HelpSystem.h"
#include "clients_gui/ImageWrapper.h"
#include "clients_gui/ProfileDialog.h"
#include "clients_gui/RichAction.h"
#include "clients_gui/RichToolBar.h"
#include "clients_gui/SymbolSizeOptionChooser.h"
#include "gaming/AgentServerMsgMgr.h"
#include "gaming/Network.h"
#include "gaming/Profile.h"
#include "gaming/StaticModel.h"
#include "gaming/statusicons.h"
#include "tools/GeneralConfig.h"

using namespace gui;

namespace
{
    template< typename T >
    struct CompositeMenu
    {
        CompositeMenu( kernel::ContextMenu* menu, QToolBar* toolBar, const QString& label, const QIcon& icons, kernel::Options& options, const std::string& option )
            : menu_( new OptionMenu< T >( menu, options, option ) )
        {
            {
                QToolButton* button = new QToolButton( toolBar );
                toolBar->addWidget( button );
                toolBarMenu_ = new OptionMenu< T >( button, options, option );
                button->setIconSet( icons );
                button->setTextLabel( label );
                button->setPopup( toolBarMenu_ );
                button->setPopupMode( QToolButton::InstantPopup );
                button->setPopupDelay( 1 );
            }
            {
                menu->insertItem( icons, label, menu_ );
            }
        }

        void AddItem( const QString& label, const T& value )
        {
            toolBarMenu_->AddItem( label, value );
            menu_->OnSelected( menu_->AddItem( label, value ) );
        }

        OptionMenu< T >* toolBarMenu_;
        OptionMenu< T >* menu_;
    };

    void Populate( OptionMenu< kernel::TristateOption >& menu )
    {
        menu.AddItem( kernel::TristateOption::AutoName(), kernel::TristateOption::Auto() );
        menu.AddItem( kernel::TristateOption::OnName(), kernel::TristateOption::On() );
        menu.AddItem( kernel::TristateOption::OffName(), kernel::TristateOption::Off() );
    }
    void Populate( OptionMenu< kernel::FourStateOption >& menu, const kernel::FourStateOption& state )
    {
        int index = menu.AddItem( kernel::FourStateOption::SelectedName(), kernel::FourStateOption::Selected() );
        if( state == kernel::FourStateOption::Selected() )
            menu.OnSelected( index );
        index = menu.AddItem( kernel::FourStateOption::SuperiorSelectedName(), kernel::FourStateOption::SuperiorSelected() );
        if( state == kernel::FourStateOption::SuperiorSelected() )
            menu.OnSelected( index );
        index = menu.AddItem( kernel::FourStateOption::ControlledName(), kernel::FourStateOption::Controlled() );
        if( state == kernel::FourStateOption::Controlled() )
            menu.OnSelected( index );
        index = menu.AddItem( kernel::FourStateOption::OnName(), kernel::FourStateOption::On() );
        if( state == kernel::FourStateOption::On() )
            menu.OnSelected( index );
        index = menu.AddItem( kernel::FourStateOption::OffName(), kernel::FourStateOption::Off() );
        if( state == kernel::FourStateOption::Off() )
            menu.OnSelected( index );
    }

    void AddSubMenu3( QToolBar* toolBar, kernel::ContextMenu* parent, const QString& label, const QIcon& iconSet, kernel::Options& options, const std::string& option )
    {
        {
            QToolButton* button = new QToolButton( toolBar );
            toolBar->addWidget( button );
            OptionMenu< kernel::TristateOption >* optionMenu = new OptionMenu< kernel::TristateOption >( button, options, option );
            Populate( *optionMenu );
            button->setIconSet( iconSet );
            button->setTextLabel( label );
            button->setPopup( optionMenu );
            button->setPopupMode( QToolButton::InstantPopup );
            button->setPopupDelay( 1 );
        }
        {
            OptionMenu< kernel::TristateOption >* optionMenuParent = new OptionMenu< kernel::TristateOption >( parent, options, option );
            Populate( *optionMenuParent );
            parent->insertItem( iconSet, label, optionMenuParent );
        }
    }

    void AddSubMenu4( QToolBar* toolBar, kernel::ContextMenu* parent, const QString& label, const QIcon& iconSet, kernel::Options& options, const std::string& option, kernel::FourStateOption state = kernel::FourStateOption::Off() )
    {
        {
            QToolButton* button = new QToolButton( toolBar );
            toolBar->addWidget( button );
            OptionMenu< kernel::FourStateOption >* optionMenu = new OptionMenu< kernel::FourStateOption >( button, options, option );
            Populate( *optionMenu, state );
            button->setIconSet( iconSet );
            button->setTextLabel( label );
            button->setPopup( optionMenu );
            button->setPopupMode( QToolButton::InstantPopup );
            button->setPopupDelay( 1 );
        }
        {
            OptionMenu< kernel::FourStateOption >* optionMenuParent = new OptionMenu< kernel::FourStateOption >( parent, options, option );
            Populate( *optionMenuParent, state );
            parent->insertItem( iconSet, label, optionMenuParent );
        }
    }

    QPixmap MakePixmap( const tools::Path& name )
    {
        return gui::Pixmap( tools::GeneralConfig::BuildResourceChildFile( tools::Path( "images/gui" ) / name + ".png" ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Menu constructor
// Created: SBO 2006-04-28
// -----------------------------------------------------------------------------
Menu::Menu( QMainWindow* pParent, kernel::Controllers& controllers, StaticModel& staticModel, QDialog& prefDialog, gui::ProfileDialog& profileDialog, const QString& license, Network& network, kernel::Logger_ABC& logger )
    : QMenuBar( pParent )
    , controllers_( controllers )
    , profileDialog_( profileDialog )
{
    // File
    gui::RichMenu* menu = new gui::RichMenu( "file", this, controllers_, tools::translate( "Menu", "&File" ) );
    menu->SetModes( eModes_None, eModes_All, true );
    new ConnectionMenu( menu, controllers, network, logger );
    menu->insertSeparator();
    menu->insertItem( tools::translate( "Menu", "&Quit" ), pParent, SLOT( close() ), Qt::CTRL + Qt::Key_Q );
    addMenu( menu );

    // Display
    menu = new gui::RichMenu( "Settings", this, controllers_, tools::translate( "Menu", "&Settings" ) );
    menu->SetModes( eModes_Default, eModes_All ^ eModes_Default, true );
    kernel::ContextMenu* subMenu = new kernel::ContextMenu( menu );
    gui::RichToolBar* toolBar = new gui::RichToolBar( controllers, pParent, "units toolbar" );
    toolBar->SetModes( eModes_Default, eModes_None, true );
    pParent->addToolBar( toolBar );
    toolBar->setLabel( tools::translate( "Menu", "Units toolbar" ) );

    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Vision lines" )   , MakePixmap( "vision_lines" )   , controllers.options_, "VisionLines", kernel::FourStateOption::Selected() );
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Vision cones" )   , MakePixmap( "vision_cones" )   , controllers.options_, "VisionCones" );
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Vision surfaces" ), MakePixmap( "vision_surfaces" ), controllers.options_, "VisionSurfaces" );
    subMenu->insertSeparator();
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Weapon ranges" ) , MakePixmap( "weapon_ranges" ), controllers.options_, "WeaponRanges" );
    subMenu->insertSeparator();
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Routes" )        , MakePixmap( "path_ahead" ) , controllers.options_, "Paths", kernel::FourStateOption::Selected() );
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Covered routes" ), MakePixmap( "path_behind" ), controllers.options_, "OldPaths" );
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Direction" )     , MakePixmap( "direction" )  , controllers.options_, "Direction" );
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Unit detail" )   , MakePixmap( "unit_detail" ), controllers.options_, "UnitDetails", kernel::FourStateOption::On() );
    subMenu->insertSeparator();
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Convex hulls" ),     MakePixmap( "convex_hulls" )    , controllers.options_, "ConvexHulls" );
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Current Mission" ),  MakePixmap( "current_mission" ) , controllers.options_, "MissionParameters", kernel::FourStateOption::Selected() );
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Decisional State" ), MakePixmap( "decisional_state" ), controllers.options_, "DecisionalState", kernel::FourStateOption::Selected() );
    {
        CompositeMenu< bool > composite( subMenu, toolBar, tools::translate( "Menu", "Destroyed units" ), MAKE_ICON( skull ), controllers.options_, "DisplayDestroyedUnits" );
        composite.AddItem( tools::translate( "Menu", "On" ), true );
        composite.AddItem( tools::translate( "Menu", "Off" ), false );
        composite.menu_->OnSelected( 0 );
    }
    menu->insertItem( tools::translate( "Menu", "Units..." ), subMenu );

    subMenu = new kernel::ContextMenu( menu );
    toolBar = new gui::RichToolBar( controllers, pParent, "logistics toolbar" );
    toolBar->SetModes( eModes_Default, eModes_None, true );
    pParent->addToolBar( toolBar );
    toolBar->setLabel( tools::translate( "Menu", "Logistics toolbar" ) );
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Links" )            , MakePixmap( "logistic_links" )        , controllers.options_, "LogisticLinks" );
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Missing links" )    , MakePixmap( "logistic_missing_links" ), controllers.options_, "MissingLogisticLinks" );
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Real time actions" ), MAKE_ICON( realtimelog )              , controllers.options_, "RealTimeLogistic" );
    {
        CompositeMenu< int > composite( subMenu, toolBar, tools::translate( "Menu", "Resource networks" ), MakePixmap( "resource_networks" ), controllers.options_, "ResourceNetworks" );
        composite.AddItem( tools::translate( "Menu", "Selected: all links" ), 2 );
        composite.AddItem( tools::translate( "Menu", "Selected: outgoing links" ), 3 );
        composite.AddItem( tools::translate( "Menu", "On" ), 0 );
        composite.AddItem( tools::translate( "Menu", "Off" ), 1 );
    }
    menu->insertItem( tools::translate( "Menu", "Logistic..." ), subMenu );

    subMenu = new kernel::ContextMenu( menu );
    toolBar = new gui::RichToolBar( controllers, pParent, "terrain toolbar" );
    toolBar->SetModes( eModes_Default, eModes_None, true );
    pParent->addToolBar( toolBar );
    toolBar->setLabel( tools::translate( "Menu", "Terrain toolbar" ) );
    AddSubMenu3( toolBar, subMenu, tools::translate( "Menu", "Small texts" )   , MAKE_ICON( textsmall )    , controllers.options_, "SmallText" );
    AddSubMenu3( toolBar, subMenu, tools::translate( "Menu", "Large texts" )   , MAKE_ICON( textbig )      , controllers.options_, "BigText" );
    AddSubMenu4( toolBar, subMenu, tools::translate( "Menu", "Tactical lines" ), MakePixmap( "tacticallines" ), controllers.options_, "TacticalLines", kernel::FourStateOption::On() );

    {
        CompositeMenu< float > composite( subMenu, toolBar, tools::translate( "Menu", "Grid" ), MakePixmap( "grid" ), controllers.options_, "GridSize" );
        composite.AddItem( tools::translate( "Menu", "100m"  ),  0.1f );
        composite.AddItem( tools::translate( "Menu", "250m" ),  0.25f );
        composite.AddItem( tools::translate( "Menu", "500m"  ),  0.5f );
        composite.AddItem( tools::translate( "Menu", "1km"  ),  1.0f );
        composite.AddItem( tools::translate( "Menu", "2.5km"  ),  2.5f );
        composite.AddItem( tools::translate( "Menu", "5km"  ),  5.0f );
        composite.AddItem( tools::translate( "Menu", "10km" ), 10.0f );
        composite.AddItem( tools::translate( "Menu", "100km" ), 100.0f );
        composite.AddItem( tools::translate( "Menu", "Off"    ),  -1 );
    }
    {
        CompositeMenu< int > composite( subMenu, toolBar, tools::translate( "Menu", "Grid type" ), MakePixmap( "grid_type" ), controllers.options_, "GridType" );
        composite.AddItem( tools::translate( "Menu", "Local" ), eCoordinateSystem_Local );
        composite.AddItem( tools::translate( "Menu", "MGRS"  ), eCoordinateSystem_Mgrs );
    }

    {
        Q3HBox* populationBox = new Q3HBox( toolBar );
        QPushButton* populationEnabled = new QPushButton( tools::translate( "Menu", "Population" ), populationBox );
        populationEnabled->setToolTip( tools::translate( "Menu", "Show population display tool" ) );
        PopulationOptionChooser* populationOptions = new PopulationOptionChooser( pParent, controllers, staticModel );
        toolBar->addWidget( populationBox );
        toolBar->addWidget( populationEnabled );
        connect( populationEnabled, SIGNAL( clicked() ), populationOptions, SLOT( Show() ) );
    }

    subMenu->insertSeparator();
    {
        new SymbolSizeOptionChooser( pParent, toolBar, MakePixmap( "symbol_increase" ), MakePixmap( "symbol_decrease" ), controllers );
    }

    menu->insertItem( tools::translate( "Menu", "Terrain..." ), subMenu );
    menu->insertSeparator();

    {
        OptionMenu< bool >* boolMenu = new OptionMenu< bool >( menu, controllers.options_, "3D", false );
        boolMenu->AddItem( tools::translate( "Menu", "2D" ), false );
        boolMenu->AddItem( tools::translate( "Menu", "3D" ), true );
        menu->insertItem( MAKE_ICON( threed ), tools::translate( "Menu", "Display mode" ), boolMenu );
        menu->insertItem( tools::translate( "Menu", "Toggle fullscreen mode" ), pParent, SLOT( ToggleFullScreen() ), Qt::Key_F12 );
        menu->insertItem( tools::translate( "Menu", "Toggle dock windows" ), pParent, SLOT( ToggleDocks() ), Qt::Key_F11 );
    }

    menu->insertSeparator();
    QAction* action = menu->addAction( tools::translate( "Menu", "&Preferences..." ), &prefDialog, SLOT( show() ), QKeySequence( Qt::CTRL + Qt::Key_P ) );
    AddModdedAction( action, eModes_Default, eModes_All ^ eModes_Default );
    addMenu( menu );

    // Profiles
    menu = new gui::RichMenu( "profiles", this, controllers_, tools::translate( "Menu", "Profiles..." ) );
    menu->SetModes( eModes_Default | eModes_Replay, eModes_All ^ ( eModes_Default | eModes_Replay ), true );
    menu->insertItem( MAKE_ICON( profile ), tools::translate( "Menu", "Profiles..." ), &profileDialog_, SLOT( exec() ) );
    profileMenu_ = insertItem( tools::translate( "Menu", "Profi&les" ), menu );
    setItemVisible( profileMenu_, false );
    addMenu( menu );

    // Exercise
    menu = new ExerciseMenu( this, controllers, tools::translate( "Menu", "&Exercise" ) );
    menu->SetModes( eModes_Default, eModes_All ^ eModes_Default, true );
    addMenu( menu );

    // Windows
    if( QMenu* menu = pParent->createPopupMenu() )
    {
        if( QAction* action = addMenu( menu ) )
        {
            action->setText( tools::translate( "Menu", "&Windows" ) );
            windowAction_ = new gui::RichAction( action, controllers_ );
            windowAction_->SetModes( eModes_Default, eModes_All ^ eModes_Default, true );
        }
    }

    // Help
    menu = new gui::RichMenu( "help", this, controllers, tools::translate( "Menu", "&?" ) );
    menu->SetModes( eModes_None, eModes_All, true );
    menu->insertItem( tools::translate( "Menu", "Help" ), pParent, SIGNAL( ShowHelp() ) );
    menu->insertSeparator();
    menu->insertItem( tools::translate( "Menu", "About" ), new AboutDialog( this, license ), SLOT( exec() ) );
    addMenu( menu );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Menu destructor
// Created: SBO 2006-04-28
// -----------------------------------------------------------------------------
Menu::~Menu()
{
    for( std::vector< gui::RichAction* >::iterator it = moddedActions_.begin(); it != moddedActions_.end(); ++it )
        delete *it;
    delete windowAction_;
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: Menu::AddModdedAction
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
void Menu::AddModdedAction( QAction* action, int hiddenModes /* = 0 */, int visibleModes /* = 0 */, bool visibleByDefault /* = true */ )
{
    if( !action )
        return;
    gui::RichAction* richAction = new gui::RichAction( action, controllers_ );
    richAction->SetModes( hiddenModes, visibleModes, visibleByDefault );
    moddedActions_.push_back( richAction );
}

// -----------------------------------------------------------------------------
// Name: Menu::NotifyUpdated
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
void Menu::NotifyUpdated( const Profile& profile )
{
    if( controllers_.GetCurrentMode() != eModes_Replay )
        setItemVisible( profileMenu_, profileDialog_.CanBeShown( profile ) );
}
