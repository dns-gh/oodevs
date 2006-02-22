// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "PopulationPanel.h"
#include "moc_PopulationPanel.cpp"

#include "App.h"
#include "MainWindow.h"
#include "Population.h"
#include "PopulationConcentration.h"
#include "PopulationFlow.h"
#include "Display.h"
#include "DisplayGroup.h"
#include "DisplayItem.h"

// -----------------------------------------------------------------------------
// Name: PopulationPanel constructor
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
PopulationPanel::PopulationPanel( QWidget* pParent )
    : InfoPanel_ABC ( pParent )
    , pPopulation_      ( 0 )
{
    display_ = new Display( this );
    display_->AddGroup( "Informations" )
                .AddItem( "Nom:", true )
                .AddItem( "Nombre de personnes vivantes:" )
                .AddItem( "Morts:" )
                .AddItem( "Domination:" );
                
    pPartsListView_ = new QListView( this );
    pPartsListView_->addColumn( tr( "Morceau" ) );
    pPartsListView_->addColumn( tr( "Hommes vivants" ) );
    pPartsListView_->addColumn( tr( "Hommes morts" ) );
    pPartsListView_->addColumn( tr( "Attitude" ) );
    pPartsListView_->addColumn( tr( "Densité vivants" ) );

    connect( this, SIGNAL( ElementSelected( SelectedElement& ) ), &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ) );
    connect( pPartsListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
    connect( &App::GetApp(), SIGNAL( PopulationConcentrationCreated( const PopulationConcentration& ) ), this, SLOT( OnPopulationConcentrationCreated( const PopulationConcentration& ) ) );
    connect( &App::GetApp(), SIGNAL( PopulationConcentrationUpdated( const PopulationConcentration& ) ), this, SLOT( OnPopulationConcentrationUpdated( const PopulationConcentration& ) ) );
    connect( &App::GetApp(), SIGNAL( PopulationConcentrationDeleted( const PopulationConcentration& ) ), this, SLOT( OnPopulationConcentrationDeleted( const PopulationConcentration& ) ) );
    connect( &App::GetApp(), SIGNAL( PopulationFlowCreated( const PopulationFlow& ) )                  , this, SLOT( OnPopulationFlowCreated( const PopulationFlow& ) ) );
    connect( &App::GetApp(), SIGNAL( PopulationFlowUpdated( const PopulationFlow& ) )                  , this, SLOT( OnPopulationFlowUpdated( const PopulationFlow& ) ) );
    connect( &App::GetApp(), SIGNAL( PopulationFlowDeleted( const PopulationFlow& ) )                  , this, SLOT( OnPopulationFlowDeleted( const PopulationFlow& ) ) );

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
// Name: PopulationPanel::OnClearSelection
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
void PopulationPanel::OnClearSelection()
{
    display_->Clear();
    pPartsListView_->clear();
    pPopulation_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel::OnUpdate
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
void PopulationPanel::OnUpdate()
{
    if( selectedItem_.pPopulation_ != 0 )
        OnPopulationUpdated( *selectedItem_.pPopulation_ );
    else if( selectedItem_.pPopulationConcentration_ != 0 )
        OnPopulationUpdated( selectedItem_.pPopulationConcentration_->GetPopulation() );
    else if( selectedItem_.pPopulationFlow_ != 0 )
        OnPopulationUpdated( selectedItem_.pPopulationFlow_->GetPopulation() );
    else
        OnClearSelection();
    
}

namespace
{
    // $$$$ AGE 2006-02-09: HIé
    template< typename T >
    QString IfSet( const T& value, const QString& message )
    {
        return value.IsSet() ? message : "";
    }
    template< typename T >
    QString IfSet( const T& value, const std::string& message )
    {
        return value.IsSet() ? message.c_str() : "";
    }
    template< typename T >
    QString IfSet( const T& value )
    {
        return value.IsSet() ? QString::number( value ) : "";
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel::OnPopulationUpdated
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
void PopulationPanel::OnPopulationUpdated( const Population& population )
{
    display_->Group( "Informations" )
                .Display( "Nom:", population.GetName() )
                .Display( "Nombre de personnes vivantes:", population.GetLivingHumans() )
                .Display( "Morts:",  population.GetDeadHumans() )
                .Display( "Domination:", IfSet( population.nDomination_, QString::number( population.nDomination_ ) + "%" ) ) ;

    if( pPopulation_ == &population )
    {
        UpdateSelected();
        return;
    }

    pPartsListView_->clear();
    pPopulation_ = &population;

    const Population::T_ConcentrationMap& concentrations = population.GetConcentrations();
    for( Population::CIT_ConcentrationMap it = concentrations.begin(); it != concentrations.end(); ++it )
    {
        new T_PopulationConcentrationItem( it->second, pPartsListView_
            , QString( it->second->GetName().c_str() )
            , QString::number( it->second->GetLivingHumans() )
            , QString::number( it->second->GetDeadHumans() ) 
            , QString( it->second->GetStringAttitude().c_str() )
            , QString::number( it->second->GetDensity() ) );
    }

    const Population::T_FlowMap& flows = population.GetFlows();
    for( Population::CIT_FlowMap it = flows.begin(); it != flows.end(); ++it )
    {
        new T_PopulationFlowItem( it->second, pPartsListView_
            , QString( it->second->GetName().c_str() )
            , QString::number( it->second->GetLivingHumans() )
            , QString::number( it->second->GetDeadHumans() ) 
            , QString( it->second->GetStringAttitude().c_str() )
            , QString::number( it->second->GetDensity() ) );
    }
    UpdateSelected();
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel::UpdateSelected
// Created: SBO 2006-01-24
// -----------------------------------------------------------------------------
void PopulationPanel::UpdateSelected()
{
    for( QListViewItemIterator it( pPartsListView_ ); it.current(); ++it )
    {
        T_PopulationConcentrationItem* pConcentrationItem = static_cast< T_PopulationConcentrationItem* >( it.current() );
        T_PopulationFlowItem*                   pFlowItem = static_cast< T_PopulationFlowItem* >( it.current() );
        if( pConcentrationItem && pConcentrationItem->GetValue() == selectedItem_.pPopulationConcentration_ )
        {
            disconnect( pPartsListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
            pPartsListView_->setSelected( it.current(), true );
            connect( pPartsListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
            return;
        }
        else if( pFlowItem && pFlowItem->GetValue() == selectedItem_.pPopulationFlow_ )
        {
            disconnect( pPartsListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
            pPartsListView_->setSelected( it.current(), true );
            connect( pPartsListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );            
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel::OnSelectionChange
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
void PopulationPanel::OnSelectionChange( QListViewItem* pItem )
{
    SelectedElement selectedElement;

    if( pItem )
    {
        if( pItem->rtti() == eConcentration )
        {
            T_PopulationConcentrationItem* pConcentrationItem = static_cast< T_PopulationConcentrationItem* >( pItem );
            assert( pConcentrationItem );
            selectedElement.pPopulationConcentration_ = const_cast< PopulationConcentration* >( pConcentrationItem->GetValue() );
        }
        else if( pItem->rtti() == eFlow )
        {
            T_PopulationFlowItem* pFlowItem = static_cast< T_PopulationFlowItem* >( pItem );
            assert( pFlowItem );
            selectedElement.pPopulationFlow_ = const_cast< PopulationFlow* >( pFlowItem->GetValue() );
        }
        else
        {
            assert( false );
        }
    }
    disconnect( pPartsListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
    emit ElementSelected( selectedElement );
    connect( pPartsListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel::OnPopulationConcentrationCreated
// Created: SBO 2006-01-24
// -----------------------------------------------------------------------------
void PopulationPanel::OnPopulationConcentrationCreated( const PopulationConcentration& concentration )
{
    if( &concentration.GetPopulation() == pPopulation_ )
    {
        new T_PopulationConcentrationItem( &concentration, pPartsListView_
                , QString( concentration.GetName().c_str() )
                , QString::number( concentration.GetLivingHumans() )
                , QString::number( concentration.GetDeadHumans() )
                , QString( concentration.GetStringAttitude().c_str() )
                , QString::number( concentration.GetDensity() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel::OnPopulationFlowCreated
// Created: SBO 2006-01-24
// -----------------------------------------------------------------------------
void PopulationPanel::OnPopulationFlowCreated( const PopulationFlow& flow )
{
    if( &flow.GetPopulation() == pPopulation_ )
    {
        new T_PopulationFlowItem( &flow, pPartsListView_
                , QString( flow.GetName().c_str() )
                , QString::number( flow.GetLivingHumans() )
                , QString::number( flow.GetDeadHumans() ) 
                , QString( flow.GetStringAttitude().c_str() )
                , QString::number( flow.GetDensity() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel::OnPopulationConcentrationUpdated
// Created: SBO 2006-01-24
// -----------------------------------------------------------------------------
void PopulationPanel::OnPopulationConcentrationUpdated( const PopulationConcentration& concentration )
{
    if( &concentration.GetPopulation() != pPopulation_ )
        return;
    for( QListViewItemIterator it( pPartsListView_ ); it.current(); ++it )
    {
        T_PopulationConcentrationItem* pConcentrationItem = static_cast< T_PopulationConcentrationItem* >( it.current() );
        if( pConcentrationItem && pConcentrationItem->GetValue() == &concentration )
        {
            pConcentrationItem->setText( 0, QString( concentration.GetName().c_str() ) );
            pConcentrationItem->setText( 1, QString::number( concentration.GetLivingHumans() ) );
            pConcentrationItem->setText( 2, QString::number( concentration.GetDeadHumans() ) );
            pConcentrationItem->setText( 3, QString( concentration.GetStringAttitude().c_str() ) );
            pConcentrationItem->setText( 4, QString::number( concentration.GetDensity() ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel::OnPopulationFlowUpdated
// Created: SBO 2006-01-24
// -----------------------------------------------------------------------------
void PopulationPanel::OnPopulationFlowUpdated( const PopulationFlow& flow )
{
    if( &flow.GetPopulation() != pPopulation_ )
        return;
    for( QListViewItemIterator it( pPartsListView_ ); it.current(); ++it )
    {
        T_PopulationFlowItem* pFlowItem = static_cast< T_PopulationFlowItem* >( it.current() );
        if( pFlowItem && pFlowItem->GetValue() == &flow )
        {
            pFlowItem->setText( 0, QString( flow.GetName().c_str() ) );
            pFlowItem->setText( 1, QString::number( flow.GetLivingHumans() ) );
            pFlowItem->setText( 2, QString::number( flow.GetDeadHumans() ) );
            pFlowItem->setText( 3, QString( flow.GetStringAttitude().c_str() ) );
            pFlowItem->setText( 4, QString::number( flow.GetDensity() ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel::OnPopulationConcentrationDeleted
// Created: SBO 2006-01-24
// -----------------------------------------------------------------------------
void PopulationPanel::OnPopulationConcentrationDeleted( const PopulationConcentration& concentration )
{
    if( &concentration.GetPopulation() != pPopulation_ )
        return;
    for( QListViewItemIterator it( pPartsListView_ ); it.current(); ++it )
    {
        T_PopulationConcentrationItem* pConcentrationItem = static_cast< T_PopulationConcentrationItem* >( it.current() );
        if( pConcentrationItem && pConcentrationItem->GetValue() == &concentration )
        {
            pPartsListView_->removeItem( it.current() );
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationPanel::OnPopulationFlowDeleted
// Created: SBO 2006-01-24
// -----------------------------------------------------------------------------
void PopulationPanel::OnPopulationFlowDeleted( const PopulationFlow& flow )
{
    if( &flow.GetPopulation() != pPopulation_ )
        return;
    for( QListViewItemIterator it( pPartsListView_ ); it.current(); ++it )
    {
        T_PopulationFlowItem* pFlowItem = static_cast< T_PopulationFlowItem* >( it.current() );
        if( pFlowItem && pFlowItem->GetValue() == &flow )
        {
            pPartsListView_->removeItem( it.current() );
            return;
        }
    }
}
