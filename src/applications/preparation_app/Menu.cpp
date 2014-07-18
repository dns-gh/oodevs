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
#include "DialogContainer.h"
#include "ExerciseDialog.h"
#include "PerformanceDialog.h"
#include "ProfileWizardDialog.h"
#include "ScoreDialog.h"
#include "SuccessFactorDialog.h"
#include "ScoreDialog.h"
#include "ModelConsistencyDialog.h"
#include "FilterDialogs.h"
#include "TerrainExportDialog.h"
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/TristateOption.h"
#include "clients_kernel/FourStateOption.h"
#include "clients_kernel/Tools.h"
#include "clients_gui/AboutDialog.h"
#include "clients_gui/HelpSystem.h"
#include "clients_gui/ImageWrapper.h"
#include "clients_gui/ObjectNameManager.h"
#include "clients_gui/OptionMenu.h"
#include "clients_gui/ProfileDialog.h"
#include "clients_gui/PreferencesDialog.h"
#include "clients_gui/resources.h"
#include "clients_gui/RichAction.h"
#include "clients_gui/RichMenu.h"
#include "tools/GeneralConfig.h"

int Menu::filtersIndex_ = 7;

namespace
{
    void Populate( gui::OptionMenu< kernel::TristateOption >& menu )
    {
        menu.AddItem( kernel::TristateOption::AutoName(), kernel::TristateOption::Auto() );
        menu.AddItem( kernel::TristateOption::OnName(), kernel::TristateOption::On() );
        menu.AddItem( kernel::TristateOption::OffName(), kernel::TristateOption::Off() );
    }
    void Populate( gui::OptionMenu< kernel::FourStateOption >& menu )
    {
        menu.AddItem( kernel::FourStateOption::OnName(), kernel::FourStateOption::On() );
        menu.AddItem( kernel::FourStateOption::SuperiorSelectedName(), kernel::FourStateOption::SuperiorSelected() );
        menu.AddItem( kernel::FourStateOption::SelectedName(), kernel::FourStateOption::Selected() );
        menu.AddItem( kernel::FourStateOption::OffName(), kernel::FourStateOption::Off() );
    }
    void AddSubMenu3( kernel::ContextMenu* parent, const QString& label, const QIcon& iconSet, kernel::Options& options, const std::string& option )
    {
        gui::OptionMenu< kernel::TristateOption >* optionMenu = new gui::OptionMenu< kernel::TristateOption >( parent, options, option );
        Populate( *optionMenu );
        parent->insertItem( iconSet, label, optionMenu );
    }
    void AddSubMenu4( kernel::ContextMenu* parent, const QString& label, const QIcon& iconSet, kernel::Options& options, const std::string& option )
    {
        gui::OptionMenu< kernel::FourStateOption >* optionMenu = new gui::OptionMenu< kernel::FourStateOption >( parent, options, option );
        Populate( *optionMenu );
        parent->insertItem( iconSet, label, optionMenu );
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
Menu::Menu( const QString& objectName, QMainWindow* pParent, kernel::Controllers& controllers, const DialogContainer& dialogs, const QString& license )
    : RichWidget< QMenuBar >( objectName, pParent )
    , controllers_( controllers )
{
    // File
    gui::SubObjectName subObject( objectName );
    {
        fileMenu_ = new gui::RichMenu( "fileMenu", this, controllers, tools::translate( "Menu", "&File" ) );
        fileMenu_->SetModes( eModes_LivingArea, eModes_All ^ eModes_LivingArea, true );
        newAction_ = fileMenu_->InsertItem( "1", tools::translate( "Menu", "&New..." ) , parent(), SLOT( New() ) , QKeySequence( Qt::CTRL + Qt::Key_N ) );
        newAction_->setIcon( MAKE_ICON( new ) );
        openAction_ = fileMenu_->InsertItem( "1", tools::translate( "Menu", "&Open..." ), parent(), SLOT( Open() ), QKeySequence( Qt::CTRL + Qt::Key_O ) );
        openAction_->setIcon( MAKE_ICON( open ) );

        QAction* action = 0;
        action = fileMenu_->InsertItem( "1", tools::translate( "Menu", "&Reload" ), parent(), SLOT( ReloadExercise() ), QKeySequence( Qt::CTRL + Qt::Key_R ) );
        action->setIcon( MAKE_ICON( refresh ) );
        AddModdedAction( action, eModes_Default, eModes_All ^ eModes_Default );
        action = fileMenu_->InsertItem( "1", tools::translate( "Menu", "Close" ), parent(), SLOT( Close() ), QKeySequence( Qt::CTRL + Qt::Key_W ) );
        AddModdedAction( action, eModes_Default, eModes_All ^ eModes_Default );

        fileMenu_->GetCategoryCreateIFN( "2" ); // Export/import category
        action = fileMenu_->InsertItem( "2", tools::translate( "Menu", "&Export..." ), &dialogs.GetTerrainExportDialog(), SLOT( exec() ), QKeySequence( Qt::CTRL + Qt::SHIFT + Qt::Key_E ) );
        AddModdedAction( action, eModes_All ^ eModes_Terrain, eModes_Terrain );

        saveAction_ = fileMenu_->InsertItem( "3", tools::translate( "Menu", "&Save" )   , parent(), SLOT( Save() ), QKeySequence( Qt::CTRL + Qt::Key_S ) );
        saveAction_->setIcon( MAKE_ICON( save ) );
        AddModdedAction( saveAction_, eModes_Default, eModes_All ^ eModes_Default );
        saveAsAction_ = fileMenu_->InsertItem( "3", tools::translate( "Menu", "Save &As" ), parent(), SLOT( SaveAs() ), QKeySequence( Qt::CTRL + Qt::SHIFT + Qt::Key_S ) );
        saveAsAction_->setIcon( MAKE_ICON( saveas ) );
        AddModdedAction( saveAsAction_, eModes_Default, eModes_All ^ eModes_Default );
        fileMenu_->InsertItem( "4", tools::translate( "Menu", "&Quit" ), pParent, SLOT( close() ), QKeySequence( Qt::CTRL + Qt::Key_Q ) );
        addMenu( fileMenu_->FillMenu() );
    }
    // Profile
    {
        gui::RichMenu* menu = new gui::RichMenu( "profiles", this, controllers, tools::translate( "Menu", "&Profiles" ) );
        menu->SetModes( eModes_All ^ eModes_Prepare, eModes_Prepare, true );
        QAction* action = menu->InsertItem( "1", tools::translate( "Menu", "View/Edit..." ), &dialogs.GetProfileDialog(), SLOT( exec() ) );
        action->setIcon( MAKE_ICON( profile ) );
        menu->InsertItem( "2", tools::translate( "Menu", "Creation wizard..." ), &dialogs.GetProfileWizardDialog(), SLOT( exec() ) );
        addMenu( menu->FillMenu() );
    }
    // Display
    {
        gui::RichMenu* menu = new gui::RichMenu( "Settings", this, controllers, tools::translate( "Menu", "&Settings" ) );
        menu->SetModes( eModes_Default, eModes_All ^ eModes_Default, true );
        kernel::ContextMenu* subMenu = new kernel::ContextMenu( menu );
        AddSubMenu4( subMenu, tools::translate( "Menu", "Links" )            , MakePixmap( "logistic_links" ), controllers.options_, "LogisticLinks" );
        AddSubMenu4( subMenu, tools::translate( "Menu", "Missing links" )    , MakePixmap( "logistic_missing_links" ), controllers.options_, "MissingLogisticLinks" );

        gui::OptionMenu< int >* resourceMenu = new gui::OptionMenu< int >( subMenu, controllers.options_, "ResourceNetworks" );
        resourceMenu->AddItem( tools::translate( "Menu", "On" ), 0 );
        resourceMenu->AddItem( tools::translate( "Menu", "Off" ), 1 );
        resourceMenu->AddItem( tools::translate( "Menu", "Selected: all links" ), 2 );
        resourceMenu->AddItem( tools::translate( "Menu", "Selected: outgoing links" ), 3 );
        subMenu->insertItem( MakePixmap( "resource_networks" ), tools::translate( "Menu", "Resource networks" ), resourceMenu );
        menu->insertItem( tools::translate( "Menu", "Logistic..." ), subMenu );

        subMenu = new kernel::ContextMenu( menu );
        AddSubMenu3( subMenu, tools::translate( "Menu", "Small text" )    , MAKE_ICON( textsmall )    , controllers.options_, "SmallText" );
        AddSubMenu3( subMenu, tools::translate( "Menu", "Large text" )    , MAKE_ICON( textbig )      , controllers.options_, "BigText" );
        AddSubMenu4( subMenu, tools::translate( "Menu", "Tactical lines" ), MAKE_ICON( tacticallines ), controllers.options_, "TacticalLines" );
        subMenu->insertSeparator();
        gui::OptionMenu< float >* gridMenu = new gui::OptionMenu< float >( subMenu, controllers.options_, "GridSize" );
        gridMenu->AddItem( tools::translate( "Menu", "Off"    ),  -1 );
        gridMenu->AddItem( tools::translate( "Menu", "100m"  ),  0.1f );
        gridMenu->AddItem( tools::translate( "Menu", "250m" ),  0.25f );
        gridMenu->AddItem( tools::translate( "Menu", "500m"  ),  0.5f );
        gridMenu->AddItem( tools::translate( "Menu", "1km"  ),  1.0f );
        gridMenu->AddItem( tools::translate( "Menu", "2.5km"  ),  2.5f );
        gridMenu->AddItem( tools::translate( "Menu", "5km"  ),  5.0f );
        gridMenu->AddItem( tools::translate( "Menu", "10km" ), 10.0f );
        gridMenu->AddItem( tools::translate( "Menu", "100km" ), 100.0f );
        subMenu->insertItem( tools::translate( "Menu", "Grid" ), gridMenu );
        auto sizeMenu = new gui::OptionMenu< int >( subMenu, controllers.options_, "GridType" );
        sizeMenu->AddItem( tools::translate( "Menu", "Local" ), eCoordinateSystem_Local );
        sizeMenu->AddItem( tools::translate( "Menu", "MGRS" ), eCoordinateSystem_Mgrs );
        subMenu->insertItem( tools::translate( "Menu", "Grid type" ), sizeMenu );
        menu->insertItem( tools::translate( "Menu", "Terrain..." ), subMenu );
        menu->insertSeparator();

        gui::OptionMenu< bool >* boolMenu = new gui::OptionMenu< bool >( menu, controllers.options_, "3D", false );
        boolMenu->AddItem( tools::translate( "Menu", "2D" ), false );
        boolMenu->AddItem( tools::translate( "Menu", "3D" ), true );
        menu->insertItem( MAKE_ICON( threed ), tools::translate( "Menu", "Display mode" ), boolMenu );
        menu->insertItem( tools::translate( "Menu", "Toggle fullscreen mode" ), pParent, SLOT( ToggleFullScreen() ), Qt::Key_F12 );
        menu->insertItem( tools::translate( "Menu", "Toggle dock windows" ), pParent, SLOT( ToggleDocks() ), Qt::Key_F11 );
        menu->insertSeparator();
        menu->insertItem( tools::translate( "Menu", "&Preferences..." ), &dialogs.GetPrefDialog(), SLOT( exec() ), Qt::CTRL + Qt::Key_P );
        addMenu( menu );
    }
    // Exercise
    {
        gui::RichMenu* menu = new gui::RichMenu( "Exercise", this, controllers_, tools::translate( "Menu", "&Exercise" ) );
        menu->SetModes( eModes_All ^ eModes_Prepare, eModes_Prepare, true );
        menu->InsertItem( "", tools::translate( "Menu", "Properties..." ), &dialogs.GetExerciseDialog(), SLOT( exec() ) );
        menu->InsertItem( "", tools::translate( "Menu", "Scores..." ), &dialogs.GetScoreDialog(), SLOT( exec() ) );
        menu->InsertItem( "", tools::translate( "Menu", "Success factors..." ), &dialogs.GetSuccessFactorDialog(), SLOT( exec() ) );
        menu->InsertItem( "", tools::translate( "Menu", "Consistency analysis..." ), &dialogs.GetConsistencyDialog(), SLOT( Display() ) );
        menu->InsertItem( "", tools::translate( "Menu", "Performance..."), &dialogs.GetPerformanceDialog(), SLOT( exec() ) );
        addMenu( menu->FillMenu() );
    }
    // Windows
    if( QMenu* menu = pParent->createPopupMenu() )
    {
        gui::ObjectNameManager::getInstance()->SetObjectName( this, objectName );
        if( QAction* action = addMenu( menu ) )
        {
            action->setText( tools::translate( "Menu", "&Windows" ) );
            AddModdedAction( action, eModes_LivingArea | eModes_Default, eModes_Prepare | eModes_Terrain );
        }
        gui::ObjectNameManager::getInstance()->RemoveRegisteredName( menu->objectName() );
    }
    // Help
    {
        gui::RichMenu* menu = new gui::RichMenu( "aide", this, controllers_, tools::translate( "Menu", "&?" ) );
        menu->SetModes( eModes_None, eModes_All, true );
        menu->InsertItem( "1", tools::translate( "Menu", "Help" ), pParent, SIGNAL( ShowHelp() ) );
        gui::AboutDialog* about = new gui::AboutDialog( this, license );
        menu->InsertItem( "2", tools::translate( "Menu", "About" ), about, SLOT( open() ) );
        addMenu( menu->FillMenu() );
    }
    // Filters
    {
        connect( &dialogs.GetFiltersDialog(), SIGNAL( RemoveFilterMenuEntry( const QString ) ), this, SLOT( RemoveFileMenuEntry( const QString ) ) );
        connect( &dialogs.GetFiltersDialog(), SIGNAL( AddFilterMenuEntry( const QString, const QObject*, const char*, QKeySequence ) ), this, SLOT( InsertFileMenuEntry( const QString, const QObject*, const char*, QKeySequence ) ) );
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
void Menu::AddModdedAction( QAction* action, int hiddenModes /* = 0 */, int visibleModes /* = 0 */, bool visibleByDefault /* = true */ )
{
    if( !action )
        return;
    gui::RichAction* richAction = new gui::RichAction( action, controllers_ );
    richAction->SetModes( hiddenModes, visibleModes, visibleByDefault );
    moddedActions_.push_back( richAction );
}

// -----------------------------------------------------------------------------
// Name: Menu::InsertFileMenuEntry
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
void Menu::InsertFileMenuEntry( const QString name, const QObject* receiver, const char* member, const QKeySequence accel )
{
    fileMenu_->insertItem( name, receiver, member, accel, -1, filtersIndex_ );
    QAction* action = fileMenu_->actions().value( filtersIndex_ );
    AddModdedAction( action, eModes_All ^ eModes_Prepare, eModes_Prepare );
}

// -----------------------------------------------------------------------------
// Name: Menu::RemoveFileMenuEntry
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
void Menu::RemoveFileMenuEntry( const QString name )
{
    for( int i = 0; i < fileMenu_->actions().size(); ++i )
    {
        QAction* action = fileMenu_->actions().value( i );
        if( action && action->text() == name )
            fileMenu_->removeAction( action );;
    }
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
