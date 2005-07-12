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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimitListViewItem.h $
// $Author: Fbd $
// $Modtime: 27/08/03 14:43 $
// $Revision: 7 $
// $Workfile: MOS_LimitListViewItem.h $
//
//*****************************************************************************

#ifndef __MOS_LimitListViewItem_h_
#define __MOS_LimitListViewItem_h_

#ifdef __GNUG__
#    pragma interface
#endif

#include "MOS_Types.h"

#include "MOS_ListViewItem_ABC.h"
#include "MOS_PointListViewItem.h"

class QListView;
class QListViewItem;
class MOS_Limit;
class MOS_LimitFuncCheckListItem;


//=============================================================================
// Created: NLD 2002-07-16
//=============================================================================
class MOS_LimitListViewItem : public MOS_ListViewItem_ABC
{
    MT_COPYNOTALLOWED( MOS_LimitListViewItem );
private:
    typedef std::vector< MOS_PointListViewItem* >  T_PointPtrVector;
    typedef const T_PointPtrVector              CT_PointPtrVector;
    typedef T_PointPtrVector::iterator          IT_PointPtrVector;
    typedef T_PointPtrVector::const_iterator    CIT_PointPtrVector;
    typedef T_PointPtrVector::reverse_iterator  RIT_PointPtrVector;

    typedef std::vector< MOS_LimitFuncCheckListItem* >   T_FuncPtrVector;
    typedef const T_FuncPtrVector                       CT_FuncPtrVector;
    typedef T_FuncPtrVector::iterator                   IT_FuncPtrVector;
    typedef T_FuncPtrVector::const_iterator             CIT_FuncPtrVector;
    typedef T_FuncPtrVector::reverse_iterator           RIT_FuncPtrVector;

public:
    MOS_LimitListViewItem( E_ItemType nItemType, MOS_Limit& Limit, QListView& listView,                           const std::string& strField, const std::string& strValue = "" );
    MOS_LimitListViewItem( E_ItemType nItemType, MOS_Limit& Limit, QListViewItem& item,                           const std::string& strField, const std::string& strValue = "" );
    MOS_LimitListViewItem( E_ItemType nItemType, MOS_Limit& Limit, QListView& item, QListViewItem& itemAfter, const std::string& strField, const std::string& strValue = "" );
   
    
    virtual ~MOS_LimitListViewItem();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    MOS_Limit&               GetLimit() const;
    MOS_PointListViewItem*  GetFirstPoint() const;
    const std::string       GetName() const;
    void                    SetName( const std::string sName );
    const MT_Vector2D       GetCenterPoint( MOS_PointListViewItem* pPoint ) const;
    MOS_PointListViewItem*  GetPointAfter( const MOS_PointListViewItem& point ) const;
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
    void                    Draw( MOS_LimitListViewItem* pSelectedLimit, MOS_PointListViewItem* pSelectedPoint ) const;
    MOS_PointListViewItem*  IsInsidePoint( const MT_Vector2D& vPos ) const;
    MOS_PointListViewItem*  IsInsideLine( const MT_Vector2D& vPos ) const;
    //@}

   
    //-------------------------------------------------------------------------
    /** @name DIN*/
    //-------------------------------------------------------------------------
    //@{
    void WriteMsg     ( ASN1T_OID& asnOID, std::string& sParam );
    //@}


private:
    MOS_Limit&                   Limit_;
    T_PointPtrVector            pointVector_;
    T_FuncPtrVector             funcVector_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_LimitListViewItem.inl"
#endif

#endif // __MOS_LimitListViewItem_h_
