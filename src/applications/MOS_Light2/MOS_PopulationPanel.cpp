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

#include "MOS_Population.h"

// -----------------------------------------------------------------------------
// Name: MOS_PopulationPanel constructor
/** @param  pParent 
*/
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
MOS_PopulationPanel::MOS_PopulationPanel(  QWidget* pParent )
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

    new QLabel( tr( "Attitude:" ), pInfoGroupBox );
    pAttitudeLabel_ = new QLabel( pInfoGroupBox );

    pPartsListView_ = new QListView( pInfoGroupBox );
    pPartsListView_->addColumn( tr( "Morceau" ) );
    pPartsListView_->addColumn( tr( "Hommes vivants" ) );
    pPartsListView_->addColumn( tr( "Hommes morts" ) );
    pPartsListView_->addColumn( tr( "attitude" ) );

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
    pAttitudeLabel_->setText( "" );
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationPanel::OnUpdate
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
void MOS_PopulationPanel::OnUpdate()
{
    if( selectedItem_.pPopulation_ != 0 )
        OnPopulationUpdated( *selectedItem_.pPopulation_ );
    else
        OnClearSelection();
    
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationPanel::OnPopulationUpdated
/** @param  population 
*/
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
void MOS_PopulationPanel::OnPopulationUpdated( MOS_Population& population )
{
    if( ! ShouldDisplay( population ) )
        return;

    pNameLabel_->setText( (population.GetName()).c_str() );

    pLivingLabel_->setText( QString( "%1" ).arg( population.GetLivingHumans() ) );

    pDeadLabel_->setText( QString( "%1" ).arg( population.GetDeadHumans() )  );

    pAttitudeLabel_->setText( "" );

    pPartsListView_->clear();
    for ( MOS_Population::iterator it = population.begin(); it != population.end(); ++it )
        new MT_ValuedListViewItem<MOS_PopulationPart_ABC*>( (*it), pPartsListView_
            , QString( (*it)->GetName().c_str() )
            , QString( "%1" ).arg( (*it)->GetLivingHumans() )
            , QString( "%1" ).arg( (*it)->GetDeadHumans() ) 
            , QString( (*it)->GetStringAttitude().c_str() ) );


}