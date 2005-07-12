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
// $ATracehive: /MVW_v10/Build/SDK/MOS_Light/sTrace/MOS_TraceListViewItem.h $
// $Author: Agn $
// $Modtime: 3/06/03 14:09 $
// $Revision: 4 $
// $Workfile: MOS_TraceListViewItem.h $
//
//*****************************************************************************

#ifndef __MOS_TraceListViewItem_h_
#define __MOS_TraceListViewItem_h_

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
class MOS_TraceSubItem;

//=============================================================================
// Created: NLD 2002-07-16
//=============================================================================
class MOS_TraceListViewItem : public MOS_ListViewItem_ABC
{
    MT_COPYNOTALLOWED( MOS_TraceListViewItem );
private:
    typedef std::vector< MOS_TraceSubItem* >                   T_TraceItemPtrVector;
    typedef const T_TraceItemPtrVector                         CT_TraceItemPtrVector;
    typedef T_TraceItemPtrVector::iterator                     IT_TraceItemPtrVector;
    typedef T_TraceItemPtrVector::const_iterator               CIT_TraceItemPtrVector;
    typedef T_TraceItemPtrVector::reverse_iterator             RIT_TraceItemPtrVector;

public:
    MOS_TraceListViewItem( E_ItemType nItemType, MOS_Trace& Trace, QListView& listView,                           const std::string& strField, const std::string& strValue = "" );
    MOS_TraceListViewItem( E_ItemType nItemType, MOS_Trace& Trace, QListViewItem& item,                           const std::string& strField, const std::string& strValue = "" );
    MOS_TraceListViewItem( E_ItemType nItemType, MOS_Trace& Trace, QListView& item, QListViewItem& itemAfter, const std::string& strField, const std::string& strValue = "" );
   
    
    virtual ~MOS_TraceListViewItem();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    MOS_Trace&                 GetTrace() const;
    const std::string       GetName() const;
    void                    CreateRect();
    //@}
    
    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{

    //@}

    //-------------------------------------------------------------------------
    /** @name GFX*/
    //-------------------------------------------------------------------------
    //@{
    void Draw( MOS_TraceListViewItem* pSelectedTrace ) const;
    bool IsInside( const MT_Vector2D& vPos ) const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Trace spécifiques*/
    //-------------------------------------------------------------------------
    //@{
    MOS_TraceSubItem&  CreateItem          ( E_ItemType nType, int nValue = 0 );
    void            DeleteItem          ( MOS_TraceSubItem& item );
    void            RegisterItem        ( MOS_TraceSubItem& item );
    void            UnregisterItem      ( MOS_TraceSubItem& item );
    //@}
 
private:
    void Initialize();

private:
    MOS_Trace&                     Trace_;
    MT_Vector2D                 vPos1_;
    MT_Vector2D                 vPos2_;
    MT_Vector2D                 vPos3_;
    T_TraceItemPtrVector           itemVector_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_TraceListViewItem.inl"
#endif

#endif // __MOS_TraceListViewItem_h_
