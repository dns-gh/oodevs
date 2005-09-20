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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentListView.inl $
// $Author: Ape $
// $Modtime: 2/06/04 10:16 $
// $Revision: 4 $
// $Workfile: MOS_AgentListView.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: MOS_AgentListView::sizeHint
/** @return 
*/
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
inline
QSize MOS_AgentListView::sizeHint() const
{
    return QSize( 230, 340 );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentListView::ToTeam
/** @param  pItem 
    @return 
*/
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
inline
MOS_Team* MOS_AgentListView::ToTeam( QListViewItem* pItem )
{
    if( pItem == 0 || pItem->rtti() != eTeam )
        return 0;
    return ((MT_ValuedListViewItem< MOS_Team*, eTeam>*)pItem)->GetValue();
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentListView::ToGtia
/** @param  pItem 
    @return 
*/
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
inline
MOS_Gtia* MOS_AgentListView::ToGtia( QListViewItem* pItem )
{
    if( pItem == 0 || pItem->rtti() != eGtia )
        return 0;
    return ((MT_ValuedListViewItem< MOS_Gtia*, eGtia>*)pItem)->GetValue();
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentListView::ToAgent
/** @param  pItem 
    @return 
*/
// Created: APE 2004-03-22
// -----------------------------------------------------------------------------
inline
MOS_Agent* MOS_AgentListView::ToAgent( QListViewItem* pItem )
{
    if( pItem == 0 || pItem->rtti() != eAgent )
        return 0;
    return ((MT_ValuedListViewItem< MOS_Agent*, eAgent>*)pItem)->GetValue();
}

