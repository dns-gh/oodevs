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
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ModeController_ABC.h"
#include "ENT/ENT_Enums_Gen.h"
#include "MainWindow.h"

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
TerrainToolBar::TerrainToolBar( QWidget* parent, kernel::Controllers& controllers )
    : gui::RichToolBar( controllers, parent, "terrainToolBar", tr( "Terrain" )/*, false*/ )
{
    // Terrain button
    switchModeButton_ = AddButton( this, this, SLOT( OnSwitchMode() ), tr( "Edit urban area" ), "resources/images/preparation/livingArea.png", true );
    // Block creation button
    blockCreationButton_ = AddButton( this, this, SLOT( OnBlockCreationMode() ), tr( "Manual block creation" ), "resources/images/preparation/CreateBlock.png", true );
    // Multi blocks creation button
    blockCreationAutoButton_ = AddButton( this, this, SLOT( OnBlockCreationAutoMode() ), tr( "Automatic block creation on built-up area" ), "resources/images/preparation/CreateBlockAuto.png", true );
    // Remove blocks button
    blockRemoveButton_ = AddButton( this, this, SLOT( OnRemoveBlocks() ), tr( "Remove urban blocks" ), "resources/images/preparation/RemoveBlockAuto.png", false );
    // Layout
    addWidget( switchModeButton_ );
    addSeparator();
    addWidget( blockCreationButton_ );
    addWidget( blockCreationAutoButton_ );
    addWidget( blockRemoveButton_ );
    // Disable buttons
    EnableBlockCreationButtons( false );
    blockRemoveButton_->setEnabled( false );
}

// -----------------------------------------------------------------------------
// Name: TerrainToolBar destructor
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
TerrainToolBar::~TerrainToolBar()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TerrainToolBar::OnSwitchMode
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
void TerrainToolBar::OnSwitchMode()
{
    if( static_cast< MainWindow* >( parent() )->SwitchToTerrainMode( switchModeButton_->isChecked() ) )
    {
        // Succeed
        UncheckBlockCreationButtons();
        EnableBlockCreationButtons( false );
        blockRemoveButton_->setEnabled( switchModeButton_->isChecked() );
    }
    else
    {
        // Canceled
        switchModeButton_->disconnect( this, SLOT( OnSwitchMode() ) );
        switchModeButton_->setChecked( !switchModeButton_->isChecked() );
        QObject::connect( switchModeButton_, SIGNAL( clicked() ), this, SLOT( OnSwitchMode() ) );
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

// $$$$ ABR 2012-05-15: TODO As soon as multi selection and urban ODB will be implemented.

// void SelectionChanged( selection )
//{
//    UncheckBlockCreationButtons();
//    EnableBlockCreationButtons( selection == district )
//}

// -----------------------------------------------------------------------------
// Name: TerrainToolBar::OnBlockCreationMode
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
void TerrainToolBar::OnBlockCreationMode()
{
    //eventStrategy_->TakeExclusiveFocus( *editorLayer_ );
    //if( editorLayer_ && editorDrawer_.get() )
    //    editorLayer_->StartPolygon( *editorDrawer_, false );
    //UncheckBlockCreationButtons();
}

// -----------------------------------------------------------------------------
// Name: TerrainToolBar::OnBlockCreationAutoMode
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
void TerrainToolBar::OnBlockCreationAutoMode()
{
    //try
    //{
    //    eventStrategy_->TakeExclusiveFocus( *editorLayer_ );
    //    if( editorLayer_ && selectionDrawer_.get() )
    //        editorLayer_->StartPolygon( *selectionDrawer_, true );
    //    UncheckBlockCreationButtons();
    //}
    //catch( std::exception& e )
    //{
    //    QMessageBox::critical( 0, tr( "Error during automatic process creation of urban blocks" ), e.what() );
    //    UncheckBlockCreationButtons();
    //}
}

// -----------------------------------------------------------------------------
// Name: TerrainToolBar::OnRemoveBlocks
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
void TerrainToolBar::OnRemoveBlocks()
{
    //bool ok;
    //int area = QInputDialog::getInt( this, tr( "Delete blocks" ), tr("Minimum size (m²): "), 100, 0, std::numeric_limits< int >::max(), 1, &ok );
    //if( ok )
    //    controller_.Delete( area );
}
