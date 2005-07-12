//*****************************************************************************
//
// $Created: NLD 2002-07-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimaListViewItem.h $
// $Author: Nld $
// $Modtime: 25/05/04 17:51 $
// $Revision: 11 $
// $Workfile: MOS_LimaListViewItem.h $
//
//*****************************************************************************

#ifndef __MOS_LimaListViewItem_h_
#define __MOS_LimaListViewItem_h_

#ifdef __GNUG__
#    pragma interface
#endif

#include "MOS_Types.h"

#include "MOS_ListViewItem_ABC.h"
#include "MOS_PointListViewItem.h"
#include "MOS_Order_Def.h"

class QListView;
class QListViewItem;
class MOS_Lima;
class MOS_LimaFuncCheckListItem;

//=============================================================================
// Created: NLD 2002-07-16
//=============================================================================
class MOS_LimaListViewItem : public MOS_ListViewItem_ABC
{
    MT_COPYNOTALLOWED( MOS_LimaListViewItem );
private:
    typedef std::vector< MOS_PointListViewItem* >  T_PointPtrVector;
    typedef const T_PointPtrVector              CT_PointPtrVector;
    typedef T_PointPtrVector::iterator          IT_PointPtrVector;
    typedef T_PointPtrVector::const_iterator    CIT_PointPtrVector;
    typedef T_PointPtrVector::reverse_iterator  RIT_PointPtrVector;

    typedef std::vector< MOS_LimaFuncCheckListItem* >   T_FuncPtrVector;
    typedef const T_FuncPtrVector                       CT_FuncPtrVector;
    typedef T_FuncPtrVector::iterator                   IT_FuncPtrVector;
    typedef T_FuncPtrVector::const_iterator             CIT_FuncPtrVector;
    typedef T_FuncPtrVector::reverse_iterator           RIT_FuncPtrVector;

public:
    MOS_LimaListViewItem( E_ItemType nItemType, MOS_Lima& lima, QListView& listView,                           const std::string& strField, const std::string& strValue = "" );
    MOS_LimaListViewItem( E_ItemType nItemType, MOS_Lima& lima, QListViewItem& item,                           const std::string& strField, const std::string& strValue = "" );
    MOS_LimaListViewItem( E_ItemType nItemType, MOS_Lima& lima, QListView& item, QListViewItem& itemAfter, const std::string& strField, const std::string& strValue = "" );
   
    
    virtual ~MOS_LimaListViewItem();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    MOS_Lima&               GetLima() const;
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
    void                    Draw( MOS_LimaListViewItem* pSelectedLima, MOS_PointListViewItem* pSelectedPoint ) const;
    MOS_PointListViewItem*  IsInsidePoint( const MT_Vector2D& vPos ) const;
    MOS_PointListViewItem*  IsInsideLine( const MT_Vector2D& vPos ) const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Func*/
    //-------------------------------------------------------------------------
    //@{
    int                         GetNbSelectFuncLima() const;
    void                        RegisterFuncLima( MOS_LimaFuncCheckListItem& func );
    void                        UnregisterFuncLima( MOS_LimaFuncCheckListItem& func );
    MOS_LimaFuncCheckListItem*  CreateFuncLima( E_FuncLimaType nFuncType );
    void                        DeleteFuncLima( MOS_LimaFuncCheckListItem& func );
    void                        FuncStateChanged( MOS_LimaFuncCheckListItem& funcItem );
    
    void SetFuncState( E_FuncLimaType nFuncType );
    //@}
    
    //-------------------------------------------------------------------------
    /** @name DIN*/
    //-------------------------------------------------------------------------
    //@{
    void WriteMsg( ASN1T_OID& asnOID, std::string& sParam );
    //@}


private:
    MOS_Lima&                   lima_;
    T_PointPtrVector            pointVector_;
    T_FuncPtrVector             funcVector_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_LimaListViewItem.inl"
#endif

#endif // __MOS_LimaListViewItem_h_
