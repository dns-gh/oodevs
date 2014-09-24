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
#include "clients_gui/ImageWrapper.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/RichSpinBox.h"
#include "clients_gui/RichWidget.h"
#include "clients_gui/SymbolSizeOptionChooser.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "ENT/ENT_Enums.h"
#include "preparation/UrbanHierarchies.h"
#include "preparation/UrbanModel.h"

namespace
{
    gui::RichWidget< QToolButton >* AddButton( const QString& objectName, QWidget* parent, const QObject *receiver, const char *member, const QString& toolTip = "", const tools::Path& iconPath = "", bool isCheckable = false )
    {
        gui::RichWidget< QToolButton >* button = new gui::RichWidget< QToolButton >( objectName, parent );
        QObject::connect( button, SIGNAL( clicked() ), receiver, member );
        if( !toolTip.isEmpty() )
            button->setToolTip( toolTip );
        if( !iconPath.IsEmpty() )
            button->setIcon( gui::Icon( iconPath ) );
        button->setCheckable( isCheckable );
        return button;
    }

    QDoubleSpinBox* CreateSwitchModeButtonContextMenu( gui::RichWidget< QToolButton >* button )
    {
        QWidget* holder = new QWidget;
        QHBoxLayout* hLayout = new QHBoxLayout;
        hLayout->setContentsMargins( 1, 1, 1, 1 );
        holder->setLayout( hLayout );

        // Description label
        hLayout->addWidget( new QLabel( tools::translate( "TerrainToolBar", "Road width (m):" ) ) );

        // Spin box
        gui::RichDoubleSpinBox* spinBox = new gui::RichDoubleSpinBox( "doubleSpinBox" );
        spinBox->setRange( 2.0, 100.0 );
        spinBox->setValue( 10.0 );
        spinBox->setSingleStep( 1.0 );
        hLayout->addWidget( spinBox );

        QMenu* menu = new kernel::ContextMenu( button );
        QWidgetAction* action = new QWidgetAction( menu );
        action->setDefaultWidget( holder );
        menu->addAction( action );

        button->setPopupDelay( 0 );
        button->setPopupMode( gui::RichWidget< QToolButton >::MenuButtonPopup );
        button->setPopup( menu );

        return spinBox;
    }
}

QPixmap MakePixmap( const std::string& name )
{
    return QPixmap( QString( "resources/images/gui/" ) + QString::fromStdString( name ) + QString( ".png" ) );
}

// -----------------------------------------------------------------------------
// Name: TerrainToolBar constructor
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
TerrainToolBar::TerrainToolBar( QWidget* parent,
                                kernel::Controllers& controllers,
                                gui::ExclusiveEventStrategy& eventStrategy,
                                const std::shared_ptr< gui::ParametersLayer >& paramLayer,
                                UrbanModel& urbanModel,
                                RemoveBlocksDialog& removeBlocksDialog )
    : gui::RichToolBar( controllers, parent, "terrainToolBar", tools::translate( "TerrainToolBar", "Terrain" ) )
    , eventStrategy_( eventStrategy )
    , paramLayer_   ( paramLayer )
    , urbanModel_   ( urbanModel )
    , selected_     ( controllers )
    , blinks_       ( 0 )
    , isAuto_       ( false )
    , changingGeom_ ( false )
{
    gui::SubObjectName subObject( "TerrainToolBar" );
    // Terrain button
    switchModeButton_ = AddButton( "switchModeButton", this, this, SLOT( OnSwitchMode() ), tools::translate( "TerrainToolBar", "Edit urban area" ), "resources/images/preparation/livingArea.png", true );
    roadWidthSpinBox_ = CreateSwitchModeButtonContextMenu( switchModeButton_ );

    // Block creation button
    blockCreationButton_ = AddButton( "blockCreationButton", this, this, SLOT( OnBlockCreation() ), tools::translate( "TerrainToolBar", "Manual block creation" ), "resources/images/preparation/CreateBlock.png", true );
    // Multi blocks creation button
    blockCreationAutoButton_ = AddButton( "blockCreationAutoButton", this, this, SLOT( OnBlockCreationAuto() ), tools::translate( "TerrainToolBar", "Automatic block creation on built-up area" ), "resources/images/preparation/CreateBlockAuto.png", true );
    // Remove blocks button
    blockRemoveButton_ = AddButton( "blockRemoveButton", this, &removeBlocksDialog, SLOT( show() ), tools::translate( "TerrainToolBar", "Remove urban blocks" ), "resources/images/preparation/RemoveBlockAuto.png", false );
    // Layout
    addWidget( switchModeButton_ );
    addSeparator();
    addWidget( blockCreationButton_ );
    addWidget( blockCreationAutoButton_ );
    addWidget( blockRemoveButton_ );
    new SymbolSizeOptionChooser( parent, this, MakePixmap( "symbol_increase" ), MakePixmap( "symbol_decrease" ), controllers );
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
    controllers_.ChangeMode( switchModeButton_->isChecked() ? eModes_Terrain : eModes_Prepare );
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
    if( GetCurrentMode() != eModes_Terrain )
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
// Name: TerrainToolBar::NotifyContextMenu
// Created: JSR 2013-01-07
// -----------------------------------------------------------------------------
void TerrainToolBar::NotifyContextMenu( const kernel::UrbanObject_ABC& object, kernel::ContextMenu& menu )
{
    if( GetCurrentMode() != eModes_Terrain )
        return;
    const UrbanHierarchies* urbanHierarchies = static_cast< const UrbanHierarchies* >( object.Retrieve< kernel::Hierarchies >() );
    if( urbanHierarchies && urbanHierarchies->GetLevel() == eUrbanLevelBlock )
    {
        selected_ = &object;
        menu.InsertItem( "Urban", tools::translate( "TerrainToolBar", "Change shape" ), this, SLOT( OnChangeShape() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainToolBar::Handle
// Created: ABR 2012-05-31
// -----------------------------------------------------------------------------
void TerrainToolBar::Handle( kernel::Location_ABC& location )
{
    if( !selected_ )
        return;
    std::vector< const kernel::UrbanObject_ABC* > intersectedBlocks;
    if( changingGeom_ )
        urbanModel_.ChangeShape( location, *selected_.ConstCast(), intersectedBlocks );
    else if( isAuto_ )
        urbanModel_.CreateAutoUrbanBlocks( location, *selected_.ConstCast(), roadWidthSpinBox_->value() );
    else
        urbanModel_.CreateManualUrbanBlock( location, *selected_.ConstCast(), intersectedBlocks );
    if( !intersectedBlocks.empty() )
    {
        blinks_ = 6;
        for( auto it = intersectedBlocks.begin(); it != intersectedBlocks.end(); ++it )
        {
            auto ptr = new kernel::SafePointer< kernel::UrbanObject_ABC >( controllers_, *it );
            urbanColors_[ ptr ] = ( *it )->Get< kernel::UrbanColor_ABC >().GetColor();
            ptr->ConstCast()->Get< kernel::UrbanColor_ABC >().SetColor( 255, 0, 0, 255 );
        }
        QTimer::singleShot( 250, this, SLOT( OnBlink() ) );
        QMessageBox::warning( this, tools::translate( "TerrainToolBar", "Warning" ), tools::translate( "TerrainToolBar", "Urban blocks cannot be created on top of existing blocks." ) );
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainToolBar::OnBlink
// Created: JSR 2013-01-15
// -----------------------------------------------------------------------------
void TerrainToolBar::OnBlink()
{
    for( auto it = urbanColors_.begin(); it != urbanColors_.end(); ++it )
    {
        if( it->first && *it->first )
        {
            if( blinks_ % 2 )
                it->first->ConstCast()->Get< kernel::UrbanColor_ABC >().SetColor( it->second );
            else
                it->first->ConstCast()->Get< kernel::UrbanColor_ABC >().SetColor( 255, 0, 0, 255 );
        }
    }
    if( --blinks_ != 0 )
        QTimer::singleShot( 250, this, SLOT( OnBlink() ) );
    else
    {
        for( auto it = urbanColors_.begin(); it != urbanColors_.end(); ++it )
            delete it->first;
        urbanColors_.clear();
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainToolBar::OnBlockCreation
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
void TerrainToolBar::OnBlockCreation()
{
    isAuto_ = false;
    changingGeom_ = false;
    eventStrategy_.TakeExclusiveFocus( paramLayer_ );
    paramLayer_->StartPolygon( *this );
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
        changingGeom_ = false;
        eventStrategy_.TakeExclusiveFocus( paramLayer_ );
        paramLayer_->StartPolygon( *this );
        eventStrategy_.ReleaseExclusiveFocus();
        UncheckBlockCreationButtons();
    }
    catch( const std::exception& e )
    {
        QMessageBox::critical( 0, tools::translate( "TerrainToolBar", "Error during automatic process creation of urban blocks" ), tools::GetExceptionMsg( e ).c_str() );
        eventStrategy_.ReleaseExclusiveFocus();
        UncheckBlockCreationButtons();
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainToolBar::OnChangeShape
// Created: JSR 2013-01-07
// -----------------------------------------------------------------------------
void TerrainToolBar::OnChangeShape()
{
    changingGeom_ = true;
    eventStrategy_.TakeExclusiveFocus( paramLayer_ );
    paramLayer_->StartPolygon( *this );
    eventStrategy_.ReleaseExclusiveFocus();
}

// -----------------------------------------------------------------------------
// Name: TerrainToolBar::NotifyModeChanged
// Created: ABR 2012-06-05
// -----------------------------------------------------------------------------
void TerrainToolBar::NotifyModeChanged( E_Modes newMode, bool useDefault, bool firstChangeToSavedMode )
{
    gui::RichToolBar::NotifyModeChanged( newMode, useDefault, firstChangeToSavedMode );
    if( newMode != eModes_Terrain )
        switchModeButton_->setChecked( false );
}
