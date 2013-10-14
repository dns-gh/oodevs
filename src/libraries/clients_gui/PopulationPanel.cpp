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
#include "LabelDisplayer.h"
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
    QVBoxLayout* layout = new QVBoxLayout( this );
    display_ = new DisplayBuilder( this, factory );
    display_->AddGroup( tools::translate( "gui::PopulationPanel", "Information" ) )
                .AddLabel( "name", tools::findTranslation( "gui::PopulationPanel", "Name:" ), true )
                .AddLabel( "healthy", tools::findTranslation( "gui::PopulationPanel", "Healthy people:" ) )
                .AddLabel( "wounded", tools::findTranslation( "gui::PopulationPanel", "Wounded people:" ) )
                .AddLabel( "contaminated", tools::findTranslation( "gui::PopulationPanel", "Contaminated people:" ) )
                .AddLabel( "dead", tools::findTranslation( "gui::PopulationPanel", "Dead people:" ) );

    pPartsListView_ = new QTreeView();
    pPartsListView_->setRootIsDecorated( false );
    pPartsListView_->setModel( &pPartsModel_ );
    pPartsModel_.setColumnCount( 7 );
    pPartsListView_->setEditTriggers( 0 );
    QStringList list;
    list.append( tools::translate( "gui::PopulationPanel", "Chunks" ) );
    list.append( tools::translate( "gui::PopulationPanel", "Healthy" ) );
    list.append( tools::translate( "gui::PopulationPanel", "Wounded" ) );
    list.append( tools::translate( "gui::PopulationPanel", "Contaminated" ) );
    list.append( tools::translate( "gui::PopulationPanel", "Dead" ) );
    list.append( tools::translate( "gui::PopulationPanel", "Mood" ) );
    list.append( tools::translate( "gui::PopulationPanel", "Alive density" ) );
    pPartsModel_.setHorizontalHeaderLabels( list );
    layout->addWidget( pPartsListView_ );

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
    const tools::Resolver< PopulationConcentration_ABC >& concentrations = static_cast< const tools::Resolver< PopulationConcentration_ABC >& >( population );
    const tools::Resolver< PopulationFlow_ABC >& flows = static_cast< const tools::Resolver< PopulationFlow_ABC >& >( population );
    ResizeModelOnNewContent( &pPartsModel_, pPartsListView_->selectionModel(), concentrations.Count() + flows.Count() );

    //concentrations
    tools::Iterator< const PopulationConcentration_ABC& > iteratorConcentration = concentrations.CreateIterator();
    int row = 0;
    while( iteratorConcentration.HasMoreElements() )
    {
        AddPopulationPartInfo( row, iteratorConcentration.NextElement() );
        ++row;
    }
        
    //flows
    tools::Iterator< const PopulationFlow_ABC& > iteratorFlow = flows.CreateIterator();
    while( iteratorFlow.HasMoreElements() )
    {
        AddPopulationPartInfo( row, iteratorFlow.NextElement() );
        ++row;
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel::GetPopulationPartInfo
// Created: NPT 2012-10-26
// -----------------------------------------------------------------------------
void PopulationPanel::AddPopulationPartInfo( int row, const PopulationPart_ABC& part )
{
    unsigned int totalHumans = part.GetHealthyHumans() + part.GetWoundedHumans() + part.GetContaminatedHumans() + part.GetDeadHumans();
    float aliveDensity = ( totalHumans == 0 ) ? 0 : part.GetDensity() * ( part.GetHealthyHumans() + part.GetWoundedHumans() + part.GetContaminatedHumans() ) / totalHumans;
    pPartsModel_.item( row, 0 )->setText( part.GetName() );
    pPartsModel_.item( row, 1 )->setText( QString::number( part.GetHealthyHumans() ) );
    pPartsModel_.item( row, 2 )->setText( QString::number( part.GetWoundedHumans() ) );
    pPartsModel_.item( row, 3 )->setText( QString::number( part.GetContaminatedHumans() ) );
    pPartsModel_.item( row, 4 )->setText( QString::number( part.GetDeadHumans() ) );
    pPartsModel_.item( row, 5 )->setText( part.GetAttitude() );
    pPartsModel_.item( row, 6 )->setText( QString::number( aliveDensity ) );
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
