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
    OnClearSelection();

    pNameLabel_  ->setText( population.GetName().c_str() );
    pLivingLabel_->setText( QString::number( population.GetLivingHumans() ) );
    pDeadLabel_  ->setText( QString::number( population.GetDeadHumans  () ) );

    const MOS_Population::T_ConcentrationMap& concentrations = population.GetConcentrations();
    for( MOS_Population::CIT_ConcentrationMap it = concentrations.begin(); it != concentrations.end(); ++it )
    {
        new MT_ValuedListViewItem< MOS_PopulationConcentration*, eConcentration >( it->second, pPartsListView_
            , QString( it->second->GetName().c_str() )
            , QString::number( it->second->GetLivingHumans() )
            , QString::number( it->second->GetDeadHumans() ) 
            , QString( it->second->GetStringAttitude().c_str() )
            , QString::number( it->second->GetDensity() ) );
        if( it->second == selectedItem_.pPopulationConcentration_ )
        {
            disconnect( pPartsListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
            pPartsListView_->setSelected( pPartsListView_->lastItem(), true );
            connect( pPartsListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
        }
    }

    const MOS_Population::T_FlowMap& flows = population.GetFlows();
    for( MOS_Population::CIT_FlowMap it = flows.begin(); it != flows.end(); ++it )
    {
        new MT_ValuedListViewItem< MOS_PopulationFlow*, eFlow >( it->second, pPartsListView_
            , QString( it->second->GetName().c_str() )
            , QString::number( it->second->GetLivingHumans() )
            , QString::number( it->second->GetDeadHumans() ) 
            , QString( it->second->GetStringAttitude().c_str() )
            , QString::number( it->second->GetDensity() ) );
        if( it->second == selectedItem_.pPopulationFlow_ )
        {
            disconnect( pPartsListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
            pPartsListView_->setSelected( pPartsListView_->lastItem(), true );
            connect( pPartsListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
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

    if( !pItem )
        return;

    if( pItem->rtti() == eConcentration )
    {
        MT_ValuedListViewItem< MOS_PopulationConcentration*, eConcentration >* pConcentrationItem = static_cast< MT_ValuedListViewItem< MOS_PopulationConcentration*, eConcentration >* >( pItem );
        selectedElement.pPopulationConcentration_ = pConcentrationItem->GetValue();
    }
    else if( pItem->rtti() == eFlow )
    {
        MT_ValuedListViewItem< MOS_PopulationFlow*, eFlow >* pFlowItem = static_cast< MT_ValuedListViewItem< MOS_PopulationFlow*, eFlow >* >( pItem );
        selectedElement.pPopulationFlow_ = pFlowItem->GetValue();
    }
    else
    {
        assert( false );
    }
    disconnect( pPartsListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
    emit ElementSelected( selectedElement );
    connect( pPartsListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
}
