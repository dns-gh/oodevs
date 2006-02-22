// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "PopulationPanel.h"

#include "Population.h"
#include "PopulationConcentration.h"
#include "PopulationFlow.h"
#include "Displayer.h"
#include "DisplayGroup.h"
#include "DisplayItem.h"
#include "Controller.h"
#include "ActionController.h"
#include "ValuedListItem.h"
#include "ListView.h"

// -----------------------------------------------------------------------------
// Name: PopulationPanel constructor
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
PopulationPanel::PopulationPanel( InfoPanel* pParent, Controller& controller, ActionController& actionController )
    : InfoPanel_ABC ( pParent, tr( "Population" ) )
    , selected_      ( 0 )
{
    display_ = new Displayer( this );
    display_->AddGroup( "Informations" )
                .AddItem( "Nom:", true )
                .AddItem( "Nombre de personnes vivantes:" )
                .AddItem( "Morts:" );

    pPartsListView_ = new ListView< PopulationPanel >( this, *this );
    pPartsListView_->addColumn( tr( "Morceau" ) );
    pPartsListView_->addColumn( tr( "Hommes vivants" ) );
    pPartsListView_->addColumn( tr( "Hommes morts" ) );
    pPartsListView_->addColumn( tr( "Attitude" ) );
    pPartsListView_->addColumn( tr( "Densité vivants" ) );

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
// Name: PopulationPanel::NotifySelected
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void PopulationPanel::NotifySelected( const Population* popu )
{
    if( selected_ != popu || ! popu )
    {
        selected_ = popu;
        if( selected_ )
        {
            Show();
            NotifyUpdated( *selected_ );
        }
        else
            Hide();
    };
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel::DisplayParts
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void PopulationPanel::DisplayParts( const Population& population )
{
    Iterator< const PopulationPart_ABC& > it = population.CreateIterator();
    ValuedListItem* item = pPartsListView_->Display( it, pPartsListView_, (ValuedListItem*)pPartsListView_->firstChild() );
    pPartsListView_->DeleteTail( item );
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel::DisplayPart
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void PopulationPanel::Display( const PopulationPart_ABC& part, ValuedListItem* at )
{
    at->SetValue( &part );
    at->setText( 0, part.GetName().c_str() );
    at->setText( 1, QString::number( part.GetLivingHumans() ) );
    at->setText( 2, QString::number( part.GetDeadHumans() ) );
    at->setText( 3, part.GetAttitude().c_str() );
    at->setText( 4, QString::number( part.GetDensity() ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void PopulationPanel::NotifyUpdated( const Population& p )
{
    if( ! isVisible() || selected_ != &p )
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
