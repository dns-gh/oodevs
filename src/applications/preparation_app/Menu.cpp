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
#include "FilterDialog.h"
#include "FilterDialogs.h"
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/TristateOption.h"
#include "clients_kernel/FourStateOption.h"
#include "clients_gui/OptionMenu.h"
#include "clients_gui/resources.h"
#include "clients_gui/AboutDialog.h"
#include "clients_gui/HelpSystem.h"
#include "tools/GeneralConfig.h"
#include "tools/Version.h"
#include "clients_kernel/Tools.h"

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

    void AddSubMenu3( kernel::ContextMenu* parent, const QString& label, const QIcon& iconSet, Options& options, const std::string& option )
    {
        OptionMenu< TristateOption >* optionMenu = new OptionMenu< TristateOption >( parent, options, option );
        Populate( *optionMenu );
        parent->insertItem( iconSet, label, optionMenu );
    }

    void AddSubMenu4( kernel::ContextMenu* parent, const QString& label, const QIcon& iconSet, Options& options, const std::string& option )
    {
        OptionMenu< FourStateOption >* optionMenu = new OptionMenu< FourStateOption >( parent, options, option );
        Populate( *optionMenu );
        parent->insertItem( iconSet, label, optionMenu );
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
Menu::Menu( QMainWindow* pParent, Controllers& controllers, QDialog& prefDialog, QDialog& profileDialog, QDialog& profileWizardDialog, QDialog& scoreDialog, QDialog& successFactorDialog, QDialog& exerciseDialog, QDialog& consistencyDialog, gui::ItemFactory_ABC& factory, const QString& license, const gui::HelpSystem& help )
    : QMenuBar    ( pParent )
    , controllers_( controllers )
{
    fileMenu_ = new kernel::ContextMenu( this );
    addMenu( fileMenu_ );
    fileMenu_->insertItem( MAKE_ICON( new ) , tools::translate( "Menu", "&New..." ) , parent(), SLOT( New() ) , Qt::CTRL + Qt::Key_N );
    fileMenu_->insertItem( MAKE_ICON( open ), tools::translate( "Menu", "&Open..." ), parent(), SLOT( Open() ), Qt::CTRL + Qt::Key_O );
    Wrap( fileMenu_->insertItem( MAKE_ICON( refresh ), tools::translate( "Menu", "&Reload" ), parent(), SLOT( ReloadExercise() ), Qt::CTRL + Qt::Key_R ) ); // $$$$ ABR 2011-06-24: Add a refresh icon
    Wrap( fileMenu_->insertItem( tools::translate( "Menu", "Close" ), parent(), SLOT( Close() ), Qt::CTRL + Qt::Key_W ) );
    fileMenu_->insertSeparator();
    // $$$$ ABR 2011-06-24: Filters dialogs insert themselves here
    fileMenu_->insertSeparator();
    saveItem_ = fileMenu_->insertItem( MAKE_ICON( save ), tools::translate( "Menu", "&Save" )   , parent(), SLOT( Save() ),   Qt::CTRL + Qt::Key_S );
    Wrap( fileMenu_->insertItem( MAKE_ICON( saveas ), tools::translate( "Menu", "Save &As" ), parent(), SLOT( SaveAs() ), Qt::CTRL + Qt::SHIFT + Qt::Key_S ) );
    fileMenu_->insertSeparator();
    fileMenu_->insertItem( tools::translate( "Menu", "&Quit" ), pParent, SLOT( close() ), Qt::CTRL + Qt::Key_Q );
    insertItem( tools::translate( "Menu", "&File" ), fileMenu_ );

    kernel::ContextMenu* menu = new kernel::ContextMenu( this );
    addMenu( menu );
    menu->insertItem( MAKE_ICON( profile ), tools::translate( "Menu", "View/Edit..." ), &profileDialog, SLOT( exec() ) );
    menu->insertSeparator();
    menu->insertItem( tools::translate( "Menu", "Creation wizard..." ), &profileWizardDialog, SLOT( exec() ) );
    Wrap( insertItem( tools::translate( "Menu", "&Profiles" ), menu ) );

    menu = new kernel::ContextMenu( this );
    addMenu( menu );
    kernel::ContextMenu* subMenu = new kernel::ContextMenu( menu );
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

    subMenu = new kernel::ContextMenu( menu );
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
    menu->insertItem( tools::translate( "Menu", "Toggle fullscreen mode" ), pParent, SLOT( ToggleFullScreen() ), Qt::Key_F12 );
    menu->insertItem( tools::translate( "Menu", "Toggle dock windows" ), pParent, SLOT( ToggleDocks() ), Qt::Key_F11 );

    menu->insertSeparator();
    menu->insertItem( tools::translate( "Menu", "&Preferences..." ), &prefDialog, SLOT( exec() ), Qt::CTRL + Qt::Key_P );
    Wrap( insertItem( tools::translate( "Menu", "&Display" ), menu ) );

    menu = new kernel::ContextMenu( this );
    addMenu( menu );
    menu->insertItem( tools::translate( "Menu", "Properties..." ), &exerciseDialog, SLOT( exec() ) );
    menu->insertItem( tools::translate( "Menu", "Scores..." ), &scoreDialog, SLOT( exec() ) );
    menu->insertItem( tools::translate( "Menu", "Success factors..." ), &successFactorDialog, SLOT( exec() ) );
    menu->insertItem( tools::translate( "Menu", "Consistency analysis..." ), &consistencyDialog, SLOT( CheckConsistency() ) );
    Wrap( insertItem( tools::translate( "Menu", "&Exercise" ), menu ) );

    QMenu* pMenu = pParent->createPopupMenu();
    pMenu->removeItemAt( 0 );
    pMenu->removeItemAt( 5 );
    addMenu( pMenu );
    insertItem( tools::translate( "Menu", "&Windows" ), pMenu );

    menu = new kernel::ContextMenu( this );
    addMenu( menu );
    menu->insertItem( tools::translate( "Menu", "Help" ), &help, SLOT( ShowHelp() ), Qt::Key_F1 );
    menu->insertSeparator();
    AboutDialog* about = new AboutDialog( this, factory, tools::translate( "Application", "Preparation" ) + " " + QString( tools::AppProjectVersion() ), license );
    menu->insertItem( tools::translate( "Menu", "About" ), about, SLOT( open() ) );
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

// -----------------------------------------------------------------------------
// Name: Menu::InsertFileMenuEntry
// Created: ABR 2011-06-24
// -----------------------------------------------------------------------------
int Menu::InsertFileMenuEntry( const QString& name, const QObject* receiver, const char* member, const QKeySequence& accel /*= 0*/, int index /*= -1*/ )
{
    int result = fileMenu_->insertItem( name, receiver, member, accel, -1, index );
    Wrap( result );
    return result;
}

// -----------------------------------------------------------------------------
// Name: Menu::RemoveFileMenuEntry
// Created: ABR 2011-06-24
// -----------------------------------------------------------------------------
void Menu::RemoveFileMenuEntry( int index )
{
    fileMenu_->removeItemAt( index );
}
