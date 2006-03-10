// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "PopulationPanel.h"

#include "Population.h"
#include "PopulationConcentration.h"
#include "PopulationFlow.h"
#include "DisplayBuilder.h"
#include "GroupDisplayer.h"
#include "LabelDisplayer.h"
#include "Controller.h"
#include "ActionController.h"
#include "ValuedListItem.h"
#include "ListDisplayer.h"

// -----------------------------------------------------------------------------
// Name: PopulationPanel constructor
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
PopulationPanel::PopulationPanel( InfoPanels* pParent, Controller& controller, ActionController& actionController )
    : InfoPanel_ABC ( pParent, tr( "Population" ) )
    , selected_      ( 0 )
{
    display_ = new DisplayBuilder( this );
    display_->AddGroup( "Informations" )
                .AddLabel( "Nom:", true )
                .AddLabel( "Nombre de personnes vivantes:" )
                .AddLabel( "Morts:" );

    pPartsListView_ = new ListDisplayer< PopulationPanel >( this, *this );
    pPartsListView_->AddColumn( "Morceau" )
                    .AddColumn( "Hommes vivants" )
                    .AddColumn( "Hommes morts" )
                    .AddColumn( "Attitude" )
                    .AddColumn( "Densit� vivants" );

    controller.Register( *this );
    actionController.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel destructor
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
PopulationPanel::~PopulationPanel()
{
    delete display_;
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel::showEvent
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationPanel::showEvent( QShowEvent* )
{
    const Population* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel::NotifySelected
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void PopulationPanel::NotifySelected( const Population* popu )
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
void PopulationPanel::DisplayParts( const Population& population )
{
    ValuedListItem* item = pPartsListView_->DisplayList( population.CreateIterator() );
    pPartsListView_->DeleteTail( item );
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel::DisplayPart
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void PopulationPanel::Display( const PopulationPart_ABC& part, Displayer_ABC& displayer, ValuedListItem* )
{
    displayer.Display( "Morceau", part.GetName().c_str() )
             .Display( "Hommes vivants", part.GetLivingHumans() )
             .Display( "Hommes morts", part.GetDeadHumans() )
             .Display( "Attitude", part.GetAttitude() )
             .Display( "Densit� vivants", part.GetDensity() );
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void PopulationPanel::NotifyUpdated( const Population& p )
{
    if( ! IsVisible() || selected_ != &p )
        return;
    display_->Group( "Informations" )
                .Display( "Nom:", p.GetName() )
                .Display( "Nombre de personnes vivantes:", p.GetLivingHumans() )
                .Display( "Morts:",  p.GetDeadHumans() );
    DisplayParts( p );
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel::NotifyDeleted
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void PopulationPanel::NotifyDeleted( const Population& popu )
{
    if( selected_ == &popu )
        NotifySelected( 0 );
}
