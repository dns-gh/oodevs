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
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_UserData_ListViewItem.h $
// $Author: Ape $
// $Modtime: 21/10/04 17:47 $
// $Revision: 1 $
// $Workfile: ADN_UserData_ListViewItem.h $
//
// *****************************************************************************

#ifndef __ADN_UserData_ListViewItem_h_
#define __ADN_UserData_ListViewItem_h_

#include <qlistview.h>

// =============================================================================
/** @class  ADN_UserData_ListViewItem
    @brief  ADN_UserData_ListViewItem
    @par    Using example
    @code
    ADN_UserData_ListViewItem;
    @endcode
*/
// Created: AGN 2003-11-03
// =============================================================================
template< class PointerType >
class ADN_UserData_ListViewItem
: public QListViewItem
{

public:
    //! @name Constructors/Destructor
    //@{
    ADN_UserData_ListViewItem( QListView* pParent, const std::string& strName, PointerType* pPointer );
    virtual ~ADN_UserData_ListViewItem();
    //@}

    //! @name Operations
    //@{
    //@}

    //! @name Accessors
    //@{
    PointerType* GetPointer();
    //@}

    //! @name Modifiers
    //@{
    //@}

    //! @name Operators
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ADN_UserData_ListViewItem( const ADN_UserData_ListViewItem& );            //!< Copy constructor
    ADN_UserData_ListViewItem& operator=( const ADN_UserData_ListViewItem& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    PointerType* pPointer_;
    //@}
};

#include "ADN_UserData_ListViewItem.inl"

#endif // __ADN_UserData_ListViewItem_h_
