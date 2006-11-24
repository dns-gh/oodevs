// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "FireResultListView.h"
#include "gaming/PopulationFireResult.h"
#include "gaming/AgentFireResult.h"
#include "gaming/Equipment.h"
#include "gaming/Casualties.h"
#include "clients_gui/SubItemDisplayer.h"
#include "clients_gui/ItemFactory_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: FireResultListView constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
FireResultListView::FireResultListView( QWidget* parent, ItemFactory_ABC& factory )
    : ListDisplayer< FireResultListView >( parent, *this, factory )
    , factory_( factory )
{
    AddColumn( tr( "Target" ) );
    AddColumn( tr( "Attrition" ) );
    
    agentDisplay_ = new SubItemDisplayer( tr( "Target" ), factory );
    agentDisplay_->AddChild( tr( "Equipments" ) );
    agentDisplay_->AddChild( tr( "Humans" ) );
}

// -----------------------------------------------------------------------------
// Name: FireResultListView destructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
FireResultListView::~FireResultListView()
{
    delete agentDisplay_;
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::Display
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void FireResultListView::Display( const PopulationFireResult* result, Displayer_ABC& displayer, ValuedListItem* item )
{
    if( ! result ) {
        delete item;
        return;
    }
    // $$$$ AGE 2006-03-10: Move in PopulationFireResult
    item->SetValue( result );
    displayer.Display( tr( "Target" ), result->target_ );
    displayer.Item( tr( "Attrition" ) ).Start( tr( "Dead:" ) ).Add( result->deadPeople_ ).End();
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::Display
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void FireResultListView::Display( const AgentFireResult* result, Displayer_ABC&, ValuedListItem* item )
{
    if( ! result ) {
        delete item;
        return;
    }
    // SubItemDisplayer;
    // $$$$ AGE 2006-03-10: Move in AgentFireResult
    item->SetValue( result );
    Displayer_ABC& displayer = (*agentDisplay_)( item );

    displayer.Display( tr( "Target" ), result->target_ );
    displayer.Display( tr( "Equipments" ), tr( " (avail, unavail, fixable):" ) );
    displayer.Display( "Troops", tr( " (officer, warrant-off., private)" ) );

    // $$$$ AGE 2006-02-28: crado. Essayer de faire un displayer qui puisse cascader 
    // $$$$ AGE 2006-02-28: les subItems et meler des listes aussi mais sans que l'interface
    // $$$$ AGE 2006-02-28: devienne horrible (MT_Archive_ABC quelqu'un ?)
    QListViewItem* last  = item->firstChild();
    QListViewItem* equipments = last;
    while( equipments && equipments->text( 0 ) != tr( "Equipments" ) ) {
        last = equipments;
        equipments = equipments->nextSibling();
    }
    if( ! equipments )
        equipments = factory_.CreateItem( item, last );
    DeleteTail( 
        DisplayList( result->CreateIterator(), equipments )
    );

    last = item->firstChild();
    QListViewItem* humans = last;
    while( humans && humans->text( 0 ) != tr( "Troops" ) ) {
        last = humans;
        humans = humans->nextSibling();
    }
    if( ! humans )
        humans = factory_.CreateItem( item, last );
    DeleteTail( 
        DisplayList( &* result->casualties_, result->casualties_+eNbrHumanWound, humans )
    );
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::Display
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void FireResultListView::Display( const Equipment& equipment, Displayer_ABC& displayer, ValuedListItem* )
{
    displayer.Display( tr( "Target" ), equipment.type_ );
    displayer.Item( tr( "Attrition" ) ).Start( equipment.available_ )
                    .Add( " / " ).Add( equipment.unavailable_ )
                    .Add( " / " ).Add( equipment.repairable_ ).End();
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::Display
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void FireResultListView::Display( const Casualties& casualties, Displayer_ABC& displayer, ValuedListItem* )
{
    displayer.Display( tr( "Target" ), casualties.wound_ );
    displayer.Item( tr( "Attrition" ) ).Start( casualties.officers_ )
                    .Add( " / " ).Add( casualties.subOfficers_ )
                    .Add( " / " ).Add( casualties.troopers_ ).End();
}
