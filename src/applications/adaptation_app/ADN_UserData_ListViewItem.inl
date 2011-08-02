// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2003-11-03 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_UserData_ListViewItem.inl $
// $Author: Ape $
// $Modtime: 3/03/05 16:42 $
// $Revision: 2 $
// $Workfile: ADN_UserData_ListViewItem.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: ADN_UserData_ListViewItem constructor
/** @param  pParent 
    @param  strName 
    @param  pPointer 
*/
// Created: AGN 2003-11-03
// -----------------------------------------------------------------------------
template< class PointerType >
ADN_UserData_ListViewItem< PointerType >::ADN_UserData_ListViewItem( Q3ListView* pParent, const std::string& strName, PointerType* pPointer )
: Q3ListViewItem( pParent, strName.c_str() )
, pPointer_( pPointer )
{
    
}

// -----------------------------------------------------------------------------
// Name: ADN_UserData_ListViewItem destructor
// Created: AGN 2003-11-03
// -----------------------------------------------------------------------------
template< class PointerType >
ADN_UserData_ListViewItem< PointerType >::~ADN_UserData_ListViewItem()
{
    
}


// -----------------------------------------------------------------------------
// Name: ADN_UserData_ListViewItem::GetPointer
/** @return 
*/
// Created: AGN 2003-11-03
// -----------------------------------------------------------------------------
template< class PointerType >
PointerType* ADN_UserData_ListViewItem< PointerType >::GetPointer()
{
    return pPointer_;
}
