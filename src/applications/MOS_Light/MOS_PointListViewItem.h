//*****************************************************************************
//
// $Created: NLD 2002-07-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_PointListViewItem.h $
// $Author: Fbd $
// $Modtime: 27/08/03 14:45 $
// $Revision: 6 $
// $Workfile: MOS_PointListViewItem.h $
//
//*****************************************************************************

#ifndef __MOS_PointListViewItem_h_
#define __MOS_PointListViewItem_h_

#include "MOS_Types.h"
#include "MOS_ListViewItem_ABC.h"
#include "MOS_ASN_Types.h"

class MOS_Lima;

//=============================================================================
// Created: NLD 2002-07-16
//=============================================================================
class MOS_PointListViewItem : public MOS_ListViewItem_ABC
{
    MT_COPYNOTALLOWED( MOS_PointListViewItem );

public:
    MOS_PointListViewItem( E_ItemType nItemType, MT_Vector2D& point, QListView& listView );
    MOS_PointListViewItem( E_ItemType nItemType, MT_Vector2D& point, QListView& listView, QListViewItem& itemParent );
    MOS_PointListViewItem( E_ItemType nItemType, MT_Vector2D& point, QListViewItem& itemParent );
    MOS_PointListViewItem( E_ItemType nItemType, MT_Vector2D& point, QListViewItem& itemParent, QListViewItem& itemAfter );
    virtual ~MOS_PointListViewItem();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    MT_Vector2D&        GetPoint();
    const MT_Vector2D&  GetPos() const;
    const MT_Vector2D   GetSize() const;
    const std::string&  GetName() const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Write*/
    //-------------------------------------------------------------------------
    //@{
    void SetPos( const MT_Vector2D& vPos );
    //@}

    //-------------------------------------------------------------------------
    /** @name GFX*/
    //-------------------------------------------------------------------------
    //@{
    void Draw( MOS_PointListViewItem* pSelectedPoint = 0) const;
    bool IsInside( const MT_Vector2D& vPos ) const;
    //@}

    //-------------------------------------------------------------------------
    /** @name DIN */
    //-------------------------------------------------------------------------
    //@{
    void WriteOrderMsg( DIN::DIN_BufferedMessage& msg );
    void WriteMsg     ( ASN1T_CoordUTM& asnPoint, std::string& sParam );
    std::string& ConvertPositionValue( std::string& strValue );
    //@}
private:
    MT_Vector2D&    point_;
    std::string     sMGRS_;
    MT_Rect         rect_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_PointListViewItem.inl"
#endif

#endif // __MOS_PointListViewItem_h_
