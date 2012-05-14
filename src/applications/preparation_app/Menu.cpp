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
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/TristateOption.h"
#include "clients_kernel/FourStateOption.h"
#include "clients_gui/OptionMenu.h"
#include "clients_gui/resources.h"
#include "clients_gui/AboutDialog.h"
#include "clients_gui/HelpSystem.h"
#include "clients_gui/RichAction.h"
#include "clients_gui/RichMenu.h"
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
Menu::Menu( QMainWindow* pParent, Controllers& controllers, QDialog& prefDialog, QDialog& profileDialog, QDialog& profileWizardDialog, 
            QDialog& scoreDialog, QDialog& successFactorDialog, QDialog& exerciseDialog, QDialog& consistencyDialog, QDialog& performanceDialog,
            gui::ItemFactory_ABC& factory, const QString& license, const gui::HelpSystem& help )
    : QMenuBar    ( pParent )
    , controllers_( controllers )
{
    // File
    {
        fileMenu_ = new kernel::ContextMenu( this );
        fileMenu_->setTitle( tools::translate( "Menu", "&File" ) );
        newAction_ = fileMenu_->InsertItem( "1", tools::translate( "Menu", "&New..." ) , parent(), SLOT( New() ) , QKeySequence( Qt::CTRL + Qt::Key_N ) );
        newAction_->setIcon( MAKE_ICON( new ) );
        openAction_ = fileMenu_->InsertItem( "1", tools::translate( "Menu", "&Open..." ), parent(), SLOT( Open() ), QKeySequence( Qt::CTRL + Qt::Key_O ) );
        openAction_->setIcon( MAKE_ICON( open ) );

        QAction* action = 0;
        action = fileMenu_->InsertItem( "1", tools::translate( "Menu", "&Reload" ), parent(), SLOT( ReloadExercise() ), QKeySequence( Qt::CTRL + Qt::Key_R ) );
        action->setIcon( MAKE_ICON( refresh ) );
        AddModdedAction( action, ePreparationMode_All, ePreparationMode_Default, ePreparationMode_All ^ ePreparationMode_Default );
        action = fileMenu_->InsertItem( "1", tools::translate( "Menu", "Close" ), parent(), SLOT( Close() ), QKeySequence( Qt::CTRL + Qt::Key_W ) );
        AddModdedAction( action, ePreparationMode_All, ePreparationMode_Default, ePreparationMode_All ^ ePreparationMode_Default );

        fileMenu_->GetCategoryCreateIFN( "2" ); // Import Export Category

        saveAction_ = fileMenu_->InsertItem( "3", tools::translate( "Menu", "&Save" )   , parent(), SLOT( Save() ), QKeySequence( Qt::CTRL + Qt::Key_S ) );
        saveAction_->setIcon( MAKE_ICON( save ) );
        AddModdedAction( saveAction_, ePreparationMode_All, ePreparationMode_Default, ePreparationMode_All ^ ePreparationMode_Default );
        saveAsAction_ = fileMenu_->InsertItem( "3", tools::translate( "Menu", "Save &As" ), parent(), SLOT( SaveAs() ), QKeySequence( Qt::CTRL + Qt::SHIFT + Qt::Key_S ) );
        saveAsAction_->setIcon( MAKE_ICON( saveas ) );
        AddModdedAction( saveAsAction_, ePreparationMode_All, ePreparationMode_Default, ePreparationMode_All ^ ePreparationMode_Default );
        fileMenu_->InsertItem( "4", tools::translate( "Menu", "&Quit" ), pParent, SLOT( close() ), QKeySequence( Qt::CTRL + Qt::Key_Q ) );
        addMenu( fileMenu_->FillMenu() );
    }

    // Profile
    {
        RichMenu* menu = new RichMenu( this, controllers, tools::translate( "Menu", "&Profiles" ) );
        menu->SetModes( ePreparationMode_All, ePreparationMode_All ^ ePreparationMode_Exercise, ePreparationMode_Exercise );
        QAction* action = menu->InsertItem( "1", tools::translate( "Menu", "View/Edit..." ), &profileDialog, SLOT( exec() ) );
        action->setIcon( MAKE_ICON( profile ) );
        menu->InsertItem( "2", tools::translate( "Menu", "Creation wizard..." ), &profileWizardDialog, SLOT( exec() ) );
        addMenu( menu->FillMenu() );
    }

    // Display
    {
        RichMenu* menu = new RichMenu( this, controllers, tools::translate( "Menu", "&Display" ) );
        menu->SetModes( ePreparationMode_All, ePreparationMode_Default, ePreparationMode_All ^ ePreparationMode_Default );
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
        menu->insertItem( tools::translate( "Menu", "Logistic..." ), subMenu );

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

        menu->insertItem( tools::translate( "Menu", "Terrain..." ), subMenu );
        menu->insertSeparator();

        OptionMenu< bool >* boolMenu = new OptionMenu< bool >( menu, controllers.options_, "3D" );
        boolMenu->AddItem( tools::translate( "Menu", "2D" ), false );
        boolMenu->AddItem( tools::translate( "Menu", "3D" ), true );
        menu->insertItem( MAKE_ICON( threed ), tools::translate( "Menu", "Display mode" ), boolMenu );
        menu->insertItem( tools::translate( "Menu", "Toggle fullscreen mode" ), pParent, SLOT( ToggleFullScreen() ), Qt::Key_F12 );
        menu->insertItem( tools::translate( "Menu", "Toggle dock windows" ), pParent, SLOT( ToggleDocks() ), Qt::Key_F11 );

        menu->insertSeparator();
        menu->insertItem( tools::translate( "Menu", "&Preferences..." ), &prefDialog, SLOT( exec() ), Qt::CTRL + Qt::Key_P );
        addMenu( menu );
    }

    // Exercise
    {
        RichMenu* menu = new RichMenu( this, controllers_, tools::translate( "Menu", "&Exercise" ) );
        menu->SetModes( ePreparationMode_All, ePreparationMode_All ^ ePreparationMode_Exercise, ePreparationMode_Exercise );
        menu->InsertItem( "", tools::translate( "Menu", "Properties..." ), &exerciseDialog, SLOT( exec() ) );
        menu->InsertItem( "", tools::translate( "Menu", "Scores..." ), &scoreDialog, SLOT( exec() ) );
        menu->InsertItem( "", tools::translate( "Menu", "Success factors..." ), &successFactorDialog, SLOT( exec() ) );
        menu->InsertItem( "", tools::translate( "Menu", "Consistency analysis..." ), &consistencyDialog, SLOT( Display() ) );
        menu->InsertItem( "", tools::translate( "Menu", "Performance..."), &performanceDialog, SLOT( exec() ) );
        addMenu( menu->FillMenu() );
    }

    // Windows
    if( QMenu* menu = pParent->createPopupMenu() )
    {
        QAction* action = menu->actions().value( 6 );
        AddModdedAction( action, ePreparationMode_None, ePreparationMode_All );
        if( QAction* action = addMenu( menu ) )
        {
            action->setText( tools::translate( "Menu", "&Windows" ) );
            AddModdedAction( action, ePreparationMode_All, ePreparationMode_LivingArea | ePreparationMode_Default, ePreparationMode_Exercise | ePreparationMode_Terrain );
        }
    }

    // Help
    {
        RichMenu* menu = new RichMenu( this, controllers_, tools::translate( "Menu", "&?" ) );
        menu->InsertItem( "1", tools::translate( "Menu", "Help" ), &help, SLOT( ShowHelp() ) );
        AboutDialog* about = new AboutDialog( this, factory, tools::translate( "Application", "Preparation" ) + " " + QString( tools::AppProjectVersion() ), license );
        menu->InsertItem( "2", tools::translate( "Menu", "About" ), about, SLOT( open() ) );
        addMenu( menu->FillMenu() );
    }
}

// -----------------------------------------------------------------------------
// Name: Menu destructor
// Created: SBO 2006-04-28
// -----------------------------------------------------------------------------
Menu::~Menu()
{
    for( std::vector< gui::RichAction* >::iterator it = moddedActions_.begin(); it != moddedActions_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: Menu::AddModdedAction
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
void Menu::AddModdedAction( QAction* action, int defaultModes /* = 0 */, int hiddenModes /* = 0 */, int visibleModes /* = 0 */ )
{
    if( !action )
        return;
    gui::RichAction* richAction = new gui::RichAction( action, controllers_ );
    richAction->SetModes( defaultModes, hiddenModes, visibleModes );
    moddedActions_.push_back( richAction );
}

// -----------------------------------------------------------------------------
// Name: Menu::InsertFileMenuTmpEntry
// Created: ABR 2011-06-24
// -----------------------------------------------------------------------------
void Menu::InsertFileMenuEntry( const QString& name, const QObject* receiver, const char* member, const QKeySequence& accel /*= 0*/, int index /*= -1*/ )
{
    fileMenu_->insertItem( name, receiver, member, accel, -1, index );
    QAction* action = fileMenu_->actions().value( index );
    AddModdedAction( action, ePreparationMode_All, ePreparationMode_All ^ ePreparationMode_Exercise, ePreparationMode_Exercise );
}

// -----------------------------------------------------------------------------
// Name: Menu::RemoveFileMenuEntry
// Created: ABR 2011-06-24
// -----------------------------------------------------------------------------
void Menu::RemoveFileMenuEntry( int index )
{
    fileMenu_->removeItemAt( index );
}

// -----------------------------------------------------------------------------
// Name: Menu::GetNewAction
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
QAction* Menu::GetNewAction() const
{
    return newAction_;
}

// -----------------------------------------------------------------------------
// Name: Menu::GetOpenAction
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
QAction* Menu::GetOpenAction() const
{
    return openAction_;
}

// -----------------------------------------------------------------------------
// Name: Menu::GetSaveAction
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
QAction* Menu::GetSaveAction() const
{
    return saveAction_;
}

// -----------------------------------------------------------------------------
// Name: Menu::GetSaveAsAction
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
QAction* Menu::GetSaveAsAction() const
{
    return saveAsAction_;
}
