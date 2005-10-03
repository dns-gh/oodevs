// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MOS_PopulationListView::ToPopulation
/** @param  pItem 
    @return 
*/
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
inline
MOS_Population* MOS_PopulationListView::ToPopulation( QListViewItem* pItem )
{
    if( pItem == 0 || pItem->rtti() != eAgent )
        return 0;
    return ((MT_ValuedListViewItem< MOS_Population*, eAgent>*)pItem)->GetValue();
}


// -----------------------------------------------------------------------------
// Name: MOS_PopulationListView::ToTeam
/** @param  pItem 
    @return 
*/
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
inline
MOS_Team* MOS_PopulationListView::ToTeam( QListViewItem* pItem )
{
    if( pItem == 0 || pItem->rtti() != eTeam )
        return 0;
    return ((MT_ValuedListViewItem< MOS_Team*, eTeam>*)pItem)->GetValue();
}