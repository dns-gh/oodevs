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
#include "MOS_Agent_ABC.h"
#include "MOS_Object_ABC.h"
#include "MOS_FireResult.h"

// -----------------------------------------------------------------------------
// Name: MOS_FireResultListView constructor
// Created: SBO 2005-08-30
// -----------------------------------------------------------------------------
MOS_FireResultListView::MOS_FireResultListView( QWidget* pParent )
    : QListView  ( pParent )
    , pOrigin_   ( 0 )
{
    this->addColumn( tr( "Cible" ) );

    connect( &MOS_App::GetApp(), SIGNAL( ConflictEnded  ( MOS_Agent_ABC&  ) ), this, SLOT( OnConflictEnded  ( MOS_Agent_ABC&  ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( ObjectExplosion( MOS_Object_ABC& ) ), this, SLOT( OnObjectExplosion( MOS_Object_ABC& ) ) );
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
// Name: MOS_FireResultListView::SetOrigin
// Created: SBO 2005-08-30
// -----------------------------------------------------------------------------
void MOS_FireResultListView::SetOrigin( MOS_Agent_ABC* pOrigin )
{
    if( pOrigin_ == pOrigin )
        return;

    pOrigin_ = pOrigin;
    pObject_ = 0;
    if( pOrigin_ == 0 )
    {
        clear();
        return;
    }

    OnFireResultCreated( pOrigin_->GetFireResult() );
}

// -----------------------------------------------------------------------------
// Name: MOS_FireResultListView::SetObject
// Created: SBO 2005-09-07
// -----------------------------------------------------------------------------
void MOS_FireResultListView::SetObject( MOS_Object_ABC* pObject )
{
    if( pObject_ == pObject )
        return;

    pOrigin_ = 0;
    pObject_ = pObject;
    if( pObject_ == 0 )
    {
        clear();
        return;
    }

    OnFireResultCreated( pObject->explosionResults_ );
}

// -----------------------------------------------------------------------------
// Name: MOS_FireResultListView::OnFireResultCreated
// Created: SBO 2005-08-31
// -----------------------------------------------------------------------------
void MOS_FireResultListView::OnFireResultCreated( const T_FireResults& fireResults )
{
    const MOS_FireResult* pSelectedValue = 0;
          bool            bOpenState = false;
    if( selectedItem() != 0 )
    {
        pSelectedValue = ( ( MT_ValuedListViewItem< const MOS_FireResult* >* )selectedItem() )->GetValue();
        bOpenState = selectedItem()->isOpen();
    }

    clear();
    for( CIT_FireResults it = fireResults.begin(); it != fireResults.end(); ++it )
    {
        MT_ValuedListViewItem< const MOS_FireResult* >* pParentItem = new MT_ValuedListViewItem< const MOS_FireResult* >( *it, this, ( *it )->GetTarget().GetName().c_str() );
        /*
        QListViewItem* pParentItem = new QListViewItem( this );
        pParentItem->setText( 0, ( *it )->GetTarget().GetName().c_str() );
        */

        // agent => agent damages
        if( ( *it )->GetEquipments().size() > 0 || ( *it )->GetHumans().size() > 0 )
        {
            BuildEquipmentResult( **it, *pParentItem );
            BuildHumanResult    ( **it, *pParentItem );
        }
        // agent => population damages
        else
        {
            QListViewItem* pItem = new QListViewItem( pParentItem );
            pItem->setText( 0, tr( "Morts: " ) + QString::number( ( *it )->GetDead() ) );
        }
        // try to reselect previous element if possible
        if( pSelectedValue && *it == pSelectedValue )
        {
            setSelected( pParentItem, true );
            pParentItem->setOpen( bOpenState );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_FireResultListView::BuildEquipmentResult
// Created: SBO 2005-12-05
// -----------------------------------------------------------------------------
void MOS_FireResultListView::BuildEquipmentResult( const MOS_FireResult& result, QListViewItem& parentItem )
{
    QListViewItem* pEquiItem = new QListViewItem( &parentItem );
    pEquiItem->setText( 0, tr( "Equipements (dispo, indispo, réparable)" ) );
    for( MOS_FireResult::CIT_FireResultEquipments itEqui = result.GetEquipments().begin(); itEqui != result.GetEquipments().end(); ++itEqui )
    {
        QListViewItem* pItem = new QListViewItem( pEquiItem );
        pItem->setText( 0, ( *itEqui ).c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_FireResultListView::BuildHumanResult
// Created: SBO 2005-12-05
// -----------------------------------------------------------------------------
void MOS_FireResultListView::BuildHumanResult( const MOS_FireResult& result, QListViewItem& parentItem )
{
    QListViewItem* pHumanItem = new QListViewItem( &parentItem );
    pHumanItem->setText( 0, tr( "Humains (officier, ss-officier, mdr)" ) );

    const MOS_FireResult::T_FireResultHuman& off   = *result.GetHumans().find( 0 )->second;
    const MOS_FireResult::T_FireResultHuman& ssoff = *result.GetHumans().find( 1 )->second;
    const MOS_FireResult::T_FireResultHuman& mdr   = *result.GetHumans().find( 2 )->second;
    
    for( uint i = 0; i < off.size(); ++i )
    {
        QListViewItem* pItem = new QListViewItem( pHumanItem );
        std::stringstream ss;
        ss << ENT_Tr::ConvertFromHumanWound( ( E_HumanWound )i, ENT_Tr::eToTr )
           << " - [" << off[ i ] << " " << ssoff[ i ] << " " << mdr[ i ] << "]";
        pItem->setText( 0, ss.str().c_str() );

    }
}

// -----------------------------------------------------------------------------
// Name: MOS_FireResultListView::OnConflictEnded
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
void MOS_FireResultListView::OnConflictEnded( MOS_Agent_ABC& origin )
{
    if( &origin == pOrigin_ )
        OnFireResultCreated( origin.GetFireResult() );
}

// -----------------------------------------------------------------------------
// Name: MOS_FireResultListView::OnObjectExplosion
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
void MOS_FireResultListView::OnObjectExplosion( MOS_Object_ABC& object )
{
    if( &object == pObject_ )
        OnFireResultCreated( object.explosionResults_ );
}
