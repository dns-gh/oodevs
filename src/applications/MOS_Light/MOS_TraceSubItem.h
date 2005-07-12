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
// $ATracehive: /MVW_v10/Build/SDK/MOS_Light/sTrace/MOS_TraceSubItem.h $
// $Author: Nld $
// $Modtime: 20/02/03 15:48 $
// $Revision: 3 $
// $Workfile: MOS_TraceSubItem.h $
//
//*****************************************************************************

#ifndef __MOS_TraceSubItem_h_
#define __MOS_TraceSubItem_h_

#ifdef __GNUG__
#    pragma interface
#endif

#include "MOS_Types.h"

#include "MOS_ListViewItem_ABC.h"
#include "MOS_Order_Def.h"

class QListView;
class QListViewItem;
class MOS_Trace;
class MOS_TraceFuncCheckListItem;


//=============================================================================
// Created: NLD 2002-07-16
//=============================================================================
class MOS_TraceSubItem : public MOS_ListViewItem_ABC
{
    MT_COPYNOTALLOWED( MOS_TraceSubItem );

public:
    MOS_TraceSubItem( E_ItemType nItemType, QListView& listView,                           const std::string& strField, const std::string& strValue = "" );
    MOS_TraceSubItem( E_ItemType nItemType, QListViewItem& item,                           const std::string& strField, const std::string& strValue = "" );
    MOS_TraceSubItem( E_ItemType nItemType, QListViewItem& item, QListViewItem& itemAfter, const std::string& strField, const std::string& strValue = "" );
    MOS_TraceSubItem( E_ItemType nItemType, QListView& item    , QListViewItem& itemAfter, const std::string& strField, const std::string& strValue = "" );
    virtual ~MOS_TraceSubItem();

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
#   include "MOS_TraceSubItem.inl"
#endif

#endif // __MOS_TraceSubItem_h_
