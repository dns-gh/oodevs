// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "UnitsPanel.h"
#include "moc_UnitsPanel.cpp"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/AgentTypes.h"
#include "UnitListView.h"
#include "UnitPreviewIcon.h"
#include "Tools.h"

using namespace kernel;
using namespace gui;

namespace
{
    ValuedComboBox< std::string >* BuildNatureFieldsCombo( QWidget* parent )
    {
        ValuedComboBox< std::string >* combo = new ValuedComboBox< std::string >( parent );
        combo->AddItem( tools::translate( "UnitsPanel", "Hierarchical view" ), "" );
        combo->AddItem( tools::translate( "UnitsPanel", "Level" ), "level" );
        combo->AddItem( tools::translate( "UnitsPanel", "Nature" ), "nature" );
        combo->AddItem( tools::translate( "UnitsPanel", "Atlas qualifier" ), "atlas" );
        return combo;
    }
}

// -----------------------------------------------------------------------------
// Name: UnitsPanel constructor
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
UnitsPanel::UnitsPanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, const AgentTypes& types, ItemFactory_ABC& factory, SymbolIcons& icons, ColorStrategy_ABC& colorStrategy )
    : InfoPanel_ABC( parent, panel, tr( "Units" ) )
    , controllers_( controllers )
{
    {
        QHBox* box = new QHBox( this );
        QPushButton* openAll  = new QPushButton( "+", box );
        openAll->setMaximumSize( 20, 20 );;
        QPushButton* closeAll = new QPushButton( "-", box );
        closeAll->setMaximumSize( 20, 20 );;
        connect( openAll , SIGNAL( clicked() ), SLOT( OpenList() ) );
        connect( closeAll, SIGNAL( clicked() ), SLOT( CloseList() ) );

        QLabel* label = new QLabel( tr( "Display type: " ), box );
        label->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
        combo_ = BuildNatureFieldsCombo( box );
        list_ = new UnitListView( this, controllers_, types, factory );
        connect( combo_, SIGNAL( activated( int ) ), SLOT( Sort() ) );
        connect( list_ , SIGNAL( selectionChanged( QListViewItem* ) ), SLOT( SelectionChanged( QListViewItem* ) ) );
    }
    {
        icon_ = new UnitPreviewIcon( this, controllers_, icons, colorStrategy );
        connect( icon_, SIGNAL( StartDrag() ), SLOT( IconDragged() ) );
    }
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitsPanel destructor
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
UnitsPanel::~UnitsPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitsPanel::Sort
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
void UnitsPanel::Sort()
{
    list_->SetSorting( combo_->GetValue() );
}

// -----------------------------------------------------------------------------
// Name: UnitsPanel::OpenList
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
void UnitsPanel::OpenList()
{
    list_->SetOpen( true );
}
    
// -----------------------------------------------------------------------------
// Name: UnitsPanel::CloseList
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
void UnitsPanel::CloseList()
{
    list_->SetOpen( false );
}

// -----------------------------------------------------------------------------
// Name: UnitsPanel::NotifyUpdated
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
void UnitsPanel::NotifyUpdated( const ModelLoaded& )
{
    Show();
}

// -----------------------------------------------------------------------------
// Name: UnitsPanel::SelectionChanged
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void UnitsPanel::SelectionChanged( QListViewItem* item )
{
    if( ValuedListItem* value = dynamic_cast< ValuedListItem* >( item ) )
    {
        if( value->IsA< const AgentType >() )
            icon_->NotifySelected( *value->GetValue< const AgentType >() );
        else if( value->IsA< const AutomatType >() )
            icon_->NotifySelected( *value->GetValue< const AutomatType >() );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitsPanel::IconDragged
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void UnitsPanel::IconDragged()
{
    if( QDragObject* drag = list_->dragObject() )
        drag->drag();
}
