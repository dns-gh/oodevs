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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ObjectListView.inl $
// $Author: Ape $
// $Modtime: 5/08/04 11:49 $
// $Revision: 1 $
// $Workfile: ObjectListView.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: ObjectListView::sizeHint
/** @return 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
inline
QSize ObjectListView::sizeHint() const
{
    return QSize( 230, 340 );
}


// -----------------------------------------------------------------------------
// Name: ObjectListView::ToObject
/** @param  pItem 
    @return 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
inline
Object_ABC* ObjectListView::ToObject( QListViewItem* pItem )
{
    if( pItem == 0 || pItem->rtti() != eObject )
        return 0;
    return ((MT_ValuedListViewItem< Object_ABC*, eObject>*)pItem)->GetValue();
}
