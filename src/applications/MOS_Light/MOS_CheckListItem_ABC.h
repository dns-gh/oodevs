//*****************************************************************************
//
// This file is part of MASA DIN library.
//
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
// All Rights Reserved. DIN is a trademark of MASA Corporation.
//
//*****************************************************************************
//
// $Created: NLD 2002-07-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_CheckListItem_ABC.h $
// $Author: Fbd $
// $Modtime: 21/01/03 16:16 $
// $Revision: 3 $
// $Workfile: MOS_CheckListItem_ABC.h $
//
//*****************************************************************************

#ifndef __MOS_CheckListItem_ABC_h_
#define __MOS_CheckListItem_ABC_h_

#ifdef __GNUG__
#    pragma interface
#endif

#include "MOS_Types.h"
#include <qListView.h>

class MOS_;

#include "MOS_ListViewItem_ABC.h"

//=============================================================================
/**
*   @class  MOS_CheckListItem_ABC
*   @brief  PUT THE COMMENTS ON THE CLASS HERE
*   @par    Using example
*   @code
*     PUT YOUR EXAMPLE CODE HERE
*   @endcode
*/
// Created: NLD 2002-07-16
//=============================================================================
class MOS_CheckListItem_ABC : public QCheckListItem
{
    MT_COPYNOTALLOWED( MOS_CheckListItem_ABC );

public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{

public:
    MOS_CheckListItem_ABC( E_ItemType nItemType, QListView& listView,                                   QCheckListItem::Type nCheckable = QCheckListItem::CheckBox, const std::string& strField = "", const std::string& strValue = "" );
    MOS_CheckListItem_ABC( E_ItemType nItemType, QListViewItem& itemParent,                             QCheckListItem::Type nCheckable = QCheckListItem::CheckBox, const std::string& strField = "", const std::string& strValue = "" );
    MOS_CheckListItem_ABC( E_ItemType nItemType, QListViewItem& itemParent, QListViewItem& itemAfter,   QCheckListItem::Type nCheckable = QCheckListItem::CheckBox, const std::string& strField = "", const std::string& strValue = "" );
    MOS_CheckListItem_ABC( E_ItemType nItemType, QListView& itemParent, QListViewItem& itemAfter,       QCheckListItem::Type nCheckable = QCheckListItem::CheckBox, const std::string& strField = "", const std::string& strValue = "" );
    virtual ~MOS_CheckListItem_ABC();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    E_ItemType GetItemType() const;
    //@}

private:
    E_ItemType nItemType_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_CheckListItem_ABC.inl"
#endif

#endif // __MOS_CheckListItem_ABC_h_
