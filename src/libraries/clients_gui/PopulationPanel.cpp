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
    : InfoPanel_ABC ( parent, panel, tr( "Etat population" ) )
    , controllers_  ( controllers )
    , selected_     ( controllers )
{
    display_ = new DisplayBuilder( this, factory );
    display_->AddGroup( tr( "Informations" ) )
                .AddLabel( tr( "Nom:" ), true )
                .AddLabel( tr( "Nombre de personnes vivantes:" ) )
                .AddLabel( tr( "Morts:" ) );

    pPartsListView_ = new ListDisplayer< PopulationPanel >( this, *this, factory );
    pPartsListView_->AddColumn( tr( "Morceau" ) )
                    .AddColumn( tr( "Hommes vivants" ) )
                    .AddColumn( tr( "Hommes morts" ) )
                    .AddColumn( tr( "Attitude" ) )
                    .AddColumn( tr( "Densité vivants" ) );

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
    displayer.Display( tr( "Morceau" ), part.GetName().c_str() )
             .Display( tr( "Hommes vivants" ), part.GetLivingHumans() )
             .Display( tr( "Hommes morts" ), part.GetDeadHumans() )
             .Display( tr( "Attitude" ), part.GetAttitude() )
             .Display( tr( "Densité vivants" ), part.GetDensity() );
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void PopulationPanel::NotifyUpdated( const Population_ABC& p )
{
    if( ! IsVisible() || selected_ != &p )
        return;
    display_->Group( tr( "Informations" ) )
                .Display( tr( "Nom:" ), p )
                .Display( tr( "Nombre de personnes vivantes:" ), p.GetLivingHumans() )
                .Display( tr( "Morts:" ),  p.GetDeadHumans() );
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
