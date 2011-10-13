// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::UnitsPanel */

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
        combo->AddItem( tools::translate( "gui::UnitsPanel", "Hierarchical view" ), "" );
        combo->AddItem( tools::translate( "gui::UnitsPanel", "Level" ), "level" );
        combo->AddItem( tools::translate( "gui::UnitsPanel", "Nature" ), "nature" );
        combo->AddItem( tools::translate( "gui::UnitsPanel", "Atlas qualifier" ), "atlas" );
        return combo;
    }
}

// -----------------------------------------------------------------------------
// Name: UnitsPanel constructor
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
UnitsPanel::UnitsPanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, const AgentTypes& types, ItemFactory_ABC& factory, SymbolIcons& icons, ColorStrategy_ABC& colorStrategy )
    : InfoPanel_ABC( parent, panel, tr( "Units" ), "UnitsPanel" )
    , controllers_( controllers )
{
    Q3VBox* vbox = new Q3VBox( this );
    vbox->setMargin( 5 );
    vbox->setSpacing( 5 );
    {
        Q3HBox* box = new Q3HBox( vbox );
        QPushButton* openAll  = new QPushButton( "+", box );
        openAll->setMaximumSize( 20, 20 );;
        QPushButton* closeAll = new QPushButton( "-", box );
        closeAll->setMaximumSize( 20, 20 );;
        connect( openAll , SIGNAL( clicked() ), SLOT( OpenList() ) );
        connect( closeAll, SIGNAL( clicked() ), SLOT( CloseList() ) );

        QLabel* label = new QLabel( tr( "Display type: " ), box );
        label->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
        combo_ = BuildNatureFieldsCombo( box );
        list_ = new UnitListView( vbox, controllers_, types, factory );
        connect( combo_, SIGNAL( activated( int ) ), SLOT( Sort() ) );
        connect( list_ , SIGNAL( selectionChanged( Q3ListViewItem* ) ), SLOT( SelectionChanged( Q3ListViewItem* ) ) );
    }
    {
        icon_ = new UnitPreviewIcon( vbox, controllers_, icons, colorStrategy );
        connect( icon_, SIGNAL( StartDrag() ), SLOT( IconDragged() ) );
    }
    setWidget( vbox );
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
void UnitsPanel::SelectionChanged( Q3ListViewItem* item )
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
    if( Q3DragObject* drag = list_->dragObject() )
        drag->drag();
}
