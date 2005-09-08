// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-08-05 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ObjectListView.inl $
// $Author: Ape $
// $Modtime: 5/08/04 11:49 $
// $Revision: 1 $
// $Workfile: MOS_ObjectListView.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: MOS_ObjectListView::sizeHint
/** @return 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
MOS_INLINE
QSize MOS_ObjectListView::sizeHint() const
{
    return QSize( 230, 340 );
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectListView::ToObject
/** @param  pItem 
    @return 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
MOS_INLINE
MOS_Object_ABC* MOS_ObjectListView::ToObject( QListViewItem* pItem )
{
    if( pItem == 0 || pItem->rtti() != eObject )
        return 0;
    return ((MT_ValuedListViewItem< MOS_Object_ABC*, eObject>*)pItem)->GetValue();
}
