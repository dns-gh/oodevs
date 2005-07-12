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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimitCheckListItem.h $
// $Author: Fbd $
// $Modtime: 21/01/03 16:35 $
// $Revision: 3 $
// $Workfile: MOS_LimitCheckListItem.h $
//
//*****************************************************************************

#ifndef __MOS_LimitCheckListItem_h_
#define __MOS_LimitCheckListItem_h_

#ifdef __GNUG__
#    pragma interface
#endif

#include "MOS_Types.h"
#include "MOS_CheckListItem_ABC.h"
#include "MOS_PointListViewItem.h"

class QListView;
class QListViewItem;
class MOS_Limit;

//=============================================================================
// Created: NLD 2002-07-16
//=============================================================================
class MOS_LimitCheckListItem : public MOS_CheckListItem_ABC
{
    MT_COPYNOTALLOWED( MOS_LimitCheckListItem );
private:
    typedef std::vector< MOS_PointListViewItem* >  T_PointPtrVector;
    typedef const T_PointPtrVector              CT_PointPtrVector;
    typedef T_PointPtrVector::iterator          IT_PointPtrVector;
    typedef T_PointPtrVector::const_iterator    CIT_PointPtrVector;
    typedef T_PointPtrVector::reverse_iterator  RIT_PointPtrVector;

public:
    MOS_LimitCheckListItem( E_ItemType nItemType, MOS_Limit& Limit, QListView& listView,                       QCheckListItem::Type nCheckable = QCheckListItem::CheckBox, const std::string& strField = "", const std::string& strValue = "" );
    MOS_LimitCheckListItem( E_ItemType nItemType, MOS_Limit& Limit, QListViewItem& item,                       QCheckListItem::Type nCheckable = QCheckListItem::CheckBox, const std::string& strField = "", const std::string& strValue = "" );
    MOS_LimitCheckListItem( E_ItemType nItemType, MOS_Limit& Limit, QListView& item, QListViewItem& itemAfter, QCheckListItem::Type nCheckable = QCheckListItem::CheckBox, const std::string& strField = "", const std::string& strValue = "" );
   
    
    virtual ~MOS_LimitCheckListItem();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    MOS_Limit&               GetLimit() const;
    MOS_PointListViewItem*  GetFirstPoint() const;
    const std::string       GetName() const;
    void                    SetName( const std::string sName );
    const MT_Vector2D       GetCenterPoint( MOS_PointListViewItem* pPoint ) const;
    MOS_PointListViewItem*  GetPointBefore( const MOS_PointListViewItem& point ) const;
    MOS_PointListViewItem*  GetPoint( const MT_Vector2D& point );
    //@}

    //-------------------------------------------------------------------------
    /** @name Points*/
    //-------------------------------------------------------------------------
    //@{
    void                   RegisterPoint  ( MOS_PointListViewItem& point, MOS_PointListViewItem* pPointAfter = 0 );
    void                   UnregisterPoint( const MOS_PointListViewItem& point );
    MOS_PointListViewItem& CreatePoint    ( MT_Vector2D& vPoint, MOS_PointListViewItem* pPointAfter = 0);
    void                   DeletePoint    ( MT_Vector2D& vPoint );
    void                   UpdateNamePos  ();
    void                   SetPos         ( const MT_Vector2D& vPos,  MT_Vector2D& vPosMovingLine );
    void                   SetDeleted     ();
    //@}

    //-------------------------------------------------------------------------
    /** @name GFX*/
    //-------------------------------------------------------------------------
    //@{
    void                    Draw( MOS_LimitCheckListItem* pSelectedLimit, MOS_PointListViewItem* pSelectedPoint ) const;
    MOS_PointListViewItem*  IsInsidePoint( const MT_Vector2D& vPos ) const;
    MOS_PointListViewItem*  IsInsideLine( const MT_Vector2D& vPos ) const;
    //@}


private:
    MOS_Limit&                   Limit_;
    T_PointPtrVector            pointVector_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_LimitCheckListItem.inl"
#endif

#endif // __MOS_LimitCheckListItem_h_
