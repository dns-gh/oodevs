// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::PopulationPanel */

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
#include "Tools.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: PopulationPanel constructor
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
PopulationPanel::PopulationPanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, ItemFactory_ABC& factory )
    : InfoPanel_ABC ( parent, panel, tools::translate( "gui::PopulationPanel", "Crowd state" ) )
    , controllers_  ( controllers )
    , selected_     ( controllers )
{
    display_ = new DisplayBuilder( this, factory );
    display_->AddGroup( tools::translate( "gui::PopulationPanel", "Information" ) )
                .AddLabel( tools::findTranslation( "gui::PopulationPanel", "Name:" ), true )
                .AddLabel( tools::findTranslation( "gui::PopulationPanel", "Healthy people:" ) )
                .AddLabel( tools::findTranslation( "gui::PopulationPanel", "Wounded people:" ) )
                .AddLabel( tools::findTranslation( "gui::PopulationPanel", "Contaminated people:" ) )
                .AddLabel( tools::findTranslation( "gui::PopulationPanel", "Dead people:" ) );

    pPartsListView_ = new ListDisplayer< PopulationPanel >( this, *this, factory );
    pPartsListView_->AddColumn( tools::translate( "gui::PopulationPanel", "Chunks" ) )
                    .AddColumn( tools::translate( "gui::PopulationPanel", "Healthy" ) )
                    .AddColumn( tools::translate( "gui::PopulationPanel", "Wounded" ) )
                    .AddColumn( tools::translate( "gui::PopulationPanel", "Contaminated" ) )
                    .AddColumn( tools::translate( "gui::PopulationPanel", "Dead" ) )
                    .AddColumn( tools::translate( "gui::PopulationPanel", "Mood" ) )
                    .AddColumn( tools::translate( "gui::PopulationPanel", "Alive density" ) );

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
    unsigned int totalHumans = part.GetHealthyHumans() + part.GetWoundedHumans() + part.GetContaminatedHumans() + part.GetDeadHumans();
    float aliveDensity = ( totalHumans == 0 ) ? 0 :
        part.GetDensity() * ( part.GetHealthyHumans() + part.GetWoundedHumans() + + part.GetContaminatedHumans() ) / totalHumans;
    displayer.Display( tools::translate( "gui::PopulationPanel", "Chunks" ), part.GetName() )
             .Display( tools::translate( "gui::PopulationPanel", "Healthy" ), part.GetHealthyHumans() )
             .Display( tools::translate( "gui::PopulationPanel", "Wounded" ), part.GetWoundedHumans() )
             .Display( tools::translate( "gui::PopulationPanel", "Contaminated" ), part.GetContaminatedHumans() )
             .Display( tools::translate( "gui::PopulationPanel", "Dead" ), part.GetDeadHumans() )
             .Display( tools::translate( "gui::PopulationPanel", "Mood" ), part.GetAttitude() )
             .Display( tools::translate( "gui::PopulationPanel", "Alive density" ), aliveDensity );
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void PopulationPanel::NotifyUpdated( const Population_ABC& p )
{
    if( ! IsVisible() || selected_ != &p )
        return;
    display_->Group( tools::translate( "gui::PopulationPanel", "Information" ) )
                .Display( tools::translate( "gui::PopulationPanel", "Name:" ), p )
                .Display( tools::translate( "gui::PopulationPanel", "Healthy people:" ), p.GetHealthyHumans() )
                .Display( tools::translate( "gui::PopulationPanel", "Wounded people:" ), p.GetWoundedHumans() )
                .Display( tools::translate( "gui::PopulationPanel", "Contaminated people:" ), p.GetContaminatedHumans() )
                .Display( tools::translate( "gui::PopulationPanel", "Dead people:" ),  p.GetDeadHumans() );
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
