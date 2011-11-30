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
#include "gaming/Explosions.h"
#include "clients_gui/SubItemDisplayer.h"
#include "clients_gui/ItemFactory_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: FireResultListView constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
FireResultListView::FireResultListView( QWidget* parent, kernel::Controllers& controllers, ItemFactory_ABC& factory )
    : ListDisplayer< FireResultListView >( parent, *this, factory )
    , controllers_( controllers )
    , factory_( factory )
    , selected_( controllers )
{
    setFrameStyle( Q3Frame::Plain );
    setMargin( 2 );
    AddColumn( tools::translate( "FireResultListView", "Date" ) );
    AddColumn( tools::translate( "FireResultListView", "Target" ) );
    AddColumn( tools::translate( "FireResultListView", "Attrition" ) );

    subDisplayer_ = new SubItemDisplayer( tools::translate( "FireResultListView", "Target" ), factory );
    subDisplayer_->AddChild( tools::translate( "FireResultListView", "Equipments" ) );
    subDisplayer_->AddChild( tools::translate( "FireResultListView", "Troops" ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: FireResultListView destructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
FireResultListView::~FireResultListView()
{
    controllers_.Unregister( *this );
    delete subDisplayer_;
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::Display
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void FireResultListView::Display( const PopulationFireResult* result, Displayer_ABC& displayer, ValuedListItem* item )
{
    if( ! result ) {
        RemoveItem( item );
        return;
    }
    // $$$$ AGE 2006-03-10: Move in PopulationFireResult
    item->SetValue( result );
    displayer.Display( tools::translate( "FireResultListView", "Date" ), result->time_ );
    displayer.Display( tools::translate( "FireResultListView", "Target" ), result->target_ );
    displayer.Item( tools::translate( "FireResultListView", "Attrition" ) );
    if( result->deadPeople_ )
        displayer.Start( tools::translate( "FireResultListView", "Dead:" ) ).Add( result->deadPeople_ );
    if( result->woundedPeople_ )
        displayer.Start( tools::translate( "FireResultListView", "Wounded:" ) ).Add( result->woundedPeople_ );
    if( result->scatteredPeople_ )
        displayer.Start( tools::translate( "FireResultListView", "Scattered:" ) ).Add( result->scatteredPeople_ );
    displayer.End();
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::Display
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void FireResultListView::Display( const AgentFireResult* result, Displayer_ABC& rootDisplayer, ValuedListItem* item )
{
    if( ! result ) {
        RemoveItem( item );
        return;
    }
    // SubItemDisplayer;
    // $$$$ AGE 2006-03-10: Move in AgentFireResult
    item->SetValue( result );
    Displayer_ABC& displayer = (*subDisplayer_)( item );

    rootDisplayer.Display( tools::translate( "FireResultListView", "Date" ), result->time_ );
    rootDisplayer.Display( tools::translate( "FireResultListView", "Target" ), result->target_ );
    displayer.Display( tools::translate( "FireResultListView", "Equipments" ), tools::translate( "FireResultListView", " (avail, unavail, repairable):" ) );
    displayer.Display( tools::translate( "FireResultListView", "Troops" ), tools::translate( "FireResultListView", " (officer, warrant-off., private)" ) );

    Q3ListViewItem* equipments = item->firstChild();
    DeleteTail(
        DisplayList( result->CreateIterator(), equipments )
    );

    Q3ListViewItem* humans = equipments->nextSibling();
    DeleteTail(
        DisplayList( &* result->casualties_, result->casualties_ + eNbrHumanWound, humans )
    );
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::Display
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void FireResultListView::Display( const Equipment& equipment, Displayer_ABC& displayer, ValuedListItem* )
{
    displayer.Display( tools::translate( "FireResultListView", "Target" ), equipment.type_ );
    displayer.Item( tools::translate( "FireResultListView", "Attrition" ) ).Start( equipment.available_ )
                    .Add( " / " ).Add( equipment.unavailable_ )
                    .Add( " / " ).Add( equipment.repairable_ + equipment.onSiteFixable_ ).End();
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::Display
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void FireResultListView::Display( const Casualties& casualties, Displayer_ABC& displayer, ValuedListItem* )
{
    displayer.Display( tools::translate( "FireResultListView", "Target" ), casualties.wound_ );
    displayer.Item( tools::translate( "FireResultListView", "Attrition" ) ).Start( casualties.officers_ )
                    .Add( " / " ).Add( casualties.subOfficers_ )
                    .Add( " / " ).Add( casualties.troopers_ ).End();
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::NotifySelected
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
void FireResultListView::NotifySelected( const kernel::Entity_ABC* element )
{
    if( element != selected_ )
    {
        selected_ = element;
        const Explosions* results = selected_ ? selected_->Retrieve< Explosions >() : 0;
        if( results )
            NotifyUpdated( *results );
        else
            clear();
    }
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::NotifyUpdated
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
void FireResultListView::NotifyUpdated( const Explosions& results )
{
    if( selected_ && selected_->Retrieve< Explosions >() == &results )
    {
        ValuedListItem* item = DisplayList( results.agentExplosions_.begin(), results.agentExplosions_.end() );
                        item = DisplayList( results.populationExplosions_.begin(), results.populationExplosions_.end(), this, item );
        DeleteTail( item );
    }
}
