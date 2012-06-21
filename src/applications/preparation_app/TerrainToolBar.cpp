// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "TerrainToolBar.h"
#include "moc_TerrainToolBar.cpp"
#include "MainWindow.h"
#include "RemoveBlocksDialog.h"
#include "clients_gui/ExclusiveEventStrategy.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ModeController_ABC.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "ENT/ENT_Enums_Gen.h"
#include "preparation/UrbanHierarchies.h"
#include "preparation/UrbanModel.h"

namespace
{
    QToolButton* AddButton( QWidget* parent, const QObject *receiver, const char *member, const QString& toolTip = "", const QString& iconPath = "", bool isCheckable = false )
    {
        QToolButton* button = new QToolButton( parent );
        QObject::connect( button, SIGNAL( clicked() ), receiver, member );
        if( !toolTip.isEmpty() )
            button->setToolTip( toolTip );
        if( !iconPath.isEmpty() )
            button->setIcon( QIcon( iconPath ) );
        button->setCheckable( isCheckable );
        return button;
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainToolBar constructor
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
TerrainToolBar::TerrainToolBar( QWidget* parent, kernel::Controllers& controllers, gui::ExclusiveEventStrategy& eventStrategy, gui::ParametersLayer& paramLayer, UrbanModel& urbanModel, RemoveBlocksDialog& removeBlocksDialog )
    : gui::RichToolBar( controllers, parent, "terrainToolBar", tr( "Terrain" ) )
    , eventStrategy_( eventStrategy )
    , paramLayer_   ( paramLayer )
    , urbanModel_   ( urbanModel )
    , selected_     ( controllers )
    , isAuto_       ( false )
{
    // Terrain button
    switchModeButton_ = AddButton( this, this, SLOT( OnSwitchMode() ), tr( "Edit urban area" ), "resources/images/preparation/livingArea.png", true );
    // Block creation button
    blockCreationButton_ = AddButton( this, this, SLOT( OnBlockCreation() ), tr( "Manual block creation" ), "resources/images/preparation/CreateBlock.png", true );
    // Multi blocks creation button
    blockCreationAutoButton_ = AddButton( this, this, SLOT( OnBlockCreationAuto() ), tr( "Automatic block creation on built-up area" ), "resources/images/preparation/CreateBlockAuto.png", true );
    // Remove blocks button
    blockRemoveButton_ = AddButton( this, &removeBlocksDialog, SLOT( show() ), tr( "Remove urban blocks" ), "resources/images/preparation/RemoveBlockAuto.png", false );
    // Layout
    addWidget( switchModeButton_ );
    addSeparator();
    addWidget( blockCreationButton_ );
    addWidget( blockCreationAutoButton_ );
    addWidget( blockRemoveButton_ );
    // Disable buttons
    EnableBlockCreationButtons( false );
    blockRemoveButton_->setEnabled( false );
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: TerrainToolBar destructor
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
TerrainToolBar::~TerrainToolBar()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: TerrainToolBar::OnSwitchMode
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
void TerrainToolBar::OnSwitchMode()
{
    QMessageBox::StandardButton ret = static_cast< MainWindow* >( parent() )->CheckSaving( true );
    if( ret == QMessageBox::Cancel )
    {
        // Canceled
        switchModeButton_->disconnect( this, SLOT( OnSwitchMode() ) );
        switchModeButton_->setChecked( !switchModeButton_->isChecked() );
        QObject::connect( switchModeButton_, SIGNAL( clicked() ), this, SLOT( OnSwitchMode() ) );
        blockRemoveButton_->setEnabled( switchModeButton_->isChecked() );
    }
    else
    {
        // Succeed
        if( ret == QMessageBox::No )
            static_cast< MainWindow* >( parent() )->ExternalReload();
        controllers_.ChangeMode( switchModeButton_->isChecked() ? ePreparationMode_Terrain : ePreparationMode_Exercise );
        UncheckBlockCreationButtons();
        EnableBlockCreationButtons( false );
        blockRemoveButton_->setEnabled( switchModeButton_->isChecked() );
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainToolBar::UncheckBlockCreationButtons
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
void TerrainToolBar::UncheckBlockCreationButtons()
{
    blockCreationButton_->setChecked( false );
    blockCreationAutoButton_->setChecked( false );
}

// -----------------------------------------------------------------------------
// Name: TerrainToolBar::EnableBlockCreationButtons
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
void TerrainToolBar::EnableBlockCreationButtons( bool enabled )
{
    blockCreationButton_->setEnabled( enabled );
    blockCreationAutoButton_->setEnabled( enabled );
}

// -----------------------------------------------------------------------------
// Name: TerrainToolBar::NotifySelected
// Created: ABR 2012-05-31
// -----------------------------------------------------------------------------
void TerrainToolBar::NotifySelected( const kernel::UrbanObject_ABC* urbanObject )
{
    selected_ = 0;
    if( GetCurrentMode() != ePreparationMode_Terrain )
        return;
    UncheckBlockCreationButtons();
    bool enabled = false;
    if( urbanObject )
        if( const UrbanHierarchies* urbanHierarchies = static_cast< const UrbanHierarchies* >( urbanObject->Retrieve< kernel::Hierarchies >() ) )
        {
            enabled = urbanHierarchies->GetLevel() == eUrbanLevelDistrict;
        }
    EnableBlockCreationButtons( enabled );
    if( enabled )
        selected_ = urbanObject;
}

// -----------------------------------------------------------------------------
// Name: TerrainToolBar::Handle
// Created: ABR 2012-05-31
// -----------------------------------------------------------------------------
void TerrainToolBar::Handle( kernel::Location_ABC& location )
{
    if( !selected_ )
        return;
    urbanModel_.CreateUrbanBlocks( location, *selected_.ConstCast(), isAuto_ );
}

// -----------------------------------------------------------------------------
// Name: TerrainToolBar::OnBlockCreation
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
void TerrainToolBar::OnBlockCreation()
{
    isAuto_ = false;
    eventStrategy_.TakeExclusiveFocus( paramLayer_ );
    paramLayer_.StartPolygon( *this );
    eventStrategy_.ReleaseExclusiveFocus();
    UncheckBlockCreationButtons();
}

// -----------------------------------------------------------------------------
// Name: TerrainToolBar::OnBlockCreationAuto
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
void TerrainToolBar::OnBlockCreationAuto()
{
    try
    {
        isAuto_ = true;
        eventStrategy_.TakeExclusiveFocus( paramLayer_ );
        paramLayer_.StartPolygon( *this );
        eventStrategy_.ReleaseExclusiveFocus();
        UncheckBlockCreationButtons();
    }
    catch( std::exception& e )
    {
        QMessageBox::critical( 0, tr( "Error during automatic process creation of urban blocks" ), e.what() );
        eventStrategy_.ReleaseExclusiveFocus();
        UncheckBlockCreationButtons();
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainToolBar::NotifyModeChanged
// Created: ABR 2012-06-05
// -----------------------------------------------------------------------------
void TerrainToolBar::NotifyModeChanged( int newMode, bool useDefault, bool firstChangeToSavedMode )
{
    gui::RichToolBar::NotifyModeChanged( newMode, useDefault, firstChangeToSavedMode );
    if( newMode != ePreparationMode_Terrain )
        switchModeButton_->setChecked( false );
}
