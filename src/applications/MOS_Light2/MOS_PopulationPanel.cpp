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

#include "MOS_Light2_pch.h"
#include "MOS_PopulationPanel.h"
#include "moc_MOS_PopulationPanel.cpp"

#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_Population.h"
#include "MOS_PopulationConcentration.h"
#include "MOS_PopulationFlow.h"

// -----------------------------------------------------------------------------
// Name: MOS_PopulationPanel constructor
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
MOS_PopulationPanel::MOS_PopulationPanel( QWidget* pParent )
    : MOS_InfoPanel_ABC ( pParent )
    , pPopulation_      ( 0 )
{
    QFont boldFont = this->font();
    boldFont.setBold( true );

    // Info groupbox
    QGroupBox* pInfoGroupBox = new QGroupBox( 2, Qt::Horizontal, tr( "Info" ), this );

    new QLabel( tr( "Nom:" ), pInfoGroupBox );
    pNameLabel_ = new QLabel( pInfoGroupBox );
    pNameLabel_->setFont( boldFont );

    new QLabel( tr( "Nombre de personnes vivantes:" ), pInfoGroupBox );
    pLivingLabel_ = new QLabel( pInfoGroupBox );

    new QLabel( tr( "Morts:" ), pInfoGroupBox );
    pDeadLabel_ = new QLabel( pInfoGroupBox );

    pPartsListView_ = new QListView( this );
    pPartsListView_->addColumn( tr( "Morceau" ) );
    pPartsListView_->addColumn( tr( "Hommes vivants" ) );
    pPartsListView_->addColumn( tr( "Hommes morts" ) );
    pPartsListView_->addColumn( tr( "Attitude" ) );
    pPartsListView_->addColumn( tr( "Densité vivants" ) );

    connect( this, SIGNAL( ElementSelected( MOS_SelectedElement& ) ), &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ) );
    connect( pPartsListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( PopulationConcentrationCreated( const MOS_PopulationConcentration& ) ), this, SLOT( OnPopulationConcentrationCreated( const MOS_PopulationConcentration& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( PopulationConcentrationUpdated( const MOS_PopulationConcentration& ) ), this, SLOT( OnPopulationConcentrationUpdated( const MOS_PopulationConcentration& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( PopulationConcentrationDeleted( const MOS_PopulationConcentration& ) ), this, SLOT( OnPopulationConcentrationDeleted( const MOS_PopulationConcentration& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( PopulationFlowCreated( const MOS_PopulationFlow& ) )                  , this, SLOT( OnPopulationFlowCreated( const MOS_PopulationFlow& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( PopulationFlowUpdated( const MOS_PopulationFlow& ) )                  , this, SLOT( OnPopulationFlowUpdated( const MOS_PopulationFlow& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( PopulationFlowDeleted( const MOS_PopulationFlow& ) )                  , this, SLOT( OnPopulationFlowDeleted( const MOS_PopulationFlow& ) ) );

}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationPanel destructor
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
MOS_PopulationPanel::~MOS_PopulationPanel()
{
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationPanel::OnClearSelection
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
void MOS_PopulationPanel::OnClearSelection()
{
    pNameLabel_    ->setText( "" );
    pLivingLabel_  ->setText( "" );
    pDeadLabel_    ->setText( "" );
    pPartsListView_->clear();
    pPopulation_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationPanel::OnUpdate
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
void MOS_PopulationPanel::OnUpdate()
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

// -----------------------------------------------------------------------------
// Name: MOS_PopulationPanel::OnPopulationUpdated
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
void MOS_PopulationPanel::OnPopulationUpdated( const MOS_Population& population )
{
    pNameLabel_  ->setText( population.GetName().c_str() );
    pLivingLabel_->setText( QString::number( population.GetLivingHumans() ) );
    pDeadLabel_  ->setText( QString::number( population.GetDeadHumans  () ) );

    if( pPopulation_ == &population )
    {
        UpdateSelected();
        return;
    }

    pPartsListView_->clear();
    pPopulation_ = &population;

    const MOS_Population::T_ConcentrationMap& concentrations = population.GetConcentrations();
    for( MOS_Population::CIT_ConcentrationMap it = concentrations.begin(); it != concentrations.end(); ++it )
    {
        new T_PopulationConcentrationItem( it->second, pPartsListView_
            , QString( it->second->GetName().c_str() )
            , QString::number( it->second->GetLivingHumans() )
            , QString::number( it->second->GetDeadHumans() ) 
            , QString( it->second->GetStringAttitude().c_str() )
            , QString::number( it->second->GetDensity() ) );
    }

    const MOS_Population::T_FlowMap& flows = population.GetFlows();
    for( MOS_Population::CIT_FlowMap it = flows.begin(); it != flows.end(); ++it )
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
// Name: MOS_PopulationPanel::UpdateSelected
// Created: SBO 2006-01-24
// -----------------------------------------------------------------------------
void MOS_PopulationPanel::UpdateSelected()
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
// Name: MOS_PopulationPanel::OnSelectionChange
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
void MOS_PopulationPanel::OnSelectionChange( QListViewItem* pItem )
{
    MOS_SelectedElement selectedElement;

    if( pItem )
    {
        if( pItem->rtti() == eConcentration )
        {
            T_PopulationConcentrationItem* pConcentrationItem = static_cast< T_PopulationConcentrationItem* >( pItem );
            assert( pConcentrationItem );
            selectedElement.pPopulationConcentration_ = const_cast< MOS_PopulationConcentration* >( pConcentrationItem->GetValue() );
        }
        else if( pItem->rtti() == eFlow )
        {
            T_PopulationFlowItem* pFlowItem = static_cast< T_PopulationFlowItem* >( pItem );
            assert( pFlowItem );
            selectedElement.pPopulationFlow_ = const_cast< MOS_PopulationFlow* >( pFlowItem->GetValue() );
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
// Name: MOS_PopulationPanel::OnPopulationConcentrationCreated
// Created: SBO 2006-01-24
// -----------------------------------------------------------------------------
void MOS_PopulationPanel::OnPopulationConcentrationCreated( const MOS_PopulationConcentration& concentration )
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
// Name: MOS_PopulationPanel::OnPopulationFlowCreated
// Created: SBO 2006-01-24
// -----------------------------------------------------------------------------
void MOS_PopulationPanel::OnPopulationFlowCreated( const MOS_PopulationFlow& flow )
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
// Name: MOS_PopulationPanel::OnPopulationConcentrationUpdated
// Created: SBO 2006-01-24
// -----------------------------------------------------------------------------
void MOS_PopulationPanel::OnPopulationConcentrationUpdated( const MOS_PopulationConcentration& concentration )
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
// Name: MOS_PopulationPanel::OnPopulationFlowUpdated
// Created: SBO 2006-01-24
// -----------------------------------------------------------------------------
void MOS_PopulationPanel::OnPopulationFlowUpdated( const MOS_PopulationFlow& flow )
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
// Name: MOS_PopulationPanel::OnPopulationConcentrationDeleted
// Created: SBO 2006-01-24
// -----------------------------------------------------------------------------
void MOS_PopulationPanel::OnPopulationConcentrationDeleted( const MOS_PopulationConcentration& concentration )
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
// Name: MOS_PopulationPanel::OnPopulationFlowDeleted
// Created: SBO 2006-01-24
// -----------------------------------------------------------------------------
void MOS_PopulationPanel::OnPopulationFlowDeleted( const MOS_PopulationFlow& flow )
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
