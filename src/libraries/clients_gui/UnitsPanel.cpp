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

#include "DragAndDropHelpers.h"
#include "RichPushButton.h"
#include "SubObjectName.h"
#include "UnitTreeView.h"
#include "UnitPreviewIcon.h"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Tools.h"

using namespace kernel;
using namespace gui;

namespace
{
    ValuedComboBox< std::string >* BuildNatureFieldsCombo( const QString& objectName, QWidget* parent )
    {
        ValuedComboBox< std::string >* combo = new ValuedComboBox< std::string >( objectName, parent );
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
UnitsPanel::UnitsPanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, const AgentTypes& types, SymbolIcons& icons, ColorStrategy_ABC& colorStrategy )
    : InfoPanel_ABC( parent, panel, tr( "Units" ), "UnitsPanel" )
    , controllers_( controllers )
{
    gui::SubObjectName subObject( "unitPanel" );
    Q3VBox* vbox = new Q3VBox( this );
    vbox->setMargin( 5 );
    vbox->setSpacing( 5 );
    {
        Q3HBox* box = new Q3HBox( vbox );
        RichPushButton* openAll  = new RichPushButton( "openAll", "+", box );
        openAll->setMaximumSize( 20, 20 );
        RichPushButton* closeAll = new RichPushButton( "closeAll", "-", box );
        closeAll->setMaximumSize( 20, 20 );
        connect( openAll , SIGNAL( clicked() ), SLOT( OpenList() ) );
        connect( closeAll, SIGNAL( clicked() ), SLOT( CloseList() ) );

        QLabel* label = new QLabel( tr( "Display type: " ), box );
        label->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
        combo_ = BuildNatureFieldsCombo( "combo", box );
        list_ = new UnitTreeView( "unitTreeView", controllers_, types, vbox );
        connect( combo_, SIGNAL( activated( int ) ), SLOT( Sort() ) );
        connect( list_->selectionModel() , SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), SLOT( SelectionChanged() ) );
    }
    {
        icon_ = new UnitPreviewIcon( vbox, controllers_, icons, colorStrategy, tr( "Drag and drop symbol to map to create a new unit." ) );
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
    list_->expandAll();
}

// -----------------------------------------------------------------------------
// Name: UnitsPanel::CloseList
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
void UnitsPanel::CloseList()
{
    list_->collapseAll();
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
void UnitsPanel::SelectionChanged()
{
    if( kernel::AgentType* type = list_->GetSelected< kernel::AgentType >() )
        icon_->NotifySelected( *type );
    else if( kernel::AutomatType* type = list_->GetSelected< kernel::AutomatType >() )
        icon_->NotifySelected( *type );
}

// -----------------------------------------------------------------------------
// Name: UnitsPanel::IconDragged
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void UnitsPanel::IconDragged()
{
    if( kernel::AgentType* type = list_->GetSelected< kernel::AgentType >() )
        dnd::CreateDragObject( type, this, Qt::CopyAction );
    else if( kernel::AutomatType* type = list_->GetSelected< kernel::AutomatType >() )
        dnd::CreateDragObject( type, this, Qt::CopyAction );
}
