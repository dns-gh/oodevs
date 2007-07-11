// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PopulationPanel.h"

#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/PopulationConcentration_ABC.h"
#include "clients_kernel/PopulationFlow_ABC.h"
#include "clients_kernel/Controllers.h"
#include "DisplayBuilder.h"
#include "GroupDisplayer.h"
#include "LabelDisplayer.h"
#include "ValuedListItem.h"
#include "ListDisplayer.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: PopulationPanel constructor
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
PopulationPanel::PopulationPanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, ItemFactory_ABC& factory )
    : InfoPanel_ABC ( parent, panel, tr( "Population state" ) )
    , controllers_  ( controllers )
    , selected_     ( controllers )
{
    display_ = new DisplayBuilder( this, factory );
    display_->AddGroup( tr( "Information" ) )
                .AddLabel( tr( "Name:" ), true )
                .AddLabel( tr( "Alive people:" ) )
                .AddLabel( tr( "Dead people:" ) );

    pPartsListView_ = new ListDisplayer< PopulationPanel >( this, *this, factory );
    pPartsListView_->AddColumn( tr( "Chunks" ) )
                    .AddColumn( tr( "Alive" ) )
                    .AddColumn( tr( "Dead" ) )
                    .AddColumn( tr( "Mood" ) )
                    .AddColumn( tr( "Alive density" ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel destructor
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
PopulationPanel::~PopulationPanel()
{
    controllers_.Unregister( *this );
    delete display_;
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel::showEvent
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationPanel::showEvent( QShowEvent* )
{
    const Population_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel::NotifySelected
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void PopulationPanel::NotifySelected( const Population_ABC* popu )
{
    if( selected_ != popu || ! popu )
    {
        selected_ = popu;
        if( selected_ ) {
            Show();
            NotifyUpdated( *selected_ );
        }
        else
            Hide();
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel::DisplayParts
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void PopulationPanel::DisplayParts( const Population_ABC& population )
{
    ValuedListItem* item = pPartsListView_->DisplayList( population.Resolver< PopulationConcentration_ABC >::CreateIterator() );
    item = pPartsListView_->DisplayList( population.Resolver< PopulationFlow_ABC >::CreateIterator(), pPartsListView_, item );
    pPartsListView_->DeleteTail( item );
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel::DisplayPart
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void PopulationPanel::Display( const PopulationPart_ABC& part, Displayer_ABC& displayer, ValuedListItem* )
{
    displayer.Display( tr( "Chunks" ), part.GetName() )
             .Display( tr( "Alive" ), part.GetLivingHumans() )
             .Display( tr( "Dead" ), part.GetDeadHumans() )
             .Display( tr( "Mood" ), part.GetAttitude() )
             .Display( tr( "Alive density" ), part.GetDensity() );
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void PopulationPanel::NotifyUpdated( const Population_ABC& p )
{
    if( ! IsVisible() || selected_ != &p )
        return;
    display_->Group( tr( "Information" ) )
                .Display( tr( "Name:" ), p )
                .Display( tr( "Alive people:" ), p.GetLivingHumans() )
                .Display( tr( "Dead people:" ),  p.GetDeadHumans() );
    DisplayParts( p );
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel::NotifyDeleted
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void PopulationPanel::NotifyDeleted( const Population_ABC& popu )
{
    if( selected_ == &popu )
        NotifySelected( 0 );
}
