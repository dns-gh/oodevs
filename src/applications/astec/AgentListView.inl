// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-15 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentListView.inl $
// $Author: Ape $
// $Modtime: 2/06/04 10:16 $
// $Revision: 4 $
// $Workfile: AgentListView.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: AgentListView::sizeHint
/** @return 
*/
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
inline
QSize AgentListView::sizeHint() const
{
    return QSize( 230, 340 );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::ToTeam
/** @param  pItem 
    @return 
*/
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
inline
Team* AgentListView::ToTeam( QListViewItem* pItem )
{
    if( pItem == 0 || pItem->rtti() != eTeam )
        return 0;
    return ((MT_ValuedListViewItem< Team*, eTeam>*)pItem)->GetValue();
}


// -----------------------------------------------------------------------------
// Name: AgentListView::ToGtia
/** @param  pItem 
    @return 
*/
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
inline
Gtia* AgentListView::ToGtia( QListViewItem* pItem )
{
    if( pItem == 0 || pItem->rtti() != eGtia )
        return 0;
    return ((MT_ValuedListViewItem< Gtia*, eGtia>*)pItem)->GetValue();
}


// -----------------------------------------------------------------------------
// Name: AgentListView::ToAgent
/** @param  pItem 
    @return 
*/
// Created: APE 2004-03-22
// -----------------------------------------------------------------------------
inline
Agent* AgentListView::ToAgent( QListViewItem* pItem )
{
    if( pItem == 0 || pItem->rtti() != eAgent )
        return 0;
    return ((MT_ValuedListViewItem< Agent*, eAgent>*)pItem)->GetValue();
}

