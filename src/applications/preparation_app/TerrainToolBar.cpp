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

    QDoubleSpinBox* CreateSwitchModeButtonContextMenu( QToolButton* button )
    {
        QWidget* holder = new QWidget;
        QHBoxLayout* hLayout = new QHBoxLayout;
        hLayout->setContentsMargins( 1, 1, 1, 1 );
        holder->setLayout( hLayout );

        // Description label
        hLayout->addWidget( new QLabel( button->tr( "Road width (m):" ) ) );

        // Spin box
        QDoubleSpinBox* spinBox = new QDoubleSpinBox;
        spinBox->setRange( 2.0, 100.0 );
        spinBox->setValue( 10.0 );
        spinBox->setSingleStep( 1.0 );
        hLayout->addWidget( spinBox );

        QMenu* menu = new kernel::ContextMenu( button );
        QWidgetAction* action = new QWidgetAction( menu );
        action->setDefaultWidget( holder );
        menu->addAction( action );

        button->setPopupDelay( 0 );
        button->setPopupMode( QToolButton::MenuButtonPopup );
        button->setPopup( menu );

        return spinBox;
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
    roadWidthSpinBox_ = CreateSwitchModeButtonContextMenu( switchModeButton_ );

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
    controllers_.ChangeMode( switchModeButton_->isChecked() ? ePreparationMode_Terrain : ePreparationMode_Exercise );
    UncheckBlockCreationButtons();
    EnableBlockCreationButtons( false );
    blockRemoveButton_->setEnabled( switchModeButton_->isChecked() );
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
    urbanModel_.CreateUrbanBlocks( location, *selected_.ConstCast(), isAuto_, roadWidthSpinBox_->value() );
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
    catch( const std::exception& e )
    {
        QMessageBox::critical( 0, tr( "Error during automatic process creation of urban blocks" ), tools::GetExceptionMsg( e ).c_str() );
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
