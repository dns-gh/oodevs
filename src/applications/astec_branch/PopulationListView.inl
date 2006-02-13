// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PopulationListView::ToPopulation
/** @param  pItem 
    @return 
*/
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
inline
Population* PopulationListView::ToPopulation( QListViewItem* pItem )
{
    if( pItem == 0 || pItem->rtti() != eAgent )
        return 0;
    return ((MT_ValuedListViewItem< Population*, eAgent>*)pItem)->GetValue();
}


// -----------------------------------------------------------------------------
// Name: PopulationListView::ToTeam
/** @param  pItem 
    @return 
*/
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
inline
Team* PopulationListView::ToTeam( QListViewItem* pItem )
{
    if( pItem == 0 || pItem->rtti() != eTeam )
        return 0;
    return ((MT_ValuedListViewItem< Team*, eTeam>*)pItem)->GetValue();
}