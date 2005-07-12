//*****************************************************************************
//
// $Created: AGN 03-08-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_ListLocalisation_ListViewItem.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 4 $
// $Workfile: MOS_MTH_ListLocalisation_ListViewItem.h $
//
//*****************************************************************************

#ifndef __MOS_MTH_ListLocalisation_ListViewItem_h_
#define __MOS_MTH_ListLocalisation_ListViewItem_h_

#include <qlistview.h>

#include "MOS_ASN_Types.h"

//*****************************************************************************
// Created: AGN 03-08-08
//*****************************************************************************
class MOS_MTH_ListLocalisation_ListViewItem
: public QListViewItem
{
    MT_COPYNOTALLOWED( MOS_MTH_ListLocalisation_ListViewItem )

public:
    enum
    {
        e_rtti = 20
    };
    
public:
    explicit MOS_MTH_ListLocalisation_ListViewItem( QListView* pParent );
    virtual ~MOS_MTH_ListLocalisation_ListViewItem();

    virtual int rtti() const;

    const T_PointVector& GetLocalisation();
    ASN1T_EnumTypeLocalisation GetType() const;

    void SetType( ASN1T_EnumTypeLocalisation nType );
    
    void ClearList();
    void AddPoint( const MT_Vector2D& point );

    
    void FillRandomParameters();
    void ReloadParameters( MT_InputArchive_ABC& archive );
    void ResaveParameters( MT_OutputArchive_ABC& archive );

private:
    T_PointVector vLocalisationPoint_;
    ASN1T_EnumTypeLocalisation nTypeLocalisation_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_MTH_ListLocalisation_ListViewItem.inl"
#endif


#endif // __MOS_MTH_ListLocalisation_ListViewItem_h_