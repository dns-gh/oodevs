//*****************************************************************************
//
// $Created: NLD 2002-07-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_RCListViewItem.h $
// $Author: Nld $
// $Modtime: 5/06/03 14:59 $
// $Revision: 12 $
// $Workfile: MOS_RCListViewItem.h $
//
//*****************************************************************************

#ifndef __MOS_RCListViewItem_h_
#define __MOS_RCListViewItem_h_

#include "MOS_Types.h"

#include "MOS_ListViewItem_ABC.h"
#include "MOS_Order_Def.h"

class QListView;
class QListViewItem;
class MOS_RC;
class MOS_RCFuncCheckListItem;
class MOS_RCSubItem;

//=============================================================================
// Created: NLD 2002-07-16
//=============================================================================
class MOS_RCListViewItem : public MOS_ListViewItem_ABC
{
    MT_COPYNOTALLOWED( MOS_RCListViewItem );
private:
    typedef std::vector< MOS_RCSubItem* >                   T_RCItemPtrVector;
    typedef const T_RCItemPtrVector                         CT_RCItemPtrVector;
    typedef T_RCItemPtrVector::iterator                     IT_RCItemPtrVector;
    typedef T_RCItemPtrVector::const_iterator               CIT_RCItemPtrVector;
    typedef T_RCItemPtrVector::reverse_iterator             RIT_RCItemPtrVector;

public:
    MOS_RCListViewItem( MOS_RC& RC, QListView& listView,                           const std::string& strField, const std::string& strValue = "" );
    MOS_RCListViewItem( MOS_RC& RC, QListViewItem& item,                           const std::string& strField, const std::string& strValue = "" );
    MOS_RCListViewItem( MOS_RC& RC, QListView& item    , QListViewItem& itemAfter, const std::string& strField, const std::string& strValue = "" );
     
    virtual ~MOS_RCListViewItem();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    MOS_RC&                 GetRC() const;
    const std::string       GetName() const;
    void                    CreateRect();
    //@}
    
    //-------------------------------------------------------------------------
    /** @name GFX */
    //-------------------------------------------------------------------------
    //@{
    void Draw( MOS_RCListViewItem* pSelectedRC ) const;
    bool IsInside( const MT_Vector2D& vPos ) const;
    //@}

private:
    void Initialize();

    //-------------------------------------------------------------------------
    /** @name Tools */
    //-------------------------------------------------------------------------
    //@{
    MOS_RCSubItem&  CreateItem    ( E_ItemType nType, int nValue = 0 );
    MOS_RCSubItem&  CreateItem    ( E_ItemType nType, const std::string& strText );
    void            DeleteItem    ( MOS_RCSubItem& item );
    void            RegisterItem  ( MOS_RCSubItem& item );
    void            UnregisterItem( MOS_RCSubItem& item );
    void            DeleteAllItems();    

    MOS_RCSubItem&      AddText                ( const std::string& strText );
    MOS_RCSubItem&      AddOrderConduite       ( E_OrderConduiteID nOrdreConduite );
    const std::string   BuildNameOrderConduite ( E_OrderConduiteID nOrderConduite ) const;
    //@}

private:
    MOS_RC&                     RC_;
    MT_Vector2D                 vPos1_;
    MT_Vector2D                 vPos2_;
    MT_Vector2D                 vPos3_;
    T_RCItemPtrVector           itemVector_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_RCListViewItem.inl"
#endif

#endif // __MOS_RCListViewItem_h_
