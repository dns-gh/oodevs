//*****************************************************************************
//
// $Created: AGN 03-08-26 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_PointList_ListViewItem.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 3 $
// $Workfile: MOS_MTH_PointList_ListViewItem.h $
//
//*****************************************************************************

#ifndef __MOS_MTH_PointList_ListViewItem_h_
#define __MOS_MTH_PointList_ListViewItem_h_

#include <qlistview.h>

#include "MOS_ASN_Types.h"

//*****************************************************************************
// Created: AGN 03-08-26
//*****************************************************************************
class MOS_MTH_PointList_ListViewItem
: public QListViewItem
{
    MT_COPYNOTALLOWED( MOS_MTH_PointList_ListViewItem )

public:
    enum
    {
        e_rtti = 21
    };
    
public:
    explicit MOS_MTH_PointList_ListViewItem( QListView* pParent );
    virtual ~MOS_MTH_PointList_ListViewItem();

    virtual int rtti() const;

    const T_PointVector& GetListPoint();

    void ClearList();
    void AddPoint( const MT_Vector2D& point );

    void FillRandomParameters();
    void ReloadParameters( MT_InputArchive_ABC& archive );
    void ResaveParameters( MT_OutputArchive_ABC& archive );

private:
    T_PointVector vLocalisationPoint_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_MTH_PointList_ListViewItem.inl"
#endif


#endif // __MOS_MTH_PointList_ListViewItem_h_