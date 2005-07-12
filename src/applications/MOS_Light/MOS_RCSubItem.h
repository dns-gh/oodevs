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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_RCSubItem.h $
// $Author: Nld $
// $Modtime: 20/02/03 15:49 $
// $Revision: 3 $
// $Workfile: MOS_RCSubItem.h $
//
//*****************************************************************************

#ifndef __MOS_RCSubItem_h_
#define __MOS_RCSubItem_h_

#ifdef __GNUG__
#    pragma interface
#endif

#include "MOS_Types.h"

#include "MOS_ListViewItem_ABC.h"
#include "MOS_Order_Def.h"

class QListView;
class QListViewItem;
class MOS_RC;
class MOS_RCFuncCheckListItem;


//=============================================================================
// Created: NLD 2002-07-16
//=============================================================================
class MOS_RCSubItem : public MOS_ListViewItem_ABC
{
    MT_COPYNOTALLOWED( MOS_RCSubItem );

public:
    MOS_RCSubItem( E_ItemType nItemType, QListView& listView,                           const std::string& strField, const std::string& strValue = "" );
    MOS_RCSubItem( E_ItemType nItemType, QListViewItem& item,                           const std::string& strField, const std::string& strValue = "" );
    MOS_RCSubItem( E_ItemType nItemType, QListViewItem& item, QListViewItem& itemAfter, const std::string& strField, const std::string& strValue = "" );
    MOS_RCSubItem( E_ItemType nItemType, QListView& item    , QListViewItem& itemAfter, const std::string& strField, const std::string& strValue = "" );
    virtual ~MOS_RCSubItem();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    void SetName( const std::string& sName );
    void SetValue( int nValue );
    int  GetValue() const;
    //@}

private:
  
    int     nValue_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_RCSubItem.inl"
#endif

#endif // __MOS_RCSubItem_h_
