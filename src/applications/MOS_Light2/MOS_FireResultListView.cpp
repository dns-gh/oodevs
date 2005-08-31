// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-30 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_FireResultListView.h"
#include "moc_MOS_FireResultListView.cpp"

#include "MOS_App.h"
#include "MOS_AgentManager.h"
#include "MOS_Agent.h"
#include "MOS_FireResult.h"

#ifndef MOS_USE_INLINE
#   include "MOS_FireResultListView.inl"
#endif

// -----------------------------------------------------------------------------
// Name: MOS_FireResultListView constructor
// Created: SBO 2005-08-30
// -----------------------------------------------------------------------------
MOS_FireResultListView::MOS_FireResultListView( QWidget* pParent )
    : QListView  ( pParent )
    , pAgent_    ( 0 )
{
    this->addColumn( tr( "Cible" ) );

    connect( &MOS_App::GetApp(), SIGNAL( AgentConflictEnded( MOS_Agent& ) ), this, SLOT( OnFireResultCreated( MOS_Agent& ) ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_FireResultListView destructor
// Created: SBO 2005-08-30
// -----------------------------------------------------------------------------
MOS_FireResultListView::~MOS_FireResultListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_FireResultListView::SetAgent
// Created: SBO 2005-08-30
// -----------------------------------------------------------------------------
void MOS_FireResultListView::SetAgent( MOS_Agent* pAgent )
{
    if( pAgent_ == pAgent )
        return;

    pAgent_ = pAgent;
    if( pAgent_ == 0 )
        return;

    OnFireResultCreated( *pAgent_ );
}

// -----------------------------------------------------------------------------
// Name: MOS_FireResultListView::OnFireResultCreated
// Created: SBO 2005-08-31
// -----------------------------------------------------------------------------
void MOS_FireResultListView::OnFireResultCreated( MOS_Agent& agent )
{
    clear();

    MOS_Agent::T_FireResults& fireResults = agent.fireResults_;
    for( MOS_Agent::CIT_FireResults it = fireResults.begin(); it != fireResults.end(); ++it )
    {
        QListViewItem* pParentItem = new QListViewItem( this );
        pParentItem->setText( 0, ( *it )->GetTarget().GetName().c_str() );
        QListViewItem* pEquiItem = new QListViewItem( pParentItem );
        pEquiItem->setText( 0, tr( "Equipements (dispo, indispo, réparable)" ) );
        for( MOS_FireResult::CIT_FireResultEquipments itEqui = ( *it )->GetEquipments().begin(); itEqui != ( *it )->GetEquipments().end(); ++itEqui )
        {
            QListViewItem* pItem = new QListViewItem( pEquiItem );
            pItem->setText( 0, ( *itEqui ).c_str() );
        }
        QListViewItem* pHumanItem = new QListViewItem( pParentItem );
        pHumanItem->setText( 0, tr( "Humains (officier, ss-officier, mdr)" ) );

        MOS_FireResult::T_FireResultHuman& off   = *( *it )->GetHumans().find( 0 )->second;
        MOS_FireResult::T_FireResultHuman& ssoff = *( *it )->GetHumans().find( 1 )->second;
        MOS_FireResult::T_FireResultHuman& mdr   = *( *it )->GetHumans().find( 2 )->second;
        
        {
            QListViewItem* pItem = new QListViewItem( pHumanItem );
            std::stringstream ss;
            ss  << "blesses urgence 1 - [" << off.nBlessesUrgence1 << " " << ssoff.nBlessesUrgence1 << " " << mdr.nBlessesUrgence1 << "]";
            pItem->setText( 0, ss.str().c_str() );
        }

        {
            QListViewItem* pItem = new QListViewItem( pHumanItem );
            std::stringstream ss;
            ss  << "blesses urgence 2 - [" << off.nBlessesUrgence2 << " " << ssoff.nBlessesUrgence2 << " " << mdr.nBlessesUrgence2 << "]";
            pItem->setText( 0, ss.str().c_str() );
        }

        {
            QListViewItem* pItem = new QListViewItem( pHumanItem );
            std::stringstream ss;
            ss  << "blesses urgence 2 - [" << off.nBlessesUrgence3 << " " << ssoff.nBlessesUrgence3 << " " << mdr.nBlessesUrgence3 << "]";
            pItem->setText( 0, ss.str().c_str() );
        }

        {
            QListViewItem* pItem = new QListViewItem( pHumanItem );
            std::stringstream ss;
            ss  << "blesses urgence extreme - [" << off.nBlessesUrgenceExtreme << " " << ssoff.nBlessesUrgenceExtreme << " " << mdr.nBlessesUrgenceExtreme << "]";
            pItem->setText( 0, ss.str().c_str() );
        }

        {
            QListViewItem* pItem = new QListViewItem( pHumanItem );
            std::stringstream ss;
            ss  << "morts - [" << off.nMorts << " " << ssoff.nMorts << " " << mdr.nMorts << "]";
            pItem->setText( 0, ss.str().c_str() );
        }

        {
            QListViewItem* pItem = new QListViewItem( pHumanItem );
            std::stringstream ss;
            ss  << "non blesses - [" << off.nNonBlesses << " " << ssoff.nNonBlesses << " " << mdr.nNonBlesses << "]";
            pItem->setText( 0, ss.str().c_str() );
        }
    }    
}
