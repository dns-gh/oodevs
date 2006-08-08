// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "PopulationPanel.h"

#include "astec_kernel/Population_ABC.h"
#include "astec_gaming/PopulationConcentration.h"
#include "astec_gaming/PopulationFlow.h"
#include "astec_kernel/Controllers.h"
#include "DisplayBuilder.h"
#include "GroupDisplayer.h"
#include "LabelDisplayer.h"
#include "ValuedListItem.h"
#include "ListDisplayer.h"

// -----------------------------------------------------------------------------
// Name: PopulationPanel constructor
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
PopulationPanel::PopulationPanel( InfoPanels* pParent, Controllers& controllers, ItemFactory_ABC& factory )
    : InfoPanel_ABC ( pParent, tr( "Etat population" ) )
    , controllers_  ( controllers )
    , selected_     ( controllers )
{
    display_ = new DisplayBuilder( this, factory );
    display_->AddGroup( "Informations" )
                .AddLabel( "Nom:", true )
                .AddLabel( "Nombre de personnes vivantes:" )
                .AddLabel( "Morts:" );

    pPartsListView_ = new ListDisplayer< PopulationPanel >( this, *this, factory );
    pPartsListView_->AddColumn( "Morceau" )
                    .AddColumn( "Hommes vivants" )
                    .AddColumn( "Hommes morts" )
                    .AddColumn( "Attitude" )
                    .AddColumn( "Densit� vivants" );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel destructor
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
PopulationPanel::~PopulationPanel()
{
    controllers_.Remove( *this );
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
    ValuedListItem* item = pPartsListView_->DisplayList( population.Resolver< PopulationConcentration >::CreateIterator() );
    item = pPartsListView_->DisplayList( population.Resolver< PopulationFlow >::CreateIterator(), pPartsListView_, item );
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
void PopulationPanel::NotifyUpdated( const Population_ABC& p )
{
    if( ! IsVisible() || selected_ != &p )
        return;
    display_->Group( "Informations" )
                .Display( "Nom:", p )
                .Display( "Nombre de personnes vivantes:", p.GetLivingHumans() )
                .Display( "Morts:",  p.GetDeadHumans() );
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
